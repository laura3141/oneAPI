
# Compilação 


## Compilação Padrão

icpx -fsycl addv.dp.cpp

## Compilação para gpu nvidia 

	icpx -fsycl -fsycl-targets=nvptx64-nvidia-cuda main.cpp -o a
	
## Compilaçao para multiplos hardwares 

	icpx -fsycl -fsycl-targets=nvptx64-nvidia-cuda,spir64 \
	-Xsycl-target-backend=nvptx64-nvidia-cuda --offload-arch=sm_80 
	-o a main.cpp

### Links úteis

https://registry.khronos.org/SYCL/specs/sycl-2020/html/sycl-2020.html#subsubsec:nd-range-class // documentaçao sycl

https://developer.codeplay.com/products/oneapi/nvidia/2024.1.0/guides/get-started-guide-nvidia // plugin cuda
