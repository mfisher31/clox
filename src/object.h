#pragma once

#include "common.h"
#include "value.h"

typedef enum {
    OBJ_STRING
} ObjType;

struct Obj {
    ObjType type;
    Obj* next;
};

struct ObjString {
    Obj obj;
    int length;
    char* chars;
};

static inline bool isObjType (Value value, ObjType type) {
    return IS_OBJ (value) && AS_OBJ (value)->type == type;
}

#define OBJ_TYPE(obj)     (AS_OBJ (value)->type)
#define IS_STRING(value)  isObjType (value, OBJ_STRING)
#define AS_STRING(value)  ((ObjString*) AS_OBJ (value))
#define AS_CSTRING(value) (((ObjString*) AS_OBJ (value))->chars)

ObjString* takeString (char* str, int length);
ObjString* copyString (const char* str, int length);
void printObject (Value value);