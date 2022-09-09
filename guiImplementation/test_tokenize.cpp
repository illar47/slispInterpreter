#include "catch.hpp"

#include <string>
#include <sstream>

#include "tokenize.hpp"

TEST_CASE( "Test Tokenizer with expected input", "[tokenize]" ) {

  std::string program = "(begin (define r 10) (* pi (* r r)))";

  std::istringstream iss(program);
  
  TokenSequenceType tokens = tokenize(iss);
  
  REQUIRE( tokens.size() == 17 );
  REQUIRE( tokens[0] == "(" );
  REQUIRE( tokens[1] == "begin" );
  REQUIRE( tokens[2] == "(" );
  REQUIRE( tokens[3] == "define" );
  REQUIRE( tokens[4] == "r" );
  REQUIRE( tokens[5] == "10" );
  REQUIRE( tokens[6] == ")" );
  REQUIRE( tokens[7] == "(" );
  REQUIRE( tokens[8] == "*" );
  REQUIRE( tokens[9] == "pi" );
  REQUIRE( tokens[10] == "(" );
  REQUIRE( tokens[11] == "*" );
  REQUIRE( tokens[12] == "r" );
  REQUIRE( tokens[13] == "r" );
  REQUIRE( tokens[14] == ")" );
  REQUIRE( tokens[15] == ")" );
  REQUIRE( tokens[16] == ")" );
}

TEST_CASE( "Test Tokenizer with truncated input", "[tokenize]" ) {

  std::string program = "(f";

  std::istringstream iss(program);
  
  TokenSequenceType tokens = tokenize(iss);
  
  REQUIRE( tokens.size() == 2 );
  REQUIRE( tokens[0] == "(" );
  REQUIRE( tokens[1] == "f" );
}

TEST_CASE( "Test Tokenizer with single non-keyword", "[tokenize]" ) {

  std::string program = "hello";

  std::istringstream iss(program);

  TokenSequenceType tokens = tokenize(iss);
  
  REQUIRE( tokens.size() == 1 );
  REQUIRE( tokens[0] == "hello" );
}

TEST_CASE( "Test Tokenizer with empty input", "[tokenize]" ) {

  std::string program;

  std::istringstream iss(program);

  TokenSequenceType tokens = tokenize(iss);
  
  REQUIRE( tokens.size() == 0 );
}

TEST_CASE( "Test Tokenizer with empty expression", "[tokenize]" ) {

  std::string program = "( )";

  std::istringstream iss(program);

  TokenSequenceType tokens = tokenize(iss);
  
  REQUIRE( tokens.size() == 2 );
  REQUIRE( tokens[0] == "(" );
  REQUIRE( tokens[1] == ")" );
}

