#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Include/hash_set.h"



unsigned int hash(const char *str) {
    const unsigned int FNV_prime = 16777619;  
    const unsigned int offset_basis = 2166136261U;  
    unsigned int hash = offset_basis;

    while (*str) {
        hash ^= (unsigned char)(*str++);  
        hash *= FNV_prime;             
    }

    return hash % HASHSET_SIZE;  
}




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


int contains(HashSet *set, const char *value) {
    unsigned int index = hash(value);
    Node *current = set->buckets[index];
    while (current) {
        if (strcmp(current->value, value) == 0) {
            return 1;  
        }
        current = current->next;
    }
    return 0;  
}
void insert(HashSet *set, const char *value) {
    if (contains(set, value)) {
        return; 
    }

    unsigned int index = hash(value);
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_node->value = strdup(value);  
    new_node->next = set->buckets[index];
    set->buckets[index] = new_node;
}

void iterate_hashset(HashSet *set, void (*callback)(const char *)) {
    for (int i = 0; i < HASHSET_SIZE; i++) {  
        Node *current = set->buckets[i];
        while (current) {  
            callback(current->value);  
            current = current->next;
        }
    }
}


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
            free(current->value);  
            free(current);         
            return;
        }
        prev = current;
        current = current->next;
    }
}

void free_hashset(HashSet *set) {
    for (int i = 0; i < HASHSET_SIZE; i++) {
        Node *current = set->buckets[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp->value);  
            free(temp);         
        }
    }
    free(set);
}
void print_string(const char *str) {
    printf("%s ", str);
}