import sys
import numpy as np
import matplotlib.pylab as plt
import scipy.io as IO
import scipy.sparse as sparse
import scipy.sparse.linalg as sla
import scipy.linalg as la
from array import array

def export_csr2bin ( M, outputfilename):
    '''
    Exporta el sistema Mx=b con multiples lados derechos al formato definido para el SPIKE
    @M: matrix sparse de entrada
    @B: lado derecho del sistema
    '''
    output_file = open( outputfilename, 'wb')

    if sparse.isspmatrix_csr( M ) == False:
        print 'Warning, input matrix is not CSR!'
        M = M.tocsr()


    # eliminamos posibles ceros fruto de la extraccion o la incorrecta creacion de la matriz
    # M.eliminate_zeros()

    # flags para los tipos de datos empleados
    DOUBLE_t   = 0
    COMPLEX_t  = 1


    if( type(M.data[0]) == np.complex128 ):
        print 'Datatype is complex'
        datatype = COMPLEX_t
    else:
        print 'Datatype is double'
        datatype = DOUBLE_t


    # print 'rhs dimensions', len(B.shape)

    # if (len(B.shape) == 1):
    # 	nrhs = 1
    # else:
    # 	nrhs = B.shape[1]

    print 'System features'
    print 'Coefficients datatype   : ', type( M.data[0] )
    print 'System dimension        : ', M.shape
    print 'nnz elements            : ', M.nnz
    # print 'Numero de RHS           : ', nrhs

    # guardamos el numero de elementos no nulos
    float_array = array('i', [M.nnz])
    float_array.tofile(output_file)

    # guardamos el numero de filas de la matriz de coeficientes
    float_array = array('i', [ M.shape[0] ])
    float_array.tofile(output_file)




    # guardamos los indices de punteros a filas
    float_array = array('i', M.indptr )
    float_array.tofile(output_file)
    
    # guardamos los inidices de columnas
    float_array = array('i', M.indices )
    float_array.tofile(output_file)
    
    if datatype == COMPLEX_t:
        # guardamos la parte real de la matriz de coeficientes
        float_array = array('d', M.data.real )
        float_array.tofile(output_file)

        # guardamos la parte imaginaria de la matriz de coeficientes
        float_array = array('d', M.data.imag )
        float_array.tofile(output_file)

    else:
        # guardamos los coeficientes del sistema como doubles
        float_array = array('d', M.data )
        float_array.tofile(output_file)


    # guardamos el vector B
    # if datatype == COMPLEX_t:
    #     float_array = array('d',  np.ravel( B.real, order='F') )
    #     float_array.tofile(output_file)

    #     float_array = array('d',  np.ravel( B.imag, order='F') )
    #     float_array.tofile(output_file)

    # else:
    #     float_array = array('d',  np.ravel( B, order='F') )
    #     float_array.tofile(output_file)

    # cerramos el fichero
    output_file.close()

    print 'Linear system was sucessfully exported: ', outputfilename

if __name__ == "__main__":
    A = IO.loadmat("Matlab/G3_circuit.mat");
    print A['Problem']
    A = A['Problem'][0][0][2].tocsr()
    export_csr2bin( A, "spike/G3_circuit.bin")
