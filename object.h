#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

/* Macro for extacting object type */
#define OBJ_TYPE(value)   (AS_OBJ(value)->type)

/* Macro for detecting object casting is safe */
#define IS_STRING(value)  isObjType(value, OBJ_STRING)

#define AS_STRING(value)  ((ObjString*) AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*) AS_OBJ(value))->chars)

typedef enum{
  OBJ_STRING,
} ObjType; 


struct Obj{
  ObjType type;
  struct Obj* next;
};

/* Structure inheritence!? 
 * Note: the first bytes of 'ObjString' exactly line up
 * with 'Obj' according to C language specification. Thus, 
 * in the OOP sense 'ObjString' is an 'Obj'. Additionally,
 * 'Obj' is an 'ObjString'. However, casting from 'Obj' to 
 * 'ObjString' pointers requires extra caution. */
struct ObjString{
  Obj obj;
  int length;
  char* chars; 
};

ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);
void printObject(Value value);

static inline bool isObjType(Value value, ObjType type){
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
