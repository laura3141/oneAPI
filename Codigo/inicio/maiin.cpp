#include <CL/sycl.hpp>
#include <iostream>

constexpr int num=160000;// expressao em tempo de compilaçao
using namespace sycl;

int main() {

    auto r = range{num};
    buffer<int> a{r}; //criaçao de um buffer de 16 inteiros chamado a 
    queue{}.submit([&](handler& h) {//conexao entre o acelerador e o host
        accessor out{a, h};
        h.parallel_for(r, [=](item<1> idx) {
        out[idx] = idx;
        });
    });

    host_accessor result{a};
    for (int i=0; i<num; ++i)
        std::cout << result[i] << "\n";

}