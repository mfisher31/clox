
#include "chunk.h"
#include "debug.h"

int main(int, const char **) {
    Chunk chunk;
    initChunk (&chunk);
    writeChunk (&chunk, OP_RETURN);
    disassembleChunk (&chunk, "test chunk");
    freeChunk (&chunk);
    return 0;
}
