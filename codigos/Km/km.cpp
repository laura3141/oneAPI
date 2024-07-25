#include <iostream>
#include <CL/sycl.hpp>
#include <random>
#include <cmath>

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

void CentrMK( int ncent, int atributos, float **centros){
    //cpu_selector sel;
    //gpu_selector sel;
    //queue qcmk(sel);

    for (int i = 0; i < ncent; i++)
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
            centros[i][j] = rand()%100;
        }
        
    }
    //return centros;
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


// ### FERRAMENTAS PARA USO ^ ###
// ###    --- CODIGO ---    v ###


void Distancia_Euclidiana(float** instancias, int nista, int atributos, float** centros, int ncent, float** distanciados){
    float aux;
    float soma;
/*
    gpu_selector selD;
    queue qcmk(selD);*/

    
    

    for (int i = 0; i < ncent; i++)
    {
        for (int j = 0; j < nista; j++)
        {
            soma = 0;
            for (int k = 0; k < atributos; k++)
            {
                
                aux = instancias[j][k] - centros[i][k];
                soma = soma + elev(aux, 2);
                
            }
            aux = sqrt(soma);
            distanciados[i][j] = aux;
        }
        
    }/*
        ker.parallel_for(range<1>(atributos), [=](auto j) {
            centros[i][j] = rand()%100;
        });*/


    //qcmk.wait(); ///Fim do Kernel
/*
    for (int i = 0; i < ncent; i++)
    {
        for (int j = 0; j < nista; j++)
        {
            soma = 0;
            for (int k = 0; k < atributos; k++)
            {
                
                aux = instancias[j][k] - centros[i][k];
                soma = soma + elev(aux, 2);
                
            }
            aux = sqrt(soma);
            distanciados[i][j] = aux;
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

    //float auxSoma[nista][atributos] = {};
    ZerarMatrix(auxSoma, nista, atributos);

    for (int i = 0; i < ncent; i++)// média dos atributos
    {
        auxCont = 0;
        for (int j = 0; j < nista; j++)
        {
            if (indexProximidade[j] == i)
            {
                for (int k = 0; k < atributos; k++)
                {
                    auxSoma[i][k] = auxSoma[i][k] + instancias[j][k];
                }
                auxCont++;
            }
        }
        for (int l = 0; l < atributos; l++)// Atualiza centroides
        {
            centros[i][l] = auxSoma[i][l] / auxCont;
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
    cout << "0= " << centros[0][0] << "\n";
    cout << "0= " << centros[0][1] << "\n";
    cout << "1= " << centros[1][0] << "\n";
    cout << "1= " << centros[1][1] << "\n==\n";
    
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

    atributos = 2;// Para teste
    nista = 4;
    ncent = 2;

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
    
    CentrMK(ncent, atributos, centros); // cria centros aleatorios
    AtrMK(nista, atributos, instancias); // cria instancias aleatorias

    instancias[0][0] = 1;// Para teste
    instancias[0][1] = 2;

    instancias[1][0] = 2;
    instancias[1][1] = 3;

    instancias[2][0] = 3;
    instancias[2][1] = 1;

    instancias[3][0] = 3;
    instancias[3][1] = 2;

    centros[0][0] = 1;
    centros[0][1] = 2;

    centros[1][0] = 3;
    centros[1][1] = 2;

    float *indexProximidade = new float [nista];// array auxiliar que guarda a que centroide pertence alguma instancia
    // pre operação - https://www.youtube.com/watch?v=njRYKzRKBPY

    do
    {
        //ZerarMatrix(centrosAnteriores, ncent, atributos);
        ClonarMatriz(centros, centrosAnteriores, ncent, atributos);
        Distancia_Euclidiana(instancias, nista, atributos, centros, ncent, distanciados);
        Agrupamento(distanciados, ncent, nista, indexProximidade);
        RemapeandoCentroid(instancias, nista, atributos, centros, ncent, indexProximidade);
        
        cout << ": " << distanciados[0][0] << " " << distanciados[1][0] << "\n";
        cout << ": " << distanciados[0][1] << " " << distanciados[1][1] << "\n";
        cout << ": " << distanciados[0][2] << " " << distanciados[1][2] << "\n";
        cout << ": " << distanciados[0][3] << " " << distanciados[1][3] << "\n=====\n";

    } while (compararMatrizes(centros, centrosAnteriores, ncent, atributos));// Criterio de parada: quando os centroides não se moverem mais
    
    
    //cout << "M-l " << means;
    /*
    for (int i = 0; i < nista; i++)
    {
        cout << i <<" - " << means[i] << "\n";
    }*/
    
    //PrintMatrix(centros, ncent, atributos);
    return 0;
}

