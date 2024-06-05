#include <CL/sycl.hpp>
#include <iostream>
#include <cstdlib> 
#include <ctime>   

using namespace std;
using namespace cl::sycl;

constexpr size_t nlin = 5000;  
constexpr size_t ncol = 5000; 
constexpr size_t nres = 5000;  

int main() {

    //matrizes
    auto r1 = range<2>(nlin, ncol);   
    auto r2 = range<2>(ncol, nres);   
    auto rf = range<2>(nlin, nres);  

    buffer<int, 2> matriz1{r1}, matriz2{r2}, matrizF{rf};
    cpu_selector selector;
    queue q(selector);

    // Inicializaçao da 1 matriz
    q.submit([&](handler& h) {
        accessor A(matriz1, h, write_only);

        h.parallel_for(r1, [=](id<2> index) {
            A[index] = index[0]%100; 
        });
    }).wait();

    // Inicializaçao da 2
    q.submit([&](handler& h) {
        accessor B(matriz2, h, write_only);

        h.parallel_for(r2, [=](id<2> index) {
            B[index] =index[1]%100; 
        });
    }).wait();

    // Multiplicaçao 
    q.submit([&](handler& h) {
        accessor A(matriz1, h, read_only);
        accessor B(matriz2, h, read_only);
        accessor C(matrizF, h, write_only);

        h.parallel_for(rf, [=](id<2> index) {
            int linha = index[0];  //valor de i
            int coluna = index[1]; //valor de j
            int soma = 0;

            for (int k = 0; k < ncol; k++) {
                soma += A[linha][k] * B[k][coluna];
            }

            C[index] = soma;
        });
    }).wait();

    // Print da matriz final 
    host_accessor aux(matrizF, read_only);
    
    for (int i = 0; i < nlin; i++) {
        for (int j = 0; j < nres; j++) {
            std::cout << aux[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
