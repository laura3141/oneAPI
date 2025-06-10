#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;


    static const int k = 2; //numero de clusters
    static const int n_instancias = 49096; //numero de instancias
    static const int n_atributos = 122;

    int c0=0;//instancias pertencentes ao cluster 0
    int c1=0;//instancias pertencentes ao cluster 1

    double instancias[n_instancias][n_atributos]; //array de instancias
    double centros [k][n_atributos-1]; //array de centroides
    


    //Procedimento para ler o csv e armazenar os dados no array de instancias
    void cria_instancias(){
        int c_instancias = 0; 

        string linha;
        string aux;
        std::ifstream arquivo("Trat2.csv");
        if (!arquivo.is_open()) { 
            std::cerr << "Não foi possível abrir o arquivo." << std::endl;
            return;
        }

        getline(arquivo, linha);

        while (getline(arquivo, linha)) {
            int p = 0;
            for (int i = 0; i < 121; i++) {
                aux = ""; 
                while (p < linha.size() && linha[p] != ',') {
                    aux += linha[p];
                    p++;
                }
                instancias[c_instancias][i] = std::stod(aux);
                p++; 
            }
            c_instancias++;
        }

        arquivo.close();
    }

    //Procedimento para inicializar os centroides com valores aleatórios 
    void inicializa_centros(){

        srand(0);
        for (int i = 0; i < k; i++) {
            
            int indicie = rand()%n_instancias;
            for (int j = 0;j < 121;j++){
                centros[i][j] = instancias[indicie][j];
            }
        }

    }

    //Procedimento para calcular a distancia de um ponto para todos os centroides 
    void calcula_distancia(int id){
        
        double menor = INFINITY;
        double distancia;

        //PARALELO
        for(int j = 0; j < 2;j++){
            double soma = 0;

            for(int k = 0; k<121;k++){
                soma += ((instancias[id][k]-centros[j][k]) * (instancias[id][k]-centros[j][k]));
            }
            distancia = std::sqrt(soma);
            if(distancia < menor){    
                menor = distancia;
                instancias[id][121]=j;
            }
        }
    }

    void atribui_cluster(){
        c0=0;
        c1=0;
        
        //PARALELO
        for(int i=0;i<n_instancias;i++){
            calcula_distancia(i);
            if(instancias[i][121]==0)c0++;
            else c1++;
        }
    }

    void atualiza_centros(){
        double soma0[121] = {0};
        double soma1[121] = {0};

        for(int i=0;i<n_instancias;i++){
            if(instancias[i][121]==0){
                for(int j=0;j<121;j++){
                    soma0[j]+=instancias[i][j];
                    
                }
            }
            else{
                for(int j=0;j<121;j++){
                    soma1[j]+=instancias[i][j];
                    
                }
            }
        }
        
        for(int j=0;j<121;j++){
            soma0[j]=soma0[j]/c0;
            soma1[j]=soma1[j]/c1;

        }
        
        for(int j=0;j<121;j++){
            centros[0][j]=soma0[j];
        }
        for(int i=0;i<121;i++){
            centros[1][i]=soma1[i];
        }
        
    }

    void kmeans(){
        cria_instancias();
        inicializa_centros();

        for(int i=0;i<100;i++){
            atribui_cluster();
            atualiza_centros();
        }
    }

int main (){
    
    kmeans();
    for(int i=0;i<n_instancias;i++){
        cout<<instancias[i][121]<<std::endl;
    }
}
