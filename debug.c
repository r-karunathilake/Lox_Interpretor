#include <stdio.h>

#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name){
  printf("== %s ==\n", name);

  for(int offset = 0; offset < chunk->count;){
    offset = disassembleInstruction(chunk, offset);
   }
}

static int constantInstruction(const char* name, Chunk* chunk, int offset){
  /* 'offset' here is the opcode then index where the constant value is stores in the constants dynamic array */
  uint8_t index = chunk->code[offset + 1];
  printf("%-16s %4d '", name, index);
  printValue(chunk->constants.values[index]);
  printf("'\n");

  /* opcode 'OP_CONSTANT' is two bytes: one for opcode and one for operand */ 
  return offset + 2; // next instruction offset 
}

static int simpleInstruction(const char* name, int offset){
  printf("%s\n", name);
  return offset + 1; // next instruction offset 
}

int disassembleInstruction(Chunk* chunk, int offset){
  printf("%04d ", offset);

  /* A single line of source code often can compile to a whole sequence 
   * of bytecode instructions, thus, the output is 
   * segmented by a '|' character */
  if(offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]){
    printf("   | ");
  }else{
    printf("%4d ", chunk->lines[offset]);
  }

  /* Operation code in this chunk */
  uint8_t instruction = chunk->code[offset]; 
  switch(instruction){
    case OP_CONSTANT:
      return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);

    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1; 
  }
}
