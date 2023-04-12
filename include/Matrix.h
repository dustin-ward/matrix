#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <stdexcept>
#include <ostream>
#include <iomanip>

template<class T>
class Matrix {
public:

    int M;
    int N;

    Matrix<T>(int M, int N) : M(M), N(N) {
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
    Matrix<T>(std::initializer_list<std::initializer_list<U>> list) {
        M = (int)list.size();
        N = (int)begin(list)->size();

        arr = new T*[M];
        for(int i=0; i<M; i++) {
            arr[i] = new T[N];
            for(int j=0; j<N; j++)
                arr[i][j] = ((begin(list)+i)->begin())[j];
        }
    }

    Matrix<T>(const Matrix &rhs) : M(rhs.M), N(rhs.N) {
        arr = new T*[rhs.M];
        for(int i=0; i<rhs.M; i++) {
            arr[i] = new T[rhs.N];
            for(int j=0; j<rhs.N; j++)
                arr[i][j] = rhs.arr[i][j];
        }
    }

    ~Matrix<T>() {
        for(int i=0; i<M; i++)
            delete[] arr[i];
        delete[] arr;
    }
    
    T& operator()(int i, int j) {
        if(i < 0 || i > this->M || j < 0 || j > this->N)
            throw std::invalid_argument("index out of range");
        return this->arr[i][j];
    }

    Matrix<T>& operator+=(const Matrix &rhs) {
        if(this->N != rhs.N || this->M != rhs.M)
            throw std::invalid_argument("incompatible matrix sizes");

        for(int i=0; i<this->M; i++)
            for(int j=0; j<this->N; j++)
                this->arr[i][j] += rhs.arr[i][j];

        return *this;
    }

    friend Matrix<T> operator+(Matrix lhs, const Matrix &rhs) {
        lhs += rhs;
        return lhs;
    }

    Matrix<T>& operator-() {
        for(int i=0; i<this->M; i++)
            for(int j=0; j<this->N; j++)
                this->arr[i][j] = -this->arr[i][j];
        return *this;
    }

    Matrix<T>& operator-=(const Matrix &rhs) {
        if(this->N != rhs.N || this->M != rhs.M)
            throw std::invalid_argument("incompatible matrix sizes");

        for(int i=0; i<this->M; i++)
            for(int j=0; j<this->N; j++)
                this->arr[i][j] -= rhs.arr[i][j];

        return *this;
    }

    friend Matrix<T> operator-(Matrix lhs, const Matrix &rhs) {
        lhs -= rhs;
        return lhs;
    }

    Matrix<T> operator*(const Matrix &rhs) {
        if(N != rhs.M)
            throw std::invalid_argument("incompatible matrix sizes");

        Matrix<T> ret(M, rhs.N);
        for(int i=0; i<M; i++)
            for(int j=0; j<rhs.N; j++)
                for(int k=0; k<N; k++)
                    ret.arr[i][j] += arr[i][k] * rhs.arr[k][j];

        return ret;
    }

    template<typename U>
    friend bool operator==(const Matrix<U> &a, const Matrix<U> &b);

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<U> &rhs);

    friend void swap(Matrix &a, Matrix &b) {
        std::swap(a.M, b.M);
        std::swap(a.N, b.N);
        std::swap(a.arr, b.arr);
    }

    Matrix<T>& operator=(Matrix rhs) {
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

protected:
    
    T** arr;
};

template<typename U>
bool operator==(const Matrix<U> &a, const Matrix<U> &b) {
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

template<typename T>
std::ostream& operator<<(std::ostream &os, const Matrix<T> &rhs) {
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
