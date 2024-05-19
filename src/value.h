#pragma once
#pragma once

#if __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef double Value;

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

void initValueArray (ValueArray* array);
void writeValueArray (ValueArray* array, Value value);
void freeValueArray (ValueArray* array);
void printValue (Value value);

#if __cplusplus
}
#endif
