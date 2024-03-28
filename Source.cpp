#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

const int n = 100;
const int m = 2;

char generateRandomChar() {
    return 'A' + rand() % 26; // Generate a random uppercase alphabet
}

void generateRandomMatrix(char matrix[][n], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = generateRandomChar();
        }
    }
}

void generateRandomMatrix(char matrix[][m], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = generateRandomChar();
        }
    }
}

void printMatrix(char matrix[][n], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void printMatrix(char matrix[][m], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int modPow(int base, int exponent, int modulus) {
    int result = 1;
    base %= modulus;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % modulus;
        exponent >>= 1;
        base = (base * base) % modulus;
    }
    return result;
}

int main() {
    srand(time(0)); // Seed the random number generator

    char T[n][n];
    char P[m][m] = {
        {'A', 'B'},
        {'C', 'D'}
    };

    generateRandomMatrix(T, n);
    //generateRandomMatrix(P, m);

    cout << "Matrix T:" << endl;
    printMatrix(T, n);

    cout << endl << "Matrix P:" << endl;
    printMatrix(P, m);

    int hashCA_1[m] = {};
    int hashCAS_1[m] = {};
    int hashCA_P1[m] = {};
    int S_H = 0, H = 0;
    for (int a = 0; a < m; a++) {
        int c = m - 1;
        for (int b = 0; b < m; b++) {
            hashCA_P1[a] += P[a][b] * modPow(26, c, 31);
            c--;
        }
    }
    for (int i = 0; i <= n - m; i++) { // outer for each row
        for (int j = 0; j <= n - m; j++) { // inner for different columns within each row
            if (j == 0 && i == 0) { // simple hash function
                int d = 0;
                for (int a = i; a < i + m; a++) {
                    int c = m - 1;
                    for (int b = j; b < j + m; b++) {
                        hashCA_1[d] += T[a][b] * modPow(26, c, 31);
                        c--;
                    }
                    hashCAS_1[d] = hashCA_1[d];
                    d++;
                }
            }
            else if (j != 0) { // rolling hash function for changing columns within a row
                    int c = m - 1;
                    for (int a = 0; a < m; a++) {
                        hashCA_1[a] -= (T[i + a][j - 1] * modPow(26, c, 31));
                        hashCA_1[a] *= 26;
                        hashCA_1[a] += T[i + a][j - 1 + m];
                    }
            }
            bool check = true;
            for (int k = 0; k < m; k++) {
                if (hashCA_1[k] != hashCA_P1[k]) {
                    check = false;
                    break;
                }
            }
            if (check) {
                bool check1 = true;
                for (int w = i, x = 0; w < i + m; w++, x++) {
                    for (int y = j, z = 0; y < j + m; y++, z++) {
                        if (T[w][y] != P[x][z]) {
                            S_H++;
                            check1 = false;
                            break;
                        }
                    }
                    if (!check1) {
                        break;
                    }
                }
                if (check1) {
                    cout << "Pattern found at: " << i + 1 << " " << j + 1 << endl;
                    cout << "Spurious Hits: " << S_H << endl;
                    H++;
                    cout << "Hits: " << H << endl;
                }
            }
        }
        // following is the logic for rolling hash for when a row is changed
        for (int a = 0; a < m - 1; a++) {
            hashCA_1[a] = hashCAS_1[a + 1];
        }
        int c = m - 1;
        hashCA_1[m - 1] = 0;
        for (int a = 0; a < m; a++) {
            hashCA_1[m - 1] += T[i + m][a] * modPow(26, c, 31);
            c--;
        }
        //hashCA_1[m-1] %= 7;
        for (int a = 0; a < m; a++) {
            hashCAS_1[a] = hashCA_1[a];
        }
    }
    cout << "Final Spurious Hits: " << S_H << endl;
    cout << "Final Hits: " << H << endl;
    return 0;
}
