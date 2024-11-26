#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Hash table entry for storing baby steps
typedef struct HashNode {
    int key;         // Modular value
    int value;       // Exponent
    struct HashNode* next; // For collision handling
} HashNode;

// Simple hash table for baby-step storage
#define TABLE_SIZE 10007  // Prime for hash table size
HashNode* hashTable[TABLE_SIZE] = {NULL};

// Function to compute (base^exp) % mod
long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) { // If exp is odd
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// Hash function
int hash(int key) {
    return abs(key) % TABLE_SIZE;
}

// Insert into the hash table
void hash_insert(int key, int value) {
    int idx = hash(key);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = key;
    newNode->value = value;
    newNode->next = hashTable[idx];
    hashTable[idx] = newNode;
}

// Search in the hash table
int hash_search(int key) {
    int idx = hash(key);
    HashNode* node = hashTable[idx];
    while (node != NULL) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    return -1; // Not found
}

// Clear the hash table
void clear_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = hashTable[i];
        while (node != NULL) {
            HashNode* temp = node;
            node = node->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
}

// BSGS algorithm
int bsgs(int g, int h, int p) {
    int m = (int)ceil(sqrt(p)); // Step size

    // Baby-step: compute g^j % p and store in hash table
    long long g_power = 1;
    for (int j = 0; j < m; j++) {
        hash_insert((int)g_power, j);
        g_power = (g_power * g) % p;
    }

    // Giant-step: compute h * (g^(-m))^i % p and check in hash table
    long long g_inv_m = mod_exp(g, p - 1 - m, p); // g^(-m) mod p
    long long giant_step = h;

    for (int i = 0; i < m; i++) {
        int j = hash_search((int)giant_step);
        if (j != -1) {
            clear_table();
            return i * m + j; // Solution found
        }
        giant_step = (giant_step * g_inv_m) % p;
    }

    clear_table();
    return -1; // No solution
}

int main() {
    int g, h, p;
    printf("Enter values for g, h, and p (prime): ");
    scanf("%d %d %d", &g, &h, &p);

    int result = bsgs(g, h, p);
    if (result != -1) {
        printf("Solution: x = %d\n", result);
    } else {
        printf("No solution found.\n");
    }

    return 0;
}
