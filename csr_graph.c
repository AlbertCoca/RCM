#include "csr_graph.h"

Graph* LoadTestGraph(void)
{
	Graph* G = AllocateGraph(8, 22);

	G->IsLaplacian = 1;

	GraphInt local_rowptr[9] = {0, 2, 6, 9, 11, 14, 17, 19, 22};
	GraphInt local_colind[22] =  {0, 4, 1, 2, 5, 7, 1, 2, 4, 3, 6, 0, 2, 4, 1, 5, 7, 3, 6, 1, 5, 7 };


	for(GraphInt i = 0; i<G->nnz; i++) G->colind[i] = local_colind[i];
	for(GraphInt i = 0; i<G->n+1; i++) G->rowptr[i] = local_rowptr[i];

	//GraphPrint(G, "Dense graph");

	return (G);
};


Graph *LoadFromBinary(const char* filename)
{
	Graph *G = (Graph*) malloc(sizeof(Graph));

	FILE *InputFile = fopen(filename, "rb");

	if ( !InputFile ) 
	{fprintf(stderr, "Unable to read input file %s\n", filename); exit(-1); }

	/* read number of nodes and vertices of the graph */
	fread( &G->nnz, sizeof(GraphInt), 1, InputFile );
	fread( &G->n  , sizeof(GraphInt), 1, InputFile );

	/* check correctness of input values */
	if ( G->n <= 0 || G->nnz <= 0 )
	{
		fprintf(stderr, "ERROR: n and nnz must be positive real numbers\n");
		exit(-1);
	}
	
	/* allocate memory for graph arrays  */
	G->rowptr = (GraphInt*) malloc ( (G->n+1) * sizeof(GraphInt) );
	G->colind = (GraphInt*) malloc ( (G->nnz) * sizeof(GraphInt) );
	
	/* load the rest of the data from the file */
	fread( G->rowptr, sizeof(GraphInt), (G->n+1), InputFile );
	fread( G->colind, sizeof(GraphInt), (G->nnz), InputFile );

	/* Close binary file */
	fclose(InputFile);

	return (G);
};

void GraphEliminateDiagonal(Graph *G)
{
	GraphInt col,nnzcount=0;
	GraphInt *copy_rowptr;
	GraphInt *copy_colind;
	GraphInt *rowptr = G->rowptr;
	GraphInt *colind = G->colind;
	const GraphInt   n = G->n;
	const GraphInt nnz = G->nnz;

	copy_rowptr = (GraphInt*) malloc ( (n+1) * sizeof(GraphInt)); 
	copy_colind = (GraphInt*) malloc ( (nnz) * sizeof(GraphInt)); 
	copy_rowptr[0] = 0;


	/* Copy all the elements to the new arrays but the diaognal elements */
	for(GraphInt row = 0; row < n; row++) {
		for(GraphInt idx = rowptr[row]; idx < rowptr[row +1]; idx++) {
			col = colind[idx];

			if ( col != row )  copy_colind[nnzcount++] = col;
		}
		copy_rowptr[row+1] = nnzcount;
	}

	/* reasing old pointers to new pointers */
	free( G->rowptr );
	free( G->colind );

	G->rowptr = copy_rowptr;
	G->colind = copy_colind;
};

int GraphDegreeOfNode(Graph *L, GraphInt NodeId)
{
	if ( NodeId >= 0 && NodeId < L->n )
	{	return (L->rowptr[NodeId+1] - L->rowptr[NodeId] ); }
	else
	{ 
		fprintf(stderr, "Node %d is out of range!\n", NodeId); exit(-1); 
		return -1;
	}
};

void GraphFree(Graph *G)
{
	if( G ) { 	
		if( G->rowptr) { free( G->rowptr ); G->rowptr = NULL; }
		if( G->colind) { free( G->colind ); G->colind = NULL; }
		free( G ); G = NULL;
	}
	else { return; }
};

Graph* AllocateGraph(const GraphInt n, const GraphInt nnz)
{
	if ( n <= 0 || nnz <= 0 ) 
	{ fprintf(stderr, "ERROR: n and nnz must be positive real numbers\n"); exit(-1); }

	fprintf(stderr, "Number of nodes %d, number of edges %d\n", n, nnz);

	Graph* G = (Graph*) malloc(sizeof(Graph));
	G->n   = n;
	G->nnz = nnz;
	G->IsLaplacian = 0; /* default value */

	G->rowptr = (GraphInt*) malloc( (n+1) * sizeof(GraphInt));
	G->colind = (GraphInt*) malloc( (nnz) * sizeof(GraphInt));

	return (G);
};

void GraphPrint(Graph *G, const char *message)
{
	if ( G->n > MAX_GRAPH_SIZE_TO_DISPLAY )
	{ fprintf(stderr, "WARNING: graph is too large to display!\n"); return; }

	if ( message )
		fprintf(stderr, "\n%s::%s\n", __FUNCTION__,  message);

	fprintf(stderr, "    Number of nodes: %d, number of edges %d\n", G->n, G->nnz);

	#if defined(CGRAPH_DEBUG)
		printf("\n");
		for(GraphInt n=0; n < (G->n+1); n++)
			fprintf(stderr, "%d  ", G->rowptr[n]);
		printf("\n");

		for(GraphInt nnz=0; nnz < G->nnz; nnz++)
			fprintf(stderr, "%d  ", G->colind[nnz]);
		printf("\n");
	#endif

	#if defined(DEBUD_DENSE_MATRIX_PRINT)
		GraphPrintAsDense(G);
	#else
		GraphPrintAsSparse(G);
	#endif
};

void GraphPrintAsSparse(Graph *G)
{
	GraphInt *rowptr = G->rowptr;
	GraphInt *colind = G->colind;
	const GraphInt n = G->n;

	fprintf(stderr, "\n");

	for(GraphInt row=0; row < n; row++) {
		fprintf(stderr, "    [%d] node: ", row);
		for(GraphInt idx = rowptr[row]; idx < rowptr[row+1]; idx++)
			fprintf(stderr, "%d  ", colind[idx]);
		fprintf(stderr, "\n");
	}
	
	fprintf(stderr, "\n");
};


void GraphPrintAsDense(Graph *G)
{ 
	GraphInt *rowptr = G->rowptr;
	GraphInt *colind = G->colind;
	const GraphInt n = G->n;

	/* Create dense matrix */
	GraphInt *M = (GraphInt*) malloc( n * n * sizeof(GraphInt));	
	M = memset( M, 0, n * n * sizeof(GraphInt));

	/* Set the elements */
	for(GraphInt row=0; row < n; row++)
		for(GraphInt idx = rowptr[row]; idx < rowptr[row+1]; idx++)
			M[row * n + colind[idx]] = (GraphInt) 1;

	/* print as a regular dense matrix */
	fprintf(stderr, "\n    ");
	for(GraphInt row=0; row < n; row++){
		for(GraphInt col=0; col < n; col++)
			fprintf(stderr, "%d  ", M[row * n + col]);
		fprintf(stderr, "\n    ");
	}
	
	fprintf(stderr, "\n");

	free(M); M = NULL;
};

void PrintDegreeOfGraphNodes(Graph *G)
{
	for(GraphInt node=0; node < G->n; node++)
		fprintf(stderr, "[%d] node - degree %d\n", node, GraphDegreeOfNode(G,node) );
};

GraphInt* GetNeighbors(Graph *G, GraphInt node)
{
	GraphInt *neighbors;
	GraphInt n = GraphDegreeOfNode(G, node);

	//Checking if this node is in the graph.
	if(n == -1){
		printf("This is not a valid node!\n");
		return NULL;
	}

	//Allocating memory.
	neighbors = (GraphInt*) malloc(sizeof(GraphInt)*n);


	//Neighbors.
	for(int i=0; i<n; i++){
		neighbors[i] = G->colind[G->rowptr[node]+i];
	}

	//Sorting by Degree.
	QuickSortByDegree(neighbors, G, 0, n-1);

	return neighbors;
}

void QuickSortByDegree(int *x, Graph *G, int first, int last)
{
    int pivot,j,i;
	if(first>=last)return;
	pivot=first;
	i=first;
	j=last;

	while(i < j){
		while(GraphDegreeOfNode(G, x[i]) < GraphDegreeOfNode(G, x[pivot]) && i < last)
			i++;

		while(GraphDegreeOfNode(G, x[i]) > GraphDegreeOfNode(G, x[pivot]) && j > i)
			j--;

		if(i>=j) break;

		int aux;
		aux = x[i];
		x[i] = x[j];
		x[j] = aux;
		i++;
		j--;
	}

	int aux;
	aux = x[pivot];
	x[pivot] = x[j];
	x[j] = aux;

	QuickSortByDegree(x, G, first,j-1);
	QuickSortByDegree(x, G, j+1,last);
}

void ChangeGraphIndex(GraphInt *newIndex, Graph *G){

	/*-------------------------------------------------------------------
		Initialization
	-------------------------------------------------------------------*/

	GraphInt *v;
	GraphInt *newRowptr;
	GraphInt *newColind;

	v 			= (GraphInt*)malloc(G->n*sizeof(GraphInt));
	newRowptr 	= (GraphInt*)malloc((G->n+1)*sizeof(GraphInt));
	newColind	= (GraphInt*)malloc(G->nnz*sizeof(GraphInt));

	/*-------------------------------------------------------------------
		Reordering columns
	-------------------------------------------------------------------*/

	for(int i = 0; i < G->n; i++)
		v[newIndex[i]] = i;

	for(int i = 0; i < G->nnz; i++)
		G->colind[i] = v[G->colind[i]];

	/*-------------------------------------------------------------------
		Reordering rows
	-------------------------------------------------------------------*/

	newRowptr[0] = 0;
	int k = 0;
	for(int i = 0; i < G->n; i++){

		//Actualize the rowptr.
		newRowptr[i+1] = newRowptr[i] + (G->rowptr[newIndex[i]+1] - G->rowptr[newIndex[i]]);

		for(int j = G->rowptr[newIndex[i]]; j < G->rowptr[newIndex[i]+1]; j++){

			//Actualize the colind
			newColind[k] = G->colind[j];
			k++;
		}
	}

	/*-------------------------------------------------------------------
		Asign new colind and rowptr and free olds.
	-------------------------------------------------------------------*/

	free(G->colind);
	free(G->rowptr);
	G->colind = newColind;
	G->rowptr = newRowptr;
}