#include "type.hpp"

Type::~Type() {

}

int IntType::type() const {
    return INT;
}

int DoubleType::type()  const {
    return DOUBLE;
}

int CharType::type() const {
    return CHAR;
}

int StringType::type() const {
    return STRING;
}

int ArrayType::type() const {
    return ARRAY;
}

Type* IntType::copy() const {
    return new IntType();
}

Type* DoubleType::copy() const {
    return new DoubleType();
}

Type* CharType::copy() const {
    return new CharType();
}

Type* StringType::copy() const {
    return new StringType();
}

Type* ArrayType::copy() const {
    return new ArrayType(_n, _t);
}

int ArrayType::getN() const {
    return _n;
}


Type* ArrayType::getType() const {
    return _t;
}

int ClassType::type() const {
    return CLASS;
}

Type *ClassType::copy() const {
    return new ClassType();
}