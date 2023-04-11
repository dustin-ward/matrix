#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include <stdexcept>
#include "../include/Matrix_Strassen.h"
using namespace std;

TEST(StrassenTests, MultiplicationInt) {
    Matrix_Strassen<int> A = {{1,2},{4,5},{7,8}};
    Matrix_Strassen<int> B = {{1,2,3},{4,5,6}};
    Matrix_Strassen<int> C = {{9,12,15},{24,33,42},{39,54,69}};

    EXPECT_EQ(A*B, C);

    Matrix_Strassen<int> D(4,4);
    EXPECT_THROW(A*D, std::invalid_argument);
}

TEST(StrassenTests, BenchmarkMultiplication) {
    Matrix_Strassen<double> A(1000,1000), B(1000,1000);
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
