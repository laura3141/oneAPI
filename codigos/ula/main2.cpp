#include <iostream>
#include <bitset>
#include <CL/sycl.hpp>

using namespace std;
using namespace sycl;

class Ula{

    public:

        std::byte a[4];
        std::byte b[4];
        std::byte notA[4];
        std::byte andAB[4];
        std::byte orAB[4];
        
    public: void calculos(){

        cpu_selector selector;
        queue fila(selector);
        range tam = range<1>(4);

        buffer buf_1(a,tam);
        buffer buf_2(notA,tam);
        buffer buf_3(b,tam);    
        buffer buf_4(andAB,tam);
        buffer buf_5(orAB,tam);

        fila.submit([&](handler &h){

            accessor ac_1(buf_1,h,read_only);//a
            accessor ac_2(buf_2,h,write_only);//notA
            accessor ac_3(buf_3,h,write_only);//b
            accessor ac_4(buf_4,h,write_only);//and
            accessor ac_5(buf_5,h,write_only);//or

            h.parallel_for(tam,[=] (id <1>i){
                ac_2[i]=~ac_1[i];
                ac_4[i]=ac_1[i]&ac_3[i];   
                ac_5[i]=ac_1[i]|ac_3[i];  
            });
            
        }).wait();
    }
};

int main(){

    Ula ula;
    int num;

    //posiçao 0 é o bit mais a direita 
    cout<<"Digite o primeiro numero: "<<std::endl;
    for(int i=0;i<4;i++){
        std::cin >> num;
        ula.a[i] = static_cast<std::byte>(num); 
    } 

    cout<<"Digite o segundo numero: "<<std::endl;
    for(int i=0;i<4;i++){
        std::cin >> num;
        ula.b[i] = static_cast<std::byte>(num); 
    }

    auto start = chrono::high_resolution_clock::now();
    ula.calculos();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    cout<<"NOT"<<std::endl;
    for(int i=0;i<4;i++){

        int a = (int)ula.notA[i];
        cout<<a<<std::endl;
    }
    
    cout<<"AND"<<std::endl;
    for(int i=0;i<4;i++){

        int a = (int)ula.andAB[i];
        cout<<a<<std::endl;
    }

    cout<<"OR"<<std::endl;
    for(int i=0;i<4;i++){

        int a = (int)ula.orAB[i];
        cout<<a<<std::endl;
    }
    
    
    
    cout << "Tempo de execução: " << duration.count() << " ms" <<std::endl;
}