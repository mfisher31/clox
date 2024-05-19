
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void repl() {
    char line[1024];
    for (;;) {
        printf ("> ");
        if (! fgets (line, sizeof (line), stdin)) {
            printf ("\n");
            break;
        }
        interpret (line);
    }
}

static char* readFile (const char* path) {
    FILE* fp = fopen (path, "rb");
    if (fp == NULL) {
        fprintf (stderr, "Could not open file \"%s\".\n", path);
        exit (74);
    }

    fseek (fp, 0L, SEEK_END);
    size_t fileSize = ftell (fp);
    rewind (fp);

    char* buffer = (char*) malloc (fileSize + 1);
    if (buffer == NULL) {
        fprintf (stderr, "Could not allocate memory for \"%s\".\n", path);
        exit (74);
    }

    const size_t bytesRead = fread (buffer, sizeof (char), fileSize, fp);
    if (bytesRead < fileSize) {
        fprintf (stderr, "Could not read file \"%s\".\n", path);
        exit (74);
    }

    buffer[bytesRead] = '\0';

    fclose (fp);
    return buffer;
}

static void runFile (const char* path) {
    char* source           = readFile (path);
    InterpretResult result = interpret (source);
    free (source);
    if (result == INTERPRET_COMPILE_ERROR)
        exit (65);
    if (result == INTERPRET_RUNTIME_ERROR)
        exit (70);
}

int main (int argc, const char** argv) {
    initVM();

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile (argv[1]);
    } else {
        fprintf (stderr, "Usage clox [path]\n");
    }

    freeVM();
    return 0;
}
