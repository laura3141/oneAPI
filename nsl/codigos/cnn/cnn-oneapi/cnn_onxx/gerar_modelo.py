import pandas as pd
import torch
import torch.nn as nn
import torch.onnx
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder, StandardScaler
import pickle

# 1) Carregar base de dados
df = pd.read_csv("codigos/cnn/cnn-oneapi/cnn_onxx/corrected_com_colunas_oficiais.csv")

# 2) Binarizar o label: 0 = normal, 1 = qualquer outro (ataque)
df["label"] = (
    df["label"]
      .str.strip()           # remove espaços
      .str.rstrip(".")       # remove o ponto final
      .apply(lambda x: 0 if x == "normal" else 1)
)

# 3) Identificar colunas categóricas e codificar
categorical_cols = df.select_dtypes(include=["object"]).columns.tolist()
for col in categorical_cols:
    le = LabelEncoder()
    df[col] = le.fit_transform(df[col])

# 4) Separar features e target
X = df.drop(columns=["label"])
y = df["label"]

# 5) Normalizar as features
scaler = StandardScaler()
X = scaler.fit_transform(X)

# Salva o scaler treinado
with open("codigos/cnn/cnn-oneapi/cnn_onxx/scaler.pkl", "wb") as f:
    pickle.dump(scaler, f)

# Se ainda não o fez, salve também a lista de colunas na ordem certa
cols_ordenadas = [
    "duration", "protocol_type", "service", "flag", "src_bytes", "dst_bytes",
    "land", "wrong_fragment", "urgent", "hot", "num_failed_logins", "logged_in",
    "num_compromised", "root_shell", "su_attempted", "num_root",
    "num_file_creations", "num_shells", "num_access_files", "num_outbound_cmds",
    "is_host_login", "is_guest_login", "count", "srv_count", "serror_rate",
    "srv_serror_rate", "rerror_rate", "srv_rerror_rate", "same_srv_rate",
    "diff_srv_rate", "srv_diff_host_rate", "dst_host_count", "dst_host_srv_count",
    "dst_host_same_srv_rate", "dst_host_diff_srv_rate",
    "dst_host_same_src_port_rate", "dst_host_srv_diff_host_rate",
    "dst_host_serror_rate", "dst_host_srv_serror_rate",
    "dst_host_rerror_rate", "dst_host_srv_rerror_rate"
]

feature_columns = cols_ordenadas   # ou a lista que você já definiu
with open("codigos/cnn/cnn-oneapi/cnn_onxx/feature_columns.pkl", "wb") as f:
    pickle.dump(feature_columns, f)

# 6) Dividir em treino e teste
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42
)

# 7) Transformar em tensores
X_train_tensor = torch.tensor(X_train, dtype=torch.float32)
y_train_tensor = torch.tensor(y_train.values, dtype=torch.long)

# 8) DataLoader
dataset = torch.utils.data.TensorDataset(X_train_tensor, y_train_tensor)
dataloader = torch.utils.data.DataLoader(dataset, batch_size=64, shuffle=True)

# 9) Definir a rede
class IntrusionNet(nn.Module):
    def __init__(self, input_size):
        super().__init__()
        self.fc = nn.Sequential(
            nn.Linear(input_size, 64),
            nn.ReLU(),
            nn.Linear(64, 32),
            nn.ReLU(),
            nn.Linear(32, 2)
        )
    def forward(self, x):
        return self.fc(x)

model = IntrusionNet(X.shape[1])
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)

# 10) Treinamento
for epoch in range(10):
    for inputs, targets in dataloader:
        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs, targets)
        loss.backward()
        optimizer.step()
    print(f"Época {epoch+1} — Loss: {loss.item():.4f}")

# 11) Exportar para ONNX
dummy_input = torch.randn(1, X.shape[1])
torch.onnx.export(
    model, dummy_input, "codigos/cnn/cnn-oneapi/cnn_onxx/modelo_intrusao.onnx",
    input_names=['input'], output_names=['output'],
    dynamic_axes={'input': {0: 'batch_size'}, 'output': {0: 'batch_size'}},
    opset_version=11
)

print("✅ Modelo exportado como modelo_intrusao.onnx")
