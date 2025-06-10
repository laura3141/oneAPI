#include <onnxruntime_cxx_api.h>
#include <iostream>
#include <vector>

int main() {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "IntrusionDetection");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);

    // Ativar o uso da oneDNN (DNNL)
    session_options.AppendExecutionProvider_Dnnl(1); // 1 thread

    // Caminho para o modelo
    const char* modelo_path = "modelo_intrusao.onnx";

    // Criar a sessão
    Ort::Session session(env, modelo_path, session_options);
    Ort::AllocatorWithDefaultOptions allocator;

    // Entradas
    std::vector<int64_t> input_dims = {1, 42}; // 1 amostra com 42 features
    std::vector<float> input_tensor_values(42, 0.5); // valores simulados

    size_t input_tensor_size = 1 * 42;
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info, input_tensor_values.data(), input_tensor_size,
        input_dims.data(), input_dims.size());

    // Nomes das entradas e saídas
    auto input_name = session.GetInputName(0, allocator);
    auto output_name = session.GetOutputName(0, allocator);

    // Inferência
    auto output_tensors = session.Run(
        Ort::RunOptions{nullptr},
        &input_name, &input_tensor, 1,
        &output_name, 1);

    // Processar saída
    float* output = output_tensors.front().GetTensorMutableData<float>();
    std::cout << "Resultado da inferência:\n";
    for (int i = 0; i < 2; ++i) {
        std::cout << "Classe " << i << ": " << output[i] << "\n";
    }

    return 0;
}
