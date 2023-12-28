#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) (type*) allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type){
  Obj* object = (Obj*) reallocate(NULL, 0, size);
  object->type = type;

  /* Updated the VM linked list object tracker */
  object->next = vm.objects;
  vm.objects = object;
  return object;
}

static ObjString* allocateString(char* chars, int length, uint32_t hash){
  ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
  string->length = length;
  string->chars = chars;
  string->hash = hash;
  tableSet(&vm.strings, string, NIL_VAL); // Intern the new string into a hash set (NIL_VAL) 
  return string;
}

static uint32_t hashString(const char* key, int length){
  /* C implementation of the 'FNV-1a' has function. */
  uint32_t hash = 2166136261u; // Initial has value
  for(int i = 0; i < length; i++){
    hash ^= (uint8_t) key[i]; // Bit mixing 
    hash *= 16777619; // Scramble bit mix 
  }

  return hash;
}

ObjString* takeString(char* chars, int length){
  uint32_t hash = hashString(chars, length);
  ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
  if(interned != NULL){
    FREE_ARRAY(char, chars, length + 1);
    return interned; 
  }
  return allocateString(chars, length, hash);
}

ObjString* copyString(const char* chars, int length){
  uint32_t hash = hashString(chars, length);
  ObjString* interned = tableFindString(&vm.strings, chars, length, hash);

  if(interned != NULL){
    return interned;
  }

  char* heapChars = ALLOCATE(char, length + 1);
  memcpy(heapChars, chars, length);
  heapChars[length] = '\0';
  return allocateString(heapChars, length, hash);
}

void printObject(Value value){
  switch(OBJ_TYPE(value)){
    case OBJ_STRING:{
      printf("%s", AS_CSTRING(value));
    }break;
  }
}
