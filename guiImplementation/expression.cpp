#include "expression.hpp"


// system includes
#include <sstream>
#include <cmath>
#include <limits>
#include <cctype>
#include <tuple>

//my includes
#include "interpreter_semantic_error.hpp"
#include <iostream>
/**
* 
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
	return ((static_cast<int>(isdigit(toCheck[0]) == 0) != 0)
		&& toCheck != "begin"
		&& toCheck != "if"
		&& toCheck != "define"
		&& toCheck != "draw"
		&& toCheck != "point"
		&& toCheck != "line"
		&& toCheck != "arc");
}

//expression for a boolean
Expression::Expression(bool tf) {
	this->head.type = BooleanType;
	this->head.value.bool_value = tf;
}

//expression for a number
Expression::Expression(double num) {
	this->head.type = NumberType;
	this->head.value.num_value = num;
}

//expression for a symbol
Expression::Expression(const std::string& sym) {
	this->head.type = SymbolType;
	this->head.value.sym_value = sym;
}

//create expression for a point
Expression::Expression(std::tuple<double, double> value) {
	this->head.type = PointType; 
	Point newPoint; 
	newPoint.x = std::get<0>(value); 
	newPoint.y = std::get<1>(value); 
	this->head.value.point_value = newPoint; 
}

//create expression for a line
Expression::Expression(std::tuple<double, double> start,
	std::tuple<double, double> end) {
	this->head.type = LineType; 

	Line newLine; 
	Point firstPt; 
	Point secondPt; 

	firstPt.x = std::get<0>(start); 
	firstPt.y = std::get<1>(start); 

	secondPt.x = std::get<0>(end);
	secondPt.y = std::get<1>(end); 

	newLine.first = firstPt; 
	newLine.second = secondPt; 

	this->head.value.line_value = newLine; 
}

//expression for an arc
Expression::Expression(std::tuple<double, double> center,
	std::tuple<double, double> start,
	double angle) {
	this->head.type = ArcType; 

	Arc newArc; 

	Point firstPt;
	Point centerPt;

	firstPt.x = std::get<0>(start);
	firstPt.y = std::get<1>(start);

	centerPt.x = std::get<0>(center);
	centerPt.y = std::get<1>(center);

	newArc.center = centerPt; 
	newArc.span = angle; 
	newArc.start = firstPt; 

	this->head.value.arc_value = newArc; 
}

bool Expression::operator==(const Expression& exp) const noexcept {
	//first check if all tail contents are equivalent (may need to fix)
	if (!this->tail.empty()) {
		if (this->tail.size() != exp.tail.size()) {
			return false; 
		}
		for (int i = 0; i < this->tail.size(); i++) {
			if (!(this->tail.at(i) == exp.tail.at(i))) {
				return false; 
			}
		}
	}
	//check if head is equivalent 
	
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

		//copy whats in header and put here

		if (this->head.type == PointType) {
			Point thisPoint = this->head.value.point_value;
			Point comparePoint = exp.head.value.point_value; 

			return (fabs(thisPoint.x - comparePoint.x) < std::numeric_limits<double>::epsilon()) &&
				(fabs(thisPoint.y - comparePoint.y) < std::numeric_limits<double>::epsilon());
		}

		if (this->head.type == LineType) {
			Point thisPoint1 = this->head.value.line_value.first;
			Point thisPoint2 = this->head.value.line_value.second; 

			Point expPoint1 = exp.head.value.line_value.first; 
			Point expPoint2 = exp.head.value.line_value.second; 

			return thisPoint1.x == expPoint1.x
				&& thisPoint1.y == expPoint1.y
				&& thisPoint2.x == expPoint2.x
				&& thisPoint2.y == expPoint2.y; 
		}
		
		if (this->head.type == ArcType) {
			Point thisPoint1 = this->head.value.arc_value.center;
			Point thisPoint2 = this->head.value.arc_value.start;

			Point expPoint1 = exp.head.value.arc_value.center;
			Point expPoint2 = exp.head.value.arc_value.start;

			Number thisSpan = this->head.value.arc_value.span; 
			Number expSpan = exp.head.value.arc_value.span; 

			//use overloaded == for point type - for points as well use the epsilon thing
			return thisPoint1 == expPoint1
				&& thisPoint2 == expPoint2
				&& fabs(thisSpan - expSpan) < std::numeric_limits<double>::epsilon(); //hopefully good??? 
		}

		//may change later 
		return true;

		//idk what to do if its NoneType

	}

	return false;
}

std::ostream& operator<<(std::ostream& out, const Expression& exp) {
	// TODO: implement this function
}

bool token_to_atom(const std::string& token, Atom& atom) {
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

	else if (isValidSymbol(token)) {
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
	//TODO: add point, arc, and Line (might need to restructure for gui tho.
	else {
		//annoying to implement right now and not super necessary. will use later. 
		std::cout << "Error: attempting to print an invalid expression" << std::endl;
		throw InterpreterSemanticError("no such procedure");
	}

	return toReturn;

}
