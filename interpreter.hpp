#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

// system includes
#include <string>
#include <istream>


// module includes
#include "expression.hpp"
#include "environment.hpp"

// Interpreter has
// Environment, which starts at a default
// parse method, builds an internal AST
// eval method, updates Environment, returns last result
class Interpreter {
public:
	bool parse(std::istream& expression) noexcept;
	Expression eval();
	Expression getAst(); 
	Expression appendToAst(Expression toAppend); 
	void clearAst(); 
	Expression setAst(Expression toSet); 
private:
	Environment env;
	Expression ast;

};


#endif