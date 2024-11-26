#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/ripemd.h>

#define BLOOM_FILTER_SIZE 1024 // Bloom filter size in bits
#define NUM_HASH_FUNCTIONS 3   // Number of hash functions

typedef struct {
    uint8_t bit_array[BLOOM_FILTER_SIZE / 8]; // Byte array to store bits
} BloomFilter;

// Set a bit in the bit array
void set_bit(BloomFilter *filter, uint32_t bit_index) {
    filter->bit_array[bit_index / 8] |= (1 << (bit_index % 8));
}

// Check if a bit is set in the bit array
int check_bit(const BloomFilter *filter, uint32_t bit_index) {
    return filter->bit_array[bit_index / 8] & (1 << (bit_index % 8));
}

// Generate hash indices using RIPEMD-160
void generate_hash_indices(const unsigned char *data, size_t data_len, uint32_t *indices, size_t num_indices) {
    unsigned char hash[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160(data, data_len, hash);

    // Split the hash into 32-bit integers for indices
    for (size_t i = 0; i < num_indices; i++) {
        uint32_t *hash_part = (uint32_t *)(hash + i * sizeof(uint32_t));
        indices[i] = *hash_part % BLOOM_FILTER_SIZE;
    }
}

// Add an element to the Bloom filter
void bloom_filter_add(BloomFilter *filter, const unsigned char *data, size_t data_len) {
    uint32_t indices[NUM_HASH_FUNCTIONS];
    generate_hash_indices(data, data_len, indices, NUM_HASH_FUNCTIONS);

    for (size_t i = 0; i < NUM_HASH_FUNCTIONS; i++) {
        set_bit(filter, indices[i]);
    }
}

// Check if an element might be in the Bloom filter
int bloom_filter_check(const BloomFilter *filter, const unsigned char *data, size_t data_len) {
    uint32_t indices[NUM_HASH_FUNCTIONS];
    generate_hash_indices(data, data_len, indices, NUM_HASH_FUNCTIONS);

    for (size_t i = 0; i < NUM_HASH_FUNCTIONS; i++) {
        if (!check_bit(filter, indices[i])) {
            return 0; // Definitely not in the filter
        }
    }
    return 1; // Might be in the filter
}

int main() {
    BloomFilter filter = {0}; // Initialize Bloom filter to zero

    const char *item1 = "Hello, Bloom!";
    const char *item2 = "Goodbye, Bloom!";
    
    bloom_filter_add(&filter, (unsigned char *)item1, strlen(item1));

    printf("Item 1 in filter? %d\n", bloom_filter_check(&filter, (unsigned char *)item1, strlen(item1))); // Should print 1
    printf("Item 2 in filter? %d\n", bloom_filter_check(&filter, (unsigned char *)item2, strlen(item2))); // Should print 0 or a false positive

    return 0;
}
