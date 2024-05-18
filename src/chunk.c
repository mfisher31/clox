
#include <stddef.h>

#include "chunk.h"

void initChunk (Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
}

void writeChunk (Chunk* chunk, uint8_t byte) {
    if (chunk->capacity <= chunk->count) {
        
    }
}
