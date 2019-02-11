#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "calcex.h"
#include "calculator.h"

using namespace std;

Calculator* calc;

//private noInteractive=false;

//void setMode(bool);

//bool getMode();

void detectCommand(int, char**);
void readEnv(char**);
bool noInteractive(int, char**);
string getText(string);
bool c(int, char**);
void compiler(ifstream&, string);
void noCompiler(ifstream&);

int main(int argc, char* argv[], char** envp) {

     string line;
     string chain;
     ifstream archivo;
     
     calc = new Calculator();

     readEnv(envp);
     detectCommand(argc, argv);

     if(noInteractive(argc, argv)){
       bool fail = false;
       
       for(int i=1; i<argc; i++){
	 fail = false;
	 chain = argv[i];

	 if(chain.size()>4 and chain.substr(chain.size()-5, chain.size())==".expr"){
	   archivo.open(argv[i], ios::in);
	   if(archivo.fail()){
	     cout << "Error when reading file: " << argv[i] << endl;
	     fail = true;
	   }

	   if(c(argc, argv) and !fail){
             compiler(archivo, chain);
	   }else{
             if(!fail)
	       noCompiler(archivo);
	   }

	   archivo.close();
	 }
       }
     }else{ 
       while(true){
	 try { 
	   cout << "> ";

	   getline(cin, line);

	   if(!cin) break;

	   int result = calc->eval(line);

	   cout << "= " << result << endl;
	 }catch(Exception ex) {
	   //cout << "Program Aborted due to exception!" << endl;
	   //cout << "*Parser Error" << endl;
	 } 
       }
     }
     delete calc;

}
  
void detectCommand(int argc, char* argv[]){
  bool v = false;
  bool e = false; 
  string var;
  string val;
  int value;

  for(int i=1; i<argc; i++){
    if(strncmp(argv[i], "-e", 2)==0){
      e = true;
    }

    if(strncmp(argv[i], "-v", 2)==0) {
      v = true;
      string chain = argv[i+1];
      int ind = chain.find_first_of('=');
      var = chain.substr(0, ind);
      val = chain.substr(ind+1);
	 
      if(e){
        if(!calc->containVar(var))
          calc->addVar(var, stoi(val));
      }else
        calc->addVar(var, stoi(val));  
    }
  }
}

bool c(int argc, char** argv) {
  for(int i=1; i<argc; i++){
    if(strncmp(argv[i], "-c", 2)==0) {
      return true; 
    }
  }
  return false;
}

void readEnv(char** envp){
  string chain;
  string var;
  string val;
  int ind;
  
  for(int i=0; envp[i]; ++i){
    chain = envp[i];
    if(chain.substr(0,7) == "CALCVAR"){
      ind = chain.find_first_of('=');
      var = chain.substr(7, ind-7);
      val = chain.substr(ind+1);
      calc->addVar(var, stoi(val));
    }
  }
}

void compiler(ifstream &archivo, string chain) {
  string line;
  string lineOut;
  string temp = "";
  
  while(!archivo.eof()){
    try { 
      getline(archivo, line);     
      
      string printer = temp + "\n# print value produced \n"+
	"OP1 := M[SP+1] \n"+
	"writeInt(OP1) \n"+
	"SP  := SP + ONE \n";
		
      lineOut = lineOut + calc->comp(line) + printer;
    }catch(Exception ex) {
      //cout << "Program Aborted due to exception!" << endl;
      //cout << "* parser error" << endl;
    } 	    	      
  }

  ofstream archivoOut;
  archivoOut.open(chain.substr(0, chain.size()-4)+"ewe", ios::out);

  archivoOut << getText(lineOut);
  archivoOut.close();
}

void noCompiler(ifstream &archivo) {
  string line;
  while(!archivo.eof()){
    try { 
      getline(archivo, line);
      int result = calc->eval(line);
		
      cout << "= " << result << endl;
    }catch(Exception ex) {
      //cout << "Program Aborted due to exception!" << endl;
      //cout << "* parser error" << endl;
    } 	    	      
  }
}

bool noInteractive(int argc, char** argv) {
  string chain;
  for(int i=1; i<argc; i++){
    chain = argv[i];
    if(chain.size()>4 and chain.substr(chain.size()-5, chain.size())==".expr"){
      return true;
    }
  }

  return false;
}

string getText(string lineOut) {
  string temp="";
  string head = temp + "\n# Generated automatic don't edit \n"+
    "SP   := 4000 \n"+
    "ONE  := 1 \n"+
    "ZERO := 0 \n";
  string endFile = temp + "\n# Memory definition \n"+
    "equ ZERO   M[0] \n"+
    "equ ONE    M[1] \n"+
    "equ OP1    M[2] \n"+
    "equ OP2    M[3] \n"+
    "equ SP     M[4] \n"+
    "equ MEMORY M[5] \n"+
    "equ VALUE  M[6] \n"+
    calc->getKeys();

  return head + lineOut + "\nhalt\n" + endFile; 
}
