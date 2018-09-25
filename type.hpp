#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <vector>
#include <string>
#include <map>

#define DOUBLE 1
#define INT 2
#define STRING 3
#define CHAR 4
#define ARRAY 5
#define CLASS 6

using namespace std;

class Type {
public:
    virtual ~Type();
    virtual int type() const = 0;
    virtual Type* copy() const = 0;
};

class DoubleType : public Type {
public:
  int type() const;
  Type* copy() const;
};

class IntType : public Type {
public:
  int type() const;
  Type* copy() const;
};

class StringType : public Type {
public:
  int type() const;
  Type* copy() const;
};

class CharType : public Type {
public:
  int type() const;
  Type* copy() const;
};

class ArrayType : public Type {
public:
  ArrayType(int n, Type* t) : _n(n), _t(t) {}
  int type() const;
  Type* copy() const;
  int getN() const ;
  Type* getType() const ;
private:
  int _n;
  Type* _t;
};

class ClassType : public Type {
public:
  int type() const;
  string name() const;
  map<string, Type*> tablica_polja() const;
  map<string, pair<Type*, Type*> > tablica_metoda() const;
  Type* copy() const;
};

#endif