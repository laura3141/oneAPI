#include <iostream>
#include <vector>
#include <cmath>
#include <random>

using namespace std;

// Função de ativação sigmoid
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Derivada do sigmoid
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// Inicializa os pesos com valores aleatórios
void initialize_weights(vector<vector<double>>& weights, int input_size, int output_size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1.0, 1.0);

    weights.resize(input_size, vector<double>(output_size));
    for (int i = 0; i < input_size; i++) {
        for (int j = 0; j < output_size; j++) {
            weights[i][j] = dis(gen);
        }
    }
}

// Produto escalar entre entrada e pesos
vector<double> forward(const vector<double>& input, const vector<vector<double>>& weights, const vector<double>& biases) {
    vector<double> output(weights[0].size());
    for (size_t j = 0; j < weights[0].size(); j++) {
        output[j] = biases[j];
        for (size_t i = 0; i < weights.size(); i++) {
            output[j] += input[i] * weights[i][j];
        }
        output[j] = sigmoid(output[j]); // Ativação
    }
    return output;
}

// Atualização dos pesos e biases com backpropagation
void backward(const vector<double>& input, const vector<double>& output, const vector<double>& target, 
            vector<vector<double>>& weights, vector<double>& biases, double learning_rate) {
    vector<double> errors(output.size());
    vector<double> deltas(output.size());

    // Calcula erro e deltas (gradiente)
    for (size_t j = 0; j < output.size(); j++) {
        errors[j] = target[j] - output[j];
        deltas[j] = errors[j] * sigmoid_derivative(output[j]);
    }

    // Atualiza pesos e biases
    for (size_t i = 0; i < weights.size(); i++) {
        for (size_t j = 0; j < weights[0].size(); j++) {
            weights[i][j] += learning_rate * deltas[j] * input[i];
        }
    }
    for (size_t j = 0; j < biases.size(); j++) {
        biases[j] += learning_rate * deltas[j];
    }
}

int main() {
    // Parâmetros da rede
    int input_size = 2;
    int hidden_size = 2;
    int output_size = 1;
    double learning_rate = 0.1;

    // Inicializa pesos e biases
    vector<vector<double>> hidden_weights, output_weights;
    
    vector<double> hidden_biases(hidden_size, 0.0), output_biases(output_size, 0.0);

    initialize_weights(hidden_weights, input_size, hidden_size);
    initialize_weights(output_weights, hidden_size, output_size);

    // Dados de treino (XOR)
    vector<vector<double>> inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    vector<vector<double>> targets = {{0}, {1}, {1}, {0}};

    // Treinamento
    for (int epoch = 0; epoch < 10000; epoch++) {
        double total_error = 0.0;

        for (size_t i = 0; i < inputs.size(); i++) {
            // Forward pass
            vector<double> hidden_output = forward(inputs[i], hidden_weights, hidden_biases);
            vector<double> final_output = forward(hidden_output, output_weights, output_biases);

            // Backward pass
            backward(hidden_output, final_output, targets[i], output_weights, output_biases, learning_rate);
            backward(inputs[i], hidden_output, targets[i], hidden_weights, hidden_biases, learning_rate);

            // Calcula erro total
            for (size_t j = 0; j < final_output.size(); j++) {
                total_error += 0.5 * pow(targets[i][j] - final_output[j], 2);
            }
        }

        if (epoch % 1000 == 0) {
            cout << "Epoch: " << epoch << " Error: " << total_error << endl;
        }
    }

    // Testa a rede
    for (size_t i = 0; i < inputs.size(); i++) {
        vector<double> hidden_output = forward(inputs[i], hidden_weights, hidden_biases);
        vector<double> final_output = forward(hidden_output, output_weights, output_biases);
        cout << "Input: " << inputs[i][0] << ", " << inputs[i][1]
            << " Output: " << final_output[0] << endl;
    }

    return 0;
}
