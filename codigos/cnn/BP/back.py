import numpy as np

class Neuron:
	def __init__(self, n_inputs):
		self.w = np.random.randn(n_inputs);	
		self.z = 0
		self.b = 1
		print("pesos =",self.w)
	def ReLU(self):
		return self.z if self.z >= 0 else 0 

	def ReLU_derivate(self):
		return 1 if self.z >= 0 else 0


class Layer:
	def __init__(self, n_neurons, n_inputs):
		self.n_neurons = n_neurons
		self.neurons = []
		self.o_layer = np.zeros(n_neurons)

		for i in range(n_neurons):
			print("Inputs Layer:",n_inputs)
			self.neurons.append(Neuron(n_inputs))
	
	def calc(self, inputs):
		for i in range(self.n_neurons):
			self.neurons[i].z = np.dot(inputs, self.neurons[i].w) + self.neurons[i].b
			print("z = ",self.neurons[i].z)
			self.neurons[i].z = self.neurons[i].ReLU()
			self.o_layer[i] = self.neurons[i].z

class Back:
	def __init__(self, n_layers, n_neurons, n_inputs):	
		self.n_layers = n_layers
		self.layers = []

		for i in range(n_layers):
			print(i)
			self.layers.append(Layer(n_neurons[i], n_inputs[i]))

	def forward(self, inputs): 
		for i in range(self.n_layers):
			for j in range(self.layers[i].n_neurons):
				if(i == 0):
					self.layers[i].calc(inputs[0])
				else:
					self.layers[i].calc(self.layers[i-1].o_layer)	
	
	#def backward(self):

X = [[0, 0], [0, 1], [1, 0], [1, 1]]
Y = [ 0, 1, 1, 0]
bp = Back(3, [3,3,1], [2,3,3])	
bp.forward(X)
