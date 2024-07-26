#include "calc_tree.hpp"
using namespace Calc;

double Calc::add::calc() {
    if(!first || !second ) {
        throw std::runtime_error("(Calc::add) first or second is nullptr");
        return 0.0;
    }

    if(!first->is_caculated()) first->get();
    if(!second->is_caculated()) second->get();

    this->value = first->get() + second->get();
    _is_caulated = true;
    dispose_child();
    return this->value;
}

double Calc::sub::calc(){
    if(!first || !second ) {
        throw std::runtime_error("(Calc::sub) first or second is nullptr");
        return 0.0;
    }

    if(!first->is_caculated()) first->get();
    if(!second->is_caculated()) second->get();

    this->value = first->get() - second->get();
    _is_caulated = true;
    dispose_child();
    return this->value;
    
}

double Calc::mul::calc() {
    if(!first || !second ) {
        throw std::runtime_error("(Calc::mul) first or second is nullptr");
        return 0.0;
    }

    if(!first->is_caculated()) first->get();
    if(!second->is_caculated()) second->get();

    this->value = first->get() * second->get();
    _is_caulated = true;
    dispose_child();
    return this->value;
}

double Calc::div::calc() {
    if(!first || !second ) {
        throw std::runtime_error("(Calc::div) first or second is nullptr");
        return 0.0;
    }

    if(!first->is_caculated()) first->get();
    if(!second->is_caculated()) {
        if(second->get() == 0.0) throw std::invalid_argument("Zero division error");
        return 0.0;
    }

    this->value = first->get() / second->get();    
    _is_caulated = true;
    dispose_child();
    return this->value;
};

double Calc::pow::calc() {
    if(!first || !second ) {
        throw std::runtime_error("(Calc::mul) first or second is nullptr");
        return 0.0;
    }

    if(!first->is_caculated()) first->get();
    if(!second->is_caculated()) second->get();

    this->value = std::pow(first->get(), second->get());
    _is_caulated = true;
    dispose_child();
    return this->value;
}

double Calc::exp::calc() {
    if(!first) {
        throw std::runtime_error("(Calc::exp) first is nullptr");
        return 0.0;
    }

    if(!first->is_caculated()) first->get();

    this->value = std::exp(first->get());
    _is_caulated = true;
    dispose_child();
    return this->value;
}

_tree_base *Calc::calc_tree::_make_node(std::string str) {
    Calc::_tree_base* node = nullptr;
    if(*str.rbegin() == ')') {
        // 그냥 괄호 닫힌 경우아니면, exp, sin, cos, tan, ..
        switch(*str.begin()) {
            default:
                throw std::runtime_error("(_make_node) parse error");
                return nullptr;
            
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            
            case '(':
                str = str.substr(1, str.length()-2);
                return _make_node(str);
            
            case 'e':
                if(str.substr(0, 4) == std::string("exp(")) {
                    str = str.substr(4, str.length()-5);
                    node = new Calc::exp();
                    node->first = _make_node(str);
                    return node;
                }
                break;
        }
        
    }

    unsigned int num_op = 0; //the number of open parenthesis
    unsigned int num_cp = 0; //the number of closed parenthesis
    std::string::size_type pos = 0;

    //+,- 찾기
    for(auto iter = str.rbegin(); iter != str.rend(); ++iter) {
        switch(*iter) {
            default:
                continue;
            
            case ')':
                ++num_cp;
                continue;
            
            case '(':
                ++num_op;
                continue;
            
            case '+':
                if(num_op != num_cp) continue;
                node = new Calc::add();
                pos = std::distance(iter, str.rend()) -1;
                node->first = _make_node(str.substr(0, pos));
                node->second = _make_node(str.substr(pos+1));
                return node;
            
            case '-':
                if(num_op != num_cp) continue;
                node = new Calc::sub();
                pos = std::distance(iter, str.rend()) -1;
                node->first = _make_node(str.substr(0, pos));
                node->second = _make_node(str.substr(pos+1));
                return node;
        }
    }

    //*, / 찾기
    num_cp = 0;
    num_op = 0;
    for(auto iter = str.rbegin(); iter != str.rend(); ++iter) {
        switch(*iter) {
            default:
                continue;
            
            case ')':
                ++num_cp;
                continue;
            
            case '(':
                ++num_op;
                continue;
            
            case '*':
                if(num_op != num_cp) continue;
                node = new Calc::mul();
                pos = std::distance(iter, str.rend()) -1;
                node->first = _make_node(str.substr(0, pos));
                node->second = _make_node(str.substr(pos+1));
                return node;
            
            case '/':
                if(num_op != num_cp) continue;
                node = new Calc::div();
                pos = std::distance(iter, str.rend()) -1;
                node->first = _make_node(str.substr(0, pos));
                node->second = _make_node(str.substr(pos+1));
                return node;
        }
    }

    //^ 찾기
    num_cp = 0;
    num_op = 0;
    for(auto iter = str.rbegin(); iter != str.rend(); ++iter) {
        switch(*iter) {
            default:
                continue;
            
            case ')':
                ++num_cp;
                continue;
            
            case '(':
                ++num_op;
                continue;
            
            case '^':
                if(num_op != num_cp) continue;
                node = new Calc::pow();
                pos = std::distance(iter, str.rend()) -1;
                node->first = _make_node(str.substr(0, pos));
                node->second = _make_node(str.substr(pos+1));
                return node;
        }
    }

    //there is no operator -> number
    std::string::size_type n = 0;
    double value = std::stod(str, &n);
    if(str.length() != n) {
        std::string msg = std::format("(_make_node) parse error: unknown symbols {}", str.substr(n));
        throw std::runtime_error(msg);
        return nullptr;
    }
    else return new Calc::num(value);

}

Calc::calc_tree::calc_tree(std::string str) {
    //공백 제거
    str.erase(std::remove(str.begin(), str.end(), ' '));
    first = _make_node(str);
}