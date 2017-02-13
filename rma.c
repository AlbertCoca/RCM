#include "linkedQueue.h" 
#include "csr_graph.h"

/*
 * Computes the degree of each node of the graph.
 * And return the node with the minimum degree.
 * Discard the nodes that already are in the Queue Q
 */
GraphInt GetNodeWithLowerDegree(Graph *G, Queue *Q, char *isInR);

int matrix_ComputeBandwidth(const int n,
								int *restrict colind,
								int *restrict rowptr,
								int *ku,
								int *kl )
{
	// TODO: es posible calcular el bw mas rapidamente accediendo
	// solamente a las posiciones extremas de las filas.
	//spike_timer_t tstart, tend;
	int row, col, idx;
	*ku = 0;
	*kl = 0;

	/* initialize timer */
	//tstart = GetReferenceTime();


	for(row = 0; row < n; row++)
	{
		for(idx = rowptr[row]; idx < rowptr[row+1]; idx++)
		{
			col = colind[idx];

			if ( row - col < *ku ) { *ku = (row - col); }
			if ( col - row < *kl ) { *kl = (col - row); }
			//*ku = ((row - col) < *ku) ? (row - col) : *ku;
			//*kl = ((col - row) < *kl) ? (col - row) : *kl;
		}
	}

	*ku = abs(*ku);
	*kl = abs(*kl);

	//tend = GetReferenceTime() - tstart;

	//	fprintf(stderr, "\n%s: took %.6lf seconds (ku %d kl %d)", __FUNCTION__, tend, *ku, *kl);

	return 1;
};

GraphInt GetNodeWithLowerDegree(Graph *G, Queue *Q, char *isInR)
{
	GraphInt min = 0;
	GraphInt val = 1e9;
	for(GraphInt node=0; node < G->n; node++){
		if(GraphDegreeOfNode(G,node) < val && isInR[node] == '0'){
			min = node;
			val = GraphDegreeOfNode(G,node);
		}
	}
	return min;
};

Queue* CuthillMcKee(Graph* G, int s){

	Queue *Q = QueueInit( CQUEUE_DECIDE );
	Queue *R = QueueInit( CQUEUE_DECIDE );
	GraphInt* neighbors;
	char *isInR;

	isInR = (char*)malloc(sizeof(char)*G->n);
	memset(isInR, '0', G->n);

	GraphInt start = GetNodeWithLowerDegree(G, R, isInR);
	if(s >= 0)start = s;
	QueueInsert(Q, start);

	//printf("Starting with node: %d\n", start);

	//RMA
	while(R->size < G->n){
		QueueValue_t actual = QueuePeek(Q);

		if(isInR[actual]=='0'){
			QueueInsert(R, actual);
			isInR[actual] = '1';
			neighbors = GetNeighbors(G, actual);
			for(int i=GraphDegreeOfNode(G, actual)-1; i >= 0; i--)
			{
				if(isInR[neighbors[i]]=='0'){
					QueueInsert(Q, neighbors[i]);
				}

			}
			//printf("R:\n");
			//QueuePrint(R);
			//printf("Q:\n");
			//QueuePrint(Q);
			//getchar();
		}

		if(Q->size < 1){
			QueueInsert(Q, GetNodeWithLowerDegree(G, R, isInR));
		}
	}

	QueueFree(Q);
	return R;
}

int main (int argc, char *argv[]){

	printf("Loading Matrix...\n");

	//Graph *T = LoadTestGraph();
	Graph *T = LoadFromBinary("TestMatrix/spike/s3rmt3m3.bin");

	int ku, kl;
	matrix_ComputeBandwidth(T->n, T->colind, T->rowptr, &ku, &kl);
	printf("Bandwith Before CuthillMcKee: %d \n", ku);
	printf("Dimension of the matrix: %d \n", T->n);

	GraphPrint(T, "Test");

	printf("Executing CuthillMcKee...\n");

	/*Do the RCM starting from every node in the graph */
	//for(int i = 0; i<T->n; i++){
	//	Graph *T = LoadFromBinary("TestMatrix/spike/s3rmt3m3.bin");
	//	Queue *R =  CuthillMcKee(T, i);
	//	ChangeGraphIndex(QueueGetList(R), T);
	//	matrix_ComputeBandwidth(T->n, T->colind, T->rowptr, &ku, &kl);
	//	printf("%d, ",ku);
	//	QueueFree(R);
	//	GraphFree(T);
	//}

	/*Compute the initial node taking one with the lower degree.*/
	Queue *R =  CuthillMcKee(T, -1);
	ChangeGraphIndex(QueueGetList(R), T);
	matrix_ComputeBandwidth(T->n, T->colind, T->rowptr, &ku, &kl);
	printf("\n");

	/*Print the new order of the graph */
	//printf("\nR:\n");
	//QueuePrint(R);
	//printf("--------------------------------------------------------------------\n\n");

	matrix_ComputeBandwidth(T->n, T->colind, T->rowptr, &ku, &kl);
	printf("Bandwith After CuthillMcKee: %d \n", ku);
	
	GraphFree(T);

	return 0;
} 
