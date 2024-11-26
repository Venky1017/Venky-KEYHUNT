#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/ripemd.h>
#include <stdlib.h>
#include <stdbool.h>

// Define Bloom filter parameters
#define BLOOM_FILTER_SIZE 1000000 // Adjust size as needed
uint8_t bloom_filter[BLOOM_FILTER_SIZE];

// Simple hash function for Bloom Filter
uint32_t hash_function(const uint8_t *data, size_t len, uint32_t seed) {
    uint32_t hash = seed;
    for (size_t i = 0; i < len; i++) {
        hash = hash * 31 + data[i];
    }
    return hash % BLOOM_FILTER_SIZE;
}

// Add a hash160 to the Bloom filter
void add_to_bloom(const uint8_t *hash160) {
    uint32_t h1 = hash_function(hash160, 20, 0x12345678);
    uint32_t h2 = hash_function(hash160, 20, 0x87654321);
    bloom_filter[h1] = 1;
    bloom_filter[h2] = 1;
}

// Check if a hash160 exists in the Bloom filter
bool check_bloom(const uint8_t *hash160) {
    uint32_t h1 = hash_function(hash160, 20, 0x12345678);
    uint32_t h2 = hash_function(hash160, 20, 0x87654321);
    return bloom_filter[h1] && bloom_filter[h2];
}

// RIPEMD-160 hash function wrapper
void ripemd160(const uint8_t *data, size_t len, uint8_t *hash_out) {
    RIPEMD160(data, len, hash_out);
}

int main() {
    const char *message = "test message";
    uint8_t hash[20];

    // Generate RIPEMD-160 hash
    ripemd160((const uint8_t *)message, strlen(message), hash);

    printf("RIPEMD-160 Hash: ");
    for (int i = 0; i < 20; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    // Add hash to Bloom filter and verify
    add_to_bloom(hash);
    if (check_bloom(hash)) {
        printf("Hash found in Bloom filter.\n");
    } else {
        printf("Hash not found in Bloom filter.\n");
    }

    return 0;
}
