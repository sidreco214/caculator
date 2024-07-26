#include <iostream>

#include <conio.h>
#include "core/calc_tree.hpp"

int main() {
    try {
        std::cout << "add test 1+1=";
        auto add = Calc::add(1, 1);
        std::cout << add.get() << std::endl;

        std::cout << "sub test 1-2=";
        Calc::sub sub(1, 2);
        std::cout << sub.get() << std::endl;

        std::cout << "mul test 2*2=";
        Calc::mul mul(2, 2);
        std::cout << mul.get() << std::endl;

        std::cout << "div test 1/2=";
        Calc::div div(1, 2);
        std::cout << div.get() << std::endl;

        std::cout << "pow test 2^3=";
        Calc::pow pow(2, 3);
        std::cout << pow.get() << std::endl;

        std::cout << "sqrt test sqrt(2)=";
        Calc::sqrt sqrt(2);
        std::cout << sqrt.get() << std::endl;

        std::cout << "exp test exp(1)=";
        Calc::exp exp(1);
        std::cout << exp.get() << std::endl;

        std::cout << "2*3+1=";
        Calc::add tree1;
        auto tree2 = new Calc::mul(2, 3);
        tree1.first = tree2;
        tree1.second = new Calc::num(1);
        std::cout << tree1.get() << std::endl;
        
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        _getch();
        return 1;
    }

    try{
        std::cout << "function: exp(5*2+2*(1+2))+1+(1+1)/2=";
        Calc::calc_tree func("exp(5*2+2*(1+2))+1+(1+1)/2");
        std::cout << func.get() << std::endl;

        std::cout << "function2: 3*exp(0.5+0.25*2+exp(2))+3/5-2*(2/(4+3)*(1+2/(1+1)))=";
        Calc::calc_tree func2("3*exp(0.5+0.25*2+exp(2))+3/5-2*(2/(4+3)*(1+2/(1+1)))");
        std::cout << func2.get() << std::endl;

        std::cout << "function3: 3*exp(2*3^2+exp(1+2)/2)+2/3*sqrt(2)=";
        Calc::calc_tree func3("3*exp(2*3^2+exp(1+2)/2)+2/3*sqrt(2)");
        std::cout << func3.get() << std::endl;
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        _getch();
        return 1;
    }

    
    _getch();
}