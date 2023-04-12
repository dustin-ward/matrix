#ifndef MATRIX_STRASSEN_H
#define MATRIX_STRASSEN_H

#include <stdexcept>
#include <algorithm>
#include "./Matrix.h"

template<class T>
class Matrix_Strassen : public Matrix<T> {
public:
    using Matrix<T>::Matrix;
    
    Matrix_Strassen<T> operator*(const Matrix_Strassen &M) {
        if(this->N != M.M)
            throw std::invalid_argument("incompatible matrix sizes");

        int retM = this->M;
        int retN = M.N;
        int maxDimension = std::max(retM, retN);

        Matrix_Strassen<T> ret(maxDimension, maxDimension);

        

        return ret;
    }

private:

    // Matrix_Strassen<T> mult_matrix(const Matrix_Strassen &lhs,
    //         const Matrix_Strassen &rhs) {
    //     
    // }

};

#endif
