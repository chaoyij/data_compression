#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data_structure.h"


// define the hash table capacity
// 4096 * capacity factor
// in order to reduce hash collision
#define CAPACITY (SIZE_LIMIT * CAPACITY_FACTOR)


// create an empty node
Node node_create(const Key k, CodeWord cw, Node next);
// delete the node
Node node_delete(Node);


// calculate the hash value
Index calculate_index(const char* s){
    unsigned long val = 0;

    while (*s != '\0'){
        val = (val << 5) + (unsigned char) *s;
        s += 1;         
    }

    val = val % CAPACITY;
    return (Index) val;
}


// create the dictionary, insert 256 chars + 1 for EOF and 1 for reflux
// so that the 256 and 257 index are reserved
// size = 4096, current_num = 258, so next index start at 258 during insert
Dictionary dictionary_create(void){
    Dictionary d = (struct _Dictionary*) malloc (sizeof(struct _Dictionary));
    if (d == NULL){
        fprintf(stderr, "Memory error: malloc, dictionary");
        exit(EXIT_FAILURE);
    }

    d->nodes = (Node*) malloc (CAPACITY * sizeof(Node));
    if (d->nodes == NULL){
        fprintf(stderr, "Memory error: malloc nodes\n");
        exit(EXIT_FAILURE);
    }

    for (Index i = 0; i < CAPACITY; i++){
        d->nodes[i] = NULL;
    }

    // fill the hash table with initial values 0 - 255
    d->current_num = 256 + 2; // initially 256 char + 1 for EOF + 1 for reflush   

    return d;
}


// release memory of the dictionary 
Dictionary dictionary_destroy(Dictionary d){
    assert(d != NULL);

    // use recursion to free all nodes
    for (Index i = 0; i < CAPACITY; i++){
        if (d->nodes[i] != NULL){
            node_delete(d->nodes[i]);
        }
    }

    free(d->nodes);
    d->nodes = NULL;
    free(d);
    d = NULL;
    return d;
}


// reset,  = destroy + create
Dictionary dictionary_reset(Dictionary d){
    assert(d != NULL);
    d = dictionary_destroy(d);
    d = dictionary_create();
    return d;
}


// check if is full, if full, need reset
bool dictionary_is_full(Dictionary d){
    assert(d != NULL);

    // note that the capacity is a multiple of 4096
    // but when we reach 4096 items, we do the reflush !!!
    return d->current_num == SIZE_LIMIT;
}


// check if the key exist, 
// if exist return the index, if not return -1 
CodeWord dictionary_search(Dictionary d, const Key k){
    assert(d != NULL && k != NULL);

    // if it is a single char, simply return its int
    if (strlen(k) == 1){
        return (int)k[0];
    }


    Index hidx = calculate_index(k);
    if (d->nodes[hidx] == NULL){
        return -1;
    }
    else{
        // if see the exact key, then return the index
        Node n = d->nodes[hidx];
        while (n != NULL && strcmp(n->k, k) != 0){
            n = n->next;
        }

        if (n == NULL){
            return -1;
        }
        else{
            return n->cw;   // find the key, return the codeword
        }
    }
}


// insert key-cw pair into the dictionary
// if hash index position is null, insert
// if not null, find the end of the linked table, then insert
CodeWord dictionary_insert(Dictionary d, const Key k){
    assert(d != NULL && k != NULL);

    // calculate the hash index
    Index hidx = calculate_index(k);

    // assign the codeword
    CodeWord cw = d->current_num;       

    if (d->nodes[hidx] == NULL){
        d->nodes[hidx] = node_create(k, cw, NULL);
    }
    else{
        Node n = d->nodes[hidx];
        while (n->next != NULL){
            n = n->next;
        }
        n->next = node_create(k, cw, NULL);
    }

    // update the counter
    d->current_num += 1;
    return cw;
}


// debug use: print the dictionary
void dictionary_print(Dictionary d){
    assert(d != NULL);

    fprintf(stdout, "-----Dictionary print-----\n");
    fprintf(stdout, "Size = %d, current_num = %d\n", SIZE_LIMIT, d->current_num);

    for (Index i = 0; i < CAPACITY; i++){

        if (i <= 255){
            // single char range
            fprintf(stdout, "[%d] %c => %d ", i, i, i);
            if (d->nodes[i] != NULL){
                Node n = d->nodes[i];
                while (n != NULL){
                    fprintf(stdout, "%s => %d ", n->k, n->cw);
                    n = n->next;
                }
            }
            fprintf(stdout, "\n");
        }
        else if (d->nodes[i] != NULL){
            Node n = d->nodes[i];
            fprintf(stdout, "[%d]  ", i);
            while (n != NULL){
                fprintf(stdout, "%s => %d ", n->k, n->cw);
                n = n->next;
            }
            fprintf(stdout, "\n");
        }
    }

    fprintf(stdout, "-----End-----\n");
    return;
}


// create a node, initialize with NULL next pointer
Node node_create(const Key k, CodeWord cw, Node next){
    Node n = (Node) malloc (sizeof(struct _Node));
    if (n == NULL){
        fprintf(stderr, "Memory error: malloc, node\n");
        exit(EXIT_FAILURE);
    }

    // allocate memory
    n->k = (char*)malloc(strlen(k)+1); // add 1 for \0
    if (n->k == NULL){
        fprintf(stderr, "Memory error: node copy key\n");
        exit(EXIT_FAILURE);
    }
    // copy the key
    strcpy(n->k, k);

    // create the index and next pointer
    n->cw = cw;
    n->next = next;

    return n;
}


// recursive to delete all nodes of the dictionary
Node node_delete(Node n){
    if (n == NULL){
        return n;
    }
    else{
        node_delete(n->next);
        free(n->k);
        free(n);
        n = NULL;
        return n;
    }
}


// array functions
// create
// 0-255 leaves empty, also reserve 256 for EOF and 257 for Reflush
Array array_create(void){

    Array a = (struct _Array*) malloc (sizeof(struct _Array));
    if (a == NULL){
        fprintf(stderr, "Memory error: array create\n");
        exit(EXIT_FAILURE);
    }

    a->current_num = 256 + 2;

    // here we only need 4096 items
    // since here we do not perform hash, so no hash collision
    a->nodes = (char**) malloc (SIZE_LIMIT * sizeof(char*));
    if (a->nodes == NULL){
        fprintf(stderr, "Memory error: array create nodes\n");
        exit(EXIT_FAILURE);
    }

    for (Index i = 0; i < SIZE_LIMIT; i++){
        a->nodes[i] = NULL;
    }

    return a;
}


// clean the array
Array array_destroy(Array a){
    assert(a != NULL);

    for (Index i = 0; i < SIZE_LIMIT; i++){
        if (a->nodes[i] != NULL){
            free(a->nodes[i]);
            a->nodes[i] = NULL;
        }
    }

    free(a->nodes);
    puts("3");
    free(a);
    puts("4");
    a = NULL;
    return a;
}


// reset = destroy + create
Array array_reset(Array a){
    assert(a != NULL);
    puts("1");
    a = array_destroy(a);
    puts("2");
    a = array_create();
    return a;
}


// check if full
bool array_is_full(const Array a){
    assert(a != NULL);
    return a->current_num == SIZE_LIMIT;
}


// search using index, return the string
// Key prev is included to avoid the unseen mistake
// that during compression, the key is created then used straight away
// without any time gap.
// Also need to take care with the return key
// if simply print it, it is fine
// but if need to modify, make sure use strcpy first
// do not change on the returned pointer directly
Key array_search(Array a, const Index idx, const Key prev){
    assert(a != NULL && idx >= 0);

    // if the index <= 255, create the key and return
    // since the bucket is actually NULL
    if (idx <= 255){
        Key ch = (Key) malloc (2 * sizeof(char));
        if (ch == NULL){
            fprintf(stderr, "Memory error: array search\n");
            exit(EXIT_FAILURE);
        }
        ch[0] = idx;
        ch[1] = '\0';
        return ch;
    }
    else if (a->nodes[idx] != NULL){
        return a->nodes[idx];
    }
    else{
        // !!! the only exception for LZW

        // the idx bucket is empty
        // use prev, concat with the prefix (first letter) 
        // of "prev" string, 
        // and add it to the array
        Key prev_copy = (Key) malloc (strlen(prev)+2);
        if (prev_copy == NULL){
            fprintf(stderr, "Memory error: array search, prev_copy\n");
            exit(EXIT_FAILURE);
        }

        // copy the prev, and copy the first letter of prev again
        strcpy(prev_copy, prev);
        strncat(prev_copy, prev, 1);

        // also insert into the array
        array_insert(a, prev_copy);
        return prev_copy;
    }
}


// insert, no codeword required, simply insert in first in order
void array_insert(Array a, const Key k){
    assert(a != NULL && k != NULL);

    a->nodes[a->current_num] = (char*) malloc (strlen(k)+1);
    if (a->nodes[a->current_num] == NULL){
        fprintf(stderr, "Memory error: array insert\n");
        exit(EXIT_FAILURE);
    }

    strcpy(a->nodes[a->current_num], k);
    a->current_num += 1;    // update the counter
    return;
}


// debug use
void array_print(const Array a){
    assert(a != NULL);

    fprintf(stdout, "-----Array print-----\n");
    fprintf(stdout, "Size = %d, current_num = %d\n", SIZE_LIMIT, a->current_num);

    for (Index i = 0; i < SIZE_LIMIT; i++){
        if (i <= 255){
            fprintf(stdout, "%d => %c\n", i, i);
        }
        else if (a->nodes[i] != NULL){
            fprintf(stdout, "%d => %s\n", i, a->nodes[i]);
        }
    }

    fprintf(stdout, "-----End-----\n");
    return;
}