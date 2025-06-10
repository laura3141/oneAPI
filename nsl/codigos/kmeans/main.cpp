#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

const int k = 3; //numero de clusters
const int tam = 9; //tam do array de pontos

using namespace std;

class Ponto{

    public: 
    double x,y;
    int cluster;

    Ponto(){

    }

    Ponto(double x,double y){
        this->x = x;
        this->y = y;
        this->cluster = -1;
    }
};

double calculo_distancia(Ponto p1, Ponto p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

void inicializa_centros(Ponto clusters[k], Ponto pontos[tam]){
    //PARALELIZAVEL
    srand(time(0));
    for (int i = 0; i < k; i++) {
        clusters[i] = pontos[rand() % tam];
    }
}

void atribuir_cluster(Ponto pontos[tam], Ponto clusters[k]){

    int menor_distancia;
    int distancia_atual;
    int novo_cluster;

    //paralelizavel
    for(int i=0;i<tam;i++){

        novo_cluster = 0;
        menor_distancia= calculo_distancia(pontos[i],clusters[0]);

        for(int j=1;j<k;j++){

            distancia_atual = calculo_distancia(pontos[i],clusters[j]);
            if(distancia_atual<menor_distancia){
                menor_distancia=distancia_atual;
                novo_cluster = j;
            }
        }
        pontos[i].cluster=novo_cluster;
    }
}

void atualiza_centros(Ponto pontos[tam], Ponto clusters[k]){

    Ponto novos_clusters[k] = {};
    Ponto soma_por_cluster[k] = {};
    int quantidade_por_cluster[k] = {};

    // Inicializar soma_por_cluster e quantidade_por_cluster com zeros
    for (int i = 0; i < k; i++) {
        soma_por_cluster[i] = Ponto(0, 0);
        quantidade_por_cluster[i] = 0;
    }

    for(int i=0; i<tam; i++){

        //independente
        soma_por_cluster[pontos[i].cluster].x+=pontos[i].x;
        soma_por_cluster[pontos[i].cluster].y+=pontos[i].y;
        quantidade_por_cluster[pontos[i].cluster]++;

    }

    for(int i=0; i<k;i++){
        novos_clusters[i].x = soma_por_cluster[i].x/quantidade_por_cluster[i];
        novos_clusters[i].y = soma_por_cluster[i].y/quantidade_por_cluster[i];
    }

    for (int i = 0; i < k; ++i) {

        clusters[i] = novos_clusters[i];
    }
}

void kmeans(Ponto pontos[tam]){

    Ponto clusters[k];
    //paralelizavel
    inicializa_centros(clusters,pontos);

    //max de iteraçoes
    for(int i=0; i<100; i++){

        atribuir_cluster(pontos, clusters);
        atualiza_centros(pontos, clusters);
    }

}

int main (){
    
    Ponto pontos[tam] = {
        {1.0, 2.0}, {1.5, 1.8}, {5.0, 8.0}, {8.0, 8.0},
        {1.0, 0.6}, {9.0, 11.0}, {8.0, 2.0}, {10.0, 2.0}, {9.0, 3.0}
    };

    kmeans(pontos);

    for(int i=0;i<tam;i++){
        std::cout << "Ponto(" << pontos[i].x << ", " << pontos[i].y << ") está no cluster " << pontos[i].cluster << std::endl;
    }
}
