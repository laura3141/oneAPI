
#include <CL/sycl.hpp> // Inclui o cabeçalho principal do SYCL

using namespace cl::sycl; 

int main() {
       gpu_selector selector; // cria um objeto gpu_selector, que é um seletor de dispositivo para escolher uma GPU como o dispositivo de computação
      //cpu_selector selector;
      //default_selector selector; 
     //host_selector selector; 

    queue q(selector); // Cria uma fila de dispositivo usando o seletor fornecido

    std::cout << "Device: " << q.get_device().get_info<info::device::name>() << std::endl; // Obtém e imprime o nome do dispositivo selecionado

    return 0; // Retorna 0 para indicar que o programa foi executado com sucesso
}

