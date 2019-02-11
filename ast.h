#pragma once
#include <string>
using namespace std;

class AST {
public:
  AST();
  virtual ~AST() = 0;
  virtual int evaluate() = 0;
  virtual string compile() = 0;
};

class BinaryNode : public AST {
public:
  BinaryNode(AST* left, AST* right);
  ~BinaryNode();

  AST* getLeftSubTree() const;
  AST* getRightSubTree() const;

private:
  AST* leftTree;
  AST* rightTree;
};

class UnaryNode : public AST {
public:
  UnaryNode(AST* sub);
  ~UnaryNode();

  AST* getSubTree() const;

private:
  AST* subTree;
};

class AddNode : public BinaryNode {
public:
  AddNode(AST* left, AST* right);
   
  int evaluate();
  string compile();
};

class SubNode : public BinaryNode {
public:
  SubNode(AST* left, AST* right);

  int evaluate();
  string compile();
};

class TimesNode : public BinaryNode {
public:
  TimesNode(AST* left, AST* right);

  int evaluate();
  string compile();
};

class DivideNode : public BinaryNode {
public:
  DivideNode(AST* left, AST* right);

  int evaluate();
  string compile();
};

class ModNode : public BinaryNode {
public:
  ModNode(AST* left, AST* right);

  int evaluate();
  string compile();
};

class NumNode : public AST {
public:
  NumNode(int n);

  int evaluate();
  string compile();

private:
  int val;
};

class VarNode : public AST {
public:
  VarNode(string s);

  int evaluate();
  string compile();

private:
  string var; 
};

class AssigNode : public UnaryNode {
public:
  AssigNode(string s, AST* sub);
  ~AssigNode();

  int evaluate();
  string compile();
private:
  string var;
};

class StoreNode : public UnaryNode {
public:
  StoreNode(AST* sub);
  ~StoreNode();
  int evaluate();
  string compile();
};

class PlusNode : public UnaryNode {
public:
  PlusNode(AST* sub);
  ~PlusNode();
  int evaluate();
  string compile();
};

class MinusNode : public UnaryNode {
public:
  MinusNode(AST* sub);
  ~MinusNode();
  int evaluate();
  string compile();
};

class CleanNode : public AST {
public:
  CleanNode();
  ~CleanNode();
  int evaluate();
  string compile();
};

class RecallNode : public AST {
public:
  RecallNode();
  ~RecallNode();
  int evaluate();
  string compile();
};
