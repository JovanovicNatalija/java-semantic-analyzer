#include "ast.hpp"
#include <iostream>
#include <cstdlib>
#include <typeinfo>
#include <map>

ExprAST::~ExprAST() {

}

ConstantExprAST::~ConstantExprAST() {
    delete _t;
}

Type *ConstantExprAST::typecheck() const {
    return _t->copy();
}


extern map<string, Type*> tablica;

Type *VariableExprAST::typecheck() const
{
    map<string, Type*>::iterator tmp = tablica.find(_name);
    if(tmp == tablica.end()) {
        cerr << "Variable '" << _name << "' used but is never defined." << endl;
        exit(EXIT_FAILURE);
    }

    return tmp->second->copy();
}

InnerExprAST::InnerExprAST(ExprAST* e1) {
    _v.resize(0);
    _v.push_back(e1);
}

InnerExprAST::InnerExprAST(ExprAST* e1, ExprAST* e2) {
    _v.resize(0);
    _v.push_back(e1);
    _v.push_back(e2);
}

InnerExprAST::~InnerExprAST() {
    for(auto e: _v)
        delete e;
}

Type *AddExprAST::typecheck() const
{
    Type *l = _v[0]->typecheck();
    Type *d = _v[1]->typecheck();

    if(!l || !d) {
        return NULL;
    }

    /* ne moze plus sa nizovima! */
    if(l->type() == ARRAY || d->type() == ARRAY) {
        cerr << "There is no operator + for array." << endl;
        return NULL;
    }

    /* string + {char, int, double, string} = string */
    if(l->type() == STRING || d->type() == STRING) {
        return new StringType();
    }

    /* double + {char, int, double} = double */
    if(l->type() == DOUBLE || d->type() == DOUBLE) {
        return new DoubleType();
    }

    /* int + {char, int} = int */
    if(l->type() == INT || d->type() == INT) {
        return new IntType();
    }

    /* char + char = char */
    return new CharType();
}

Type *SubExprAST::typecheck() const
{
    Type *l = _v[0]->typecheck();
    Type *d = _v[1]->typecheck();

    if(!l || !d) {
        return NULL;
    }

    /* ne moze - sa nizovima! */
    if(l->type() == ARRAY || d->type() == ARRAY) {
        cout << "There is no operator - for array." << endl;
        return NULL;
    }

    /* ne moze - sa stringovima! */
    if(l->type() == STRING || d->type() == STRING) {
        cout << "There is no operator - for string." << endl;
        return NULL;
    }

    /* double - {char, int, double} = double */
    if(l->type() == DOUBLE || d->type() == DOUBLE) {
        return new DoubleType();
    }

    /* int - {char, int} = int */
    if(l->type() == INT || d->type() == INT) {
        return new IntType();
    }

    /* char - char = char */
    return new CharType();
}

Type *MulExprAST::typecheck() const
{
    Type *l = _v[0]->typecheck();
    Type *d = _v[1]->typecheck();

    if(!l || !d) {
        return NULL;
    }

    /* ne moze * sa nizovima! */
    if(l->type() == ARRAY || d->type() == ARRAY) {
        cout << "There is no operator * for array." << endl;
        return NULL;
    }

    /* ne moze * sa stringovima! */
    if(l->type() == STRING || d->type() == STRING) {
        cout << "There is no operator * for string." << endl;
        return NULL;
    }

    /* double * {char, int, double} = double */
    if(l->type() == DOUBLE || d->type() == DOUBLE) {
        return new DoubleType();
    }

    /* int * {char, int} = int */
    if(l->type() == INT || d->type() == INT) {
        return new IntType();
    }

    /* char * char = char */
    return new CharType();
}

Type *DivExprAST::typecheck() const
{
    Type *l = _v[0]->typecheck();
    Type *d = _v[1]->typecheck();

    if(!l || !d) {
        return NULL;
    }

    /* ne moze / sa nizovima! */
    if(l->type() == ARRAY || d->type() == ARRAY) {
        cout << "There is no operator / for array." << endl;
        return NULL;
    }

    /* ne moze / sa stringovima! */
    if(l->type() == STRING || d->type() == STRING) {
        cout << "There is no operator / for string." << endl;
        return NULL;
    }

    /* double / {char, int, double} = double */
    if(l->type() == DOUBLE || d->type() == DOUBLE) {
        return new DoubleType();
    }

    /* int / {char, int} = int */
    if(l->type() == INT || d->type() == INT) {
        return new IntType();
    }

    /* char / char = char */
    return new CharType();
}

Type* AssignExprAST::typecheck() const {
    map<string, Type *>::iterator tmp = tablica.find(_s);
    if (tmp == tablica.end())
    {
        cerr << "Variable '" << _s << "' was not declared." << endl;
        exit(EXIT_FAILURE);
    }

    Type* exprType = _v[0]->typecheck();
    Type* varType = _t;

    /* ako je prom tipa ARRAY */
    if (varType->type() == ARRAY) {
        if(exprType->type() != ARRAY) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to array. " << endl;
            return NULL;
        }
        else {
            ArrayType* e = (ArrayType*)exprType;
            ArrayType* v = (ArrayType*)varType;
            if(e->getType()->type() != v->getType()->type()) {
                cerr << "Incompatible types of arrays. " << endl;
                return NULL;
            }
        }  
    }

    /* ako je prom tipa STRING */
    if(varType->type() == STRING) {
        if(exprType->type() != STRING) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa DOUBLE */
    if(varType->type() == DOUBLE) {
        int t = exprType->type();
        if(t == ARRAY || t == STRING) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa INT */
    if(varType->type() == INT) {
        int t = exprType->type();
        /*  cak i za double ne moze da kastuje u int tj izlazi greska da 
            ce doci do gubitka informacija ukoliko se to uradi */ 
        if(t == ARRAY || t == STRING || t == DOUBLE) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa CHAR */
    if(varType->type() == CHAR) {
        int t = exprType->type();
        if(t == ARRAY || t == STRING || t == DOUBLE) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je sve ok vracamo tip prom */
    return varType->copy();
}

Type* PredefineExprAST::typecheck() const {
    map<string, Type *>::iterator tmp = tablica.find(_s);
    if (tmp == tablica.end())
    {
        cerr << "Variable '" << _s << "' was not declared." << endl;
        exit(EXIT_FAILURE);
    }

    Type* exprType = _v[0]->typecheck();
    Type* varType = tmp->second;
    

    if(!exprType || !varType) {
        return NULL;
    } 

    /* ako je prom tipa ARRAY */
    if (varType->type() == ARRAY) {
        if(exprType->type() != ARRAY) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to array. " << endl;
            return NULL;
        }
        else {
            ArrayType* e = (ArrayType*)exprType;
            ArrayType* v = (ArrayType*)varType;
            if(e->getType()->type() != v->getType()->type()) {
                cerr << "Incompatible types of arrays. " << endl;
                return NULL;
            }
        }  
    }

    /* ako je prom tipa STRING */
    if(varType->type() == STRING) {
        if(exprType->type() != STRING) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa DOUBLE */
    if(varType->type() == DOUBLE) {
        int t = exprType->type();
        if(t == ARRAY || t == STRING) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa INT */
    if(varType->type() == INT) {
        int t = exprType->type();
        /*  cak i za double ne moze da kastuje u int tj izlazi greska da 
            ce doci do gubitka informacija ukoliko se to uradi */ 
        if(t == ARRAY || t == STRING || t == DOUBLE) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa CHAR */
    if(varType->type() == CHAR) {
        int t = exprType->type();
        if(t == ARRAY || t == STRING || t == DOUBLE) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je sve ok vracamo tip prom */
    return varType->copy();
}

Type* PredefineArrayExprAST::typecheck() const {
    map<string, Type *>::iterator tmp = tablica.find(_s);
    if (tmp == tablica.end())
    {
        cerr << "Variable '" << _s << "' was not declared." << endl;
        exit(EXIT_FAILURE);
    }

    Type* n = _v[0]->typecheck();
    if (n->type() != INT)
        return NULL;

    Type* exprType = _v[1]->typecheck();

    if(tmp->second->type() != ARRAY) {
        cerr << "Variable '" << _s << "' isn't type of array." << endl;
        return NULL; 
    }
    ArrayType* varType = (ArrayType*)tmp->second;

    Type* elementOfArrayType = varType->getType();

    if(!exprType || !elementOfArrayType) {
        return NULL;
    } 
    
    /* ako je prom tipa STRING */
    if(elementOfArrayType->type() == STRING) {
        if(exprType->type() != STRING) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << elementOfArrayType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa DOUBLE */
    if(elementOfArrayType->type() == DOUBLE) {
        int t = exprType->type();
        if(t == ARRAY || t == STRING) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << elementOfArrayType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa INT */
    if(elementOfArrayType->type() == INT) {
        int t = exprType->type();
        /*  cak i za double ne moze da kastuje u int tj izlazi greska da 
            ce doci do gubitka informacija ukoliko se to uradi */ 
        if(t == ARRAY || t == STRING || t == DOUBLE) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << elementOfArrayType->type() << endl;
            return NULL;
        }
            
    }

    /* ako je prom tipa CHAR */
    if(elementOfArrayType->type() == CHAR) {
        int t = exprType->type();
        if(t == ARRAY || t == STRING || t == DOUBLE) {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << elementOfArrayType->type() << endl;
            return NULL;
        }
    }

    /* ako je sve ok vracamo tip prom */
    return elementOfArrayType->copy();
}

Type* ElementOfArrayExprAST::typecheck() const {
    map<string, Type *>::iterator tmp = tablica.find(_s);
    if (tmp == tablica.end())
    {
        cerr << "Variable '" << _s << "' was not declared." << endl;
        exit(EXIT_FAILURE);
    }

    Type *exprType = _v[0]->typecheck();

    if (tmp->second->type() != ARRAY) // || tmp->second->type() != STRING)
    {
        cerr << "Variable '" << _s << "' isn't type of array." << endl;
        return NULL;
    }
    ArrayType *varType = (ArrayType *)tmp->second;

    Type *elementOfArrayType = varType->getType();

    if (!exprType || !elementOfArrayType)
    {
        return NULL;
    }

    /* ako je prom tipa STRING */
    if (elementOfArrayType->type() == STRING)
    {
        if (exprType->type() != STRING)
        {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << elementOfArrayType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa DOUBLE */
    if (elementOfArrayType->type() == DOUBLE)
    {
        int t = exprType->type();
        if (t == ARRAY || t == STRING)
        {
            cerr << "10 Incompatible types: " << exprType->type() << " cannot be converted to " << elementOfArrayType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa INT */
    if (elementOfArrayType->type() == INT)
    {
        int t = exprType->type();
        /*  cak i za double ne moze da kastuje u int tj izlazi greska da 
            ce doci do gubitka informacija ukoliko se to uradi */
        if (t == ARRAY || t == STRING || t == DOUBLE)
        {
            cerr << "11 Incompatible types: " << exprType->type() << " cannot be converted to " << elementOfArrayType->type() << endl;
            return NULL;
        }
    }

    /* ako je prom tipa CHAR */
    if (elementOfArrayType->type() == CHAR)
    {
        int t = exprType->type();
        if (t == ARRAY || t == STRING || t == DOUBLE)
        {
            cerr << "12 Incompatible types: " << exprType->type() << " cannot be converted to " << elementOfArrayType->type() << endl;
            return NULL;
        }
    }

    /* ako je sve ok vracamo tip prom */
    return elementOfArrayType->copy();
}

Type* SeqExprAST::typecheck() const {
    Type* e1 = _v[0]->typecheck();
    Type* e2 = _v[1]->typecheck();
    if(!e1 || !e2)
        return NULL;
    
    return e2->copy();
}

/* za konstruktor samo proverimo telo konstruktora */
Type* Constructor::typecheck() const {
    return _v[0]->typecheck();
}

/* za metod proverimo telo metoda i proverimo da li se poklapa tip povratne vrednosti sa onim sto se vraca */
Type* Method::typecheck() const {
    Type* body = _v[0]->typecheck();
    
    if(!body) {
        return NULL;
    }

    Type* retInstructionType = _v[1]->typecheck();

    /* ako je povratna vrednost niz onda se samo niz mora vratiti. I to niz elemenata istog tipa! */
    if(_retType->type() == ARRAY) {
        if(retInstructionType->type() != ARRAY)
            return NULL;
        else {
            ArrayType* retTypeA = (ArrayType*) _retType;
            ArrayType* retInstructionTypeA = (ArrayType*) retInstructionType;
            if(retTypeA->type() != retInstructionTypeA->type())
                return NULL;
        }
    }

    /* ako je povratna vrednost string onda se samo string mora vratiti */
    if(_retType->type() == STRING) {
        if(retInstructionType->type() != STRING)
            return NULL;
    }

    /* ako je povratna vrednost double onda se ne sme vratiti string ili niz */
    if(_retType->type() == DOUBLE) {
        if(retInstructionType->type() == STRING || retInstructionType->type() == ARRAY)
            return NULL;
    }

    /* ako je povratna vrednost int onda se ne sme vratiti double, string ili niz */
    if(_retType->type() == INT) {
        if(retInstructionType->type() == STRING || retInstructionType->type() == ARRAY || retInstructionType->type() == DOUBLE) 
            return NULL;
    }

    /* ako je povratna vrednost char onda se ne sme vratiti double, string ili niz */
    if(_retType->type() == CHAR) {
        if(retInstructionType->type() == STRING || retInstructionType->type() == ARRAY || retInstructionType->type() == DOUBLE) 
            return NULL;
    }

    return _retType->copy();
}

/* mislim da je svejedno sta vraca samo da ne vrati NULL */
Type* EmptyAST::typecheck() const {
    return new IntType();
}

string Constructor::getName() {
    return string(_name.c_str());
}