#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]){
  initVM();
  Chunk chunk;
  initChunk(&chunk);
 
  /* Insert 1.2 */
  int index = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, index, 123);
  
  /* Insert 3.4 */
  index = addConstant(&chunk, 3.4);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, index, 123);

  /* + */
  writeChunk(&chunk, OP_ADD, 123);

  /* Insert 5.6 */
  index = addConstant(&chunk, 5.6);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, index, 123);

  writeChunk(&chunk, OP_DIVIDE, 123);
  writeChunk(&chunk, OP_NEGATE, 123);
  writeChunk(&chunk, OP_RETURN, 123);
  //disassembleChunk(&chunk, "test chunk");

  /* Execute the bytecode through the VM */
  interpret(&chunk);
  freeVM();
  freeChunk(&chunk);
  return(0);
}
