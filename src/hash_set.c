#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Include/hash_set.h"



unsigned int hash(const int value) {
    // const unsigned int FNV_prime = 16777619;  
    // const unsigned int offset_basis = 2166136261U;  
    // unsigned int hash = offset_basis;

    // while (*str) {
    //     hash ^= (unsigned char)(*str++);  
    //     hash *= FNV_prime;             
    // }

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


void remove_string(HashSet *set, const int value) {
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

int main(){
    HashSet *set = create_hashset();

    // Test: Inserting values
    insert(set, 10);
    insert(set, 20);
    insert(set, 30);
    insert(set, 10);  // Duplicate
    insert(set, 40);

    printf("HashSet contains: ");
    iterate_hashset(set, print_value);
    printf("\n");

    // Test: Contains function
    printf("Contains 20: %s\n", contains(set, 20) ? "Yes" : "No");
    printf("Contains 50: %s\n", contains(set, 50) ? "Yes" : "No");

    // Test: Removing values
    remove_string(set, 20);
    printf("After removing 20: ");
    iterate_hashset(set, print_value);
    printf("\n");

    remove_string(set, 50);  // Removing non-existent value
    printf("After trying to remove 50 (non-existent): ");
    iterate_hashset(set, print_value);
    printf("\n");
    printf("vishal\n");
    // Test: Edge cases
    // insert(set, -1);  // Negative value
    insert(set, 0);   // Zero
    printf("After inserting 0: ");
    iterate_hashset(set, print_value);
    printf("\n");

    free_hashset(set);
}
