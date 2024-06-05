#include <CL/sycl.hpp>
#include <iostream>

using namespace std;
using namespace cl::sycl;

constexpr size_t nlin=2;
constexpr size_t ncol=3;

int main(){
    
    auto r =range<2>(nlin,ncol); //[2][3]
    buffer<int,2> matriz1 {r},matriz2 {r},matrizF {r};
    cpu_selector seletor;
    queue q(seletor);

    q.submit([&](handler &h){

        accessor A(matriz1,h,read_write);
        accessor B(matriz2,h,read_write);
        accessor C(matrizF,h,read_write);

        h.parallel_for(range<2>(nlin,ncol),[=](id<2> index){
            int linha = index[0];//i
            int coluna = index[1];//j
            A[index]=rand();
            B[index]=rand();
            for(int i=0;i<nlin;i++){
                for(int j=0;j<ncol;j++){
                    
                }
            }

        });
    });

    host_accessor aux(matrizF,read_only);

        for(int i=0;i<nlin;i++){
            for(int j=0;j<ncol;j++){
                std::cout<<aux[i][j]<<std::endl;

            }
        }
} 