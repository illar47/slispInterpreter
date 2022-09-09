#include "catch.hpp"

#include <string>

#include "expression.hpp"

TEST_CASE( "Test Type Inference", "[types]" ) {

  Atom a;
  
  std::string token = "True";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == BooleanType);
  REQUIRE(a.value.bool_value == true);

  token = "False";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == BooleanType);
  REQUIRE(a.value.bool_value == false);

  token = "1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 1);
  
  token = "-1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == -1);

  token = "var";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == "var");

  token = "1abc";
  REQUIRE(!token_to_atom(token, a));

  token = "var1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == token);

}

TEST_CASE( "Test Expression Constructors", "[types]" ) {

  Expression exp1;

  REQUIRE(exp1 == Expression());
}
