#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <stdexcept>
#include <ostream>
#include <iomanip>

template<class T>
class Matrix {
public:
    Matrix<T>(int m, int n) : m(m), n(n) {
        if(m <= 0 || n <= 0)
            throw std::invalid_argument("invalid matrix size");
        arr = new T*[m];
        for(int i=0; i<m; i++) {
            arr[i] = new T[n];
            for(int j=0; j<n; j++)
                arr[i][j] = {};
        }
    }

    template<typename U>
    Matrix<T>(std::initializer_list<std::initializer_list<U>> list) {
        m = (int)list.size();
        n = (int)begin(list)->size();

        arr = new T*[m];
        for(int i=0; i<m; i++) {
            arr[i] = new T[n];
            for(int j=0; j<n; j++)
                arr[i][j] = ((begin(list)+i)->begin())[j];
        }
    }

    ~Matrix<T>() {
        for(int i=0; i<m; i++)
            delete[] arr[i];
        delete[] arr;
    }
    
    T& operator()(const int& i, const int& j) {
        if(i < 0 || i > m || j < 0 || j > n)
            throw std::invalid_argument("index out of range");
        return arr[i][j];
    }

    Matrix<T> operator+(const Matrix &M) {
        if(n != M.n || m != M.m)
            throw std::invalid_argument("incompatible matrix sizes");
        
        Matrix<T> ret(m,n);
        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++)
                ret.arr[i][j] = arr[i][j] + M.arr[i][j];

        return ret;
    }
    
    Matrix<T> operator*(const Matrix &M) {
        if(n != M.m)
            throw std::invalid_argument("incompatible matrix sizes");

        Matrix<T> ret(m, M.n);
        for(int i=0; i<m; i++)
            for(int j=0; j<M.n; j++)
                for(int k=0; k<n; k++)
                    ret.arr[i][j] += arr[i][k] * M.arr[k][j];

        return ret;
    }

    template<typename U>
    friend bool operator==(const Matrix<U> &a, const Matrix<U> &b);

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<U> &M);

private:
    int m;
    int n;
    
    T** arr;
};

template<typename U>
bool operator==(const Matrix<U> &a, const Matrix<U> &b) {
    bool ret = a.m == b.m && a.n == b.n;
    if(!ret) return ret;

    for(int i=0; i<a.m; i++) {
        for(int j=0; j<a.n; j++) {
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
std::ostream& operator<<(std::ostream &os, const Matrix<T> &M) {
    os << "Matrix(" << M.m << ", " << M.n << "):\n";
    for(int i=0; i<M.m; i++) {
        os << "|";
        for(int j=0; j<M.n; j++) {
            os << std::fixed << std::setprecision(2) << std::setw(4)
               << M.arr[i][j];
            if(j < M.n-1)
                os << " ";
        }
        os << "|";
        if(i < M.m-1)
            os << "\n";
    }
    return os;
}

#endif
