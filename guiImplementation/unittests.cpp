#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include <string>
#include <sstream>

#include "tokenize.hpp"
#include "expression.hpp"
#include "interpreter.hpp"

//my includes
#include <stdio.h>
#include <math.h>

// Helper functions 

/**
* helper function from test_interpreter file 
*/
static Expression runInterp(const std::string& program) {

	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);
	if (!ok) {
		std::cerr << "parsing error when calling:  " << program << std::endl;
	}
	REQUIRE(ok == true);

	Expression result;
	REQUIRE_NOTHROW(result = interp.eval());

	return result;
}

/**
* helper function that determines if a provided string can
* be parsed or not
*
* @param program - the string to be parsed and checked
* @return true if the string is NOT able to be parsed, else return false
*/
Boolean badParse(const std::string& program) {

	std::istringstream iss(program);

	Interpreter interp;

	return !interp.parse(iss);
}

/**
* helper function that tests for a good parse, but a bad evaluation 
* by utilizing the testing framework 
* 
* @param program - the calculation to parse and check 
*/

void badEval(const std::string& program) {
	std::istringstream iss(program);

	Interpreter interp;
	bool ok = interp.parse(iss); 

	REQUIRE(ok == true); 
	Expression result; 
	REQUIRE_THROWS(result = interp.eval()); 
}
/**
* Helper function that detemines if a provided string can be evaluated 
* or not 
* 
* @param program - the string (i.e the program) to be parsed and to 
	attempt to evaluate
* @return true if the string is NOT able to be evaluated, else return false
* @throw error if cannot be parsed. 
*/

/**
* helper function from test_interpreter file
*/
Expression runInterpFile(const std::string& fname) {

	std::ifstream ifs(fname);
	REQUIRE(ifs.good() == true);

	Interpreter interp;

	bool ok = interp.parse(ifs);
	if (!ok) {
		std::cerr << "parsing error when calling: " << fname << std::endl;
	}
	REQUIRE(ok == true);

	Expression result;
	REQUIRE_NOTHROW(result = interp.eval());

	return result;
}

//tokenize tests 

/**
* testing basic comment checking ablility 
*/
TEST_CASE("test Tokenizer with  one comment", "[tokenize]") {
	std::string program = "(define r 10) ; (* r r) \n (* 2 2)";

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);

	REQUIRE(tokens.size() == 10); 
	REQUIRE(tokens[0] == "("); 
	REQUIRE(tokens[1] == "define");
	REQUIRE(tokens[2] == "r");
	REQUIRE(tokens[3] == "10");
	REQUIRE(tokens[4] == ")");
	REQUIRE(tokens[5] == "(");
	REQUIRE(tokens[6] == "*");
	REQUIRE(tokens[7] == "2");
	REQUIRE(tokens[8] == "2");
	REQUIRE(tokens[9] == ")");
}

/**
* checking that the tokenizer is able to handle weird and varried comment placement
*/
TEST_CASE("test Tokenizer with multiple coments", "[tokenize]") {
	std::string program = "(define r 10) ; (* r r) \n ;another comment for fun \n (* 2 2) ;last comments";

	std::istringstream iss(program);

	TokenSequenceType tokens = tokenize(iss);


	REQUIRE(tokens.size() == 10);
	REQUIRE(tokens[0] == "(");
	REQUIRE(tokens[1] == "define");
	REQUIRE(tokens[2] == "r");
	REQUIRE(tokens[3] == "10");
	REQUIRE(tokens[4] == ")");
	REQUIRE(tokens[5] == "(");
	REQUIRE(tokens[6] == "*");
	REQUIRE(tokens[7] == "2");
	REQUIRE(tokens[8] == "2");
	REQUIRE(tokens[9] == ")");
}

//expression tests 

/*
* TODO:
* - test Operator== for all edge cases (theres alot to be tested T-T)
* - test constructors for Arc, Line, and Point 
* 
* - print expression???
*/



//test printExpression
TEST_CASE("test printExpression functionality - basic ", "[expression]") {
	Expression newExpression; 
	std::string toPrint; 
	newExpression.head.value.num_value = 45; 
	newExpression.head.type = NumberType; 
	toPrint = newExpression.printExpression(newExpression); 

	REQUIRE(toPrint == "(45)"); 

	newExpression.head.value.bool_value = true; 
	newExpression.head.type = BooleanType; 
	toPrint = newExpression.printExpression(newExpression);

	REQUIRE(toPrint == "(True)"); 

	newExpression.head.value.sym_value = "a"; 
	newExpression.head.type = SymbolType; 
	toPrint = newExpression.printExpression(newExpression);

	REQUIRE(toPrint == "(a)"); 

	newExpression.head.type = NoneType; 
	REQUIRE_THROWS(toPrint = newExpression.printExpression(newExpression));

}

//interpreter tests

TEST_CASE("test division by zero handling", "[interpreter]") {
	
	Expression result = runInterp("(/ 4 0)"); 
	REQUIRE(isinf(result.head.value.num_value)); 
}

TEST_CASE("testing improper operations", "[interpeter]") {
	 
	//math operations with booleans provided
	REQUIRE(badParse("+ true true")); 
	REQUIRE(badParse("* true false")); 
	REQUIRE(badParse("- false true")); 
	REQUIRE(badParse("/ false false")); 

	//boolean operations with numeric values provided 

 
}

TEST_CASE("testing invalid special operations", "[interpreter]") {
	Interpreter interp; 

	//define without a valid symbol 
	REQUIRE(badParse("(define)")); 
	REQUIRE(badParse("(define 3 4)")); 
	REQUIRE(badParse("define begin 5")); 

	//begin not at begining
	REQUIRE(badParse("(- 4 (begin (* 4 5)))")); 

	//multiple begins 
	REQUIRE(badParse("(begin (-4 (begin (* 4 5))))")); 
	
	//if with invalid params 65
	badEval("(if 5 4 8)"); 
}

TEST_CASE("testing parsing", "[interpreter]") {
	Interpreter interp; 
	std::istringstream input("(- a b)");
	REQUIRE(interp.parse(input)); 
}


TEST_CASE("testing get, set, clear, and append", "[interpreter]") {
	Interpreter interp; 
	std::istringstream input("(- a b)");
	//empty ast - getAst() test 
	Expression ast = interp.getAst(); 
	REQUIRE(ast.head.type == NoneType); 

	//ast with values in it - getAst() test 
	interp.parse(input); 
	ast = interp.getAst();  
	REQUIRE(ast.head.type == SymbolType); 
	REQUIRE(ast.head.value.sym_value == "-"); 
	REQUIRE(ast.tail.size() == 2); 

	//setAst() test
	Expression newAst; 
	newAst.head.type = BooleanType; 
	newAst.head.value.bool_value = true;

	interp.setAst(newAst); 
	ast = interp.getAst();
	REQUIRE(ast.head.type == BooleanType); 
	REQUIRE(ast.head.value.bool_value == true); 
	REQUIRE(ast.tail.empty()); 

	//clear Ast test
	interp.clearAst(); 
	ast = interp.getAst(); 
	REQUIRE(ast.head.type == NoneType); 
	REQUIRE(ast.tail.empty()); 
	


	std::istringstream beginInput("(begin (define a 1) (define b 2))");
    std::istringstream toAppend("(if ( < a b) b a)"); 

	REQUIRE(interp.parse(beginInput)); 
	REQUIRE(interp.getAst().head.type == NoneType); 
	REQUIRE(interp.getAst().head.value.sym_value == "begin"); 
	REQUIRE(interp.getAst().tail.size() == 2); 

	Interpreter tempInterp;
	REQUIRE(tempInterp.parse(toAppend)); 
	REQUIRE(tempInterp.getAst().head.type == NoneType);
	REQUIRE(tempInterp.getAst().head.value.sym_value == "if"); 

	interp.appendToAst(tempInterp.getAst());

	REQUIRE(interp.getAst().head.type == NoneType);
	REQUIRE(interp.getAst().head.value.sym_value == "begin");
	REQUIRE(interp.getAst().tail.size() == 3); 

	ast = interp.eval(); 
	REQUIRE(ast.head.type == NumberType); 
	REQUIRE(ast.head.value.num_value == 2);
}

TEST_CASE("invalid appends for appendToAst() method", "[interpreter]") {
	
	Interpreter interp;
	Interpreter appendCreator;
	Expression ast; 
	std::istringstream input1("(- a b)");
	std::istringstream input2("(begin (define a 1) (define b 2))");
	std::istringstream input3("(begin (- 4 2))");

	//current AST doesn't start with special type begin 
	interp.parse(input1); 
	appendCreator.parse(input1); 
	ast = interp.getAst(); 

	REQUIRE(ast.head.type == SymbolType); 
	REQUIRE(ast.head.value.sym_value == "-"); 
	REQUIRE(ast.tail.size() == 2); 

	interp.appendToAst(appendCreator.getAst()); 
	ast = interp.getAst(); 

	REQUIRE(ast.head.type == SymbolType);
	REQUIRE(ast.head.value.sym_value == "-");
	REQUIRE(ast.tail.size() == 2);

	//toAppend starts with special type begin 

	interp.parse(input2); 
	appendCreator.parse(input3); 
	ast = interp.getAst(); 

	REQUIRE(ast.head.type == NoneType); 
	REQUIRE(ast.head.value.sym_value == "begin"); 
	REQUIRE(ast.tail.size() == 2); 

	interp.appendToAst(appendCreator.getAst()); 
	ast = interp.getAst(); 

	REQUIRE(ast.head.type == NoneType);
	REQUIRE(ast.head.value.sym_value == "begin");
	REQUIRE(ast.tail.size() == 2);


}

//slip tests ? 