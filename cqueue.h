/*
 * =====================================================================================
 *
 *       Filename:  cqueue.h
 *
 *    Description:  Implementation of a simple FIFO Queue structure.
 *    							It is implemented using a plain array that it is
 *    							dynamically expanded if needed.
 *
 *        Version:  1.0
 *        Created:  17/01/2017 18:23:58
 *       Revision:  0.1
 *       Compiler:  gcc
 *
 *         Author:  Samuel Rodriguez Bernabeu (samuel.rodriguez@bsc.es) 
 *   Organization:  Barcelona Supercomputing Center
 *
 * =====================================================================================
 */
#pragma once

#define CQUEUE_DEBUG_INFO       1
#define CQUEUE_DEFAULT_SIZE     3
#define CQUEUE_MAX_PRINT_SIZE  20
#define CQUEUE_INCREASE_FACTOR  2
#define CQUEUE_DECIDE           0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * The user is allowed to define the datatype used by the queue. If so,
 * he also must provide the associated QueuePrintValue_t function.
 */
typedef int QueueValue_t;

static void inline QueuePrintValue_t (QueueValue_t v)
{ fprintf(stderr, "%d", v); };


typedef struct {
	int FrontIndex;
	int size;
	QueueValue_t *values;
} Queue;

/*
 * Creates a new queue structure. If MaxSizeGuess is provided (i.e. not
 * set to CQUEUE_DECIDE) that would be the initial size of the queue. 
 */
Queue* QueueInit( int MaxSizeGuess );

void QueuePrint(Queue *Q);
void QueuePrintSimple(Queue* Q);

void QueueFree(Queue *Q);

void QueueInsert(Queue *Q, QueueValue_t v);

QueueValue_t QueuePeek(Queue *Q);

int QueueIsFull(Queue *Q);

int QueueIsEmpty(Queue *Q);

void QueueIncreaseSize(Queue *Q);


/*
 * Return "True" if the value is in the Queue and 
 * "False" if it is not.
 */
int QueueValueIsIn(Queue *Q, QueueValue_t value);

/*
 * Return "True" if the value is in the Queue and 
 * "False" if it is not. Using binary search!
 */
int QueueValueIsInOptimized(Queue *Q, QueueValue_t value);

/*
 * Comparison function for binary Search and QuickSort.
 */
int cmpfunc (const void * a, const void * b);

const QueueValue_t* QueueGetPointerToValues(Queue *Q);
