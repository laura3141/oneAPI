gcc -g -Wall -Werror -c main.c
gcc -g -Wall -Werror -c layer.c
gcc -g -Wall -Werror -c neuron.c
gcc -pthread -lpthread -o backprop main.o layer.o neuron.o -lm
rm *.o
