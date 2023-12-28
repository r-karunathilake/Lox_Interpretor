#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "table.h"
#include "value.h"

#define STACK_MAX 256

typedef struct{
  Chunk* chunk;
  /* Instruction pointer for VM bytecode 
   * execution location tracking */
  uint8_t* ip; 
  Value stack[STACK_MAX];
  Value* stackTop; 
  Table globals; // Hash table to store global variables
  Table strings; // Hash table for string interning 
  Obj* objects;  // Linked list head to every 'Obj' created 
}VM;

typedef enum{
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
}InterpretResult;

/* Expose the VM externally for updating 
 * linked list head pointer 'objects' */
extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();

#endif
