#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include <stdexcept>
#include "./Matrix.h"
using namespace std;

// uniform_real_distribution<double> unif(0,INT_MAX/2.0);
// default_random_engine re;
// for(int i=0; i<1000; i++)
//     for(int j=0; j<1000; j++)
//         A(i,j) = unif(re);
// for(int i=0; i<1000; i++)
//     for(int j=0; j<1000; j++)
//         B(i,j) = unif(re);

TEST(BasicTests, Constructor) {
    Matrix<int> A(2,2);

    Matrix<int> B = {{1,2},{3,4}};
    EXPECT_EQ(B(0,0), 1);
    EXPECT_EQ(B(0,1), 2);
    EXPECT_EQ(B(1,0), 3);
    EXPECT_EQ(B(1,1), 4);

    EXPECT_THROW(Matrix<int>(-1,0), std::invalid_argument);
}

TEST(BasicTests, AdditionInt) {
    Matrix<int> A = {{1,2,3},{4,5,6},{7,8,9}};
    Matrix<int> B = {{1,1,1},{1,1,1},{1,1,1}};
    Matrix<int> C = {{2,3,4},{5,6,7},{8,9,10}};

    EXPECT_EQ(A+B, C);

    Matrix<int> D(4,4);
    EXPECT_THROW(A+D, std::invalid_argument);
}

TEST(BasicTests, MultiplicationInt) {
    Matrix<int> A = {{1,2},{4,5},{7,8}};
    Matrix<int> B = {{1,1,1},{1,1,1},{1,1,1}};
    Matrix<int> C = {{2,3,4},{5,6,7},{8,9,10}};

    EXPECT_EQ(A+B, C);

    Matrix<int> D(4,4);
    EXPECT_THROW(A+D, std::invalid_argument);
}

