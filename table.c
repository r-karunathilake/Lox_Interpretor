#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

/* Loading factor for the hash table */
#define TABLE_MAX_LOAD 0.75

void initTable(Table* table){
  table->count = 0;
  table->capacity = 0;
  table->entries = NULL;
}

void freeTable(Table* table){
  FREE_ARRAY(Entry, table->entries, table->capacity);
  initTable(table); // Leave the memory in a known state
}

static Entry* findEntry(Entry* entries, int capacity, ObjString* key){
  /* Bind the key hash code to a valid array index */
  uint32_t index = key->hash % capacity;
  Entry* tombstone = NULL;
  /* WARNING: implicit assumption of eventually finding empty hash-table bucket 
   * to prevent infinite loop based on strict table loading factor enforcement in 
   * 'tableSet()' function */
  for(;;){
    Entry* entry = &entries[index];
    if(entry->key == NULL){ // could be a tombstone
      if(IS_NIL(entry->value)){ // NOT a tombstone since 'entry->value == BOOL_VAL(true)' 
        /* Return the tombstone as the new entry if 
         * NOT NULL. Otherwise, return the entry. This makes
         * sure insertion operations can fill the tombstone index
         * instead of adding to the end of the probe sequence */ 
        return tombstone != NULL ? tombstone : entry; 
      }
      else{
        /* Found a tombstone */
        if(tombstone == NULL){
          /* Keep track of the tombstone and continue 
           * the hash table probe sequence */  
          tombstone = entry; 
        }
      }
    }
    else if(entry->key == key){
      /* We fond the key */
      return entry;
    }

    /* Linear probing with automatic wrapping to the beginning */
    index = (index + 1) % capacity; 
  }
}

bool tableGet(Table* table, ObjString* key, Value* value){
  if(table->count == 0){
    return false;
  }

  Entry* entry = findEntry(table->entries, table->capacity, key);
  if(entry->key == NULL){
    return false; 
  }

  *value = entry->value;
  return true;
}

static void adjustCapacity(Table* table, int capacity){
  Entry* entries = ALLOCATE(Entry, capacity);
  for(int i = 0; i < capacity; i++){
    entries[i].key = NULL;
    entries[i].value = NIL_VAL; 
  }

  table->count = 0; // Ensures tombstone entires are not counted  
  /* Re-enter previous key-value pairs to the new has table*/
  for(int i = 0; i < table->capacity; i++){
    Entry* entry = &table->entries[i];
    if(entry->key == NULL){
      continue;
    }

    Entry* dest = findEntry(entries, capacity, entry->key);
    dest->key = entry->key;
    dest->value = entry->value;
    table->count++;
  }

  /* Release memory for old array */
  FREE_ARRAY(Entry, table->entries, table->capacity); 

  table->entries = entries;
  table->capacity = capacity; 
}

bool tableSet(Table* table, ObjString* key, Value value){
  if(table->count + 1 > table->capacity * TABLE_MAX_LOAD){
    int capacity = GROW_CAPACITY(table->capacity);
    adjustCapacity(table, capacity);
  }

  Entry* entry = findEntry(table->entries, table->capacity, key);
  bool isNewKey = entry->key == NULL;
  /* Make sure to increment hash table 'count' ONLY if 
   * a new key-value pair is created. */
  if(isNewKey && IS_NIL(entry->value)){ // second condition makes sure tombstone buckets are not counted as new entries
    table->count++;
  }

  entry->key = key;
  entry->value = value;

  return isNewKey; 
}

void tableAddAll(Table* from, Table* to){
  for(int i = 0; i < from->capacity; i++){
    Entry* entry = &from->entries[i];
    if(entry->key != NULL){
      tableSet(to, entry->key, entry->value);
    }
  }
}

ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash){
  if(table->count == 0){
    return NULL;
  }

  uint32_t index = hash % table->capacity;
  for(;;){
    Entry* entry = &table->entries[index];
    if(entry->key == NULL){
      if(IS_NIL(entry->value)){
        /* Found an empty non-tombstone entry */
        return NULL;
      }
      else if(entry->key->length == length && entry->key->hash == hash 
              && memcmp(entry->key->chars, chars, length) == 0){
        return entry->key; 
      }
    }
    index = (index + 1) % table->capacity;
  }
}

bool tableDelete(Table* table, ObjString* key){
  if(table->count == 0){
    return false;
  }

  // Find the entry
  Entry* entry = findEntry(table->entries, table->capacity, key);
  if(entry->key == NULL){
    return false;
  }

  // Place a tombstone in the entry 
  entry->key = NULL;
  entry->value = BOOL_VAL(true);
  return true; 
}
