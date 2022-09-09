#include "tokenize.hpp"
#include <cctype>

#include <iostream>

TokenSequenceType tokenize(std::istream & seq){
  TokenSequenceType tokens;
  std::string currToken; 

 //for contents of seq (i.e. input)
  while (seq >> currToken) {
	  //check if comment (i.e. has a ;) - add to tokens up until ; then skip to next line
	  int findSemi = currToken.find(';'); 
	  if ( findSemi != -1) {
		  if (findSemi > 0) {
			  tokens.push_back(currToken.substr(0, findSemi)); 
		  }
		  char temp[256]; 
		  seq.getline(temp, 256);  //skip to next line of input

		  
	  }
	  //check if character is OPEN token and save accordingly
	  else if (currToken[0] == OPEN) {
		  //check for and add all OPEN tokens
		  int index = 0;
		  while (index < currToken.size() && currToken[index] == OPEN) {
			  tokens.push_back("("); 
			  index++; 
		  }
		  //if space-delimited string exits after OPEN token add it 
		  if (index < currToken.size()) {
			  int start = index; 
			  //additional check for extra CLOSED Characters 
			  while (index < currToken.size() && currToken[index] != CLOSE)
			  {
				  index++; 
			  }
			  tokens.push_back(currToken.substr(start, index - 1));
			  while (index < currToken.size()) {
				  tokens.push_back(")"); 
				  index++;
			  }
		  }
		  
	  }
	  //check if character is CLOSE token and save accordingly
	  else if (currToken[currToken.size() - 1] == CLOSE) {
		  int index = currToken.size() - 1; 
		  int closeCount = 0; 
		  //find index of delimited string token (if it exits) and number of CLOSE tokens
		  while (index >= 0 && currToken[index] == CLOSE) {
			  closeCount++; 
			  index--; 
		  }
		  //if it exists, add delimted string token 
		  if (index >= 0) {
			  int openCheck = 0;
			  //additional check for extra OPEN Characters
			  while (currToken[openCheck] == OPEN) {
				  openCheck++; 
			  }
			  for (int i = 0; i < openCheck; i++) {
				  tokens.push_back("("); 
			  }

			  tokens.push_back(currToken.substr(openCheck, index + 1));
			  
		  } 

		  //for the closeCount add CLOSE token
		  for (int i = 0; i < closeCount; i++) {
			  tokens.push_back(")"); 
		  }
		  
	  }
	  //Base case save of token
	  else {
		  tokens.push_back(currToken); 
	  }

  }
  //return finalized sequence of tokens 
  return tokens;
}
