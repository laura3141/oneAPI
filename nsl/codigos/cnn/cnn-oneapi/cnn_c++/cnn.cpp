#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <Eigen/Dense>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/init_rules/glorot_init.hpp>
#include <mlpack/methods/ann/activation_functions/relu_function.hpp>
#include <mlpack/core.hpp>

using namespace Eigen;
using namespace mlpack;
using namespace mlpack::ann;

// Função para carregar dados de um arquivo CSV
MatrixXd carregarDados(const std::string& caminho, int linhas, int colunas) {
    std::ifstream arquivo(caminho);
    std::string linha;
    MatrixXd dados(linhas, colunas);
    int linhaAtual = 0;

    while (std::getline(arquivo, linha) && linhaAtual < linhas) {
        std::stringstream linhaStream(linha);
        std::string valor;
        int colunaAtual = 0;

        while (std::getline(linhaStream, valor, ',') && colunaAtual < colunas) {
            dados(linhaAtual, colunaAtual) = std::stod(valor);
            colunaAtual++;
        }
        linhaAtual++;
    }
    return dados;
}

// Função para normalizar os dados
void normalizarDados(MatrixXd& dados) {
    VectorXd min = dados.colwise().minCoeff();
    VectorXd max = dados.colwise().maxCoeff();
    dados = (dados.rowwise() - min.transpose()).array().rowwise() / (max - min).transpose().array();
}

// Função para One-Hot Encoding
MatrixXd oneHotEncoding(const MatrixXd& dados, int colIdx, int numCategorias) {
    MatrixXd oneHot(dados.rows(), numCategorias);
    for (int i = 0; i < dados.rows(); i++) {
        int categoria = static_cast<int>(dados(i, colIdx));
        oneHot(i, categoria) = 1;
    }
    return oneHot;
}

// Definir e treinar a CNN
void definirTreinarCNN(const arma::mat& x_train, const arma::mat& y_train) {
    FFN<NegativeLogLikelihood<>, GlorotInitialization> modelo;
    modelo.Add<Convolution<>>(1, 32, 3, 3, 1, 1, 1, 1, 83, 1);
    modelo.Add<ReLU<> >();
    modelo.Add<MaxPooling<>>(2, 2, 2, 2);
    modelo.Add<Dropout<> >(0.5);
    
    modelo.Add<Convolution<>>(32, 64, 3, 3, 1, 1, 1, 1);
    modelo.Add<ReLU<> >();
    modelo.Add<MaxPooling<>>(2, 2, 2, 2);
    modelo.Add<Dropout<> >(0.5);
    
    modelo.Add<Linear<>>(256);
    modelo.Add<ReLU<> >();
    modelo.Add<Dropout<> >(0.5);
    modelo.Add<Linear<>>(2);
    modelo.Add<LogSoftMax<> >();

    // Configurando e treinando o modelo
    ens::AdamOptimizer otim;
    modelo.Train(x_train, y_train, otim);
}

// Função para calcular métricas de avaliação
void calcularMetricas(const arma::mat& y_true, const arma::mat& y_pred) {
    int verdadeirosPositivos = 0, falsosPositivos = 0, falsosNegativos = 0, verdadeirosNegativos = 0;
    
    for (size_t i = 0; i < y_true.n_cols; ++i) {
        if (y_true(0, i) == 1 && y_pred(0, i) == 1) verdadeirosPositivos++;
        else if (y_true(0, i) == 0 && y_pred(0, i) == 1) falsosPositivos++;
        else if (y_true(0, i) == 1 && y_pred(0, i) == 0) falsosNegativos++;
        else if (y_true(0, i) == 0 && y_pred(0, i) == 0) verdadeirosNegativos++;
    }
    
    double acuracia = (verdadeirosPositivos + verdadeirosNegativos) / static_cast<double>(y_true.n_cols);
    double precisao = verdadeirosPositivos / static_cast<double>(verdadeirosPositivos + falsosPositivos);
    double recall = verdadeirosPositivos / static_cast<double>(verdadeirosPositivos + falsosNegativos);
    double f1 = 2 * (precisao * recall) / (precisao + recall);
    
    std::cout << "Acurácia: " << acuracia << "\n";
    std::cout << "Precisão: " << precisao << "\n";
    std::cout << "Recall: " << recall << "\n";
    std::cout << "F1-Score: " << f1 << "\n";
}

// Função para salvar o modelo
void salvarModelo(FFN<NegativeLogLikelihood<>, GlorotInitialization>& modelo) {
    data::Save("cnn_intrusion_detection_model.xml", "modelo", modelo);
}

// Função para salvar as métricas de avaliação
void salvarMetricas(double acuracia, double precisao, double recall, double f1) {
    std::ofstream arquivo("evaluation_results.txt");
    arquivo << "Acurácia: " << acuracia << "\n";
    arquivo << "Precisão: " << precisao << "\n";
    arquivo << "Recall: " << recall << "\n";
    arquivo << "F1-Score: " << f1 << "\n";
    arquivo.close();
}

// Função principal
int main() {
    // Carregar dados
    MatrixXd dadosTreino = carregarDados("/caminho/para/treino.csv", 1000, 42); // Ajuste linhas/colunas
    MatrixXd dadosTeste = carregarDados("/caminho/para/teste.csv", 1000, 42); 

    // Normalizar dados e pré-processamento
    normalizarDados(dadosTreino);
    normalizarDados(dadosTeste);

    // Preparação de dados para a CNN
    arma::mat x_train = arma::conv_to<arma::mat>::from(dadosTreino);
    arma::mat y_train = arma::conv_to<arma::mat>::from(dadosTeste);

    // Definir e treinar CNN
    definirTreinarCNN(x_train, y_train);

    // Avaliar e salvar modelo e métricas
    calcularMetricas(y_train, /* predições */);
    salvarModelo(modelo);
    salvarMetricas(/* métricas calculadas */);
    
    return 0;
}
