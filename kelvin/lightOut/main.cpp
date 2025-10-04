#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdint>

using namespace std;

using ScalarTypeT = int;
using Matrix = vector<vector<ScalarTypeT>>;

// sestaveni matice podle kelvinu
static Matrix buildA(int n) {
    const int N = n * n;
    Matrix A(N, vector<ScalarTypeT>(N, 0));
    const int dirs[5][2] = {{0,0},{-1,0},{1,0},{0,-1},{0,1}};
    auto idx = [n](int x, int y){ return x + n*y; };

    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < n; ++x) {
            int col = idx(x,y);
            for (auto &d : dirs) {
                int nx = x + d[0], ny = y + d[1];
                if (0 <= nx && nx < n && 0 <= ny && ny < n) {
                    int row = idx(nx,ny);
                    A[row][col] = 1;
                }
            }
        }
    }
    return A;
}

// gauss elimination podle reseni ze cviceni
static vector<ScalarTypeT> solveAxB(Matrix A, vector<ScalarTypeT> B) {
    const int N = (int)A.size();
    if (N == 0) return {};
    if ((int)A[0].size() != N || (int)B.size() != N) return {};

    for (int i = 0; i < N; ++i) {
        B[i] &= 1;
        for (int j = 0; j < N; ++j) A[i][j] &= 1;
    }

    int row = 0;
    vector<int> pivotColOfRow(N, -1);

    for (int col = 0; col < N && row < N; ++col) {
        int pivot = -1;
        for (int r = row; r < N; ++r) if (A[r][col]) { pivot = r; break; }
        if (pivot == -1) continue;

        if (pivot != row) { swap(A[pivot], A[row]); swap(B[pivot], B[row]); }

        for (int r = row + 1; r < N; ++r) {
            if (A[r][col]) {
                for (int c = col; c < N; ++c) A[r][c] ^= A[row][c];
                B[r] ^= B[row];
            }
        }
        pivotColOfRow[row] = col;
        ++row;
    }

    for (int r = row; r < N; ++r) {
        bool allZero = true;
        for (int c = 0; c < N; ++c) if (A[r][c]) { allZero = false; break; }
        if (allZero && B[r]) return {};
    }

    // back-substitution
    vector<ScalarTypeT> X(N, 0);
    for (int r = row - 1; r >= 0; --r) {
        int col = pivotColOfRow[r];
        if (col < 0) continue;
        int sum = 0;
        for (int c = col + 1; c < N; ++c) if (A[r][c] && X[c]) sum ^= 1;
        X[col] = (B[r] ^ sum) & 1;
    }
    return X;
}

int main(int argc, char* argv[]) {
    //testovani vstupu
    if (argc < 2) {
        cerr << "Invalid number of arguments\n";
        return -1;
    }
    const int n = stoi(argv[1]);
    if (argc != n*n + 2) {
        cerr << "Invalid number of arguments\n";
        return -1;
    }

    vector<int> rhs;
    rhs.reserve(n*n);
    for (int i = 0; i < n*n; ++i) {
        int v = stoi(argv[i+2]);
        rhs.push_back(v & 1);
    }

    Matrix A = buildA(n);
    vector<ScalarTypeT> b(rhs.begin(), rhs.end());
    vector<ScalarTypeT> x = solveAxB(A, b);

    if (x.empty()) return 0; // pro zadne reseni prazdny vystup

    for (int i = 0; i < (int)x.size(); ++i) {
        if (i) cout << ' ';
        cout << (x[i] & 1);
    }
    cout << ' ';
    cout << "\n";
    return 0;
}
