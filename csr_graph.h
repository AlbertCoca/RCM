/*
 * =====================================================================================
 *
 *       Filename:  csr_graph.h
 *
 *    Description:  An small library to work with graphs represented as
 *    							sparse (csr) matrices
 *
 *        Version:  1.0
 *        Created:  19/01/2017 18:06:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Samuel Rodriguez Bernabeu (samuel.rodriguez@bsc.es)
 *   Organization:  Barcelona Supercomputing Center
 *
 * =====================================================================================
 */

/*
 * TODO:
 * - Initialize IsLaplacian flag properly.
 * 
 * OPTIMIZATION
 * - PrintAsDense function can be done without using a buffer
 * - Deletion of diagonal can be done more efficiently using binary search
 *   to find the diagonal element and memcpy for the large mem copies of
 *   the two chunks before and after the diagonal element.
 * - Assume aligned for row and col pointers.
 *
 *  - General OpenMP support can be added easily!
 *  - Scalabe intel malloc?
 *
 */


#pragma once

#define __CGRAPH__               1
#define __CGRAPH_MAJOR_VERSION__ 1
#define __CGRAPH_MINOR_VERSION__ 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MAX_GRAPH_SIZE_TO_DISPLAY 20
#define DEBUD_DENSE_MATRIX_PRINT   1
#define CGRAPH_DEBUG               1

#if !defined(restrict)
	#define restrict __restrict__
#endif	

typedef int GraphInt;

typedef struct
{
	GraphInt  n;           /* Number of vertices */
	GraphInt  nnz;         /* Number of edges    */
	int       IsLaplacian; /* Is it a Laplacian? */
	GraphInt  *rowptr;
	GraphInt  *colind;
} Graph;

/*
 * Generates a test graph for with we have a reference solution.
 */
Graph* LoadTestGraph(void);


/*
 * Loads a graph from an sparse matrix binary file
 * */
Graph *LoadFromBinary(const char* filename);

/*
 * Checks if a given graph is symmetric.
 *
 * This function is intended to be called only internally.
 */
int GraphIsSymmetric(Graph *G);

/*
 * Computes (and returns) the Laplacian of a given graph.
 *
 * This function is only intended to be called internally.
 */
Graph* GraphComputeLaplacian(Graph *G);

/*
 * Eliminates the diagonal entries of the input sparse matrix.
 */
void GraphEliminateDiagonal(Graph *G);


/*
 * Computes the degree of a given node of a graph (G) given the Laplacian
 * (L) of G.
 *
 * Since the Laplacian of the matrix is symmetric, it is really easy to
 * compute the degree of the i-th node of the graph as the number of elements 
 * in the i-th row.
 */
GraphInt GraphDegreeOfNode(Graph *L, GraphInt NodeId);

/*
 * Destructor for the Graph structure
 */
void GraphFree(Graph *G);

/*
 * Allocates memory for a graph with n vertices and nnz edges.
 * This function is intended to be called internally only.
 */
Graph* AllocateGraph(GraphInt n, GraphInt nnz);

/*
 * Displays a dense representation of the graph. If the graph is too
 * large (controlled by the MAX_GRAPH_SIZE_TO_DISPLAY) then, this
 * functions bypass to its sparse version.
 */
void GraphPrint(Graph *G, const char *message);

/*
 * Prints the graph as a dense matrix.
 * This function is only intended for debugging purposes.
 */
void GraphPrintAsDense  (Graph *G);

/*
 * Prints the graph as a sparse matrix.
 * Must be the default choice for showing small graphs..
 */
void GraphPrintAsSparse (Graph *G);

/*
 * Computes the degree of each node of the graph.
 * This function is intended to be called internally only
 */
void PrintDegreeOfGraphNodes(Graph *G);


/*
* Get a list with all the neighbors of node in a graph.
* The length of this list the degree of this node.
* This list is sorted by the number of vertex of each
* neighbor.
*/
GraphInt* GetNeighbors(Graph *G, GraphInt node);

/*
* Sort a list of nodes by degre, x is the list of nodes.
* This should be used only internaly.
*/
void QuickSortByDegree(int *x, Graph *G, int first, int last);

/*
* Given a list of labels for a graph it reorder the graph with
* the new label order.
*/
void ChangeGraphIndex(GraphInt *newIndex, Graph *G);
