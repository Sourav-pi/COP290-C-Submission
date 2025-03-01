#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Include/hash_set.h"



unsigned int hash(const char *str) {
    const unsigned int FNV_prime = 16777619;  // FNV prime constant
    const unsigned int offset_basis = 2166136261U;  // FNV offset basis
    unsigned int hash = offset_basis;

    while (*str) {
        hash ^= (unsigned char)(*str++);  // XOR with the current character
        hash *= FNV_prime;              // Multiply by the prime constant
    }

    return hash % HASHSET_SIZE;  // Map hash to a bucket index
}



// Initialize the HashSet
HashSet* create_hashset() {
    HashSet *set = (HashSet *)malloc(sizeof(HashSet));
    if (!set) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < HASHSET_SIZE; i++) {
        set->buckets[i] = NULL;
    }
    return set;
}

// Check if a string exists in the HashSet
int contains(HashSet *set, const char *value) {
    unsigned int index = hash(value);
    Node *current = set->buckets[index];
    while (current) {
        if (strcmp(current->value, value) == 0) {
            return 1;  // Found
        }
        current = current->next;
    }
    return 0;  // Not found
}

// Insert a string into the HashSet
void insert(HashSet *set, const char *value) {
    if (contains(set, value)) {
        return;  // Already exists, no duplicates allowed
    }

    unsigned int index = hash(value);
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_node->value = strdup(value);  // Duplicate the string
    new_node->next = set->buckets[index];
    set->buckets[index] = new_node;
}

void iterate_hashset(HashSet *set, void (*callback)(const char *)) {
    for (int i = 0; i < HASHSET_SIZE; i++) {  // Loop through all buckets
        Node *current = set->buckets[i];
        while (current) {  // Traverse the linked list in each bucket
            callback(current->value);  // Call the callback function with the string
            current = current->next;
        }
    }
}


// Remove a string from the HashSet
void remove_string(HashSet *set, const char *value) {
    unsigned int index = hash(value);
    Node *current = set->buckets[index];
    Node *prev = NULL;

    while (current) {
        if (strcmp(current->value, value) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                set->buckets[index] = current->next;
            }
            free(current->value);  // Free the duplicated string
            free(current);         // Free the node itself
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Free all memory used by the HashSet
void free_hashset(HashSet *set) {
    for (int i = 0; i < HASHSET_SIZE; i++) {
        Node *current = set->buckets[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp->value);  // Free the duplicated string
            free(temp);         // Free the node itself
        }
    }
    free(set);
}
void print_string(const char *str) {
    printf("%s ", str);
}