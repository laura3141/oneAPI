#include <CL/sycl.hpp>
#include <iostream>


using namespace std;
using namespace sycl;

int main(){

    int a[2]={1,2};
    int b[2]={3,5};
    int c[2]={1,1};

    cpu_selector selector;
    queue fila(selector);

    auto tam = range<1>(2);

    buffer buf_1(a,tam);
    buffer buf_2(b,tam);
    buffer buf_r(c,tam);

    fila.submit([&](handler &h){

        accessor acc_1(buf_1,h,read_only);
        accessor acc_2(buf_2,h,read_only);
        accessor acc_r(buf_r,h,write_only);

        h.parallel_for(tam,[=](id <1>i ){
            acc_r[i]=acc_1[i]+acc_2[i];
        });
    }).wait();
    
    for(int j=0;j<2;j++){
        cout<<c[j]<<std::endl;
    }
    
    

}