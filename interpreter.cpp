#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>
#include <iostream>

// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"

//my includes 
#include <map>
#include <math.h>
#include <stdio.h>

std::map<Symbol, Expression> definitions;
Expression evalRecursive(Expression& curr); 
Expression computeSpecial(Expression toCompute); 

/**
* A helper function that checks if a string is one of the special form commands. 
*/
bool isSpecialForm(std::string toCheck) {
	return toCheck == "begin" || toCheck == "if" || toCheck == "define"; 
}

/**
* Helper function that determines if a string is a valid operator 
*/
bool isOperator(std::string toCheck) {
	return toCheck == "not" || toCheck == "and" || toCheck == "or"
		|| toCheck == "<" || toCheck == ">=" || toCheck == ">" || toCheck == "<="
		|| toCheck == "=" || toCheck == "+" || toCheck == "-"
		|| toCheck == "*" || toCheck == "/" || toCheck == "log10" || toCheck == "pow"; 
}

/**
* Helper function that determines if operator is a binary operator
* 
* note - is both binary and unary depending on use
*/
bool isBinaryOperator(std::string toCheck) {
	return toCheck == "<" || toCheck == ">=" || toCheck == ">" || toCheck == "<="
		|| toCheck == "=" || toCheck == "-"
		|| toCheck == "/" ||  toCheck == "pow";
}

/**
* helper function that determines if operator is a unary operator
* 
* note - is both binary and unary depending on use 
* 
*/
bool isUnaryOperator(std::string toCheck) {
	return toCheck == "not"
		||  toCheck == "-"
	    || toCheck == "log10";
}

/** 
* Helper function for checking if something is an m-ary operator
*/
bool isMaryOperator(std::string toCheck) {
	return toCheck == "and" || toCheck == "or" || toCheck == "+" || toCheck == "*"; 
}

/**
* updates the AST by storing noted values (i.e. numOfVals starting from the back)
* into the tail of the provided head
*/
void updateExpressionAST(Expression &head, std::vector<Expression> &expressions, int &numOfVals) {
	//check that provided head is the back of expressions 
	
	expressions.pop_back(); 
	//sos send help XD
	if (expressions.size() >= numOfVals - 1) {
		std::vector<Expression> newVect;
		for (int i = 0; i < numOfVals - 1 && !expressions.empty(); i++) {
			newVect.push_back(expressions.back()); 
			expressions.pop_back(); 
		}
		head.tail = newVect; 
		newVect.clear(); 
		//figure out how to deconstruct 
	}
	else {
		head.tail = expressions;
		expressions.clear();
	}
	
	expressions.push_back(head); 
	//numOfVals = expressions.size(); 

}

/**
* Itterates through a generated list of tokens to construct a tree of values
* to later evaluate. 
*/
bool Interpreter::parse(std::istream & expression) noexcept{
  // return true if input is valid. otherwise, return false.

	//Expression newAst; //idk if needed 

	std::vector<Expression> prevExpressions; //a place to store expressions. 
	definitions.clear(); 
	//tokenize expression 
	TokenSequenceType tokens = tokenize(expression); 


	//check if valid type
	if (tokens.size() == 1) {
		Atom checkAtom; 
		bool check = token_to_atom(tokens.front(), checkAtom); 
		if (check && 
			(checkAtom.type == SymbolType || checkAtom.type == NoneType)) {
			return false; 
		}
	}
	//check if tokens contains any values 
	else if (tokens.empty()) {
		return false;
	}
	std::string currToken = tokens.back(); 
	int tokenSize = tokens.size(); 
	int openCount = 0; //count of the number of OPEN tokens currently found. 
	int numOfVals = 0; 
	
	//for each token, check if it's an atom. 
	for (int i = 0; i < tokenSize; i++) {
		//actions 
		Atom currAtom; //atom to hold valid token 

		bool isAtom = token_to_atom(currToken, currAtom); //attempt to transform token into an atom

		//check if atom was sucessfully made. 
		/*if (!isAtom && !isSpecialForm(currToken)
			&& currToken != ")" && currToken != "(") {
			return false; 
		}*/

		//if atom was made
		if (isAtom || currToken == ")" || currToken == "(") {
			//check if is OPEN or CLOSE
			//if OPEN: note that
			if (currToken == ")") {
				openCount++; 
				numOfVals = 0; 
			}
			//if CLOSE: check that marked as closed then check that most recent atom is an operator. store all previous expressions into that one's tail 
			else if (currToken == "(") {
				//invalid cases: no closing bracket recorded, no pre recorded expressions
				if (openCount <= 0 || prevExpressions.empty()) {
					return false; 
				}
			
				Expression newHead; 
				//store operation in expression if valid 
				if (!prevExpressions.empty()) {
					 newHead = prevExpressions.back();
				}
				 
				//if operator: check that there are enough values either unary or binary depending on operation
				if (newHead.head.type == SymbolType 
					&& isOperator(newHead.head.value.sym_value)) {
					//check rest of values in vector are NOT Operators
					if (prevExpressions.size() == 1) {
						return false; 
					}
					
					// - check: has only 1 -2 values aside from the head
					if (newHead.head.value.sym_value == "-" && numOfVals <= 3) {
						//store normally if only one or two operations
						updateExpressionAST(newHead, prevExpressions, numOfVals); 
					}
					
					//unary operation check: only has 1 value aside from head
					else if (isUnaryOperator(newHead.head.value.sym_value) && numOfVals <= 2) {
						updateExpressionAST(newHead, prevExpressions, numOfVals);
					}
					//binary operation check: has exactly 2 values aside from head
					else if (isBinaryOperator(newHead.head.value.sym_value) && numOfVals == 3) {
						updateExpressionAST(newHead, prevExpressions, numOfVals); 
					}
					//m-ary check: has more than 1 values aside from head
					else if (isMaryOperator(newHead.head.value.sym_value) && numOfVals > 2){
						updateExpressionAST(newHead, prevExpressions, numOfVals); 
					}
					else {
						return false; 
					}
					
					
				}
				
				//if not operator: check that there is only the one value and that it is not a symbol 
				else if (numOfVals == 1
					&& prevExpressions.back().head.type != SymbolType
					&& prevExpressions.back().head.type != NoneType) {
				}
				else if (prevExpressions.back().head.type == SymbolType 
					&& numOfVals == 1 && openCount > 1 && tokens.size() > 1) {

				}
				//check if it's special and update
				else if (isSpecialForm(prevExpressions.back().head.value.sym_value)) {
					newHead = prevExpressions.back(); 
					if (prevExpressions.back().head.value.sym_value == "define") {
						if (numOfVals < 3) {
							return false; 
						}
						if (prevExpressions.at(prevExpressions.size() - 2).head.type == NoneType
							|| prevExpressions.at(prevExpressions.size() - 2).head.type == NumberType) {
							return false; 
						}
						numOfVals = 3; 
					}
					if (prevExpressions.back().head.value.sym_value == "begin" && i + 1 < tokenSize) {
						return false; 
					}
					updateExpressionAST(newHead, prevExpressions, numOfVals); 
				}
				else {
					return false; 
				}

				//check if list of expressions is empty and that curr atom is empty

				openCount--; 
				numOfVals = 0;
				if (openCount > 0) {
					numOfVals = prevExpressions.size(); 
				}
				

				 
			}


			//if issue with OPEN or CLOSE values return false (i.e. CLOSE and no OPEN, or left OPEN)

			//if not open or close, create an expression with empty tail.
			else {
				//add check for when it's the only object in the list?

				//Expression newExpress = new Expression(currAtom); 
				prevExpressions.push_back(Expression (currAtom)); 
				numOfVals++; 
			}
		}

		//if token is a special form
		else {
			//if special-form = "begin" - case where previous token was begin and now new stuffs? 
			if (isSpecialForm(currToken)) {
				prevExpressions.push_back(Expression(currToken)); 
				prevExpressions.back().head.type = NoneType;
				numOfVals++; 
			}
			//just in case~
			else {
				return false; 
			}
		}
		//increment currToken
		tokens.pop_back(); 
		if (!tokens.empty()) {
			currToken = tokens.back();
		}
		
	}
	//if not properly closed
	if ((openCount > 0)
		|| (!isOperator(prevExpressions.back().head.value.sym_value)
			&& !isSpecialForm(prevExpressions.back().head.value.sym_value) && prevExpressions.size() > 1)
		|| (prevExpressions.size() != 1)) {
		return false; 
	}

	/*//check if last value i.e. top of tree is an operator or special symbol 
	//TODO modify for final version later - check the head of newAst instead!
	else if (!isOperator(prevExpressions.back().head.value.sym_value)
		&& !isSpecialForm(prevExpressions.back().head.value.sym_value) && prevExpressions.size() > 1) {
		return false; 
	}
	//add check for complete pairs of OPEN and CLOSE
	// add check for stored expressions being updated (in case there were no OPEN CLOSE PAIRS)
	//update the internal ast
	else if (prevExpressions.size() != 1) {
		return false; 
	}*/
	this->ast = prevExpressions.front(); 


    return true;
};

/**
* Realized that I am dumb and that I don't need to have two separate 
* getVal functions. Hopefully this one works T-T
*/
Expression getVal(Expression& toFind) {

	std::string label = toFind.head.value.sym_value;
	//search it and find it's pair 
	if (definitions.find(label) == definitions.end()) {
		std::cout << "Error: unknown symbol" << std::endl;
		throw InterpreterSemanticError("undefined value");
	}
    return definitions.at(label); 
	
}
/**
* helper function that determiens the numeric equivalent
* (if it has been logged) of a provided SymbolType value
*/
Number getNumVal(Expression& toFind) {
	//have a lib of pairs with letter and num vals 
	std::string label = toFind.head.value.sym_value; 
	//search it and find it's pair 
	if (definitions.find(label)== definitions.end()) {
		std::cout << "Error: unknown symbol" << std::endl; 
		throw InterpreterSemanticError("undefined value"); 
	}
	Number value = definitions.at(label).head.value.num_value; 
	toFind.head.value.num_value = value; 
	//store in num section
	return value;

}

/**
* helper function that determines the boolean equivalent
* (if it has been logged) of a provided symbolType value
*/
Boolean getBoolVal(Expression& toFind) {
	std::string label = toFind.head.value.sym_value; 
	if (definitions.find(label) == definitions.end()) {
		std::cout << "Error: unknown symbol" << std::endl;
		throw InterpreterSemanticError("undefined value");
	}
	if (definitions.at(label).head.type != BooleanType) {
		std::cout << "Error: invalid request" << std::endl;
		throw InterpreterSemanticError("invalid search");
		
	}

	return (definitions.at(label).head.value.bool_value);

	 

}
 /**
 * Helper function for computing special operations 
 */
 Expression computeSpecial(Expression toCompute){
	 Expression result; 
	 //begin operation: evaluate each expression in order to the last
	 if (toCompute.head.value.sym_value == "begin") {
		 //should be fine
		 for (int i = 0; i < toCompute.tail.size(); i++) {
			 toCompute.tail.at(i) = evalRecursive(toCompute.tail.at(i)); 
			 
		 }
		 result = toCompute.tail.at(toCompute.tail.size() - 1); 
		 //throw error if theres more above it? 
	 }
	 //if operation: eval result of exp 2 if exp 1 eval to true, also eval 3
	 else if (toCompute.head.value.sym_value == "if") {
		 //error if exp 1 isn't a bool 
		 Expression toCheck = evalRecursive(toCompute.tail.at(0)); 
		 if (toCheck.head.type != BooleanType) {
			 //throw error 
			 std::cout << "Error: user input invalid compare type"; 
			 throw InterpreterSemanticError("invalid parameter for if"); 
		 }

		 //if exp 1 is true eval exp 2
		 if (toCheck.head.value.bool_value) {
			 result = evalRecursive(toCompute.tail.at(1)); 
			 //undo hardcode later
			 result.head.type = NumberType; 
		 }
		 //else eval exp 3
		 else {
			 result = evalRecursive(toCompute.tail.at(2)); 
			 result.head.type = NumberType;
		 }
	 }
	 //define operation 
	 else if (toCompute.head.value.sym_value == "define") {
		
		 
		 
		 //it's an error to re-define a symbol 
		 Symbol label = toCompute.tail.front().head.value.sym_value;
		 Expression value = evalRecursive(toCompute.tail.back());

		 //check if theres a symbol as a defintion and find it's value
		 if (value.head.type == SymbolType && definitions.find(value.head.value.sym_value) != definitions.end()) {
			 value = definitions.at(value.head.value.sym_value); 
		 }
		 toCompute.tail.back() = value; 
		 definitions.insert(std::pair<Symbol, Expression>(label,value));
		 result = value; 
	 }
	 return result; 
 }

 Expression binaryOp(std::string op, Expression &toCompute, Expression curr) {
	 Expression toReturn; 
	 // < operation 
	 if (op == "<") {
		 Expression val1 = toCompute.tail.at(0);
		 Expression val2 = toCompute.tail.at(1);
		 //check if either is a symbol that needs to be converted. 
		 if (val1.head.type == SymbolType) {
			 getNumVal(val1);
		 }
		 if (val2.head.type == SymbolType) {
			 getNumVal(val2);
		 }
		 //preform operation
		 toReturn.head.type = BooleanType;
		 toReturn.head.value.bool_value = val1.head.value.num_value
			 < val2.head.value.num_value;
	 }
	 //>= operation 
	 else if (op == ">=") {
		 Expression val1 = toCompute.tail.at(0);
		 Expression val2 = toCompute.tail.at(1);
		 //check if either is a symbol that needs to be converted. 
		 if (val1.head.type == SymbolType) {
			 getNumVal(val1);
		 }
		 if (val2.head.type == SymbolType) {
			 getNumVal(val2);
		 }
		 //preform operation

		 toReturn.head.type = BooleanType;
		 toReturn.head.value.bool_value = val1.head.value.num_value
			 >= val2.head.value.num_value;
	 }
	 //> operation 
	 else if (op == ">") {
		 Expression val1 = toCompute.tail.at(0);
		 Expression val2 = toCompute.tail.at(1);
		 //check if either is a symbol that needs to be converted. 
		 if (val1.head.type == SymbolType) {
			 getNumVal(val1);
		 }
		 if (val2.head.type == SymbolType) {
			 getNumVal(val2);
		 }
		 //preform operation
		 toReturn.head.type = BooleanType;
		 toReturn.head.value.bool_value = val1.head.value.num_value
			 > val2.head.value.num_value;
	 }
	 //<= operation 
	 else if (op == "<=") {
		 Expression val1 = toCompute.tail.at(0);
		 Expression val2 = toCompute.tail.at(1);
		 //check if either is a symbol that needs to be converted. 
		 if (val1.head.type == SymbolType) {
			 getNumVal(val1);
		 }
		 if (val2.head.type == SymbolType) {
			 getNumVal(val2);
		 }
		 //preform operation
		 toReturn.head.type = BooleanType;
		 toReturn.head.value.bool_value = val1.head.value.num_value
			 <= val2.head.value.num_value;
	 }
	 //= operation 
	 else if (op == "=") {
		 Expression val1 = toCompute.tail.at(0);
		 Expression val2 = toCompute.tail.at(1);
		 //check if either is a symbol that needs to be converted. 
		 if (val1.head.type == SymbolType) {
			 getNumVal(val1);
		 }
		 if (val2.head.type == SymbolType) {
			 getNumVal(val2);
		 }
		 //preform operation
		 toReturn.head.type = BooleanType;
		 toReturn.head.value.bool_value = val1.head.value.num_value
			 == val2.head.value.num_value;
	 }
	 //- operation 
	 else if (op == "-") {
		 if (toCompute.tail.size() == 1) {
			 Expression val1 = toCompute.tail.at(0);
			 toReturn.head.type = NumberType;
			 toReturn.head.value.num_value = (val1.head.value.num_value) * -1;
		 }
		 else {
			 Expression val1 = toCompute.tail.at(0);
			 Expression val2 = toCompute.tail.at(1);
			 //check if either is a symbol that needs to be converted. 
			 if (val1.head.type == SymbolType) {
				 getNumVal(val1);
			 }
			 if (val2.head.type == SymbolType) {
				 getNumVal(val2);
			 }
			 //preform operation
			 toReturn.head.type = NumberType;
			 toReturn.head.value.num_value = val1.head.value.num_value
				 - val2.head.value.num_value;
		 }
	 }
	 // / operation 
	 else if (op == "/") {
		 Expression val1 = toCompute.tail.at(0);
		 Expression val2 = toCompute.tail.at(1);
		 //check if either is a symbol that needs to be converted. 
		 if (val1.head.type == SymbolType) {
			 getNumVal(val1);
		 }
		 if (val2.head.type == SymbolType) {
			 getNumVal(val2);
		 }
		 //preform operation
		 toReturn.head.type = NumberType;
		 toReturn.head.value.num_value = val1.head.value.num_value
			 / val2.head.value.num_value;
	 }
	 //pow operation 
	 else if (op == "pow") {
		 Expression val1 = toCompute.tail.at(0);
		 Expression val2 = toCompute.tail.at(1);
		 //check if either is a symbol that needs to be converted. 
		 if (val1.head.type == SymbolType) {
			 getNumVal(val1);
		 }
		 if (val2.head.type == SymbolType) {
			 getNumVal(val2);
		 }
		 //preform operation
		 toReturn.head.type = NumberType;
		 toReturn.head.value.num_value = std::pow(val1.head.value.num_value
			 , val2.head.value.num_value);
	 }
	 return toReturn; 
 }

 /**
 * helper function for making computations 
 * 
 * TODO - break into smaller pieces later for readability
 */
Expression compute(Expression &toCompute) {
	Expression toReturn; 
	Expression curr; 
	std::string op = toCompute.head.value.sym_value; 
	//is operator 
	if (!isSpecialForm(op)) {
		if (isBinaryOperator(op)) {
			Expression calculation = binaryOp(op, toCompute, curr); 

			toReturn.head.type = calculation.head.type; 
			if (calculation.head.type == NumberType) {
				toReturn.head.value.num_value = calculation.head.value.num_value;
			}
			else if (calculation.head.type == BooleanType) {
				toReturn.head.value.bool_value = calculation.head.value.bool_value;
			}
			else {
				toReturn.head.value.sym_value = calculation.head.value.sym_value;
			}
			
		}
		//not operation 
		if (op == "not") {
			//assuming a bool has been paired with it 
			curr = toCompute.tail.back(); 
			toReturn.head.type = BooleanType; 
			toReturn.head.value.bool_value = !(curr.head.value.bool_value); 
		}
		//and operation : use this setup on other operations 
		else if (op == "and") {
			Boolean result = true; 
			Boolean currVal = true; 
			for (int i = 0; i < toCompute.tail.size(); i++) {
				if (toCompute.tail.at(i).head.type == SymbolType) {
					currVal = getBoolVal(toCompute.tail.at(i)); 
				}
				else if (toCompute.tail.at(i).head.type == BooleanType) {
					currVal = toCompute.tail.at(i).head.value.bool_value; 
				}
				else {
					//throw an error? 
					currVal = false; 
				}
				if (!currVal) {
					result = false; 
					break; 
				}
			}
			toReturn.head.type = BooleanType; 
			toReturn.head.value.bool_value = result; 
		}
		//or operation 
		else if (op == "or") {
			Boolean result = false; 
			for (int i = 0; i < toCompute.tail.size(); i++) {
				if (toCompute.tail.at(i).head.value.bool_value) {
					result = true; 
					break; 
				}
			}
			toReturn.head.type = BooleanType; 
			toReturn.head.value.bool_value = result; 
		}
		
		//+ operation 
		else if (op == "+") {
			Number sum = 0; 
			for (int i = 0; i < toCompute.tail.size(); i++) {
				//get type of values, convert / find stuff (if symbol) if necessary
				curr = toCompute.tail.at(i);
				if (curr.head.type == SymbolType) {
					//if not properly saved, would need a catch mechanism. 
					sum += getNumVal(curr); 
				}
				else if (curr.head.type == NumberType) {
					sum += curr.head.value.num_value; 
				}
				else {
					//idk how this would work. probs caught by parse 
				}
			}
			//store computation 
			toReturn.head.type = NumberType; 
			toReturn.head.value.num_value = sum; 
		}
		//- operation 
		else if (op == "-") {
			if (toCompute.tail.size() == 1) {
				Expression val1 = toCompute.tail.at(0);
				toReturn.head.type = NumberType; 
				toReturn.head.value.num_value = (val1.head.value.num_value) * -1; 
			}
			else {
				Expression val1 = toCompute.tail.at(0);
				Expression val2 = toCompute.tail.at(1);
				//check if either is a symbol that needs to be converted. 
				if (val1.head.type == SymbolType) {
					getNumVal(val1);
				}
				if (val2.head.type == SymbolType) {
					getNumVal(val2);
				}
				//preform operation
				toReturn.head.type = NumberType;
				toReturn.head.value.num_value = val1.head.value.num_value
					- val2.head.value.num_value;
			}
		}
		//* operation
		else if (op == "*") {
			Number product = 1;
			for (int i = 0; i < toCompute.tail.size(); i++) {
				//get type of values, convert / find stuff (if symbol) if necessary
				curr = toCompute.tail.at(i);
				if (curr.head.type == SymbolType) {
					//if not properly saved, would need a catch mechanism. 
					product *= getNumVal(curr);
				}
				else if (curr.head.type == NumberType) {
					product *= curr.head.value.num_value;
				}
				else {
					//idk how this would work. probably caught by parse
				}
				// preform operation 
			}
			//store computation 
			toReturn.head.type = NumberType; 
			toReturn.head.value.num_value = product; 
		}
		
		//log10 operation 
		else if (op == "log10") {
			Expression val1 = toCompute.tail.at(0);
			//check if is a symbol that needs to be converted. 
			if (val1.head.type == SymbolType) {
				getNumVal(val1);
			}
			//preform operation
			toReturn.head.type = NumberType; 
			toReturn.head.value.num_value = std::log10(val1.head.value.num_value); 
		}
		//may need to add a throw here - any invalid operations should be caught by parse tho. 
		return toReturn; 
	}
	//is special op

	return computeSpecial(toCompute); 

}
	

/**
* helper function that determines if the tails of a vector 
* have all be evaluated
*/
bool atEndOfTree(std::vector<Expression> toCheck) {
	for (int i = 0; i < toCheck.size(); i++) {
		if (!toCheck.at(i).tail.empty()) {
			return false; 
		}
	}
	return true; 
}

/**
* Recursive evaluation, climb down tree as needed
* to preform stored operations 
*/
Expression evalRecursive(Expression &curr) {
	if (curr.tail.empty()) {
		if (curr.head.type == SymbolType) {
			//Number num = getNumVal(curr); 
			//Expression newBoi = new Expression(); 
			//newBoi.head.type = NumberType; 
			//newBoi.head.value.num_value = num; 
			return getVal(curr); 
		}
		return curr; 
	}
	//check if it's a special op: 
	//mem leak issue around here?
	if (isSpecialForm(curr.head.value.sym_value)) {
		return computeSpecial(curr);
	}
	//if tail tails aren't empty, recursively solve all of them
	if (!atEndOfTree(curr.tail)) {

		for (int i = 0; i < curr.tail.size(); i++) {
			curr.tail.at(i) = evalRecursive(curr.tail.at(i)); 
		}
	}
	// compute the operation 
	return compute(curr); 
}


/**
* utilizes the functions above to compute the final result 
*/
Expression Interpreter::eval(){
  // TODO: implement this function
	if (ast.head.type == NoneType && !isSpecialForm(ast.head.value.sym_value)) {
		std::cout << "Error: unknown symbol" << std::endl;
		throw InterpreterSemanticError("no such procedure");
	}

	//if it is only a small boi
	if (this->ast.tail.empty()) {
	
		return this->ast; 
	}

	
	return evalRecursive(this->ast); 
	
	//what to do about division by zero?
}

/**
* simple get method to allow user to gain access to the AST without modifying it
*/
Expression Interpreter::getAst(){
	return this->ast;
}

/**
* Lets user append addtional expressions to an AST providing the head is a begin 
* statement. 
*/
Expression Interpreter::appendToAst(Expression toAppend){
	//check if it is possible to append toAppend to the tree

	if (this->ast.head.type == NoneType 
		&& this->ast.head.value.sym_value == "begin"
		&& !(toAppend.head.value.sym_value == "begin" 
			&& toAppend.head.type == NoneType)) {

		this->ast.tail.push_back(toAppend); 
	}
	return this->ast;
}

/**
* Allows user to clear the ast if there is an error during construction 
* or parsing. simply re-initializes everything 
*/
void Interpreter::clearAst(){
	this->ast.head.type = NoneType; 
	this->ast.tail.clear(); 
}

/**
* lets us update the AST to some other value 
*/
Expression Interpreter::setAst(Expression toSet)
{
	this->ast.head = toSet.head; 
	this->ast.tail = toSet.tail; 

	return this->ast;
}
