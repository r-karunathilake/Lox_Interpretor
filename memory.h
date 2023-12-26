#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"
#include "object.h"

#define ALLOCATE(type, count) (type*) reallocate(NULL, 0, sizeof(type) * count)
#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

/* Scale the dynamic byte code array by factor of 2 */
#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity * 2))

#define GROW_ARRAY(type, pointer, oldCapacity, newCapacity) \
        (type*) reallocate(pointer, sizeof(type) * (oldCapacity), sizeof(type) * (newCapacity))

#define FREE_ARRAY(type, pointer, capacity) \
        reallocate(pointer, sizeof(type) * (capacity), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);
void freeObjects();

#endif
