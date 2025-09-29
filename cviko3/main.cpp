#include <iostream>
#include <vector>
#include <cmath>

using std::vector;

using ScalarTypeT = double;
using Matrix = vector<vector<ScalarTypeT>>;

void printMatrix(const Matrix &mat)
{
    for (const auto &row : mat)
    {
        for (const auto &val : row)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

void printVec(const vector<ScalarTypeT> &vec)
{
    for (const auto &val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

vector<ScalarTypeT> solveAxB(Matrix A, vector<ScalarTypeT> B)
{
    for (size_t i = 0; i < A.size(); i++)
    {
        size_t pivot = i;
        ScalarTypeT pivotValue = std::abs(A[i][i]);
        for(size_t j = i + 1;j < A.size();j++) {
            if(std::abs(A[j][i]) > pivotValue) {
                pivotValue = std::abs(A[j][i]);
                pivot = j;
            }
        }
        if(pivot != i) {
            std::swap(A[i], A[pivot]);
            std::swap(B[i], B[pivot]);
        }
        for (size_t j = i + 1; j < A.size(); j++)
        {
            ScalarTypeT temp = A[j][i] / A[i][i];
            for (size_t k = i + 1; k < A.size(); k++)
            {
                A[j][k] -= temp * A[i][k];
            }
            B[j] -= temp * B[i];
        }
    }

    for (int i = A.size() - 1; i >= 0; i--)
    {
        ScalarTypeT sum = 0;
        for(size_t j = i + 1;j < A.size();j++) {
            sum += A[i][j] * B[j];
        }
        B[i] = (B[i] - sum) / A[i][i];
    }
    return B;
}

int main()
{
    Matrix mat = {
        {1, 2},
        {2, 1}};
    vector<ScalarTypeT> vec = {1, 1};

    auto x = solveAxB(mat, vec);
    printVec(x);
    return 0;
}