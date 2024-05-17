#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    int numeros[1000000];
    const int TAM = 1000000;
    // Preenchendo o vetor com numeros aleatorios
    unsigned seed = time(0);
    srand(seed);
    for (int i=0; i<TAM; ++i)
        numeros[i] = rand()%1000000;
    
    // Bubble Sort - O(n^2)
    int aux;
    for (int i=0; i<TAM; ++i){
        for (int j=i; j<TAM; ++j){
            if (numeros[i]>numeros[j]){
                aux = numeros[i];
                numeros[i] = numeros[j];
                numeros[j] = aux;
            }
        }   
    }
    for (int i=0; i<TAM; ++i)
        cout << numeros[i] << " ";
    return 0;
}
        