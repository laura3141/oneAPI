#include  <CL/sycl.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sycl;

const int k = 3; //numero de clusters
const int tam = 150; //tam do array de pontos

class Ponto{

    public: 

    double sepal_length,sepal_width,petal_length,petal_width;
    int cluster;

    Ponto(){

    }

    Ponto(double sepal_length, double sepal_width, double petal_length, double petal_width){

        this->sepal_length = sepal_length;
        this->sepal_width = sepal_width;
        this->petal_length = petal_length;
        this->petal_width= petal_width;
        this->cluster = -1;
        
    }
};

void cria_instancias(Ponto instancias[tam]) {

    std::string linha;
    std::string aux;
    double atributos[4];
    int c_instancias = 0;

    std::ifstream arquivo("./IRIS.csv");
    if (!arquivo) { // Verifique se o arquivo foi aberto com sucesso
        std::cerr << "Não foi possível abrir o arquivoo." << std::endl;
        return;
    }
    std::getline(arquivo, linha);

    while (std::getline(arquivo, linha)) {
        int p = 0;
        for (int i = 0; i < 4; i++) {
            aux = ""; // Limpa aux no início de cada iteração
            while (p < linha.size() && linha[p] != ',') {
                aux += linha[p];
                p++;
            }
            atributos[i] = std::stod(aux);
            p++; 
        }

        instancias[c_instancias].sepal_length = atributos[0];
        instancias[c_instancias].sepal_width = atributos[1];
        instancias[c_instancias].petal_length = atributos[2];
        instancias[c_instancias].petal_width = atributos[3];
        c_instancias++;
    }

    arquivo.close();
}

void inicializa_centros(Ponto clusters[k], Ponto pontos[tam]){
    //PARALELIZAVEL ??
    srand(0);
    for (int i = 0; i < k; i++) {
        clusters[i] = pontos[rand() % tam];
    }
}

double calculo_distancia(Ponto p1, Ponto p2) {
    return std::sqrt((p1.sepal_length - p2.sepal_length) * (p1.sepal_length - p2.sepal_length) +
                     (p1.sepal_width - p2.sepal_width) * (p1.sepal_width - p2.sepal_width) +
                     (p1.petal_length - p2.petal_length) * (p1.petal_length - p2.petal_length) +
                     (p1.petal_width - p2.petal_width) * (p1.petal_width - p2.petal_width));
}


void atribuir_cluster(Ponto pontos[tam], Ponto clusters[k]){

    cpu_selector seletor;
    queue fila(seletor);

    auto escopo_pontos = range<1>(tam);
    auto escopo_clusters= range<1>(k);

    buffer buf_clusters(clusters,escopo_clusters);
    buffer buf_pontos(pontos,escopo_pontos);
    
    fila.submit([&](handler &h){
        
        accessor acc_clusters(buf_clusters,h,read_only);
        accessor acc_pontos(buf_pontos,h,read_write);

        h.parallel_for(escopo_pontos,[=](id <1>i ){
            
            Ponto p1 = acc_pontos[i];
            Ponto p2 = acc_clusters[0];

            double menor_distancia= calculo_distancia(p1,p2);
            double distancia_atual;
            int novo_cluster = 0;

            for(int j=1;j<k;j++){

                Ponto p3 = acc_clusters[j];
                
                distancia_atual = calculo_distancia(p1,p3);
                
                if(distancia_atual<menor_distancia){
                    menor_distancia=distancia_atual;
                    novo_cluster = j;
                }
        }
        acc_pontos[i].cluster=novo_cluster;
        
        });
    }).wait();
}

void test(Ponto novos_clusters[k],Ponto soma_por_cluster[k], int quantidade_por_cluster[k], Ponto clusters[k]){

    for(int i=0; i<k;i++){
        if(quantidade_por_cluster>0){
            novos_clusters[i].sepal_length = soma_por_cluster[i].sepal_length/quantidade_por_cluster[i];
            novos_clusters[i].sepal_width = soma_por_cluster[i].sepal_width/quantidade_por_cluster[i];
            novos_clusters[i].petal_width = soma_por_cluster[i].petal_width/quantidade_por_cluster[i];
            novos_clusters[i].petal_length = soma_por_cluster[i].petal_length/quantidade_por_cluster[i];
        }
        
    }

    for (int i = 0; i < k; ++i) {

        clusters[i] = novos_clusters[i];
    }
}

void atualiza_centros(Ponto pontos[tam], Ponto clusters[k]){

    Ponto novos_clusters[k];
    Ponto soma_por_cluster[k];
    int quantidade_por_cluster[k];

    for (int i = 0; i < k; i++) {
        soma_por_cluster[i] = Ponto(0, 0,0,0);
        quantidade_por_cluster[i] = 0;
    }

    cpu_selector seletor;
    queue fila(seletor);

    auto escopo_pontos = range<1>(tam);
    auto escopo_clusters = range<1>(k);
    

    buffer buf_clusters(clusters,escopo_clusters);
    buffer buf_pontos(pontos,escopo_pontos);
    buffer buf_novos_clusters(novos_clusters,escopo_clusters);
    buffer buf_soma(soma_por_cluster,escopo_clusters);
    buffer buf_quantidade(quantidade_por_cluster, escopo_clusters);


    fila.submit([&](handler &h){

        
        accessor acc_pontos(buf_pontos,h,read_only);
        accessor acc_soma(buf_soma,h,read_write);
        accessor acc_quantidade(buf_quantidade,h,read_write);

        h.parallel_for(escopo_pontos,[=](id <1>i ){
                
            acc_soma[acc_pontos[i].cluster].petal_length+= acc_pontos[i].petal_length;
            acc_soma[acc_pontos[i].cluster].petal_width+= acc_pontos[i].petal_width;
            acc_soma[acc_pontos[i].cluster].sepal_length+= acc_pontos[i].sepal_length;
            acc_soma[acc_pontos[i].cluster].sepal_width+= acc_pontos[i].sepal_width;
            acc_quantidade[acc_pontos[i].cluster]++;

        });
    }).wait();
    test(novos_clusters,soma_por_cluster,quantidade_por_cluster,clusters);
}

void kmeans(Ponto pontos[tam]){

    cria_instancias(pontos);
    Ponto clusters[k];
    //paralelizavel
    inicializa_centros(clusters,pontos);

    int c=0;
    while(c<1000){

        atribuir_cluster(pontos, clusters);
        atualiza_centros(pontos, clusters);
        c++;
    }
    
}

int main (){

    Ponto pontos[tam];
    kmeans(pontos);

    for(int i=0;i<tam;i++){
        std::cout << "Ponto" << i<<" está no cluster " << pontos[i].cluster << std::endl;
    }
}
