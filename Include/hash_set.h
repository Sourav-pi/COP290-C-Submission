#ifndef HASH_SET_H
#define HASH_SET_H

#define HASHSET_SIZE 1 // Number of buckets
// Node structure for linked list
typedef struct Node {
    int value;
    struct Node *next;
} Node;

// HashSet structure
typedef struct HashSet {
    Node *buckets[HASHSET_SIZE];  // Array of linked lists
} HashSet;


HashSet* create_hashset(void);

// Check if a string exists in the HashSet
int contains(HashSet *, int );

// Insert a string into the HashSet
void insert(HashSet *, const int) ;

void iterate_hashset(HashSet *, void (*callback)(int)) ;


// Remove a string from the HashSet
void remove_string(HashSet *, const int);

// Free all memory used by the HashSet
void free_hashset(HashSet *) ;

#endif 