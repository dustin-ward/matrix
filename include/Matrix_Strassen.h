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
        if(this->n != M.m)
            throw std::invalid_argument("incompatible matrix sizes");

        int retM = this->m;
        int retN = M.n;
        int maxDimension = std::max(retM, retN);

        Matrix_Strassen<T> ret(this->m, M.n);

        return ret;
    }

private:

    // Matrix_Strassen<T>

};

#endif
