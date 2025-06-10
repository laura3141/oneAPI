#include  <CL/sycl.hpp>
#include <fstream>
#include <cmath>

using namespace std;
using namespace sycl;


    static const int k = 7; //numero de clusters

    static const int n_instancias = 2335310; //numero de instancias
    static const int n_atributos = 78;

    int c[7];

    
    size_t N_inst = n_instancias;
    size_t N_atr = n_atributos;

    std::vector<float> instancias(N_inst*N_atr);
    std::vector<int> resultados(n_instancias);
    std::vector<float> centros(7*N_atr);
    std::vector<int> labels(n_instancias);

    //Procedimento para ler o csv e armazenar os dados no array de instancias
    void cria_instancias(){
        int c_instancias = 0; 
    
        string linha, linha2;
        string aux;
    
        std::ifstream arquivo("dados_7l.csv");
        std::ifstream arquivo2("rotulos_7l.csv");
    
        getline(arquivo, linha);   // descarta header
        getline(arquivo2, linha2); // descarta header
    
        while (getline(arquivo, linha) && getline(arquivo2, linha2)) {
            labels[c_instancias] = std::stoi(linha2);
    
            int p = 0;
            for (int j = 0; j < n_atributos; j++) {
                aux = "";
                while (p < linha.size() && linha[p] != ',') {
                    aux += linha[p];
                    p++;
                }
    
                try {
                    instancias[c_instancias * n_atributos + j] = std::stof(aux);
                } catch (const std::exception& e) {
                    std::cerr << "Erro ao converter: '" << aux << "'. " << e.what() << std::endl;
                    instancias[c_instancias * n_atributos + j] = 0.0f;
                }   
    
                p++; // avança além da vírgula
            }
    
            c_instancias++;
        }

        printf("Instâncias carregadas: %d \n",c_instancias);
    }
    

    //Procedimento para inicializar os centroides com valores aleatórios 
    void inicializa_centros(){

        srand(0);

        for (int i = 0; i < k; i++) {
            
            int indicie = rand()%n_instancias;

            for (int j = 0;j < n_atributos;j++){
                centros[i*n_atributos+j] = instancias[indicie*n_atributos+j];
               

                cout<<centros[i*n_atributos+j]<<"  ";
            }

        }

    }

    void conta_instancias(){

        for(int i=0;i<7;i++){
            c[i]=0;
        }

        for(int i=0;i<n_instancias;i++){

            if(resultados[i]==0)c[0]++;
            else if(resultados[i]==1)c[1]++;
            else if(resultados[i]==2)c[2]++;
            else if(resultados[i]==3)c[3]++;
            else if(resultados[i]==4)c[4]++;
            else if(resultados[i]==5)c[5]++;
            else c[6]++;
        }
    }

    void atribui_cluster(){

        cpu_selector seletor;
        queue q(seletor);

        {

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

                    for(int j = 0; j < k;j++){
                        float soma = 0;

                        for(int n = 0; n<n_atributos;n++){
                            soma += ((A[i*n_atributos+n]-B[j*n_atributos+n]) * (A[i*n_atributos+n]-B[j*n_atributos+n]));
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
        //host_accessor sync_result(resultado, read_only);//  garante que o conteúdo do buffer resultado seja copiado de volta para resultados antes que
    }
    

    void atualiza_centros(){

        conta_instancias();

        float soma[k][n_atributos]= {0};

        for(int i=0;i<n_instancias;i++){

            if(resultados[i]==0){
                for(int j=0;j<n_atributos;j++){
                    soma[0][j]+=instancias[i*n_atributos+j];
                    
                }
            }
            else if(resultados[i]==1){
                for(int j=0;j<n_atributos;j++){
                    soma[1][j]+=instancias[i*n_atributos+j];
                    
                }
            }
            else if(resultados[i]==2){
                for(int j=0;j<n_atributos;j++){
                    soma[2][j]+=instancias[i*n_atributos+j];
                    
                }
            }
            else if(resultados[i]==3){
                for(int j=0;j<n_atributos;j++){
                    soma[3][j]+=instancias[i*n_atributos+j];
                    
                }
            }
            else if(resultados[i]==4){
                for(int j=0;j<n_atributos;j++){
                    soma[4][j]+=instancias[i*n_atributos+j];
                    
                }
            }
            else if(resultados[i]==5){
                for(int j=0;j<n_atributos;j++){
                    soma[5][j]+=instancias[i*n_atributos+j];
                    
                }
            }
        
            else{
                for(int j=0;j<n_atributos;j++){
                    soma[6][j]+=instancias[i*n_atributos+j];
                    
                }
            }
        }

        for(int i=0;i<7;i++){

            for(int j=0;j<n_atributos;j++){
                soma[i][j]=soma[i][j]/c[i];
                
                
    
            }
    
        }
        
        int p =0;
        for(int j=0;j<n_atributos*7;j++){
            if(j>0&&j%n_atributos==0){
                p++;
            }
            centros[j]=soma[p][j];
            

        }
    }

    void kmeans(){
        cria_instancias();
        inicializa_centros();
    
        for(int i=0;i<1;i++){
            atribui_cluster();
            atualiza_centros();
        }

    }

int main (){
    
    kmeans();
    for(int i=0;i<n_instancias;i++){
        printf("%d\n",resultados[i]);
    }
  
} 