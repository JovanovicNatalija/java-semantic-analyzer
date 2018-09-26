%option noyywrap
%option nounput
%option noinput

%{
    #include <iostream>
    #include <cstdlib>
    #include <string>
    #include <cstring>
    #include "type.hpp"
    #include "ast.hpp"
    
    using namespace std;
    #include "parser.tab.hpp"
%}

%% 
"public"    return public_token;
"static"    return static_token;
"void"      return void_token;
"new"       return new_token;
"main"      return main_token;
"Main"      return Main_token;
"class"     return class_token; 
"private"   return private_token;
"return"    return return_token;
"int"|"double"|"char"|"String" {
    yylval.s = new string(yytext);
    return tip_token;
}
[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.s = new string(yytext);
    return id_token;
}
[+-]?[0-9]+\.[0-9]+ {
    return double_token;
}
[+-]?[0-9]+ {
    yylval.i = atoi(yytext);
    return int_token;
}
\'.\' {
    return char_token;
}
\".+\" {
    return string_token;
}
[+*/;()\[\]=,{}\.-] {
    return *yytext;
}
[\t\n ] {}
. {
    cerr << "Leksicka greska, nepoznat parametar " << endl;
    exit(EXIT_FAILURE);
}
%%