// Copyright © Intel Corporation
//
// SPDX-License-Identifier: MIT
// =============================================================
#include <sycl/sycl.hpp>
using namespace sycl;
static const int N = 16;
int main(){
  
  queue q;
  std::cout << "Device: " << q.get_device().get_info<info::device::name>() << "\n";


  int *data = malloc_shared<int>(N, q);//area de memoria para 16 inteiros vinculada a fila q
    
 
  for(int i=0; i<N; i++) data[i] = i;//preenche o vetor com os indicies 
 
  
  q.parallel_for(range<1>(N), [=] (id<1> i){//id<1> é um tipo de dado que representa um identificador de indicie{
    data[i] *= 2;
  }).wait();

  //# Print Output
  for(int i=0; i<N; i++) std::cout << data[i] << "\n";

  free(data, q);
  return 0;
}
