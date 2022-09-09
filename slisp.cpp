#include <cstdlib>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string.h>

#include "interpreter_semantic_error.hpp"
#include "interpreter.hpp"
#include "expression.hpp"
#include "test_config.hpp.in"  //added .in to remove error... idk if that fixes it


/**
* helper function that starts up a new interpreter object 
* and attempts to compute the given function 
* @param toParse - the provided formula to parse 
*/


bool run(Interpreter &savedExpressions, std::string toParse) {
	//initalizations 
	Interpreter interp;
	Expression result;
	std::string toPrint;

	std::istringstream input(toParse);
	bool ok = interp.parse(input); //parse the input 
	//append the interp ast to the savedExpressions Tree 
	savedExpressions.appendToAst(interp.getAst()); 

	
	//add check if valid 
	if (ok) {
		try {
			result = savedExpressions.eval();
		}
		catch (InterpreterSemanticError) {
			//reinitalize savedExpressions
			savedExpressions.clearAst();
			return false;

		}
		
		toPrint = result.printExpression(result);
		std::cout << toPrint << std::endl;
	}
	else {
		//parse error output
		std::cout << "Error: unable to parse" << std::endl; 
		//reinitalize savedExpressions
		savedExpressions.clearAst();
		return false; 
		
	}

	return true; 
	
}

/**
* Does all of the input parsing and determines what type of run the user is implementing
*/
int main(int argc, char **argv) {
	Interpreter savedExpressions;
	Expression head; 
	head.head.type = NoneType; 
	head.head.value.sym_value = "begin"; 
	savedExpressions.setAst(head); 

	bool isValid = true; 
 
	//interactive mode has no input 
	if (argc == 1) {
		char currInput[256];
		bool terminateProgram = false;

		while (!terminateProgram) {
			std::cout << "slisp> ";
			
			std::cin.getline(currInput, 256); 

			//User has typed end of file character: ctrl-k windows and ctrl-d on unix
			if (std::cin.eof()) {
				terminateProgram = true; 
			}
			else {
				//find a way to save the tree unless it's invalid 
				isValid = run(savedExpressions, currInput);
			}
			
		}
		

	}
	
	//check for valid smp or text file
	else if (argc == 2) {
		std::ifstream ifs(argv[1]);
		//initalizations 
		Interpreter interp;
		Expression result;
		std::string toPrint;

		//is valid filename 
		if (ifs) {
			bool ok = interp.parse(ifs); //parse the input 
			try {
				result = interp.eval();
			}
			catch (InterpreterSemanticError) {
				//output handled in eval() method. 
			}

			//add check if valid 
			if (ok) {

				toPrint = result.printExpression(result);
				std::cout << toPrint << std::endl;
			}
		}
		else {
			std::cout << "Error: invalid filename" << std::endl; 
			return EXIT_FAILURE; 
		}
	}

	//check for -e flag 
	else if (strcmp(argv[1], "-e") == 0) {
		//might need to check for quotation marks 
		std::string toCompute;  

		for (int i = 2; i < argc - 1; i++) {
			toCompute.append(argv[i] + ' ');
		}

		toCompute.append(argv[argc - 1]); 
		if (toCompute[0] == '"') {
			toCompute = toCompute.substr(1, toCompute.size() - 1); 
		}

		isValid = run(savedExpressions, toCompute);
		
	}
	else {
		std::cout << "Error: invalid run parameters" << std::endl; 
		return EXIT_FAILURE; 
	}

	
	
	
	if (!isValid) {
		return EXIT_FAILURE; 
	}

	return EXIT_SUCCESS;
}
