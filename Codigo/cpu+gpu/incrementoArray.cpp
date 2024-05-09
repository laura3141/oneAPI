#include <CL/sycl.hpp>
#include <iostream>
#include <chrono>

class ParallelTask;

// Função que recebe como parametros uma fila de execução, e um buffer(area de memoria compartilhada entre o host e os aceleradores
void increment(sycl::queue queue, sycl::buffer<int, 1>& buf) {

    queue.submit([&](sycl::handler& cgh) {
    //acessa o buffer para leitura e escrita
        auto data = buf.get_access<sycl::access::mode::read_write>(cgh);
        
        cgh.parallel_for<class ParallelTask>(sycl::range<1>(buf.get_count()), [=](sycl::id<1> idx) {
            data[idx] += 1; //incrementa de 1 em 1 o elemento do buffer
        });
    }).wait();
}


int main() {


    constexpr size_t count = 1000000;

    // Criar contexto e fila para dispositivos CPU e GPU
    sycl::cpu_selector Cpu_selector;
    sycl::gpu_selector Gpu_selector;
    sycl::queue cpu_queue(Cpu_selector);
    sycl::queue gpu_queue(Gpu_selector);

    // Criar buffer compartilhado entre a CPU e a GPU
    sycl::buffer<int, 1> buf(count);

    // Preencher o buffer com valores iniciais
    {
        auto data = buf.get_access<sycl::access::mode::write>();
        for (size_t i = 0; i < count; ++i) {
            data[i] = i;
        }
    }

    // Realizar incremento em paralelo utilizando a CPU
    auto start = std::chrono::steady_clock::now();
    increment(gpu_queue, buf);
    increment(cpu_queue, buf);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Initial Vector add1 successfully completed on device - took "
                << (end - start).count() << " nano-secs\n";
                
       
    // Ler os resultados de volta do buffer
    auto result = buf.get_access<sycl::access::mode::read>();
    for (size_t i = 0; i < count; ++i) {
        std::cout << "Elemento " << i << ": " << result[i] << std::endl;
    }

    return 0;
}

