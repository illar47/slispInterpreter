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
	Expression computeSpecial(Expression toCompute);
	Expression compute(Expression& toCompute); 
	Expression evalRecursive(Expression& curr); 
	Expression computeGraphic(Expression toCompute); 
	std::vector<Expression> toDraw;
private:
	Environment env;
	Expression ast;


};


#endif