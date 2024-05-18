#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "chunk.h"

void disassembleChunk (Chunk* chunk, const char* name);
int disassembleInstruction (Chunk* chunk, int offset);

#ifdef __cplusplus
}
#endif
