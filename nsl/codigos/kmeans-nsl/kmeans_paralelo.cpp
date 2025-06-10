#include  <CL/sycl.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace sycl;


    static const int k = 2; //numero de clusters

    static const int n_instancias = 49096; //numero de instancias
    static const int n_atributos = 121;

    int c0=0;//instancias pertencentes ao cluster 0
    int c1=0;//instancias pertencentes ao cluster 1

    
    size_t N_inst = n_instancias;
    size_t N_atr = n_atributos;

    std::vector<float> instancias(N_inst*121);
    std::vector<int> resultados(n_instancias);
    std::vector<float> centros(2*N_atr);
    
    //Procedimento para ler o csv e armazenar os dados no array de instancias
    void cria_instancias(){
        int c_instancias = 0; 

        string linha;
        string aux;
        std::ifstream arquivo("./bases/base_completa.csv");
        getline(arquivo, linha);
        
        while (getline(arquivo, linha)) {
            
            int p = 0;
            for (int j = 0; j< 121; j++) {
                
                aux = ""; 
                while (p < linha.size() && linha[p] != ',') {
                    aux += linha[p];
                    p++;
                }
                try {
                    instancias[c_instancias * n_atributos + j] = std::stof(aux);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Erro: Não foi possível converter '" << aux << "' para float. Motivo: " << e.what() << std::endl;
                    instancias[c_instancias * n_atributos + j] = 0.0f; // Define um valor padrão ou lida com o erro
                } catch (const std::out_of_range& e) {
                    std::cerr << "Erro: O valor '" << aux << "' está fora do intervalo para float. Motivo: " << e.what() << std::endl;
                    instancias[c_instancias * n_atributos + j] = 0.0f; // Define um valor padrão ou lida com o erro
                } catch (...) {
                    std::cerr << "Erro desconhecido ao converter '" << aux << "' para float." << std::endl;
                    instancias[c_instancias * n_atributos + j] = 0.0f; // Define um valor padrão ou lida com o erro
                }
                p++; 
            }
                c_instancias++;
            
        }

    }

    //Procedimento para inicializar os centroides com valores aleatórios 
    void inicializa_centros(){

        srand(4);
        for (int i = 0; i < k; i++) {
            
            int indicie = rand()%n_instancias;
            for (int j = 0;j < 121;j++){
                centros[i*n_atributos+j] = instancias[indicie*n_atributos+j];
            }
        }

    }

    void conta_instancias(){
        c0=0;
        c1=0;
        for(int i=0;i<n_instancias;i++){
            if(resultados[i]==0)c0++;
            else c1++;
        }
    }

    void atribui_cluster(){

        gpu_selector seletor;
        queue q(seletor);


        buffer instancia(instancias);
        buffer centro (centros);
        buffer resultado(resultados);

        range<1> global_size(n_instancias);
        
        q.submit([&](handler & h ){ 
        //acessadores para os buffers
        accessor A(instancia,h,read_only);
        accessor B(centro,h,read_only);
        accessor C(resultado,h,write_only);
        
            //kernel que sera executado
            h.parallel_for(range<1>{global_size},[=](id <1>i){

                float menor = INFINITY;
                float distancia;

                for(int j = 0; j < 2;j++){
                    float soma = 0;

                    for(int k = 0; k<121;k++){
                        soma += ((A[i*n_atributos+k]-B[j*n_atributos+k]) * (A[i*n_atributos+k]-B[j*n_atributos+k]));
                    }

                    distancia = soma;

                    if(distancia < menor){    
                        menor = distancia;
                        C[i]=j;
                    }
                }
                
            });
        });
        q.wait();
    }
    

    void atualiza_centros(){
        conta_instancias();
        
        

        float soma0[121] = {0};
        float soma1[121] = {0};

        for(int i=0;i<n_instancias;i++){
            if(resultados[i]==0){
                for(int j=0;j<121;j++){
                    soma0[j]+=instancias[i*n_atributos+j];
                    
                }
            }
            else{
                for(int j=0;j<121;j++){
                    soma1[j]+=instancias[i*n_atributos+j];
                    
                }
            }
        }
        
        for(int j=0;j<121;j++){
            soma0[j]=soma0[j]/c0;
            
            soma1[j]=soma1[j]/c1;

        }
        
        for(int j=0;j<121;j++){
            centros[j]=soma0[j];
        }
        for(int i=0;i<121;i++){
            centros[n_atributos+i]=soma1[i];
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
        cout<<resultados[i]<<std::endl;
    }
    
}
