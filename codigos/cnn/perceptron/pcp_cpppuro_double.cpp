#include <iostream>
#include <bitset>
#include <iostream>
#include <bitset>
#include <stdlib.h> 
#include <math.h>
#include <chrono>
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
	
	char* toBin(int x){
		int l = floor((log(x)/log(2))+1);
		char* resp = (char*)malloc(l*sizeof(char));
		l--;
		while(x>0){
			resp[l--] = (x%2)+48;
			x = x/2;
		}
		return resp;
	}
	
	char ctoi(char c){
	return c-48;}
	
	void generate_tt(){
			int valor = 0; 
		 std::string binary = std::bitset<2>(valor).to_string(); //to binary;
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
				saida = saida ^ (int)tt[i][j];
			}
			valor++;
			binary = std::bitset<2>(valor).to_string();
			this->saidas[i]=saida;
			}
	}
	
	void print_tt(){
		for(int i =0;i<pow(2,entradas-1);i++){
			for(int j = 0;j<entradas-1;j++){
				cout<<tt[i][j] << " ";
			}
			cout << "\n";
			}
	}
	
	void trainBool(){
		generate_tt();
	//print_tt();
		train(tt,saidas,epocas);}
	
	double somar(int* a){
	double soma = 0.0;
	for(int i = 0;i<entradas-1;i++){
		soma +=a[i]*pesos[i];
	}
	soma+=pesos[entradas-1];
	return soma;
	}
	
	void corrigir(int* a,int saidaD, double saidaC){
		for(int i = 0;i<entradas-1;i++){
			pesos[i] = pesos[i]+(tda*a[i]*(saidaD-saidaC));
		}
		pesos[entradas-1] = pesos[entradas-1] + (tda*(saidaD-saidaC)); 
	}
	
	void train(int** amostras,int*saidas,int epocas){
	int e = 0;
	int saida;
	while(e<epocas){
	for(int i = 0;i<pow(2,entradas-1);i++){
		saida = somar(amostras[i]);
		if(saida!=saidas[i]){
		corrigir(amostras[i],saidas[i],saida);
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
	Perceptron x = Perceptron(5, 0.3,100000);
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
