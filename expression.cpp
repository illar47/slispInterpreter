#include "expression.hpp"

#include <cmath>
#include <limits>
#include <cctype>
#include<iostream>
// system includes
#include <sstream>

//my includes
#include "interpreter_semantic_error.hpp"

/**
* helper function to determine if a value is a float or not.
*/
bool isFloat(std::string toTry) {

	char* end = nullptr; 
	double val = strtod(toTry.c_str(), &end); 
	return end != toTry.c_str() && *end == '\0' && val != HUGE_VAL; 
}


/**
* Helper function to deterimine if something can be a valid symbol.
*/
bool isValidSymbol(std::string toCheck) {
	//check if string starts with a number (should also confirm it's not numeric)
	//check if invalid word
	//quality keeps making me edit this -_-
	return ((static_cast<int>(isdigit(toCheck[0]) == 0) !=0)
		&& toCheck != "begin"
		&& toCheck != "if"
		&& toCheck != "define");
}

Expression::Expression(bool tf){

	this->head.type = BooleanType; 
	this->head.value.bool_value = tf; 
}

Expression::Expression(double num){
	
	this->head.type = NumberType; 
	this->head.value.num_value = num; 
}

Expression::Expression(const std::string & sym){
	this->head.type = SymbolType; 
	this->head.value.sym_value = sym; 
}

bool Expression::operator==(const Expression & exp) const noexcept{
  
	//check if type is equal 
	if (this->head.type == exp.head.type) {
		//is bool type
		if (this->head.type == BooleanType) {
			return this->head.value.bool_value == exp.head.value.bool_value; 
		}
		//is num type 
		if (this->head.type == NumberType) {
			return this->head.value.num_value == exp.head.value.num_value; 
		}
		//is symb type
		if (this->head.type == SymbolType) {
			return this->head.value.sym_value == exp.head.value.sym_value; 
		}
		
		//may change later 
		return true; 
	
		//idk what to do if its NoneType

		//might also want to implement check on tail equv. (recursion?)
	}
	
  return false;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){
  // TODO: implement this function
  return out;
}

bool token_to_atom(const std::string & token, Atom & atom){
  // return true if it a token is valid. otherwise, return false. - what are invalid tokens? 

	//check if token is valid type. 

	//types: NoneType, BooleanType, NumberType, ListType, SymbolType
	// 
	//check if Bool value - is this case sensitive?
	if (token == "True" || token == "False") {
		atom.type = BooleanType; 
		if (token == "True") {
			atom.value.bool_value = true; 
		}
		else {
			atom.value.bool_value = false; 
		}
		
	}

	//check if valid number value
	else if (isFloat(token)) {

		atom.type = NumberType; 
		atom.value.num_value = std::stof(token); //should work?
	}

	//special valid number case: pi
	else if (token == "pi") {
		atom.type = NumberType; 
		atom.value.num_value = atan2(0, -1); 
	}

	//check if valid symbol value

	else if(isValidSymbol(token)) {
		atom.type = SymbolType; 
		atom.value.sym_value = token; 
	}
	//invalid input
	else {
		return false; 
	}
	//else return false

  return true;
}

std::string Expression::printExpression(Expression toPrint) {
	std::string toReturn;
	if (toPrint.head.type == NumberType) {
		std::string temp = std::to_string(toPrint.head.value.num_value); 
		int nonZeroIndex = temp.size() - 1; 
		bool nonZeroFound = false; 

		for (nonZeroIndex; nonZeroIndex > 0 && !nonZeroFound; nonZeroIndex--) {
			if (temp[nonZeroIndex] != '0' || temp[nonZeroIndex] == '.') {
				nonZeroFound = true; 
			}
		}
		
		

		toReturn = "(" + temp.substr(0, nonZeroIndex + 1) + ")";

		//std::cout << "(" << toPrint.head.value.num_value << ")"; 
	}
	else if (toPrint.head.type == SymbolType) {
		toReturn = "(" + toPrint.head.value.sym_value + ")";
	}
	else if (toPrint.head.type == BooleanType) {
		bool val = toPrint.head.value.bool_value; 
		if (val) {
			toReturn = "(True)"; 
		}
		else {
			toReturn = "(False)"; 
		}
		
	}
	else {
		//annoying to implement right now and not super necessary. will use later. 
		std::cout << "Error: attempting to print an invalid expression" << std::endl;
		throw InterpreterSemanticError("no such procedure");
	}

	return toReturn; 
	
}


