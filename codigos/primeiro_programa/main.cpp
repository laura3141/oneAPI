#include <CL/sycl.hpp>
#include <iostream>

constexpr int num=16;// expressao em tempo de compilaçao
using namespace sycl;

int main() {

    auto r = range{num};//criaçao de um "vetor" de 16 elementos
    buffer<int> a{r}; //dados compartilhados entre o host e os aceleradores, 
    
    queue{}.submit([&](handler& h) {//conexao entre o acelerador e o host
        accessor out{a, h};
        h.parallel_for(r, [=](item<1> idx) {
        out[idx+1] = idx;
        });
    });

    host_accessor result{a};
    for (int i=0; i<num; ++i)
        std::cout << result[i] << "\n";
    std::cout << result[16] << "\n";

}
