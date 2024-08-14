#include <iostream>
#include <math.h>
#include <ctime>

class Rede {
public:
	int nEntradas, nSaidas, nHidden;
	double **hidden;
	double *saidas;
	
	Rede(int nEntradas, int nSaidas, int nHidden) {
		this->nEntradas = nEntradas;
		this->nSaidas = nSaidas;
	    this->nHidden = nHidden;
		
		this->hidden = (Neuronio**)malloc((nEntradas+ 1) * sizeof(Neuronio*));
	    for(int i = 0; i < nEntradas + 1; i++) {
				this->hidden[i] = (Neuronio*)malloc((nHidden + 1) * sizeof(Neuronio));
		}
		
		this->saidas = (Neuronio*)malloc((nSaidas) * sizeof(Neuronio));
		
	}
	
	/*void initWeights() {
			for(int i = 0; i < this->nEntradas + 1; i++) {
					for(int j = 0; j < this->nHidden + 1; j++) {
						this->hidden[i][j] = rand() % 100 / 100.0;
						std::cout << hidden[i][j] << " ";
				    } 
				    std::cout << std::endl;
			}							
			
			for(int i = 0; i < this->nHidden + 1; i++) {
					for(int j = 0; j < this->nSaidas + 1; j++) {
						this->saidas[i][j] = rand() % 100 / 100.0;
						std::cout << saidas[i][j] <<	 " ";
				    } 
				    std::cout << std::endl;
			}
	}*/
	
	double sum(double **entrada) {
		double **resp = (double**)malloc((nSaidas + 1) * sizeof(double*));
		for(int i = 0; i < nEntradas + 1; i++) 
			resp[i] = (double*)malloc((nEntradas + 1) * sizeof(double));
			
		for(int i = 0; i < nEntradas + 1; i++) {
			for(int j = 0; j < nHidden + 1; j++) {
					resp += hidden[i][j] * entradas[i][j];
			}
		}
		return resp;
	}
	
	void backPropagate(){
		for(int i = nHidden;i>-1;i--){
			 double* layer = hidden[i]
			}
		}
	
	void feedForward() {
			
	}
	
};

class Neuronio {
public:
	int entradas, erro;
	double *pesos;
	double saida;
	
	Neuronio (int entradas) {
			this->entradas = entradas;
			this->saidas = saidas;
			this->pesos = (double*)malloc((entradas + 1)* sizeof(double));
			for(int i = 0; i < 2; i++) {
				pesos[i] = rand() % 100 / 100.0;
				// std::cout << pesos[i] << std::endl;
			}
			this->saida = (double*)malloc((entradas + 1)* sizeof(double));
	}
	
	double sum(double* entrada){
			double resp = 0.;
			for(int i = 0;i<entradas;i++){
				resp+=pesos[i]*entrada[i];
				}
				return sigmoid(resp);
	}
	
	double sigmoid(double saida){
		return (1/(1-exp(-saida)));
	}
	
	double derivative(double saida){
		return (saida*(1.0-saida));
		}
	
	void calcErro(double saida, double saidaD){
		erro =  ((saida-saidaD)	* derivative(saida));
	}
};

int main() {
		srand(time(nullptr));
		Neuronio *n = new Neuronio(2, 1);
		Rede *z = new Rede(10, 10, 1);
		return 0;
}
