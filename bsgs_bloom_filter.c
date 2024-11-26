#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ripemd.h> // For RIPEMD-160
#include <stdint.h>

// Bloom filter size and hash functions count
#define BLOOM_FILTER_SIZE 10000
#define BLOOM_HASH_COUNT 3

// Bloom filter structure
typedef struct {
    uint8_t bits[BLOOM_FILTER_SIZE];
} BloomFilter;

// Function to add an element to the Bloom filter
void bloom_add(BloomFilter *bf, const uint8_t *data, size_t len) {
    for (int i = 0; i < BLOOM_HASH_COUNT; i++) {
        uint32_t hash = RIPEMD160(data, len, NULL)[i] % BLOOM_FILTER_SIZE;
        bf->bits[hash] = 1;
    }
}

// Function to check if an element might exist in the Bloom filter
int bloom_check(const BloomFilter *bf, const uint8_t *data, size_t len) {
    for (int i = 0; i < BLOOM_HASH_COUNT; i++) {
        uint32_t hash = RIPEMD160(data, len, NULL)[i] % BLOOM_FILTER_SIZE;
        if (!bf->bits[hash]) {
            return 0; // Definitely not in the Bloom filter
        }
    }
    return 1; // Might be in the Bloom filter
}

// Main function
int main() {
    BloomFilter bf = {0};

    // Example data
    const char *input1 = "example1";
    const char *input2 = "example2";

    // Add to Bloom filter
    bloom_add(&bf, (const uint8_t *)input1, strlen(input1));

    // Check elements
    printf("Checking input1: %s\n", bloom_check(&bf, (const uint8_t *)input1, strlen(input1)) ? "Maybe" : "No");
    printf("Checking input2: %s\n", bloom_check(&bf, (const uint8_t *)input2, strlen(input2)) ? "Maybe" : "No");

    return 0;
}
