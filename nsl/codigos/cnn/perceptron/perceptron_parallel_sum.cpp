#include <CL/sycl.hpp>
#include <iostream>
#include <bitset>
#include <stdlib.h> 
#include <math.h>
using namespace cl::sycl;
using namespace std;
class Perceptron{
private:
	int entradas;
	double* pesos;
	int** tt;
	int* saidas;
	double tda;
	int epocas;
	public: 
	Perceptron(int e = 2, double tda = 0.3, int epocas = 1000){
	this->entradas = e+1;
	this->tda = tda;
	this->epocas = epocas;
	pesos = (double*)malloc((this->entradas)*sizeof(double));
	for(int i = 0;i<this->entradas;i++){
		pesos[i] = (double)1/(rand()%100);
	}
	}
	char ctoi(char c){
	return c-48;}
	
	char* toBin(int x){
 int l = entradas-1;
 char* resp = (char*)malloc((l+1)*sizeof(char)) ;
 resp[l]='\0';
 l--;
 while(x>0){
	 resp[l]=(x%2) + 48;
	 x/=2;
	 l--;
 }
 while(l>=0){
	 resp[l] = '0';
	 l--;
 }
return resp;
}
	void generate_tt(){
			int valor = 0; 
			char* binary = (char*)malloc((entradas-1)*sizeof(char));
			binary = toBin(valor); //to binary;
    int saida = ctoi(binary[0]);
		tt = (int**)malloc(pow(2,entradas-1)*sizeof(int*));
		this->saidas = (int*)malloc(pow(2,entradas-1)*sizeof(int));
		for(int i =0;i<pow(2,entradas-1);i++){
			tt[i] = (int*)malloc((entradas-1)*sizeof(int));
		}
		for(int i =0;i<pow(2,entradas-1);i++){
			saida = ctoi(binary[0]);
			for(int j = 0;j<entradas-1;j++){
				tt[i][j]=ctoi(binary[j]);
				saida = saida | (int)tt[i][j];
			}
			valor++;
			binary = toBin(valor);
			this->saidas[i]=saida;
			}
	}
	
	void print_tt(){
		for(int i =0;i<pow(2,entradas-1);i++){
			for(int j = 0;j<entradas-1;j++){
				cout<<tt[i][j] << " ";
				}
				cout << "Saida: " << saidas[i] << "\n";
			}
	}
	
	int somar(int* a, double* pesos, double* r, int i){
		double soma = 0.0;
		for(int j = 0; j<entradas-1;j++)
		{
			soma += pesos[j]*a[j];
		}
		soma += pesos[entradas-1];
		return soma > 0 ? 1:0;
	}
	
	double somar(int* a){
		double soma = 0.0;
		for(int i = 0;i<entradas-1;i++){
		soma +=a[i]*pesos[i];
		}
		soma+=pesos[entradas-1];
		soma = soma> 0? 1.0:0.0;
		return (int)soma;
	}
	
	void corrigir(int* a, double* pesos, int saida,int saidaD, double tda){
		sycl::queue myQueue(sycl::cpu_selector{});
		myQueue.submit([&](sycl::handler &h) {    		
    		h.parallel_for(range<1>(entradas-1), [=](auto i){
           	pesos[i] = pesos[i] + (tda*a[i]*(saidaD-saida));
        	});
    		}).wait();
		pesos[entradas-1] = pesos[entradas-1] + (tda*1*(saidaD-saida));
	}
		
	void trainBool(){
	generate_tt();
	//print_tt();
	train(tt,saidas,1000);}
	
	void train(int **amostras, int *saidas, int epocas ){
		int e = 0; 
		int saida = 0;
		double* pesos_ptr = this->pesos;
    		while(e<epocas){
    		for(int i = 0;i<pow(2,entradas-1);i++){
    		saida = somar(amostras[i]);
    		if(saida!=saidas[i]){
    		corrigir(amostras[i], pesos_ptr, saida, saidas[i], tda);
    		}
    		}
    		e++;
    		}
	}
	void predict(int* a){
	cout<< "Predição: "<<somar(a) << "\n";
	}
};
int main(){
	Perceptron x = Perceptron(22, 0.3,5000);
	//cout <<"AAAAAAAAAAAAAAAA";
	int e1[] = {0,0,0,0,0};
	int e2[] = {0,1,0,0,0};
	int e3[] = {1,0,0,0,0};
	int e4[] = {1,1,1,1,1};
	auto inicio = std::chrono::high_resolution_clock::now();
	x.trainBool();
	x.predict(e1);
	x.predict(e2);
	x.predict(e3);
	x.predict(e4);
	auto resultado = std::chrono::high_resolution_clock::now() - inicio;
long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(resultado).count();
    cout << "Time taken by program is : "
        << microseconds; 	//x.toBin(513);
}
    


