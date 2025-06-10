import numpy as np

class Neuron:
	def __init__(self, n_inputs):
		self.w = np.random.randn(n_inputs);	
		self.z = 0
		self.b = 1
		# print("pesos =",self.w)
	def ReLU(self):
		return self.z if self.z >= 0 else 0 

	def ReLU_derivative(self):
		return 1 if self.z >= 0 else 0


class Layer:	
	def __init__(self, n_neurons, n_inputs):
		self.n_neurons = n_neurons
		self.neurons = []
		self.o_layer = np.zeros(n_neurons)
		self.error = 0.0
		for i in range(n_neurons):
			print("Inputs Layer:",n_inputs)
			self.neurons.append(Neuron(n_inputs))
	
	def calc(self, inputs):
		for i in range(self.n_neurons):
			self.neurons[i].z = np.dot(inputs, self.neurons[i].w) + self.neurons[i].b
			#
			# print("z = ",self.neurons[i].z)
			self.neurons[i].z = self.neurons[i].ReLU()
			self.o_layer[i] = self.neurons[i].z

	def MSE(self,target):
		error = 0.0
		for i in self.neurons:
			error += np.power(target -i.z,2)
		self.error = error/self.n_neurons
		print("error = ",self.error)

	def MSE_derivative(self, output):
		print("self o layer",self.o_layer[0])
		print("output", output)
		return -2*(1/self.n_neurons) * (output-self.o_layer[0])
	
	def adjust(self, output, e_total, p_layer):
		for i in range(self.n_neurons):
			for w in range(len(self.neurons[i].w)):
				#print("w = ",w)
				self.neurons[i].w[w] = self.neurons[i].w[w] - 0.3*(p_layer.o_layer[w]*self.neurons[i].ReLU_derivative()*e_total)
				#print("w_adjust = ",w)
				self.neurons[i].b = self.neurons[i].b- 0.3*(self.neurons[i].ReLU_derivative()*e_total)
	
	def adjust1(self, output, e_total, input):
		for i in range(self.n_neurons):
			for w in range(len(self.neurons[i].w)):
				#print("w = ",w)
				self.neurons[i].w[w] = self.neurons[i].w[w] - 0.3*(input[w]*self.neurons[i].ReLU_derivative()*e_total)
				self.neurons[i].b = self.neurons[i].b- 0.3*(self.neurons[i].ReLU_derivative()*e_total)		
				#print("w_adjust = ",w)
class Back:
	def __init__(self, n_layers, n_neurons, n_inputs, lr):	
		self.n_layers = n_layers
		self.layers = []
		self.e_total = 0
		self.lr = lr
		for i in range(n_layers):
			# print(i)
			self.layers.append(Layer(n_neurons[i], n_inputs[i]))

	def forward(self, inputs,epocas):
		for e in range(epocas): 
			for input in range(len(inputs)):
				for i in range(self.n_layers):
					for j in range(self.layers[i].n_neurons):
						if(i == 0):
							self.layers[i].calc(inputs[input])
						else:
							self.layers[i].calc(self.layers[i-1].o_layer)
				#print("teste")
				self.backward(Y[input],inputs[input])

	def backward(self, output, j):
		#print("teste", self.n_layers)
		self.e_total = self.layers[self.n_layers - 1].MSE_derivative(output)
		print("erro ",self.e_total)
		for i in range(2, -1, -1):
			#print("Teste inside", i )	
			if i !=0:
				self.layers[i].adjust(output, self.e_total, self.layers[i-1])	
			else:
				self.layers[i].adjust1(output, self.e_total, j)

	def predict(self, input):
		for i in range(self.n_layers):
					for j in range(self.layers[i].n_neurons):
						if(i == 0):
							self.layers[i].calc(input)
						else:
							self.layers[i].calc(self.layers[i-1].o_layer)
		return(self.layers[2].o_layer)

X = [[0, 0], [0, 1], [1, 0], [1, 1]]
Y = [ 0, 1, 1, 0]
bp = Back(3, [2,2,1], [2,2,2], 0.01)	
bp.forward(X,1000)
print(bp.predict([0,0]))	
print(bp.predict([0,1]))	
print(bp.predict([1,0]))	
print(bp.predict([1,1]))	
