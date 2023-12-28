#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

/* Operation code */ 
typedef enum{
  OP_CONSTANT,
  OP_NIL,
  OP_TRUE,
  OP_FALSE,
  OP_POP,
  OP_GET_GLOBAL,
  OP_DEFINE_GLOBAL,
  OP_SET_GLOBAL,
  OP_EQUAL,
  OP_GREATER,
  OP_LESS,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NOT,
  OP_NEGATE,
  OP_PRINT,
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
