#ifndef __CALC_TREE_HPP__
#define __CALC_TREE_HPP__

#include <cmath>

#include <typeinfo>
#include <stdexcept>

#include <string>
#include <format>

namespace Calc {

//binary base tree
class _tree_base {
    protected:
    bool _is_caulated = false;
    double value = 0.0;

    public:
    _tree_base* first;
    _tree_base* second;

    protected:
    void dispose_child() {
        if(first) {delete first; first = nullptr;}
        if(second) {delete second; second = nullptr;}
    }

    public:
    _tree_base(_tree_base* first = nullptr, _tree_base* second = nullptr) : first(first), second(second) {
        //std::cout << "create node" << std::endl;
    }
    virtual ~_tree_base() {
        dispose_child();
        //std::cout << "delete node" << std::endl;
    }

    virtual double get() {
        throw std::runtime_error(std::format("({}) invalid access to method:get", typeid(*this).name()));
        return 0.0;
    }
    inline bool is_caculated() {return _is_caulated;}
};

class num : public _tree_base {
    public:
    num(const double value) : _tree_base(nullptr, nullptr) {
        this->value = value;
        this->_is_caulated = true;
    }

    double get() override {
        return value;
    }
};

//operator node
class _op_node : public _tree_base {
    public:
    virtual double calc()=0;
    
    double get() override {
        if(is_caculated()) return value;
        else               return calc();
    }
};

class add : public _op_node {
    public:
    add() = default;
    add(double first, double second) {
        this->first = new num(first);
        this->second = new num(second);
    };
    double calc();
};


class sub : public _op_node {
    public:
    sub() = default;
    sub(double first, double second) {
        this->first = new num(first);
        this->second = new num(second);
    }
    double calc();
};

class mul : public _op_node {
    public:
    mul() = default;
    mul(double first, double second) {
        this->first = new num(first);
        this->second = new num(second);
    }
    double calc();
};

class div : public _op_node {
    public:
    div() = default;
    div(double first, double second) {
        this->first = new num(first);
        this->second = new num(second);
    }
    double calc();
};

class pow : public _op_node {
    public:
    pow() = default;
    pow(double first, double second) {
        this->first = new num(first);
        this->second = new num(second);
    }
    double calc();
};

class sqrt : public _op_node {
    public:
    sqrt() = default;
    sqrt(double first) {
        this->first = new num(first);
    }
    double calc();
};

class exp : public _op_node {
    public:
    exp() = default;
    exp(double first) {
        this->first = new num(first);
    }
    double calc();
};

class calc_tree : _tree_base {
    private:
    _tree_base* _make_node(std::string str);

    public:
    calc_tree(std::string str);
    double get() {
        return first->get();
    }
};

}
#endif