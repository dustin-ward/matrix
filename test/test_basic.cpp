#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include <stdexcept>
#include "../include/Matrix.h"
using namespace std;

TEST(BasicTests, Constructor) {
    Matrix<int> A(2,2);

    Matrix<int> B = {{1,2},{3,4}};
    EXPECT_EQ(B(0,0), 1);
    EXPECT_EQ(B(0,1), 2);
    EXPECT_EQ(B(1,0), 3);
    EXPECT_EQ(B(1,1), 4);

    EXPECT_THROW(Matrix<int>(-1,0), std::invalid_argument);
}

TEST(BasicTests, CopyConstructor) {
    Matrix<int> A = {{1,2},{3,4}};
    Matrix<int> B(A);
    EXPECT_EQ(B(0,0), 1);
    EXPECT_EQ(B(0,1), 2);
    EXPECT_EQ(B(1,0), 3);
    EXPECT_EQ(B(1,1), 4);
}

TEST(BasicTests, Swap) {
    Matrix<int> A = {{1,2},{3,4}};
    Matrix<int> B = {{4,3},{2,1}};

    swap(A,B);
    
    EXPECT_EQ(A(0,0), 4);
    EXPECT_EQ(A(0,1), 3);
    EXPECT_EQ(A(1,0), 2);
    EXPECT_EQ(A(1,1), 1);

    EXPECT_EQ(B(0,0), 1);
    EXPECT_EQ(B(0,1), 2);
    EXPECT_EQ(B(1,0), 3);
    EXPECT_EQ(B(1,1), 4);
}

TEST(BasicTests, Assignment) {
    Matrix<int> A = {{1,2},{3,4}};
    Matrix<int> B = A;
    EXPECT_EQ(B(0,0), 1);
    EXPECT_EQ(B(0,1), 2);
    EXPECT_EQ(B(1,0), 3);
    EXPECT_EQ(B(1,1), 4);
}

TEST(BasicTests, Resize) {
    Matrix<int> A = {{1,2},{3,4}};
    Matrix<int> A_expected = {{1,2,0},{3,4,0},{0,0,0}};
    A.resize(3,3);
    EXPECT_EQ(A, A_expected);

    Matrix<int> B = {{1,2,3},{4,5,6},{7,8,9}};
    Matrix<int> B_expected = {{1,2},{4,5}};
    B.resize(2,2);
    EXPECT_EQ(B, B_expected);
    
    Matrix<int> C = {{1,2},{3,4},{5,6}};
    Matrix<int> C_expected = {{1,2,0,0},{3,4,0,0},{5,6,0,0},{0,0,0,0}};
    C.resize(4,4);
    EXPECT_EQ(C, C_expected);
}

TEST(BasicTests, Negation) {
    Matrix<int> A = {{1,2},{3,4}};
    A = -A;
    EXPECT_EQ(A(0,0), -1);
    EXPECT_EQ(A(0,1), -2);
    EXPECT_EQ(A(1,0), -3);
    EXPECT_EQ(A(1,1), -4);
}

TEST(BasicTests, AdditionInt) {
    Matrix<int> A = {{1,2,3},{4,5,6},{7,8,9}};
    Matrix<int> B = {{1,1,1},{1,1,1},{1,1,1}};
    Matrix<int> C = {{2,3,4},{5,6,7},{8,9,10}};

    EXPECT_EQ(A+B, C);

    Matrix<int> D(4,4);
    EXPECT_THROW(A+D, std::invalid_argument);
}

TEST(BasicTests, SubtractionInt) {
    Matrix<int> A = {{1,2,3},{4,5,6},{7,8,9}};
    Matrix<int> B = {{1,1,1},{1,1,1},{1,1,1}};
    Matrix<int> C = {{0,1,2},{3,4,5},{6,7,8}};

    EXPECT_EQ(A-B, C);

    Matrix<int> D(4,4);
    EXPECT_THROW(A-D, std::invalid_argument);
}

TEST(BasicTests, MultiplicationInt) {
    Matrix<int> A = {{1,2},{4,5},{7,8}};
    Matrix<int> B = {{1,2,3},{4,5,6}};
    Matrix<int> C = {{9,12,15},{24,33,42},{39,54,69}};

    EXPECT_EQ(A*B, C);

    Matrix<int> D(4,4);
    EXPECT_THROW(A*D, std::invalid_argument);
}

TEST(BasicTests, BenchmarkMultiplication) {
    Matrix<double> A(1000,1000), B(1000,1000);
    uniform_real_distribution<double> unif(0,INT_MAX/2.0);
    default_random_engine re;
    for(int i=0; i<1000; i++)
        for(int j=0; j<1000; j++)
            A(i,j) = unif(re);
    for(int i=0; i<1000; i++)
        for(int j=0; j<1000; j++)
            B(i,j) = unif(re);
    A*B;
}
