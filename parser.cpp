#include "parser.h"
#include "calcex.h"
#include <string>
#include <sstream>

Parser::Parser(istream* in) {
   scan = new Scanner(in);
}

Parser::~Parser() {
   try {
      delete scan;
   } catch (...) {}
}

AST* Parser::parse() {
   return prog();
}

AST* Parser::prog() {
   AST* result = expr();
   Token* t = scan->getToken();

   if (t->getType() != eof) {
     //cout << "Syntax Error: Expected EOF, found token at column " << t->getCol() << endl;
     cout << "* parser error" << endl;
     throw ParseError;
   }

   return result;
}

AST* Parser::expr() {
   return restExpr(term());
}

AST* Parser::restExpr(AST* e) {
   Token* t = scan->getToken();

   if (t->getType() == add) {
      return restExpr(new AddNode(e,term()));
   }

   if (t->getType() == sub)
      return restExpr(new SubNode(e,term()));

   scan->putBackToken();

   return e;
}

AST* Parser::term() {
  return restTerm(storable());
}

AST* Parser::restTerm(AST* e) {
  Token* t = scan->getToken();

  if(t->getType() == times){
    return restTerm(new TimesNode(e, storable()));
  }

  if(t->getType() == divide){
    return restTerm(new DivideNode(e, storable()));
  }

  if(t->getType() == mod){
    return restTerm(new ModNode(e, storable()));
  }

  scan->putBackToken();

  return e; 
}

AST* Parser::storable() {
  return memOperation(factor()); 
}

AST* Parser:: memOperation(AST* e) {
  Token* t= scan->getToken();

  if(t->getType() == keyword){
    if(t->getLex() == "S"){
      return new StoreNode(e);
    }else if(t->getLex() == "P"){
      return new PlusNode(e);
    }else if(t->getLex() == "M"){
      return new MinusNode(e);
    }else{
      //cout << "Expected S, P or M" << endl;
      cout << "* parser error" << endl;
      throw ParseError;
    }
  }

  scan->putBackToken();

  return e;  
}

AST* Parser::factor() {
  AST* result;
  Token* t = scan->getToken();

  if(t->getType() == identifier){
    return new VarNode(t->getLex());
  }
  
  if(t->getType() == number){
    istringstream in(t->getLex());
    int val;
    in >> val;
    return new NumNode(val);  
  }
  
  if(t->getType() == keyword)
    if(t->getLex() == "let")
      return assignable();
    else if(t->getLex() == "R")
      return new RecallNode();
    else if(t->getLex() == "C"){
      return new CleanNode();
    }else{
      //cout << "Expected R or C" << endl;
      cout << "* parser error" << endl;
      throw ParseError;
  }
  
  if(t->getType() == lparen){
    result = expr();
    t = scan->getToken();
    
    if(t->getType() == rparen)
      return result;
    else{
      //cout << "Expected )" << endl;
      cout << "* parser error" << endl;
      throw ParseError;
    }  
  }
      
  //cout << "Expected number, R, C, (" << endl;
  cout << "* parser error" << endl;
  throw ParseError; 
}

AST* Parser::assignable() {
  Token* t = scan->getToken();

  if(t->getType() == identifier) {
    Token* to = scan->getToken();

    if(to->getType() == equals)
      return new AssigNode(t->getLex(), expr());
  }
  
  cout << "* parser error" << endl;
  throw ParseError;   
}
   
