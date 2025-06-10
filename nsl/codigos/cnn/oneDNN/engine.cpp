#include <CL/sycl.hpp>
#include <iostream>
#include <dnnl.hpp>
using namespace dnnl;
int main(){
	engine eng = engine(engine::kind::cpu,0);
	stream st =  stream(eng);
	double vl = 0.0;
	memory::desc ds = memory::desc({2,19},memory::data_type::f32,memory::format_tag::ab);
	memory mem = memory(ds,eng);
    // Declare a pointer to the array
    float* data = (float*)mem.get_data_handle();
	for(int i = 0;i<2;i++){
		for(int j = 0;j<19;j++){
			data[i*19+j] = vl++;
			std::cout << data[i*19+j];
		}
		std::cout<<"\n\n";
	}
	std::cout << engine::get_count(st.get_engine().get_kind())<<"\n" << version()->patch << "\n"<< ds.get_size()<< "\n"<<mem.data_type_size(memory::data_type::f32)<< "\n" << "\n";
}