#pragma once

#if __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "value.h"

typedef enum OpCode {
    OP_CONSTANT,
    OP_RETURN
} OpCode;

typedef struct Chunk {
    int count;
    int capacity;
    uint8_t* code;
    ValueArray constants;
} Chunk;

void initChunk (Chunk* chunk);
void freeChunk (Chunk* chunk);
void writeChunk (Chunk* chunk, uint8_t byte);
int addConstant (Chunk* chunk, Value value);

#if __cplusplus
}
#endif
