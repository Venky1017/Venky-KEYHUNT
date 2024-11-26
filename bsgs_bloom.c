#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BLOOM_SIZE 1000003 // Size of Bloom Filter (a prime number)
#define NUM_HASHES 3       // Number of hash functions

typedef struct {
    unsigned char *bits; // Bit array for the filter
    size_t size;         // Size of the filter
} BloomFilter;

// Hash functions for Bloom Filter
unsigned int hash1(int value, int size) {
    return (value % size);
}

unsigned int hash2(int value, int size) {
    return ((value / size) % size);
}

unsigned int hash3(int value, int size) {
    return ((value / 13) % size);
}

// Initialize the Bloom Filter
BloomFilter* create_bloom_filter(size_t size) {
    BloomFilter *filter = (BloomFilter *)malloc(sizeof(BloomFilter));
    filter->bits = (unsigned char *)calloc(size, sizeof(unsigned char));
    filter->size = size;
    return filter;
}

// Add an element to the Bloom Filter
void bloom_add(BloomFilter *filter, int value) {
    unsigned int h1 = hash1(value, filter->size);
    unsigned int h2 = hash2(value, filter->size);
    unsigned int h3 = hash3(value, filter->size);

    filter->bits[h1] = 1;
    filter->bits[h2] = 1;
    filter->bits[h3] = 1;
}

// Check if an element is in the Bloom Filter
int bloom_check(BloomFilter *filter, int value) {
    unsigned int h1 = hash1(value, filter->size);
    unsigned int h2 = hash2(value, filter->size);
    unsigned int h3 = hash3(value, filter->size);

    return filter->bits[h1] && filter->bits[h2] && filter->bits[h3];
}

// Free the Bloom Filter
void free_bloom_filter(BloomFilter *filter) {
    free(filter->bits);
    free(filter);
}

// Modular exponentiation: (base^exp) % mod
int mod_exp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Baby-Step Giant-Step Algorithm using Bloom Filter
int bsgs_with_bloom(int g, int h, int p) {
    int m = (int)ceil(sqrt(p));
    BloomFilter *filter = create_bloom_filter(BLOOM_SIZE);

    // Baby step: Add g^j (mod p) to the Bloom Filter
    for (int j = 0; j < m; ++j) {
        int value = mod_exp(g, j, p);
        bloom_add(filter, value);
    }

    // Giant step: Check if h * g^(-im) (mod p) exists in the Bloom Filter
    int g_inv_m = mod_exp(g, p - 1 - m, p); // g^(-m) mod p
    int current = h;

    for (int i = 0; i < m; ++i) {
        if (bloom_check(filter, current)) {
            // Check all baby steps to find the exact match
            for (int j = 0; j < m; ++j) {
                if (mod_exp(g, j, p) == current) {
                    free_bloom_filter(filter);
                    return i * m + j; // Found the solution
                }
            }
        }
        current = (current * g_inv_m) % p;
    }

    free_bloom_filter(filter);
    return -1; // No solution found
}

int main() {
    int g = 5;  // Generator
    int h = 8;  // Target
    int p = 23; // Prime modulus

    int x = bsgs_with_bloom(g, h, p);
    if (x != -1) {
        printf("Solution: x = %d\n", x);
    } else {
        printf("No solution found.\n");
    }

    return 0;
}
