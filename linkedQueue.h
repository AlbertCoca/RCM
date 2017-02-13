/*
 * =====================================================================================
 *
 *       Filename:  linkedQueue.h
 *
 *    Description:  Implementation of a linked FIFO Queue structure.
 *					It is implemented using a linked list.
 *
 *        Version:  1.0
 *        Created:  09/02/2017 18:23:58
 *       Revision:  0.1
 *       Compiler:  gcc
 *
 *         Author:  Albert Coca Abello (acocaabe@bsc.es)
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

typedef struct QueueElement{
	QueueValue_t value;
	 struct QueueElement *next;
} QueueElement;

typedef struct {
	int size;
	QueueElement *FIRST;
	QueueElement *LAST;
} Queue;

Queue* QueueInit();

void QueuePrint(Queue *Q);

void QueueInsert(Queue *Q, QueueValue_t v);

void QueueFree(Queue *Q);

QueueValue_t QueuePeek(Queue *Q);

int QueueIsEmpty(Queue *Q);

QueueValue_t* QueueGetList(Queue *Q);
