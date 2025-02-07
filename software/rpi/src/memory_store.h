#ifndef OPENDRO_SOFTWARE_RPI_MEMORY_H
#define OPENDRO_SOFTWARE_RPI_MEMORY_H

#define NUM_MEMORY_SLOTS 32

#include <stdint.h>
#include <stdbool.h>

typedef struct memory_vec_t {
	float x, y, z;
} memory_vec_t;

typedef struct memory_store_t {
	// bitfield, 0 for not used, 1 for in use
	uint32_t memory_slot_usage[NUM_MEMORY_SLOTS / 32];
	// bit field, 0 for scalar, 1 for vector
	uint32_t memory_slot_type[NUM_MEMORY_SLOTS / 32];
	memory_vec_t memory_slots[NUM_MEMORY_SLOTS];
} memory_store_t;

void init_memory_store(memory_store_t *store);
bool memory_slot_in_use(memory_store_t *store, uint32_t slot);
void memory_slot_free(memory_store_t *store, uint32_t slot);
bool memory_slot_is_vector(memory_store_t *store, uint32_t slot);
void memory_slot_read_vector(memory_store_t *store, memory_vec_t* vector, uint32_t slot);
void memory_slot_write_vector(memory_store_t *store, uint32_t slot, const memory_vec_t* vector);
void memory_slot_read_scalar(memory_store_t *store, float* scalar, uint32_t slot);
void memory_slot_write_scalar(memory_store_t *store, uint32_t slot, float scalar);

#endif // OPENDRO_SOFTWARE_RPI_MEMORY_H
