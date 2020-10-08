#ifndef _PQ_H_
#define _PQ_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"

#define SIZE_FACTOR 2
#define ASCII_SIZE 256

struct _PriorityQueue{
    int size;
    int count;
    TreeNode *queue;
};

typedef struct _PriorityQueue *PriorityQueue;


PriorityQueue PriorityQueueCreate(void);
PriorityQueue PriorityQueueDestroy(PriorityQueue);

// double the size every time
void PriorityQueueIncreaseSize(PriorityQueue);

int GetSize(PriorityQueue);
int GetCount(PriorityQueue);

bool IsPriorityQueueValid(PriorityQueue);
bool IsPriorityQueueFull(PriorityQueue);

void IncreaseCount(PriorityQueue);
void DecreaseCount(PriorityQueue);

void PriorityQueueInsertTreeNode(PriorityQueue, TreeNode);
TreeNode PriorityQueueGetTreeNode(PriorityQueue);



#endif 