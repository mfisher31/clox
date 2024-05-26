#pragma once

#include <stdint.h>

#include "chunk.h"
#include "common.h"
#include "value.h"

typedef enum {
    OBJ_STRING,
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

struct ObjString {
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

static inline bool isObjType (Value value, ObjType type) {
    return IS_OBJ (value) && AS_OBJ (value)->type == type;
}

#define OBJ_TYPE(value)    (AS_OBJ (value)->type)
#define IS_FUNCTION(value) isObjType (value, OBJ_FUNCTION)
#define IS_STRING(value)   isObjType (value, OBJ_STRING)
#define IS_NATIVE(value)   isObjType (value, OBJ_NATIVE)
#define IS_CLOSURE(value)  isObjType (value, OBJ_CLOSURE)

#define AS_FUNCTION(value) ((ObjFunction*) AS_OBJ (value))
#define AS_STRING(value)   ((ObjString*) AS_OBJ (value))
#define AS_CSTRING(value)  (((ObjString*) AS_OBJ (value))->chars)
#define AS_NATIVE(value)   (((ObjNative*) AS_OBJ (value))->function)
#define AS_CLOSURE(value)  ((ObjClosure*) AS_OBJ (value))

ObjFunction* newFunction();
ObjUpvalue* newUpvalue (Value* slot);
ObjClosure* newClosure (ObjFunction* function);
ObjNative* newNative (NativeFn function);
ObjString* takeString (char* str, int length);
ObjString* copyString (const char* str, int length);
void printObject (Value value);