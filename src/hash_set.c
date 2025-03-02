#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Include/hash_set.h"



unsigned int hash(const int value) {
    return value % HASHSET_SIZE;  
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


int contains(HashSet *set, const int value) {
    unsigned int index = hash(value);
    Node *current = set->buckets[index];
    while (current) {
        if (current->value== value) {
            return 1;  
        }
        current = current->next;
    }
    return 0;  
}
void insert(HashSet *set, const int value) {
    if (contains(set, value)) {
        return; 
    }

    unsigned int index = hash(value);
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_node->value = value;  
    new_node->next = set->buckets[index];
    set->buckets[index] = new_node;
}

void iterate_hashset(HashSet *set, void (*callback)(int)) {
    for (int i = 0; i < HASHSET_SIZE; i++) {  
        Node *current = set->buckets[i];
        while (current) {  
            callback(current->value);  
            current = current->next;
        }
    }
}


void remove_element(HashSet *set, const int value) {
    unsigned int index = hash(value);
    Node *current = set->buckets[index];
    Node *prev = NULL;

    while (current) {
        if (current->value== value) {
            if (prev) {
                prev->next = current->next;
            } else {
                set->buckets[index] = current->next;
            }
              
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
            free(temp);         
        }
    }
    free(set);
}
void print_value(int a){
    printf("%d ",a);
}