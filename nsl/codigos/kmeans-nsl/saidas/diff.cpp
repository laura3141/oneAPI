#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream arquivo1("resultado_esperado.txt");
    std::ifstream arquivo2("saida.txt");

    if (!arquivo1.is_open() || !arquivo2.is_open()) {
        std::cerr << "Erro ao abrir um dos arquivos." << std::endl;
        return 1;
    }

    std::string linha1, linha2;
    int diferencas = 0;
    
    // Lê e compara as linhas até o fim de ambos os arquivos
    while (std::getline(arquivo1, linha1) && std::getline(arquivo2, linha2)) {
        if (linha1 != linha2) {
            diferencas++;
        }
    }

    /*Conta as linhas adicionais como diferenças
    while (std::getline(arquivo1, linha1)) {
        diferencas++;
    }
    while (std::getline(arquivo2, linha2)) {
        diferencas++;
    }*/ 

    std::cout << "Número de linhas diferentes: " << diferencas << std::endl;

    arquivo1.close();
    arquivo2.close();

    return 0;
}
