#include "ast.hpp"
#include <iostream>
#include <cstdlib>
#include <typeinfo>
#include <map>

extern Type* getVarFromTable(string s);
extern void end_scope();

extern map<string, Class*> tablicaKlasa;

ExprAST::~ExprAST() {

}

ConstantExprAST::~ConstantExprAST() {
    delete _t;
}

Type *ConstantExprAST::typecheck() const {
    return _t->copy();
}


Type *VariableExprAST::typecheck() const
{
    Type* t = getVarFromTable(_name);

    return t->copy();
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

    /* ne moze + sa klasama! */
    if(l->type() == CLASS || d->type() == CLASS) {
        cerr << "There is no operator + for class." << endl;
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

    /* ne moze - sa klasama! */
    if (l->type() == CLASS || d->type() == CLASS)
    {
        cerr << "There is no operator - for class." << endl;
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

    /* ne moze * sa klasama! */
    if (l->type() == CLASS || d->type() == CLASS)
    {
        cerr << "There is no operator * for class." << endl;
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

    /* ne moze / sa klasama! */
    if (l->type() == CLASS || d->type() == CLASS)
    {
        cerr << "There is no operator / for class." << endl;
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

extern map < string, vector < Type* > > tableOfVars; 

/* prepravljeno */
Type* AssignExprAST::typecheck() const {
    map < string, vector < Type* > >::iterator tmp = tableOfVars.find(_s);
    if(tmp == tableOfVars.end()) {
        cerr << "Variable '" << _s << "' used, but never defined." << endl;
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

    /* ako je prom tipa CLASS */
    if (varType->type() == CLASS)
    {
        int t = exprType->type();
        ClassType* ct_v = (ClassType*)varType;
        ClassType* ct_e = (ClassType*)exprType;
        if (t != CLASS || ct_v->getName() != ct_e->getName())
        {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je sve ok vracamo tip prom */
    return varType->copy();
}

Type* PredefineExprAST::typecheck() const {
    Type* varType = getVarFromTable(_s);
    Type* exprType = _v[0]->typecheck();
    
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

     /* ako je prom tipa CLASS */
    if (varType->type() == CLASS)
    {
        int t = exprType->type();
        ClassType *ct_v = (ClassType *)varType;
        ClassType *ct_e = (ClassType *)exprType;
        if (t != CLASS || ct_v->getName() != ct_e->getName())
        {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je sve ok vracamo tip prom */
    return varType->copy();
}

Type* PredefineArrayExprAST::typecheck() const {
    Type* varType = getVarFromTable(_s);

    Type* n = _v[0]->typecheck();
    if (n->type() != INT)
        return NULL;

    Type* exprType = _v[1]->typecheck();

    if(varType->type() != ARRAY) {
        cerr << "Variable '" << _s << "' isn't type of array." << endl;
        return NULL; 
    }
    ArrayType* varType2 = (ArrayType*)varType;

    Type* elementOfArrayType = varType2->getType();

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

    /* ako je prom tipa CLASS */
    if (elementOfArrayType->type() == CLASS)
    {
        int t = exprType->type();
        ClassType* ct_v = (ClassType*)varType;
        ClassType* ct_e = (ClassType*)exprType;
        if (t != CLASS || ct_v->getName() != ct_e->getName())
        {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
            return NULL;
        }
    }

    /* ako je sve ok vracamo tip prom */
    return elementOfArrayType->copy();
}

Type* ElementOfArrayExprAST::typecheck() const {
    Type* varType = getVarFromTable(_s);

    Type *exprType = _v[0]->typecheck();

    if (varType->type() != ARRAY)
    {
        cerr << "Variable '" << _s << "' isn't type of array." << endl;
        return NULL;
    }

    ArrayType *varType2 = (ArrayType *)varType;

    Type *elementOfArrayType = varType2->getType();

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

    /* ako je prom tipa CLASS */
    if (elementOfArrayType->type() == CLASS)
    {
        int t = exprType->type();
        ClassType* ct_v = (ClassType*)varType;
        ClassType* ct_e = (ClassType*)exprType;
        if (t != CLASS || ct_v->getName() != ct_e->getName())
        {
            cerr << "Incompatible types: " << exprType->type() << " cannot be converted to " << varType->type() << endl;
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
    Type* constrTypeCheck = _v[0]->typecheck();
    end_scope();
    return constrTypeCheck->copy();
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

    end_scope();

    return _retType->copy();
}

Type* Class::typecheck() const {
    Type* constr = _constructor->typecheck();
    if(constr == NULL)
        return NULL;
    /* za slucaj da nema type bolje neka bude bilo koji tip nego null */
    Type* method = new IntType();
    for(Method* m: _methods) {
        method = m->typecheck();
        if(method == NULL)
            return NULL;
    }

    end_scope();
    return method->copy();
}

Type* MainClass::typecheck() const {
    return _body->typecheck();
}

Type *MethodExprAST::typecheck() const {
    Type *varType = _v[0]->typecheck();
    if(!varType) {
        return NULL;
    }

    if(varType->type() != CLASS) {
        cerr << "Incomatibile type! It must be class type." << endl;
        exit(EXIT_FAILURE);
    }

    /* dohvata ime klase */
    ClassType* ct = (ClassType*)varType;
    string name = ct->getName();
    map<string, Class*>::iterator tmp = tablicaKlasa.find(name);
    /* mora da postoji */
    Class* varClass = tmp->second;

    /* dohvata imena metoda date klase i proverava da li data klasa postoji */

    vector<Method*> varClassMethods = varClass->getMethods();
    Method* method = NULL;
    for(auto m : varClassMethods) {
        if(m->getName() == _methodName) {
            method = m;
        }
    }
    /* ako ne postoji */
    if(method == NULL) {
        cerr << "Method " << _methodName << " does not exist in class " << name << endl;
        exit(EXIT_FAILURE);
    }

    /* proverava se dimenzija niza argumenata */
    if(method->getSize() != _arrayArg->size()) {
        cerr << "Invalid dimension" << endl;
        exit(EXIT_FAILURE);
    }

    vector<Type*> *types = method->getTypes();

    vector<ExprAST*>::iterator it_ex;
    vector<Type*>::iterator it_ty;
    for (it_ex = _arrayArg->begin(), it_ty = types->begin(); it_ex != _arrayArg->end(), it_ty != types->end(); it_ex++, it_ty++) {
        ExprAST* e = (ExprAST*)*it_ex;
        Type* t = (Type*)*it_ty;
        if(e->typecheck()->type() != t->type()) {
            cerr << e->typecheck()->type() << endl << t->type() << endl;
            cerr << "Types must match!" << endl;
            exit(EXIT_FAILURE);
        }
    }

        /* vraca se povratni tip metoda */
        return method->getRetType();
}

Type* FieldExprAST::typecheck() const {
    Type *varType = _v[0]->typecheck();
    if (!varType)
    {
        return NULL;
    }

    if(varType->type() != CLASS) {
        cerr << "Incomatibile type! It must be class type." << endl;
        exit(EXIT_FAILURE);
    }

    /* dohvata ime klase */
    ClassType *ct = (ClassType *)varType;
    string name = ct->getName();
    map<string, Class *>::iterator tmp = tablicaKlasa.find(name);
    /* mora da postoji */
    Class *varClass = tmp->second;

    vector<Field*> varClassFields = varClass->getFields();
    Field* field = NULL;
    for(auto f : varClassFields) {
        if (f->getName() == _fieldName) {
            field = f;
        }
    }

    /* ako ne postoji */
    if(field == NULL) {
        cerr << "Method " << _fieldName << " does not exist." << endl;
        exit(EXIT_FAILURE);
    }

    return field->getType();
}

vector<Method*> Class::getMethods() {
    return _methods;
}

vector<Field*> Class::getFields() {
    return _fields;
}

string Field::getName() {
    return _name;
}

string Method::getName() {
    return _name;
}

Type *Method::getRetType() {
    return _retType;
}

unsigned Method::getSize() {
    return _arrayArg.size();
}

vector<Type*> *Method::getTypes() {
    vector<Type*> *types = new vector<Type*>;
    for(auto e : _arrayArg) {
        types->push_back(e.first);
    }
    return types;
}

/* mislim da je svejedno sta vraca samo da ne vrati NULL */
Type* EmptyAST::typecheck() const {
    return new IntType();
}

string Constructor::getName() {
    return _name;
}