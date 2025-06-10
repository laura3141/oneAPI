#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void multiplica(int a[200][300], int b[300][200], int result[200][200]) {
    for(int i = 0; i < 200; i++) {
        for(int k = 0; k < 200; k++) {
            int soma = 0;
            for(int j = 0; j < 300; j++) {
                soma += a[i][j] * b[j][k];
            }
            result[i][k] = soma;
        }
    }
}

int main() {
    int a[200][300];
    int b[300][200];
    int result[200][200] = {0};

    // Initialize matrix 'a'
    for(int i = 0; i < 200; i++) {
        for(int j = 0; j < 300; j++) {
            a[i][j] = rand() % 10; // Using % 10 for smaller numbers
        }
    }

    // Initialize matrix 'b'
    for(int i = 0; i < 300; i++) {
        for(int j = 0; j < 200; j++) {
            b[i][j] = rand() % 10; // Using % 10 for smaller numbers
        }
    }

    // Perform matrix multiplication
    multiplica(a, b, result);

    // Print the result matrix
    printf("Result matrix:\n");
    for(int i = 0; i < 200; i++) {
        for(int j = 0; j < 200; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
