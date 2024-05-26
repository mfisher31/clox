
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "object.h"
#include "vm.h"

void* reallocate (void* pointer, size_t oldSize, size_t newSize) {
    (void) oldSize;
    if (newSize == 0) {
        free (pointer);
        return NULL;
    }
    void* result = realloc (pointer, newSize);
    if (result == NULL)
        exit (1);
    return result;
}

static void freeObject (Obj* object) {
    switch (object->type) {
        case OBJ_STRING: {
            ObjString* string = (ObjString*) object;
            FREE_ARRAY (char, string->chars, string->length + 1);
            FREE (ObjString, string);
            break;
        }

        case OBJ_FUNCTION: {
            ObjFunction* f = (ObjFunction*) object;
            freeChunk (&f->chunk);
            FREE (ObjFunction, f);
            break;
        }

        case OBJ_CLOSURE: {
            ObjClosure* c = (ObjClosure*) object;
            FREE_ARRAY (ObjUpvalue*, c->upvalues, c->upvalueCount);
            FREE (ObjClosure, object); // closure doesn't own the fn
            break;
        }

        case OBJ_UPVALUE: {
            FREE (ObjUpvalue, object);
            break;
        }

        case OBJ_NATIVE: {
            FREE (ObjNative, object);
            break;
        }
    }
}

void freeObjects() {
    Obj* object = vm.objects;
    while (object != NULL) {
        Obj* next = object->next;
        freeObject (object);
        object = next;
    }
}
