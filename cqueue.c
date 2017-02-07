/*
 * =====================================================================================
 *
 *       Filename:  cqueue.c
 *
 *    Description:  Implementation of a simple FIFO Queue structure
 *
 *        Version:  1.0
 *        Created:  17/01/2017 18:25:33
 *       Revision:  0.1
 *       Compiler:  gcc
 *
 *         Author:  Samuel Rodriguez Bernabeu (samuel.rodriguez@bsc.es)
 *   Organization:  Barcelona Supercomputing Center
 *
 * =====================================================================================
 */

#include "cqueue.h"

Queue* QueueInit( int MaxSizeGuess )
{
#if defined(CQUEUE_DEBUG_INFO)
	fprintf(stderr, "INFO: creating new queue struct\n");
#endif

	Queue *Q = (Queue*) malloc(sizeof(Queue));
	Q->FrontIndex = -1;
	Q->size   = ( MaxSizeGuess ) ? MaxSizeGuess : CQUEUE_DEFAULT_SIZE;
	Q->values = (QueueValue_t*) malloc( Q->size * sizeof(QueueValue_t)); 
	
	/* set to zero */
	Q->values = memset( Q->values, 0, Q->size * sizeof(QueueValue_t));

	fprintf(stderr, "The size of the queue is %d elements\n", Q->size);

	return (Q);
};

void QueuePrint( Queue* Q)
{
		fprintf(stderr, "INFO: queue size %d\n", Q->size);
		fprintf(stderr, "INFO: queue FrontIndex %d\n", Q->FrontIndex);

		for(int i=0; i < Q->size; i++) {
			fprintf(stderr, "    [%d] - ", i);
		  QueuePrintValue_t(	Q->values[i] );

		  fprintf(stderr, "  %s\n", (i == Q->FrontIndex) ? "<-" : ""); 
		}
};

void QueuePrintSimple(Queue* Q)
{
	printf("[");
	for(int i=0; i <= Q->FrontIndex; i++) {
		printf("%d, ", Q->values[i]);
	}
	printf("]\n");
}

void QueueFree( Queue *Q )
{
	if (Q) { free( Q->values ); Q->values = NULL; }
};	

void QueueInsert(Queue *Q, QueueValue_t v)
{
	if ( QueueIsFull(Q) ) QueueIncreaseSize(Q);

	for(int i = Q->FrontIndex; i >= 0; i--)
		Q->values[i+1] = Q->values[i];

	Q->values[0] = v;
	Q->FrontIndex++;
}

QueueValue_t QueuePeek(Queue *Q)
{
	if ( !QueueIsEmpty(Q) )
		return Q->values[Q->FrontIndex--];
	else
		fprintf(stderr, "WARNING: the Queue is empty\n");
	return 0;
};

int QueueIsFull(Queue *Q)
{
	return ( (Q->FrontIndex +1) == Q->size );
};

int QueueIsEmpty(Queue *Q)
{
	return (Q->FrontIndex == -1 );
};

void QueueIncreaseSize(Queue *Q)
{
	//int OldQueueSize = Q->size;
	Q->size = CQUEUE_INCREASE_FACTOR * Q->size;
	Q->values = realloc( Q->values, Q->size * sizeof(QueueValue_t));
};

const QueueValue_t* QueueGetPointerToValues(Queue *Q)
{
	return ((const QueueValue_t*) Q->values);
};

//TODO: Do this with qsort and bsearch
int QueueValueIsIn(Queue *Q, QueueValue_t value)
{
	for(int i = 0; i <= Q->FrontIndex; i++){
		if(value == Q->values[i]) return 1;
	}
	return 0;
}

int QueueValueIsInOptimized(Queue *Q, QueueValue_t value)
{
	//printf("Searching for %d\n", value );
	QueueValue_t *v;
	int n = Q->FrontIndex + 1;
	v = malloc(sizeof(QueueValue_t)*n);

	memcpy(v, Q->values, sizeof(QueueValue_t)*n);
	qsort(v, n, sizeof(QueueValue_t), cmpfunc);

	QueueValue_t *item = (QueueValue_t*) bsearch(&value, v, n, sizeof(QueueValue_t), cmpfunc);

	if(item == NULL)return 0;
	return 1;

}

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}



