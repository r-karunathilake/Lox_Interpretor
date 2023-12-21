#include <stdlib.h>
#include <stdio.h>

#include "memory.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize){
  if(newSize == 0){
    free(pointer);
    return NULL;
  }

  void* result = realloc(pointer, newSize);
  /* Was the memory allocation successful? */
  if(result == NULL){
    puts("Error: unable to allocate memory!");
    exit(1);
  }

  return result; 
}
