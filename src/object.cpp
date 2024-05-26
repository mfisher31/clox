#include <stdio.h>
#include <string.h>

#include "common.h"
#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

static Obj* allocateObject (size_t size, ObjType type) {
    Obj* object      = (Obj*) reallocate (NULL, 0, size);
    object->type     = type;
    object->next     = vm.objects;
    object->isMarked = false;
    vm.objects       = object;

#ifdef DEBUG_LOG_GC
    printf ("%p allocate %zu for %d\n", (void*) object, size, type);
#endif

    return object;
}

#define ALLOCATE_OBJ(type, objectType) \
    (type*) allocateObject (sizeof (type), objectType)

static ObjString* allocateString (char* chars, int length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ (ObjString, OBJ_STRING);
    string->length    = length;
    string->chars     = chars;
    string->hash      = hash;
    push ((OBJ_VAL (string)));
    tableSet (&vm.strings, string, NIL_VAL);
    pop();
    return string;
}

// FNV-la hash algo
static uint32_t hashString (const char* key, int length) {
    uint32_t hash = 2166136261U;
    for (int i = 0; i < length; ++i) {
        hash ^= (uint8_t) key[i];
        hash *= 16777619;
    }
    return hash;
}

ObjFunction* newFunction() {
    ObjFunction* f  = ALLOCATE_OBJ (ObjFunction, OBJ_FUNCTION);
    f->arity        = 0;
    f->upvalueCount = 0;
    f->name         = NULL;
    initChunk (&f->chunk);
    return f;
}

ObjUpvalue* newUpvalue (Value* slot) {
    ObjUpvalue* uv = ALLOCATE_OBJ (ObjUpvalue, OBJ_UPVALUE);
    uv->location   = slot;
    uv->next       = nullptr;
    uv->closed     = NIL_VAL;
    return uv;
}

ObjClass* newClass (ObjString* name) {
    ObjClass* klass = ALLOCATE_OBJ (ObjClass, OBJ_CLASS);
    klass->name     = name;
    return klass;
}

ObjInstance* newInstance (ObjClass* klass) {
    ObjInstance* inst = ALLOCATE_OBJ (ObjInstance, OBJ_INSTANCE);
    inst->klass       = klass;
    initTable (&inst->fields);
    return inst;
}

ObjClosure* newClosure (ObjFunction* function) {
    ObjUpvalue** upvalues = ALLOCATE (ObjUpvalue*, function->upvalueCount);
    for (int i = 0; i < function->upvalueCount; ++i)
        upvalues[i] = nullptr;

    ObjClosure* c   = ALLOCATE_OBJ (ObjClosure, OBJ_CLOSURE);
    c->function     = function;
    c->upvalues     = upvalues;
    c->upvalueCount = function->upvalueCount;
    return c;
}

ObjNative* newNative (NativeFn function) {
    ObjNative* f = ALLOCATE_OBJ (ObjNative, OBJ_NATIVE);
    f->function  = function;
    return f;
}

ObjString* takeString (char* str, int length) {
    uint32_t hash       = hashString (str, length);
    ObjString* interned = tableFindString (&vm.strings, str, length, hash);
    if (interned != NULL) {
        FREE_ARRAY (char, str, length + 1);
        return interned;
    }

    return allocateString (str, length, hash);
}

ObjString* copyString (const char* chars, int length) {
    uint32_t hash       = hashString (chars, length);
    ObjString* interned = tableFindString (&vm.strings, chars, length, hash);
    if (interned != NULL)
        return interned;

    char* heapChars = ALLOCATE (char, length + 1);
    memcpy (heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString (heapChars, length, hash);
}

void printFunction (ObjFunction* function) {
    if (function->name == NULL) {
        printf ("<script>");
        return;
    }

    printf ("<fn %s>", function->name->chars);
}

void printObject (Value value) {
    switch (OBJ_TYPE (value)) {
        case OBJ_CLASS:
            printf ("%s", AS_CLASS (value)->name->chars);
            break;
        case OBJ_INSTANCE:
            printf ("%s instance", AS_INSTANCE (value)->klass->name->chars);
            break;
        case OBJ_STRING:
            printf ("%s", AS_CSTRING (value));
            break;
        case OBJ_CLOSURE:
            printFunction (AS_CLOSURE (value)->function);
            break;
        case OBJ_FUNCTION:
            printFunction (AS_FUNCTION (value));
            break;
        case OBJ_UPVALUE:
            printf ("upvalue");
            break;
        case OBJ_NATIVE:
            printf ("<native fn>");
            break;
    }
}
