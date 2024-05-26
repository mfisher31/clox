#pragma once

#include "common.h"
#include "value.h"

typedef struct {
    ObjString* key;
    Value value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry* entries;
} Table;

void initTable (Table* table);
void freeTable (Table* table);
void tableRemoveWhite (Table* table);
void markTable (Table* table);

bool tableGet (Table* table, ObjString* key, Value* value);
/** Returns true if the key was newly added. */
bool tableSet (Table* table, ObjString* key, Value value);
bool tableDelete (Table* table, ObjString* key);

void tableAddAll (Table* from, Table* to);
ObjString* tableFindString (Table* table, const char* chars, int length, uint32_t hash);