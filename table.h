#ifndef clox_table_h
#define clox_table_h

#include "common.h"
#include "value.h"

/* Each entry in the hash table is a key-value pair */
typedef struct {
  /* Key is always a string */
  ObjString* key;
  Value value;
}Entry;

typedef struct{
  int count;
  int capacity;
  /* The hash table is a dynamic array of entries */
  Entry* entries; 
}Table;

void initTable(Table* table);
void freeTable(Table* table);
bool tableSet(Table* table, ObjString* key, Value value);
bool tableDelete(Table* table, ObjString* key);
void tableAddAll(Table* from, Table* to);
ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash);
bool tableGet(Table* table, ObjString* key, Value* value);

#endif 
