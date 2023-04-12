#ifndef MATRIX_STRASSEN_H
#define MATRIX_STRASSEN_H

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "./Matrix.h"

template<class T>
class Matrix_Strassen : public Matrix<T> {
public:
    using Matrix<T>::Matrix;
    
    Matrix_Strassen<T> operator*(Matrix_Strassen rhs) {
        if(this->N != rhs.M)
            throw std::invalid_argument("incompatible matrix sizes");

        Matrix_Strassen<T> lhs = *this;
        int retM = lhs.M;
        int retN = rhs.N;
        unsigned int maxDimension = std::max(std::max(lhs.M,lhs.N), rhs.N);
        int newSize = 1 << (32 - __builtin_clz(maxDimension));
        lhs.resize(newSize,newSize);
        rhs.resize(newSize,newSize);
        
        Matrix_Strassen<T> ret = mult_matrix(lhs, rhs);
        ret.resize(retM,retN);
        return ret;
    }

    Matrix_Strassen<T> operator+(Matrix_Strassen<T> rhs) {
        if(this->N != rhs.N || this->M != rhs.M)
            throw std::invalid_argument("incompatible matrix sizes");
        
        Matrix_Strassen<T> ret = *this;
        for(int i=0; i<this->M; i++)
            for(int j=0; j<this->N; j++)
                ret.arr[i][j] += rhs.arr[i][j];

        return ret;
    }

    Matrix_Strassen<T> operator-(Matrix_Strassen<T> rhs) {
        if(this->N != rhs.N || this->M != rhs.M)
            throw std::invalid_argument("incompatible matrix sizes");
        
        Matrix_Strassen<T> ret = *this;
        for(int i=0; i<this->M; i++)
            for(int j=0; j<this->N; j++)
                ret.arr[i][j] -= rhs.arr[i][j];

        return ret;
    }

private:

    Matrix_Strassen<T> mult_matrix(Matrix_Strassen lhs, Matrix_Strassen rhs) {
        int N = lhs.N;

        if(N == 1)
            return {{lhs(0,0) * rhs(0,0)}};

        Matrix_Strassen<T> A(N/2, N/2);
        Matrix_Strassen<T> B(N/2, N/2);
        Matrix_Strassen<T> C(N/2, N/2);
        Matrix_Strassen<T> D(N/2, N/2);
        Matrix_Strassen<T> E(N/2, N/2);
        Matrix_Strassen<T> F(N/2, N/2);
        Matrix_Strassen<T> G(N/2, N/2);
        Matrix_Strassen<T> H(N/2, N/2);
        for(int i=0; i<N/2; i++) {
            for(int j=0; j<N/2; j++) {
                A(i,j) = lhs(i,j);
                B(i,j) = lhs(i,j+(N/2));
                C(i,j) = lhs(i+(N/2),j);
                D(i,j) = lhs(i+(N/2),j+(N/2));

                E(i,j) = rhs(i,j);
                F(i,j) = rhs(i,j+(N/2));
                G(i,j) = rhs(i+(N/2),j);
                H(i,j) = rhs(i+(N/2),j+(N/2));
            }
        }

        Matrix_Strassen<T> p1 = mult_matrix(A, F-H);
        Matrix_Strassen<T> p2 = mult_matrix(A+B, H);
        Matrix_Strassen<T> p3 = mult_matrix(C+D, E);
        Matrix_Strassen<T> p4 = mult_matrix(D, G-E);
        Matrix_Strassen<T> p5 = mult_matrix(A+D, E+H);
        Matrix_Strassen<T> p6 = mult_matrix(B-D, G+H);
        Matrix_Strassen<T> p7 = mult_matrix(A-C, E+F);

        Matrix_Strassen<T> UL = p5 + p4 - p2 + p6;
        Matrix_Strassen<T> UR = p1 + p2;
        Matrix_Strassen<T> BL = p3 + p4;
        Matrix_Strassen<T> BR = p1 + p5 - p3 - p7;

        Matrix_Strassen<T> ret(N,N);
        for(int i=0; i<N/2; i++) {
            for(int j=0; j<N/2; j++) {
                ret(i,j) = UL(i,j);
                ret(i,j+(N/2)) = UR(i,j);
                ret(i+(N/2),j) = BL(i,j);
                ret(i+(N/2),j+(N/2)) = BR(i,j);
            }
        }
        return ret;
    }

};

#endif
