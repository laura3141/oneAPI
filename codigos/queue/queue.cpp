#include <iostream>
#include <CL/sycl.hpp>

namespace sycl = cl::sycl;

int main(int argc, char **) {
  
//criaçao de vetores de 4 espaços de float

sycl::float4 a = {1.0, 1.0, 1.0, 1.0};
sycl::float4 b = {1.0,1.0,1.0,1.0};
  
sycl::float4 c={0.0,0.0,0.0,0.0};
  
//criaçao da fila de excecução para um seletor padrao 
sycl::queue myQueue(sycl::default_selector{}); 
 
  {
    std::cout << "sycl::float4 vectors check - a.w, b.w, c.w:\n"
              << a.w() << ", " << b.w() << ", " << c.w() << std::endl;
    std::cout << "sycl::queue check - selected device:\n"
              << myQueue.get_device().get_info<sycl::info::device::name>()
              << std::endl;
  }

  return 0;
}

