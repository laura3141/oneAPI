#include <iostream>
#include <bitset>
#include <CL/sycl.hpp>

using namespace std;
using namespace sycl;

class Ula{
    public:
        std::bitset <32> a;
        std::bitset <2048> b;
        std::bitset <32> notA;
    
    public: Ula(){
        a.set();
    }

    public: void portaNot(){

        cpu_selector selector;
        queue fila(selector);
        auto tam = range<1>(1);
        buffer buf_1(&a,tam);
        buffer buf_2(&notA,tam);

        fila.submit([&](handler &h){
            accessor ac_1(buf_1,h,read_only);
            accessor ac_2(buf_2,h,write_only);

            h.parallel_for(tam,[=] (id <1>i){
                ac_2[i]=~ac_1[i];
            });
        }).wait();

    }
};

int main(){
    Ula ula;
    cout << "a " <<std::endl;
    auto start = chrono ::high_resolution_clock::now();
    ula.portaNot();
    
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> duration = end - start;
    
    cout << "Resultado: " << ula.notA <<std::endl;
    cout << "Tempo de execução: " << duration.count() << " ms" <<std::endl;

} //1629.1 ms 1620