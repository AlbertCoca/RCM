#include "cqueue.h" 
#include "csr_graph.h"

/*
 * Computes the degree of each node of the graph.
 * And return the node with the minimum degree.
 * Discard the nodes that already are in the Queue Q
 */
GraphInt GetNodeWithLowerDegree(Graph *G, Queue *Q);

GraphInt GetNodeWithLowerDegree(Graph *G, Queue *Q)
{
	GraphInt min = 0;
	GraphInt val = 1e9;
	for(GraphInt node=1; node < G->n; node++){
		if(GraphDegreeOfNode(G,node) < val && !QueueValueIsIn(Q, node)){
			min = node;
			val = GraphDegreeOfNode(G,node);
		}
	}
	return min;
};

Queue* CuthillMcKee(Graph* G){

	Queue *Q = QueueInit( CQUEUE_DECIDE );
	Queue *R = QueueInit( CQUEUE_DECIDE );

	GraphInt* neighbors;

	GraphInt start = GetNodeWithLowerDegree(G, R);
	QueueInsert(Q, start);


	printf("Starting with node: %d\n", start);

	//RMA
	while(R->FrontIndex < G->n-1){
		//printf("R->FrontIndex: %d G->n-1: %d\n", R->FrontIndex, G->n-1);
		QueueValue_t actual = QueuePeek(Q);

		if(!QueueValueIsIn(R, actual)){
			QueueInsert(R, actual);
			neighbors = GetNeighbors(G, actual);
			for(int i=GraphDegreeOfNode(G, actual)-1; i >= 0; i--){
				if(!QueueValueIsIn(R, neighbors[i]))
					QueueInsert(Q, neighbors[i]);
			}
			//printf("R:\n");
			//QueuePrintSimple(R);
			//printf("Q:\n");
			//QueuePrintSimple(Q);
			//getchar();
		}

		if(Q->FrontIndex == -1 && R->FrontIndex < 8){
			QueueInsert(Q, GetNodeWithLowerDegree(G, R));
		}
	}

	QueueFree(Q);
	return R;
}

int main (int argc, char *argv[]){
	//Graph *T = LoadTestGraph();

	Graph *T = LoadFromBinary("TestMatrix/spike/G3_circuit.bin");

	GraphPrint(T, "Test");

	Queue *R =  CuthillMcKee(T);

	QueuePrintSimple(R);

	ChangeGraphIndex(R->values, T);
	GraphPrint(T, "Test After changing indexes");
	
	QueueFree(R);
	GraphFree(T);

	return 0;
} 
