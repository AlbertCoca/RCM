#include "linkedQueue.h"

Queue* QueueInit(){
	Queue *Q;
	Q = malloc(sizeof(Queue));

	Q->FIRST = NULL;
	Q->LAST = NULL;
	Q->size = 0;
	return Q;
}

void QueuePrint(Queue *Q){

	QueueElement *ptr;
	ptr = Q->FIRST;

	if(Q->FIRST == NULL){
		printf("Empty Queue!\n");
		return;
	}
	printf("[");
	while(ptr->next!=NULL){
		printf("%d, ", ptr->value);
		ptr = ptr->next;
	}
	printf("%d]\n", ptr->value);
}

void QueueInsert(Queue *Q, QueueValue_t v){
	QueueElement *new;
	new = malloc(sizeof(QueueElement));
	new->value = v;
	new->next = NULL;

	if(Q->FIRST == NULL){
		Q->FIRST = new;
		Q->LAST = new;
	}
	else{
		Q->LAST->next = new;
		Q->LAST = new;
	}
	Q->size++;
}

QueueValue_t QueuePeek(Queue *Q){
	if(Q->FIRST == NULL){
		printf("Warning Queue is Empty!!!\n");
		return -1;
	}

	QueueValue_t value;
	value = Q->FIRST->value;
	QueueElement *del;
	del = Q->FIRST;
	Q->FIRST = Q->FIRST->next;
	Q->size--;

	//When we peek the last element of the list.
	if (Q->FIRST == NULL)Q->LAST = NULL;

	free(del);
	return value;
}

void QueueFree(Queue *Q){
	QueueElement *ptr;
	QueueElement *aux;
	ptr = Q->FIRST;

	if(Q->FIRST == NULL){
		return;
	}

	while(ptr->next!=NULL){
		aux = ptr;
		ptr = ptr->next;
		free(aux);
	}
	free(Q);
}

int QueueIsEmpty(Queue *Q){
	if(Q->FIRST == NULL)return 1;
	else return 0;
}

QueueValue_t* QueueGetList(Queue *Q){
	if (Q->FIRST == NULL) return NULL;

	QueueValue_t *v;
	v = (QueueValue_t*)malloc(sizeof(QueueValue_t)*Q->size);
	QueueElement *value;
	value = Q->FIRST;

	for(int i=0; i<Q->size; i++){
		v[i] = value->value;
		value = value->next;
	}
	return v;
}

