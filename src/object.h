#pragma once

#include <stdint.h>

#include "chunk.h"
#include "common.h"
#include "table.h"
#include "value.h"

typedef enum {
    OBJ_STRING,
    OBJ_BOUND_METHOD,
    OBJ_CLASS,
    OBJ_INSTANCE,
    OBJ_CLOSURE,
    OBJ_FUNCTION,
    OBJ_UPVALUE,
    OBJ_NATIVE
} ObjType;

struct Obj {
    ObjType type;
    bool isMarked;
    Obj* next;
};

typedef struct {
    Obj obj;
    int arity;
    int upvalueCount;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

typedef Value (*NativeFn) (int, Value*);

typedef struct {
    Obj obj;
    NativeFn function;
} ObjNative;

struct ObjUpvalue {
    Obj obj;
    Value* location;
    Value closed;
    ObjUpvalue* next;
};

typedef struct {
    Obj obj;
    ObjFunction* function;
    ObjUpvalue** upvalues;
    int upvalueCount;
} ObjClosure;

struct ObjClass {
    Obj obj;
    ObjString* name;
    Table methods;
};

struct ObjInstance {
    Obj obj;
    ObjClass* klass;
    Table fields;
};

struct ObjBoundMethod {
    Obj obj;
    Value receiver;
    ObjClosure* method;
};

struct ObjString {
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

static inline bool isObjType (Value value, ObjType type) {
    return IS_OBJ (value) && AS_OBJ (value)->type == type;
}

#define OBJ_TYPE(value) (AS_OBJ (value)->type)

#define IS_BOUND_METHOD(value) isObjType (value, OBJ_BOUND_METHOD)
#define IS_CLASS(value)        isObjType (value, OBJ_CLASS)
#define IS_INSTANCE(value)     isObjType (value, OBJ_INSTANCE)
#define IS_FUNCTION(value)     isObjType (value, OBJ_FUNCTION)
#define IS_STRING(value)       isObjType (value, OBJ_STRING)
#define IS_NATIVE(value)       isObjType (value, OBJ_NATIVE)
#define IS_CLOSURE(value)      isObjType (value, OBJ_CLOSURE)

#define AS_BOUND_METHOD(value) ((ObjBoundMethod*) AS_OBJ (value))
#define AS_CLASS(value)        ((ObjClass*) AS_OBJ (value))
#define AS_INSTANCE(value)     ((ObjInstance*) AS_OBJ (value))
#define AS_FUNCTION(value)     ((ObjFunction*) AS_OBJ (value))
#define AS_STRING(value)       ((ObjString*) AS_OBJ (value))
#define AS_CSTRING(value)      (((ObjString*) AS_OBJ (value))->chars)
#define AS_NATIVE(value)       (((ObjNative*) AS_OBJ (value))->function)
#define AS_CLOSURE(value)      ((ObjClosure*) AS_OBJ (value))

ObjFunction* newFunction();
ObjUpvalue* newUpvalue (Value* slot);
ObjClass* newClass (ObjString* name);
ObjInstance* newInstance (ObjClass* klass);
ObjBoundMethod* newBoundMethod (Value receiver, ObjClosure* closure);
ObjClosure* newClosure (ObjFunction* function);
ObjNative* newNative (NativeFn function);
ObjString* takeString (char* str, int length);
ObjString* copyString (const char* str, int length);
void printObject (Value value);