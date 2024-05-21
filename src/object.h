#pragma once

#include <stdint.h>

#include "chunk.h"
#include "common.h"
#include "value.h"

typedef enum {
    OBJ_STRING,
    OBJ_FUNCTION
} ObjType;

struct Obj {
    ObjType type;
    Obj* next;
};

typedef struct {
    Obj obj;
    int arity;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

struct ObjString {
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

static inline bool isObjType (Value value, ObjType type) {
    return IS_OBJ (value) && AS_OBJ (value)->type == type;
}

#define OBJ_TYPE(obj)      (AS_OBJ (value)->type)
#define IS_FUNCTION(value) isObjType (value, OBJ_STRING)
#define IS_STRING(value)   isObjType (value, OBJ_STRING)
#define AS_FUNCTION(value) ((ObjFunction*) AS_OBJ (value))
#define AS_STRING(value)   ((ObjString*) AS_OBJ (value))
#define AS_CSTRING(value)  (((ObjString*) AS_OBJ (value))->chars)

ObjFunction* newFunction();

ObjString* takeString (char* str, int length);
ObjString* copyString (const char* str, int length);
void printObject (Value value);