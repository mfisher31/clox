
#include <stdint.h>
#include <stdio.h>

#include "debug.h"

static int simpleInstruction (const char* name, int offset) {
    printf ("%s\n", name);
    return offset + 1;
}

static int constantInstruction (const char* name, const Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf ("%-16s %4d '", name, constant);
    printValue (chunk->constants.values[constant]);
    printf ("'\n");
    return offset + 2;
}

void disassembleChunk (Chunk* chunk, const char* name) {
    printf ("== %s ==\n", name);
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction (chunk, offset);
    }
}

int disassembleInstruction (Chunk* chunk, int offset) {
    printf ("%04d ", offset);
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf ("   | ");
    } else {
        printf ("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction ("OP_CONSTANT", chunk, offset);
            break;
        case OP_NEGATE:
            return simpleInstruction ("OP_NEGATE", offset);
            break;

        case OP_NIL:
            return simpleInstruction ("OP_NIL", offset);
            break;
        case OP_FALSE:
            return simpleInstruction ("OP_FALSE", offset);
            break;
        case OP_TRUE:
            return simpleInstruction ("OP_TRUE", offset);
            break;

        case OP_POP:
            return simpleInstruction ("OP_POP", offset);
            break;
        case OP_DEFINE_GLOBAL:
            return constantInstruction ("OP_DEFINE_GLOBAL", chunk, offset);
            break;
        case OP_GET_GLOBAL:
            return constantInstruction ("OP_GET_GLOBAL", chunk, offset);
            break;

        case OP_EQUAL:
            return simpleInstruction ("OP_EQUAL", offset);
            break;
        case OP_GREATER:
            return simpleInstruction ("OP_GREATER", offset);
            break;
        case OP_LESS:
            return simpleInstruction ("OP_LESS", offset);
            break;

        case OP_ADD:
            return simpleInstruction ("OP_ADD", offset);
            break;
        case OP_SUBTRACT:
            return simpleInstruction ("OP_SUBTRACT", offset);
            break;
        case OP_MULTIPLY:
            return simpleInstruction ("OP_MULTIPLY", offset);
            break;
        case OP_DIVIDE:
            return simpleInstruction ("OP_DIVIDE", offset);
            break;
        case OP_NOT:
            return simpleInstruction ("OP_NOT", offset);
            break;
        case OP_PRINT:
            return simpleInstruction ("OP_PRINT", offset);
            break;
        case OP_RETURN:
            return simpleInstruction ("OP_RETURN", offset);
            break;
        default:
            printf ("Unknown opocode %d\n", instruction);
            return offset + 1;
            break;
    }
}
