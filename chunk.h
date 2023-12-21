#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

/* Operation code */ 
typedef enum{
  OP_CONSTANT,
  OP_RETURN,
} OpCode; 

/* Dynamic array to hold bytecode 
 * instructions */
typedef struct {
  int count;    // Number of allocated entries 
  int capacity; // Total number of entries 
  uint8_t* code;
  int* lines;   // Code line numbers
  ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk, Value value);

#endif
