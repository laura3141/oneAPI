#include <CL/sycl.hpp>
using namespace cl::sycl;

int main() {
    const int width = 4;
    const int height = 4;

    // Criação de um range bidimensional
    auto r = range<2>{height, width};

    // Criação de um buffer bidimensional
    buffer<int, 2> a{r};

    // Submissão de comandos à fila
    queue{}.submit([&](handler& h) {
        // Criação de um accessor para o buffer
        accessor out{a, h};

        // Definição do kernel paralelo para range bidimensional
        h.parallel_for(r, [=](item<2> idx) {
            // Obtendo os índices do range bidimensional
            int row = idx.get_id(0);
            int col = idx.get_id(1);
            // Atribuindo valores baseados nos índices (ex: linear index ou row-major order)
            out[idx] = row * width + col;
        });
    });

    // Criação de um host_accessor para ler os dados do buffer no host após a execução do kernel
    host_accessor result{a};

    // Impressão dos resultados armazenados no buffer
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            std::cout << result[row][col] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}

