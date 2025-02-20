#ifndef HASH_SET_H
#define HASH_SET_H

#define HASHSET_SIZE 10 // Number of buckets
// Node structure for linked list
typedef struct Node {
    char *value;
    struct Node *next;
} Node;

// HashSet structure
typedef struct HashSet {
    Node *buckets[HASHSET_SIZE];  // Array of linked lists
} HashSet;


HashSet* create_hashset(void);

// Check if a string exists in the HashSet
int contains(HashSet *, const char *);

// Insert a string into the HashSet
void insert(HashSet *, const char *) ;

void iterate_hashset(HashSet *, void (*callback)(const char *)) ;


// Remove a string from the HashSet
void remove_string(HashSet *, const char *);

// Free all memory used by the HashSet
void free_hashset(HashSet *) ;

#endif 