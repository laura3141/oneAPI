# Conceitos 

## Computação Heterogênea

Aplicações projetadas para ter sua execução compartilhada entre diferentes arquiteturas

- **Host (CPU)**: Responsável por tarefas pouco paralelizáveis.
- **Aceleradores**: Responsáveis por tarefas altamente paralelizáveis.

## Tipos de Processadores

### Processador Single Core
Apenas um núcleo de processamento físico em um chip de processador. Isso significa que ele só pode executar uma tarefa ou thread de cada vez.

### Processador Multi Core
Possui dois ou mais núcleos de processamento físicos integrados em um único chip de processador. Cada núcleo é capaz de executar tarefas independentes, o que permite ao processador realizar múltiplas tarefas simultaneamente.

## Ferramentas de Programação Paralela

### OpenCL
Estrutura baseada em C para programação paralela em baixo nível, projetada para ser executada em diferentes dispositivos.

### SYCL
Camada de abstração de nível mais alto construída sobre o OpenCL, oferecendo uma experiência de programação mais familiar e amigável para desenvolvedores C++.

## Hierarquia de Memória

Estrutura organizacional de diferentes tipos e níveis de memória em um sistema de computação, variando em termos de capacidade, velocidade, custo por byte e proximidade ao processador. Esta estrutura é projetada para otimizar o desempenho do sistema.

### Localidade

- **Localidade Espacial**: Tendência em acessar dados próximos a outros dados que foram recentemente acessados.
- **Localidade Temporal**: Tendência em acessar os mesmos dados em um curto período de tempo.

