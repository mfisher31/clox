#pragma once

#if __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum OpCode {
    OP_RETURN
} OpCode;

typedef struct Chunk {
    int count;
    int capacity;
    uint8_t* code;
} Chunk;

void initChunk (Chunk* chunk);
void freeChunk (Chunk* chunk);
void writeChunk (Chunk* chunk, uint8_t byte);

#if __cplusplus
}
#endif
