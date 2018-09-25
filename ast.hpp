#ifndef _AST_HPP_
#define _AST_HPP_

#include <vector>
#include <string>
#include <iostream>
#include "type.hpp"

using namespace std;

class ExprAST {
public:
  virtual ~ExprAST();
  virtual Type *typecheck() const = 0;
};

class ConstantExprAST : public ExprAST {
public:
    ConstantExprAST(Type* t) 
    : _t(t) 
    {}
    ~ConstantExprAST();
    Type *typecheck() const;

  private:
    Type* _t;
};

class VariableExprAST : public ExprAST {
public:
    VariableExprAST(string name) 
    : _name(name)
    {}
    Type *typecheck() const;

  private:
    string _name;
};

class InnerExprAST : public ExprAST {
public:
    InnerExprAST(ExprAST* e1);
    InnerExprAST(ExprAST *e1, ExprAST *e2);
    InnerExprAST(ExprAST *e1, ExprAST *e2, ExprAST *e3);
    InnerExprAST(vector<ExprAST *> v)
        : _v(v)
    {}
    ~InnerExprAST();

  protected:
    vector<ExprAST*> _v;
};

class AddExprAST : public InnerExprAST {
public:
    AddExprAST(ExprAST* e1, ExprAST* e2) 
    : InnerExprAST(e1, e2) 
    {}
    Type *typecheck() const;
};

class SubExprAST : public InnerExprAST {
public:
    SubExprAST(ExprAST* e1, ExprAST* e2) 
    : InnerExprAST(e1, e2) 
    {}
    Type *typecheck() const;
};

class MulExprAST : public InnerExprAST {
public:
    MulExprAST(ExprAST* e1, ExprAST* e2) 
    : InnerExprAST(e1, e2) 
    {}
    Type *typecheck() const;
};

class DivExprAST : public InnerExprAST {
public:
    DivExprAST(ExprAST* e1, ExprAST* e2) 
    : InnerExprAST(e1, e2) 
    {}
    Type *typecheck() const;
};

class AssignExprAST : public InnerExprAST
{
  public:
    AssignExprAST(string s, ExprAST *e, Type *t)
        : InnerExprAST(e), _s(s), _t(t)
    {}
    Type *typecheck() const;

  private:
    string _s;
    Type *_t;
};

class PredefineExprAST : public InnerExprAST {
  public:
    PredefineExprAST(string s, ExprAST *e)
        : InnerExprAST(e), _s(s)
    {}
    Type *typecheck() const;

  private:
    string _s;
};

class PredefineArrayExprAST : public InnerExprAST {
public:
    PredefineArrayExprAST(string s, ExprAST *e1, ExprAST *e2)
        : InnerExprAST(e1, e2), _s(s)
    {}
    Type* typecheck() const;
private:
    string _s;
};

class ElementOfArrayExprAST : public InnerExprAST {
public:
    ElementOfArrayExprAST(string s, ExprAST* e) 
        : InnerExprAST(e), _s(s)
    {}
    Type* typecheck() const;
private:
    string _s;
};

class SeqExprAST : public InnerExprAST {
public:
    SeqExprAST(ExprAST* e1, ExprAST* e2)
        : InnerExprAST(e1, e2)
    {}
    Type* typecheck() const;
};

#endif