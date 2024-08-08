#include <iostream>
#include <CL/sycl.hpp>
#include <random>
#include <cmath>

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

bool compararMatrizes(float **matriz1, float **matriz2, int linhas, int colunas) {
    // Verificar se as matrizes têm o mesmo número de linhas e colunas
    if (linhas != linhas || colunas != colunas) {
        return true;
    }

    // Comparar cada elemento das matrizes
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            if (matriz1[i][j] != matriz2[i][j]) {
                return true;
            }
        }
    }

    // Se todas as comparações forem iguais, as matrizes são iguais
    return false;
}

void ClonarMatriz(float **matriz1, float **matriz2, int rows, int cols){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matriz2[i][j] = matriz1[i][j];
        }
    }
}

void ZerarMatrix(float **matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}


void CentrMK( int ncent, int atributos, int nista, float **centros, float **instancias){
    //cpu_selector sel;
    //gpu_selector sel;
    //queue qcmk(sel);
    int random;

    for (int i = 0; i < ncent; i++)
    {
        /*
        qcmk.submit([&](handler& ker){
            ker.parallel_for(range<1>(atributos), [=](auto j) {
                centros[i][j] = rand()%100;
            });
        });
        qcmk.wait();*/
        random = rand()%nista -1;

        for (int j = 0; j < atributos; j++)
        {
            centros[i][j] = instancias[random][j];
        }
        
    }
    //return centros;
}

bool AtrEnt(int nista, int atributos, float **instancias){
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
    while (std::getline(arquivo, linha)) { // Leia linha por linha

        j = 0;
        for (int i = 0; i < 4; i++)
        {
            while (linha[j] != ',')
            {
                valorString += linha[j];
                j++;
            }
            j++;
            instancias[indexInsta][i] = std::stof(valorString);
            valorString = "";
        }indexInsta++;
    }
/*
    for (int  i = 0; i < nista; i++)
    {
        for (int j = 0; j < atributos; j++)
        {
            cout << "instancia: " << i << "-" << j << " = " << instancias[i][j] << "\n";
        }
        
    }*/
    

    arquivo.close();
    return true;
}

void Saida(int nista, float* indexProximidade){
    string nomeArquivo = "./IRIS_out.txt";
    ofstream arquivo(nomeArquivo);
    string texto = "";
    
    // Verifica se o arquivo foi aberto com sucesso
    if (!arquivo) {
        cerr << "Não foi possível abrir o arquivo para escrita." << "\n";
    }else{
        for (int i = 0; i < nista; i++)// 
        {
            texto += " [" + to_string(i) + "]= " + to_string(static_cast<int>(indexProximidade[i])) + "\n";
            //cout << " " + to_string(i) + "_" + to_string(indexProximidade[i]) + "\n";
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


void Distancia_Euclidiana(float** instancias, int nista, int atributos, float** centros, int ncent, float** distanciados){ // Manhattan
    //gpu_selector selD;
    //queue q(selD);

    //float *aux = malloc_shared<float>(1, q);
    //aux[0] = 1;
    float soma;
    //float *somaV = malloc_shared<float>(atributos, q);

    for (int i = 0; i < ncent; i++)
    {
        for (int j = 0; j < nista; j++)
        {
            soma = 0;
            for (int k = 0; k < atributos; k++)
            {
                soma = soma + elev(instancias[j][k] - centros[i][k], 2);
            }
            distanciados[i][j] = sqrt(soma);
        }
        
    }

    cout << ": " << distanciados[0][0] << " " << distanciados[1][0] <<  " " << distanciados[2][0] <<  " " << distanciados[3][0] << "\n";
    cout << ": " << distanciados[0][1] << " " << distanciados[1][1] <<  " " << distanciados[2][1] <<  " " << distanciados[3][1] << "\n";
    cout << ": " << distanciados[0][2] << " " << distanciados[1][2] <<  " " << distanciados[2][2] <<  " " << distanciados[3][2] << "\n";
    cout << ": " << distanciados[0][3] << " " << distanciados[1][3] <<  " " << distanciados[2][3] <<  " " << distanciados[3][3] << "\n";
    cout << ": " << distanciados[0][4] << " " << distanciados[1][4] <<  " " << distanciados[2][4] <<  " " << distanciados[3][4] << "\n";
    cout << ": " << distanciados[0][5] << " " << distanciados[1][5] <<  " " << distanciados[2][5] <<  " " << distanciados[3][5] << "\n";
    
/*
    for (int i = 0; i < ncent; i++)
    {
        for (int j = 0; j < nista; j++)
        {
            soma = 0;
            for (int k = 0; k < atributos; k++)
            {
                soma = soma + elev(instancias[j][k] - centros[i][k], 2);
            }
            distanciados[i][j] = sqrt(soma);
        }
        
    }*/
    
}

void Agrupamento(float **distanciados, int ncent, int nista, float *indexProximidade){// identifica as instancias com os centroides
    float menorAux;
    //int indexProximidade;

    for (int j = 0; j < nista; j++){
        menorAux = 2147483647.0;// INT_MAX
        for (int i = 0; i < ncent; ++i) {
            if (distanciados[i][j] < menorAux) {
                menorAux = distanciados[i][j];
                indexProximidade[j] = i;
            }
        }

    }
}

void RemapeandoCentroid(float** instancias, int nista, int atributos, float** centros, int ncent, float* indexProximidade){
    int auxCont;
    float **auxSoma = new float*[nista];
    for (int i = 0; i < nista; ++i) {
        auxSoma[i] = new float[atributos];
    }
/*
    cout << "0= " << centros[0][0] << "\n";
    cout << "0= " << centros[0][1] << "\n";
    cout << "1= " << centros[1][0] << "\n";
    cout << "1= " << centros[1][1] << "\n";
    cout << "2= " << centros[2][0] << "\n";
    cout << "2= " << centros[2][1] << "\n==\n";*/

    //float auxSoma[nista][atributos] = {};
    ZerarMatrix(auxSoma, nista, atributos);

    for (int i = 0; i < ncent; i++)// média dos atributos
    {
        auxCont = 0;
        for (int j = 0; j < nista; j++)
        {
            cout << j << " indexProximidade " << indexProximidade[j] << "  i " << i << "\n";
            if (indexProximidade[j] == i)
            {
                for (int k = 0; k < atributos; k++)
                {
                    cout << " " << (auxSoma[i][k] + instancias[j][k]) << " = " << auxSoma[i][k] << " + " << instancias[j][k] << "\n";
                    auxSoma[i][k] = auxSoma[i][k] + instancias[j][k];
                }
                auxCont++;
            }
        }
        if (auxCont != 0)
        {
            for (int l = 0; l < atributos; l++)// Atualiza centroides
            {
                cout << " " << (auxSoma[i][l] / auxCont) << " = " << auxSoma[i][l] << " / " << auxCont << "\n";
                centros[i][l] = auxSoma[i][l] / auxCont;
            }
        }
        
    }
    /*
    for (int i = 0; i < ncent; i++) // Atualiza centroides
    {
        for (int j = 0; j < atributos; j++)
        {
            centros[i][j] = auxSoma[i][j];
        }
    }*/
   /*
    cout << "0= " << centros[0][0] << "\n";
    cout << "0= " << centros[0][1] << "\n";
    cout << "1= " << centros[1][0] << "\n";
    cout << "1= " << centros[1][1] << "\n";
    cout << "2= " << centros[2][0] << "\n";
    cout << "2= " << centros[2][1] << "\n==\n";*/
    
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
    ncent = 4;

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

        /*
        instancias[0][0] = 1;// Para teste
        instancias[0][1] = 2;

        instancias[1][0] = 1.5;
        instancias[1][1] = 1.8;

        instancias[2][0] = 5;
        instancias[2][1] = 8;

        instancias[3][0] = 8;
        instancias[3][1] = 8;

        instancias[4][0] = 1;
        instancias[4][1] = 0.6;

        instancias[5][0] = 9;
        instancias[5][1] = 11;

        instancias[6][0] = 8;
        instancias[6][1] = 2;

        instancias[7][0] = 10;
        instancias[7][1] = 2;

        instancias[8][0] = 9;
        instancias[8][1] = 3;*/

        /*centros[0][0] = 5.1; Padrao Laura
        centros[0][1] = 3.5;
        centros[0][2] = 1.4;
        centros[0][3] = 0.2;

        centros[1][0] = 4.9;
        centros[1][1] = 3;
        centros[1][2] = 1.4;
        centros[1][3] = 0.2;

        centros[2][0] = 4.7;
        centros[2][1] = 3.2;
        centros[2][2] = 1.3;
        centros[2][3] = 0.2;

        centros[3][0] = 4.6;
        centros[3][1] = 3.1;
        centros[3][2] = 1.5;
        centros[3][3] = 0.2;*/

        /*centros[0][0] = 7; //Padrao de canto:
        centros[0][1] = 0;
        centros[0][2] = 0;
        centros[0][3] = 0;

        centros[1][0] = 1;
        centros[1][1] = 4;
        centros[1][2] = 1;
        centros[1][3] = 1;

        centros[2][0] = 2;
        centros[2][1] = 2;
        centros[2][2] = 6;
        centros[2][3] = 2;

        centros[3][0] = 3;
        centros[3][1] = 3;
        centros[3][2] = 3;
        centros[3][3] = 2;*/
        


        float *indexProximidade = new float [nista];// array auxiliar que guarda a que centroide pertence alguma instancia
        // pre operação - https://www.youtube.com/watch?v=njRYKzRKBPY

        do
        {
            //ZerarMatrix(centrosAnteriores, ncent, atributos);
            ClonarMatriz(centros, centrosAnteriores, ncent, atributos);
            Distancia_Euclidiana(instancias, nista, atributos, centros, ncent, distanciados);
            Agrupamento(distanciados, ncent, nista, indexProximidade);
            RemapeandoCentroid(instancias, nista, atributos, centros, ncent, indexProximidade);
            
            cout << ": " << distanciados[0][0] << " " << distanciados[1][0] <<  " " << distanciados[2][0] <<  " " << distanciados[3][0] << "\n";
            cout << ": " << distanciados[0][1] << " " << distanciados[1][1] <<  " " << distanciados[2][1] <<  " " << distanciados[3][1] << "\n";
            cout << ": " << distanciados[0][2] << " " << distanciados[1][2] <<  " " << distanciados[2][2] <<  " " << distanciados[3][2] << "\n";
            cout << ": " << distanciados[0][3] << " " << distanciados[1][3] <<  " " << distanciados[2][3] <<  " " << distanciados[3][3] << "\n";
            cout << ": " << distanciados[0][4] << " " << distanciados[1][4] <<  " " << distanciados[2][4] <<  " " << distanciados[3][4] << "\n";
            cout << ": " << distanciados[0][5] << " " << distanciados[1][5] <<  " " << distanciados[2][5] <<  " " << distanciados[3][5] << "\n";
            cout << "0= " << centros[0][0] << "\n";
            cout << "0= " << centros[0][1] << "\n";
            cout << "1= " << centros[1][0] << "\n";
            cout << "1= " << centros[1][1] << "\n";
            cout << "2= " << centros[2][0] << "\n";
            cout << "2= " << centros[2][1] << "\n==\n";

        } while (compararMatrizes(centros, centrosAnteriores, ncent, atributos));// Criterio de parada: quando os centroides não se moverem mais

        /*
        cout << "\n Laura" << "\n";
        for(int i=0;i<9;i++){
            std::cout << "Ponto(" << instancias[i][0] << ", " << instancias[i][1] << ") está no cluster " << indexProximidade[i] << std::endl;
        }
        std::cout << "Ponto(" << instancias[55][0] << ", " << instancias[55][1] << ") está no cluster " << indexProximidade[55] << std::endl;
        */
        Saida(nista, indexProximidade);
        
        //cout << "M-l " << means;
        /*
        for (int i = 0; i < nista; i++)
        {
            cout << i <<" - " << means[i] << "\n";
        }*/
        
        //PrintMatrix(centros, ncent, atributos);
    }
    return 0;
}

