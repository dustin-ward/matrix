#ifndef MATRIX_STRASSEN_H
#define MATRIX_STRASSEN_H

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>

template<class T>
class Matrix_Strassen {
public:

    int M;
    int N;

    Matrix_Strassen<T>(int M, int N) : M(M), N(N) {
        if(M <= 0 || N <= 0)
            throw std::invalid_argument("invalid matrix size");
        arr = new T*[M];
        for(int i=0; i<M; i++) {
            arr[i] = new T[N];
            for(int j=0; j<N; j++)
                arr[i][j] = {};
        }
    }

    template<typename U>
    Matrix_Strassen<T>(std::initializer_list<std::initializer_list<U>> list) {
        M = (int)list.size();
        N = (int)begin(list)->size();

        arr = new T*[M];
        for(int i=0; i<M; i++) {
            arr[i] = new T[N];
            for(int j=0; j<N; j++)
                arr[i][j] = ((begin(list)+i)->begin())[j];
        }
    }

    Matrix_Strassen<T>(const Matrix_Strassen &rhs) : M(rhs.M), N(rhs.N) {
        arr = new T*[rhs.M];
        for(int i=0; i<rhs.M; i++) {
            arr[i] = new T[rhs.N];
            for(int j=0; j<rhs.N; j++)
                arr[i][j] = rhs.arr[i][j];
        }
    }

    ~Matrix_Strassen<T>() {
        for(int i=0; i<M; i++)
            delete[] arr[i];
        delete[] arr;
    }
    
    T& operator()(int i, int j) {
        if(i < 0 || i > this->M || j < 0 || j > this->N)
            throw std::invalid_argument("index out of range");
        return this->arr[i][j];
    }

    Matrix_Strassen<T>& operator+=(const Matrix_Strassen &rhs) {
        if(this->N != rhs.N || this->M != rhs.M)
            throw std::invalid_argument("incompatible matrix sizes");

        for(int i=0; i<this->M; i++)
            for(int j=0; j<this->N; j++)
                this->arr[i][j] += rhs.arr[i][j];

        return *this;
    }

    friend Matrix_Strassen<T> operator+(Matrix_Strassen lhs,
            const Matrix_Strassen &rhs) {
        lhs += rhs;
        return lhs;
    }

    Matrix_Strassen<T>& operator-() {
        for(int i=0; i<this->M; i++)
            for(int j=0; j<this->N; j++)
                this->arr[i][j] = -this->arr[i][j];
        return *this;
    }

    Matrix_Strassen<T>& operator-=(const Matrix_Strassen &rhs) {
        if(this->N != rhs.N || this->M != rhs.M)
            throw std::invalid_argument("incompatible matrix sizes");

        for(int i=0; i<this->M; i++)
            for(int j=0; j<this->N; j++)
                this->arr[i][j] -= rhs.arr[i][j];

        return *this;
    }

    friend Matrix_Strassen<T> operator-(Matrix_Strassen lhs,
            const Matrix_Strassen &rhs) {
        lhs -= rhs;
        return lhs;
    }

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

    template<typename U>
    friend bool operator==(const Matrix_Strassen<U> &a,
            const Matrix_Strassen<U> &b);

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os,
            const Matrix_Strassen<U> &rhs);

    friend void swap(Matrix_Strassen &a, Matrix_Strassen &b) {
        std::swap(a.M, b.M);
        std::swap(a.N, b.N);
        std::swap(a.arr, b.arr);
    }

    Matrix_Strassen<T>& operator=(Matrix_Strassen rhs) {
        swap(*this,rhs);
        return *this;
    }

    void resize(int M, int N) {
        T** temp = new T*[M];
        for(int i=0; i<M; i++) {
            temp[i] = new T[N];
            for(int j=0; j<N; j++) {
                if(i<this->M && j<this->N)
                    temp[i][j] = this->arr[i][j];
                else
                    temp[i][j] = {};
            }
        }
        for(int i=0; i<this->M; i++)
            delete[] this->arr[i];
        delete[] this->arr;

        this->arr = temp;
        this->M = M;
        this->N = N;
    }

    Matrix_Strassen<T> transpose() const {
        Matrix_Strassen<T> temp(this->N, this->M);

        for(int i=0; i<this->M; i++)
            for(int j=0; j<this->N; j++)
                temp.arr[j][i] = this->arr[i][j];

        return temp;
    }

protected:
    
    T** arr;

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

template<typename U>
bool operator==(const Matrix_Strassen<U> &a, const Matrix_Strassen<U> &b) {
    bool ret = a.M == b.M && a.N == b.N;
    if(!ret) return ret;

    for(int i=0; i<a.M; i++) {
        for(int j=0; j<a.N; j++) {
            if(a.arr[i][j] != b.arr[i][j]) {
                ret = false;
                break;
            }
        }
        if(!ret) break;
    }

    return ret;
}

template<typename U>
std::ostream& operator<<(std::ostream& os, const Matrix_Strassen<U> &rhs) {
    os << "Matrix(" << rhs.M << ", " << rhs.N << "):\n";
    for(int i=0; i<rhs.M; i++) {
        os << "|";
        for(int j=0; j<rhs.N; j++) {
            os << std::fixed << std::setprecision(2) << std::setw(4)
                << rhs.arr[i][j];
            if(j < rhs.N-1)
                os << " ";
        }
        os << "|";
        if(i < rhs.M-1)
            os << "\n";
    }
    return os;
}

#endif
