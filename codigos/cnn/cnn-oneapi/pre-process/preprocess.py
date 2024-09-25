import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder, StandardScaler, OneHotEncoder
from sklearn.impute import SimpleImputer
from sklearn.decomposition import PCA
from sklearn.ensemble import IsolationForest

# Função para carregar o arquivo ARFF e converter para DataFrame
arff_file_path = "codigos/cnn/CNN-OneAPI/pre-process/nsl-kdd/archive/KDDTest-21.arff"
def load_arff_to_dataframe(file_path):
    # Ler o arquivo ARFF usando pandas
    data = pd.read_csv(file_path, comment='@', header=None)
    
    # Identificar colunas de atributos e a coluna de rótulo
    with open(file_path, 'r') as f:
        lines = f.readlines()
    
    attribute_names = []
    for line in lines:
        if line.startswith('@attribute'):
            attribute = line.split()[1].strip("'")
            attribute_names.append(attribute)
    
    # Atribuir nomes das colunas ao DataFrame
    data.columns = attribute_names
    return data

# Carregar os dados do arquivo ARFF
data = load_arff_to_dataframe(arff_file_path)

label_encoders = {}
for column in data.select_dtypes(include=['object']).columns:
    if data[column].nunique() > 2: 
        data = pd.get_dummies(data, columns=[column])
    else: 
        le = LabelEncoder()
        data[column] = le.fit_transform(data[column])
        label_encoders[column] = le

scaler = StandardScaler()
numerical_columns = data.select_dtypes(include=['float64', 'int64']).columns
data[numerical_columns] = scaler.fit_transform(data[numerical_columns])

# Tratamento de valores ausentes
imputer = SimpleImputer(strategy='mean')
data[numerical_columns] = imputer.fit_transform(data[numerical_columns])

# ivisão dos dados em conjuntos de treino e teste
train_data, test_data = train_test_split(data, test_size=0.2, random_state=42)

# Remoção de outliers usando Isolation Forest
iso_forest = IsolationForest(contamination=0.05, random_state=42)
outliers = iso_forest.fit_predict(train_data[numerical_columns])

# Filtrar outliers (os valores -1 indicam outliers)
train_data = train_data[outliers != -1]

# Seleção ou extração de atributos usando PCA
pca = PCA(n_components=0.95, random_state=42)  # Manter 95% da variância
train_data_pca = pca.fit_transform(train_data[numerical_columns])
test_data_pca = pca.transform(test_data[numerical_columns])

# Exibir a forma dos dados pré-processados
train_data_pca.shape, test_data_pca.shape


# Salvar os dados de treino e teste em arquivos CSV
np.savetxt('codigos/cnn/CNN-OneAPI/pre-process/train_data_pca.csv', train_data_pca, delimiter=',')
np.savetxt('codigos/cnn/CNN-OneAPI/pre-process/test_data_pca.csv', test_data_pca, delimiter=',')
