#include "memory_store.h"

void init_memory_store(memory_store_t *store) {
	for (uint32_t i = 0; i < NUM_MEMORY_SLOTS / 32; ++i) {
		store->memory_slot_type[i] = 0;
		store->memory_slot_usage[i] = 0;
	}

	memory_vec_t zero_vec = {.x = 0.f, .y = 0.f, .z = 0.f};
	for (uint32_t i = 0; i < NUM_MEMORY_SLOTS; ++i) {
		store->memory_slots[i] = zero_vec;
	}
}

bool memory_slot_in_use(memory_store_t *store, uint32_t slot) {
	if (slot > NUM_MEMORY_SLOTS) {
		return false;
	}

	return (store->memory_slot_usage[slot >> 5] >> (slot & 0x31)) & 0x1;
}

void memory_slot_free(memory_store_t *store, uint32_t slot) {
	if (slot > NUM_MEMORY_SLOTS) {
		return;
	}

	store->memory_slot_usage[slot >> 5] &= ~(1 << (slot & 0x31));
}

bool memory_slot_is_vector(memory_store_t *store, uint32_t slot) {
	if (slot > NUM_MEMORY_SLOTS) {
		return false;
	}

	return (store->memory_slot_type[slot >> 5] >> (slot & 0x31)) & 0x1;
}

void memory_slot_read_vector(memory_store_t *store, memory_vec_t* vector, uint32_t slot) {
	if (slot > NUM_MEMORY_SLOTS) {
		return;
	}

	*vector = store->memory_slots[slot];
}

void memory_slot_write_vector(memory_store_t *store, uint32_t slot, const memory_vec_t* vector) {
	if (slot > NUM_MEMORY_SLOTS) {
		return;
	}

	store->memory_slot_usage[slot >> 5] |= (1 << (slot & 0x31));
	store->memory_slot_type[slot >> 5] |= (1 << (slot & 0x31));
	store->memory_slots[slot] = *vector;
}

void memory_slot_read_scalar(memory_store_t *store, float* scalar, uint32_t slot) {
	if (slot > NUM_MEMORY_SLOTS) {
		return;
	}

	*scalar = store->memory_slots[slot].x;
}

void memory_slot_write_scalar(memory_store_t *store, uint32_t slot, float scalar) {
	if (slot > NUM_MEMORY_SLOTS) {
		return;
	}

	store->memory_slot_usage[slot >> 5] |= (1 << (slot & 0x31));
	store->memory_slot_type[slot >> 5] &= ~(1 << (slot & 0x31));
	store->memory_slots[slot].x = scalar;
}
