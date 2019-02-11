#pragma once

#include <string>
#include <map>
 
using namespace std;


class Calculator {
 public:
   Calculator();

   int eval(string expr);
   string comp(string expr);
   void store(int val);
   void plus(int val);
   void minus(int val);
   int clean();
   int recall();
   void addVar(string var, int val);
   bool containVar(string var);
   int getVal(string var);
   string getKeys();

 private:
   int memory;
   map<string, int> variables; 
};

extern Calculator* calc;



