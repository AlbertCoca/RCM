import scipy.sparse as sp
import scipy.io as IO
import numpy as np
import matplotlib.pylab as plt
import networkx as nx

def showMatrix(A):
	fig = plt.figure()
	plt.imshow(A, interpolation='nearest')
	plt.colorbar()
	plt.show()

def getPerfiferalVertex(G, R=[]):
	m = 0
	l = 1e9
	for n in G:
		print n
		if n not in R:
			if(len(G.neighbors(n)) < l):
				m = n
				l = len(G.neighbors(n))
	return m

def sortByVertexOrder(G, array):
	less = []
	equal = []
	greater = []

	if len(array) > 1:
		pivot = array[len(array)/2]
		for x in array:
			if len(G.neighbors(x)) < len(G.neighbors(pivot)):
				less.append(x)
			if len(G.neighbors(x)) == len(G.neighbors(pivot)):
				equal.append(x)
			if len(G.neighbors(x)) > len(G.neighbors(pivot)):
				greater.append(x)
		return sortByVertexOrder(G, less)+equal+sortByVertexOrder(G, greater)
	else:
		return array

def delRendudant(R, neighbors):
	r = []
	for n in neighbors:
		if n in R: pass
		else:r.append(n)

	return r

A = [[1,0,0,0,1,0,0,0],
	 [0,1,1,0,0,1,0,1],
	 [0,1,1,0,1,0,0,0],
	 [0,0,0,1,0,0,1,0],
	 [1,0,1,0,1,0,0,0],
	 [0,1,0,0,0,1,0,1],
	 [0,0,0,1,0,0,1,0],
	 [0,1,0,0,0,1,0,1]]

A = IO.loadmat("TestMatrix/Matlab/ash85.mat");
#A = sp.csr_matrix(A)
print A;
A = A['Problem'][0][0][1].tocsr()


#Initialization
#A = sp.csr_matrix(A)
G = nx.from_numpy_matrix(np.matrix(A.todense()))

#nx.draw(G, node_color='c',edge_color='k', with_labels=True)
#plt.show()

#Main
start = getPerfiferalVertex(G)

R = []
Q = [start]

n = len(A.toarray()[0])

while(len(R) < n):
	print "R:",R, "Q:",Q

	if Q[0] not in R:
		R.append(Q[0])
		Q.extend(sortByVertexOrder(G, delRendudant(R, G.neighbors(Q[0]))))
	del Q[0]
	if(len(Q) == 0 and len(R) < 8):
		Q.append(getPerfiferalVertex(G, R))


mapping = {}
R = list(reversed(R))
for i in range(n):
	mapping[R[i]] = i

G = nx.relabel_nodes(G,mapping)

#for n in mapping:
#	print n, " - ", mapping[n]

#print R

showMatrix(A.todense())
showMatrix(nx.to_numpy_matrix(G))

#plot
#nx.draw(G, node_color='c',edge_color='k', with_labels=True)
#plt.show()


