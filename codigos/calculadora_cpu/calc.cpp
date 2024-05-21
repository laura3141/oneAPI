#include <iostream>
#include <CL/sycl.hpp>

using namespace sycl;
using namespace std;
//class vector_addition

int main(){
    float num1 = 1;
    float num2 = 2;
    char op = '+';
    float result = 0;

    //default_selector device_selector;
    cpu_selector selector;
    queue q(selector);

    cout << "Running on " << q.get_device().get_info<info::device::name>() << "\n";
    cout << "Calculadora de 2 numeros\nDigite o num1: ";
    cin >> num1;
    cout << "Digite o num2: ";
    cin >> num2;
    cout << "Digite a operação (+, -, *, /): ";
    cin >> op;
    
    buffer<float, 1> num1_buff(&num1, range<1>(1));
    buffer<float, 1> num2_buff(&num2, range<1>(1));
    buffer<char, 1> op_buff(&op, range<1>(1));
    buffer<float, 1> res_buff(&result, range<1>(1));

    queue{}.submit([&](handler& calc) {
        accessor o{op_buff, calc}; //auto o = op_buff.get_access<access::mode::read>(calc);
        accessor n1{num1_buff, calc}; //auto n1 = num1_buff.get_access<access::mode::read>(calc);
        accessor n2{num2_buff, calc}; //auto n2 = num2_buff.get_access<access::mode::read>(calc);

        accessor r{res_buff, calc}; //auto r = res_buff.get_access<access::mode::discard_write>(calc);

        
        
        calc.single_task<class vector_addition>([=]() {

            if (op == '+') {
                r[0] = n1[0] + n2[0];
            }else if(op == '-'){
                r[0] = n1[0] - n2[0];
            }else if(op == '*'){
                r[0] = n1[0] * n2[0];
            }else if(op == '/'){
                r[0] = n1[0] / n2[0];
            }
        });

    });
    

    //('basic_ostream<char, char_traits<char>>' and 'host_accessor<float, 1, access::mode::read_write>')

    host_accessor re{res_buff};
    //cout << "blabla-bla\n";
    cout << "Res: " << result << "\n";
    q.wait();

    return 0;
}