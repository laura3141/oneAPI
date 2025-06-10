#include <CL/sycl.hpp>
#include <iostream>

using namespace cl::sycl;
using namespace std;

int main(){

    // tamanho do espaço de iteraçao
    int N=10000000;
    //range de 1 dimensao e 100 iterações
    auto r= range<1>(N);
    //criaçao do buffer passando o range criado 
    buffer<int>a {r},b{r},c{r};
    gpu_selector seletor;
    queue q(seletor);
    q.submit([&](handler & h ){
        //acessadores para os buffers
        accessor A(a,h,read_write);
        accessor B(b,h,read_write);
        accessor C(c,h,write_only);
        
        //kernel que sera executado
        h.parallel_for(range<1>(N), [=](auto i){
            A[i]=i;
            B[i]=i;
            C[i] = A[i] + B[i];
        });
    });
    host_accessor aux(c,read_only);
    for(int i=0;i<N;i++){
        cout<<aux[i]<<std::endl;
    }
}
    


