#include <iostream>
#include <CL/sycl.hpp>
#include <random>
#include <cmath>

/*
//cpu_selector sel;
    gpu_selector sel;
    queue qcmk(sel);
    buffer<float, 2> instancias_buff(instancias[0], range<2>(nista, atributos));
    qcmk.submit([&](handler& ker){accessor instancias_ac{instancias_buff, ker};}).wait();
    host_accessor instancias_r{instancias_buff};

    ker.parallel_for(range<2>(height, width), [=](item<2> idx) {});
*/

#include <fstream>
#include <string>

using namespace sycl;
using namespace std;

void PrintMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void ClonarMatriz(float **matriz1, float **matriz2, int rows, int cols){ // Paralelo
    gpu_selector sel;
    queue qcmk(sel);
    buffer<float, 2> matriz1_buff(matriz1[0], range<2>(rows, cols));
    buffer<float, 2> matriz2_buff(matriz2[0], range<2>(rows, cols));

    qcmk.submit([&](handler& ker){
        accessor matriz1_ac{matriz1_buff, ker};
        accessor matriz2_ac{matriz2_buff, ker};
        ker.parallel_for(range<2>(rows, cols), [=](item<2> idx) {
            matriz2_ac[idx] = matriz1_ac[idx];
        });
    }).wait();
    host_accessor matriz1_r{matriz1_buff};
    host_accessor matriz2_r{matriz2_buff};
    
/*
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matriz2[i][j] = matriz1[i][j];
        }
    }*/
} 

bool compararMatrizes(float **matriz1, float **matriz2, int linhas, int colunas) { // Paralelo
    bool diferenca = false;
    //bool backup = false;
    // Verificar se as matrizes têm o mesmo número de linhas e colunas
    if (linhas != linhas || colunas != colunas) {
        return true;
    }
    gpu_selector sel;
    queue qcmk(sel);

    buffer<float, 2> matriz1_buff(matriz1[0], range<2>(linhas, colunas));
    buffer<float, 2> matriz2_buff(matriz2[0], range<2>(linhas, colunas));
    buffer<bool, 1> diferenca_buff(&diferenca, range<1>(1));

    qcmk.submit([&](handler& ker){
        accessor matriz1_ac{matriz1_buff, ker};
        accessor matriz2_ac{matriz2_buff, ker};
        accessor diferenca_ac{diferenca_buff, ker};
        ker.parallel_for(range<2>(linhas, colunas), [=](item<2> idx) {
            int row = idx.get_id(0);
            int col = idx.get_id(1);
            if (matriz1_ac[idx] != matriz2_ac[idx]) {
                diferenca_ac[0] = true;
            }
        });
    }).wait();
    host_accessor matriz1_r{matriz1_buff};
    host_accessor matriz2_r{matriz2_buff};
    host_accessor diferenca_r{diferenca_buff};


    // Comparar cada elemento das matrizes
    /*
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            if (matriz1_r[i][j] != matriz2_r[i][j]) {
                backup = true;
            }
        }
    }

    cout << "----I\n";
    cout << "diferenca: " << diferenca << "\n";
    cout << "diferenca_r: " << diferenca_r[0] << "\n";
    cout << "backup: "<< backup << "\n";
    cout << "----F\n";
    */

    // Se todas as comparações forem iguais, as matrizes são iguais
    return diferenca;
}

void ZerarMatrix(float **matrix, int rows, int cols) {/*
    gpu_selector sel;
    queue qcmk(sel);
    buffer<float, 2> matrix_buff(matrix[0], range<2>(rows, cols));

    qcmk.submit([&](handler& ker){
        accessor matrix_ac{matrix_buff, ker};
        ker.parallel_for(range<2>(rows, cols), [=](item<2> idx) {
            //int i = idx.get_id(0);
            //int j = idx.get_id(1);
            matrix_ac[idx] = 0.0;
        });
    }).wait();
    host_accessor matrix_r{matrix_buff};*/

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = 0.0;
        }
    }
} // Paralelizar

void CentrMK( int ncent, int atributos, int nista, float **centros, float **instancias){ // Paralelo
    //cpu_selector sel;
    gpu_selector sel;
    queue qcmk(sel);
    int random;
    buffer<float, 2> instancias_buff(instancias[0], range<2>(nista, atributos));
    buffer<float, 2> centros_buff(centros[0], range<2>(ncent, atributos));
    

    srand(0);
    for (int i = 0; i < ncent; i++)
    {
        random = rand() % nista;

        qcmk.submit([&](handler& ker){
            accessor instancias_ac{instancias_buff, ker};
            accessor centros_ac{centros_buff, ker};

            ker.parallel_for(range<1>(atributos), [=](item<1> j) {
                int linha = j.get_id(0);
                centros_ac[i][j] = instancias_ac[random][j];
            });
        });
        qcmk.wait();
        host_accessor instancias_r{instancias_buff};
        host_accessor centros_r{centros_buff};

        /*

        for (int j = 0; j < atributos; j++)
        {
            //centros[i][j] = instancias_r[random][j];
            cout << random << " Ins: " << instancias_r[random][j] << "\n";
        }
        cout << "\n";*/
    }
    //return centros;
}

bool AtrEnt(int nista, int atributos, float **instancias){ // LINEAR
    std::ifstream arquivo("./IRIS.csv"); // Abra o arquivo para leitura
    std::string valorString = "";
    int contAtrb = 0;
    int indexInsta = 0;
    int j = 0;

    if (!arquivo) { // Verifique se o arquivo foi aberto com sucesso
        std::cerr << "Não foi possível abrir o arquivo." << std::endl;
        return false;
    }

    std::string linha;
    std::getline(arquivo, linha);

    //cpu_selector sel;
    gpu_selector sel;
    queue qcmk(sel);
    buffer<float, 2> instancias_buff(instancias[0], range<2>(nista, atributos));
    host_accessor instancias_r{instancias_buff};

    while (std::getline(arquivo, linha)) { // Leia linha por linha
        j = 0;
        for (int i = 0; i < 4; i++)
        {
            while (linha[j] != ',')
            {
                valorString += linha[j];
                j++;
                //cout << " " << valorString;
            }
            j++;
            instancias_r[indexInsta][i] = std::stof(valorString);
            valorString = "";
        }indexInsta++;
        //cout << "\n";
    }
    
    

    arquivo.close();
    return true;
}

void Saida(int nista, float* indexProximidade){
    string nomeArquivo = "./IRIS_outParalelo.txt";
    ofstream arquivo(nomeArquivo);
    string texto = "";
    
    buffer<float, 1> indexProximidade_buff(indexProximidade, range<1>(nista));
    host_accessor indexProximidade_r{indexProximidade_buff};
    
    
    // Verifica se o arquivo foi aberto com sucesso
    if (!arquivo) {
        cerr << "Não foi possível abrir o arquivo para escrita." << "\n";
    }else{
        for (int i = 0; i < nista; i++)// 
        {
            texto += to_string(static_cast<int>(indexProximidade_r[i])) + "\n";
            //cout << " " + to_string(i) + "_" + to_string(indexProximidade_r[i]) + "\n";
        }
        
        // Escreve o texto no arquivo
        arquivo << texto;
        arquivo.close();
        cout << "Dados gravados com sucesso em " << nomeArquivo << "\n";
    }
}

void AtrMK( int nista, int atributos, float **instancias){
    //cpu_selector sel;
    //gpu_selector sel;
    //queue qcmk(sel);

    for (int i = 0; i < nista; i++)
    {
        /*
        qcmk.submit([&](handler& ker){
            ker.parallel_for(range<1>(atributos), [=](auto j) {
                centros[i][j] = rand()%100;
            });
        });
        qcmk.wait();*/
        
        for (int j = 0; j < atributos; j++)
        {
            instancias[i][j] = rand()%100;
        }
        
    }
    //return instancias;
}

float elev(float a, float b){
    float aux = 1;
    for (int i = 0; i < b; i++)
    {
        aux = aux*a;
    }
    //cout << "ele: " << aux << "\n";
    return aux;
}


// ### FERRAMENTAS PARA USO ^ #########################################################################################################
// ###    --- CODIGO ---    v #########################################################################################################


void Distancia_Euclidiana(float** instancias, int nista, int atributos, float** centros, int ncent, float** distanciados){ // Paralelo.single_task
    float soma;

    gpu_selector sel;
    queue qcmk(sel);
    buffer<float, 2> instancias_buff(instancias[0], range<2>(nista, atributos));
    buffer<float, 2> centros_buff(centros[0], range<2>(ncent, atributos));
    buffer<float, 2> distanciados_buff(distanciados[0], range<2>(ncent, nista));
    buffer<float, 1> soma_buff(&soma, range<1>(1));

    qcmk.submit([&](handler& ker){
        accessor instancias_ac{instancias_buff, ker};
        accessor centros_ac{centros_buff, ker};
        accessor distanciados_ac{distanciados_buff, ker};
        accessor soma_ac{soma_buff, ker};

        ker.single_task([=]() {
            for (int i = 0; i < ncent; i++)
            {
                for (int j = 0; j < nista; j++)
                {
                    soma_ac[0] = 0;
                    for (int k = 0; k < atributos; k++) {
                        soma_ac[0] = soma_ac[0] + ((instancias_ac[j][k] - centros_ac[i][k]) * (instancias_ac[j][k] - centros_ac[i][k]));
                    }
                    distanciados_ac[i][j] = sqrt(soma_ac[0]);
                }
            }
        });
    }).wait();
    
    host_accessor instancias_r{instancias_buff};
    host_accessor centros_r{centros_buff};
    host_accessor distanciados_r{distanciados_buff};
    
/*
    for (int i = 0; i < ncent; i++)
    {
        for (int j = 0; j < nista; j++)
        {
            //soma = 0;
            for (int k = 0; k < atributos; k++)
            {
                //soma = soma + ((instancias_r[j][k] - centros_r[i][k]) * (instancias_r[j][k] - centros_r[i][k]));
            }
            //distanciados_r[i][j] = sqrt(soma);
            cout << ":: " << distanciados_r[i][j] << "\n";
        }
    }*/
}

void Agrupamento(float **distanciados, int ncent, int nista, float *indexProximidade){ // LINEAR
    float menorAux; // identifica as instancias com os centroides
    gpu_selector sel;
    queue qcmk(sel);
    
    buffer<float, 2> distanciados_buff(distanciados[0], range<2>(ncent, nista));
    buffer<float, 1> indexProximidade_buff(indexProximidade, range<1>(nista));
    
    host_accessor distanciados_r{distanciados_buff};
    host_accessor indexProximidade_r{indexProximidade_buff};

    for (int j = 0; j < nista; j++){
        menorAux = distanciados_r[0][j]; //2147483647.0;// INT_MAX
        indexProximidade_r[j] = 0; // Padrao
        for (int i = 0; i < ncent; ++i) {
            if (distanciados_r[i][j] < menorAux) {
                menorAux = distanciados_r[i][j];
                indexProximidade_r[j] = i;
            }
        }
    }

    /*
        menorAux = distanciados[0][j]; //2147483647.0;// INT_MAX
        indexProximidade[j] = 0; // Padrao
        for (int i = 0; i < ncent; ++i) {
            if (distanciados[i][j] < menorAux) {
                menorAux = distanciados[i][j];
                indexProximidade[j] = i;
            }
        }

    for (int i = 0; i < nista; i++)
    {
        cout << "idxP: " << indexProximidade[i] << "\n";
    }*/
    
}

void RemapeandoCentroid(float** instancias, int nista, int atributos, float** centros, int ncent, float* indexProximidade){ // LINEAR
    //gpu_selector sel;
    //queue qcmk1(sel);
    //queue qcmk2(sel);

    int auxCont;
    float **auxSoma = new float*[nista];
    for (int i = 0; i < nista; ++i) {
        auxSoma[i] = new float[atributos];
    }
    
    //float auxSoma[nista][atributos] = {};
    ZerarMatrix(auxSoma, nista, atributos);

    buffer<float, 2> instancias_buff(instancias[0], range<2>(nista, atributos));
    buffer<float, 2> centros_buff(centros[0], range<2>(ncent, atributos));
    buffer<float, 1> indexProximidade_buff(indexProximidade, range<1>(nista));
    host_accessor indexProximidade_r{indexProximidade_buff};
    //buffer<float, 2> auxSoma_buff(auxSoma[0], range<2>(nista, atributos));
    buffer<int, 1> auxCont_buff(&auxCont, range<1>(1));
    
    
/*
    for (int i = 0; i < ncent; i++)// média dos atributos -- Linear
    {
        auxCont = 0;
        for (int j = 0; j < nista; j++) // Linear
        {
            if (indexProximidade_r[j] == i)
            {
                //qcmSomar(auxSoma, instancias, nista, atributos, i, j);
                auxCont++;
            }
        }
        if (auxCont != 0)
        {
            //qcmDiv(centros, ncent, atributos, auxSoma, nista, auxCont, i);
        }
    }*/

    host_accessor instancias_r{instancias_buff};
    host_accessor centros_r{centros_buff};
    //host_accessor auxSoma_r{auxSoma_buff};
    host_accessor auxCont_r{auxCont_buff};
    
    
    

    for (int i = 0; i < ncent; i++)// média dos atributos -- Linear
    {
        auxCont_r[0] = 0;
        for (int j = 0; j < nista; j++) // Linear
        {
            //cout << j << " indexProximidade_r " << indexProximidade_r[j] << "  i " << i << "\n";
            
            if (indexProximidade_r[j] == i)
            {
                for (int k = 0; k < atributos; k++) // || PArale_1
                {
                    //cout << " " << (auxSoma[i][k] + instancias_r[j][k]) << " = " << auxSoma[i][k] << " + " << instancias_r[j][k] << "\n";
                    auxSoma[i][k] = auxSoma[i][k] + instancias_r[j][k];
                }
                auxCont_r[0]++;
            }
        }
        //cout << "auxCont_r[0]: " << auxCont_r[0] << "\n";
        if (auxCont_r[0] != 0)
        {
            for (int l = 0; l < atributos; l++)// Atualiza centroides || PArale_2
            {
                //cout << " " << (auxSoma[i][l] / auxCont_r[0]) << " = " << auxSoma[i][l] << " / " << auxCont_r[0] << "\n";
                centros_r[i][l] = auxSoma[i][l] / auxCont_r[0];
            }
        }
    }
    //delete[] auxSoma;
    
}

int main(){
    int nista;// numero de instancias
    int ncent;// numero de centroides
    int atributos;// numero de atributos
    cout << "Quantidade de atributos: ";
    //cin >> atributos;
    cout << "\nQuantas instacias serão criadas: ";
    //cin >> nista;
    cout << "\nQuantas centroides serão usados: \n";
    //cin >> ncent;

    atributos = 4;// Para teste
    nista = 150;
    ncent = 3;

    //int instancias[nista][atributos];
    float **instancias = new float*[nista]; // matriz de instancias
    for (int i = 0; i < nista; ++i) {
        instancias[i] = new float[atributos];
    }

    //int centros[ncent][atributos];
    float **centros = new float*[ncent];// matriz de centroides
    for (int i = 0; i < ncent; ++i) {
        centros[i] = new float[atributos];
    }

    //float distanciados[ncent][nista]
    float **distanciados = new float*[ncent];// miatrix com a distancia(Euclidiana) dos centroides de cada instancia 
    for (int i = 0; i < ncent; ++i) {
        distanciados[i] = new float[nista];
    }

    //int centros[ncent][atributos];
    float **centrosAnteriores = new float*[ncent];// matriz de centroides anteriores
    for (int i = 0; i < ncent; ++i) {
        centrosAnteriores[i] = new float[atributos];
    }
    
    
    //AtrMK(nista, atributos, instancias); // cria instancias aleatorias
    if (AtrEnt(nista, atributos, instancias)){

        CentrMK(ncent, atributos, nista, centros, instancias); // cria centros na posição de uma instacia aleatoria
        // Usa volores de entrada para as instancias

        float *indexProximidade = new float [nista];// array auxiliar que guarda a que centroide pertence alguma instancia
        // pre operação - https://www.youtube.com/watch?v=njRYKzRKBPY
//int tt = 0;
        do
        {
            //ZerarMatrix(centrosAnteriores, ncent, atributos);
            //cout << "\nFALGf1 - "<< tt << "\n";
            ClonarMatriz(centros, centrosAnteriores, ncent, atributos);
            Distancia_Euclidiana(instancias, nista, atributos, centros, ncent, distanciados);
            Agrupamento(distanciados, ncent, nista, indexProximidade);
            
            RemapeandoCentroid(instancias, nista, atributos, centros, ncent, indexProximidade);
            //cout << "FALGf2 - "<< tt << "\n";
            //tt++;
        } while (compararMatrizes(centros, centrosAnteriores, ncent, atributos));// Criterio de parada: quando os centroides não se moverem mais
        Saida(nista, indexProximidade);

    }
    return 0;
}

