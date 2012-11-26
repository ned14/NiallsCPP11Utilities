/* NiallsCPP11Utilities
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "NiallsCPP11Utilities.hpp"

using namespace NiallsCPP11Utilities;
using namespace std;

TEST_CASE("is_nullptr/works", "Tests that is_nullptr() works")
{
  REQUIRE( is_nullptr(nullptr) == true );
  REQUIRE( is_nullptr((void *) 0) == true );
  REQUIRE( is_nullptr(NULL) == true );
  REQUIRE( is_nullptr(0) == true );
  REQUIRE( is_nullptr((void *) 42) == false );
  REQUIRE( is_nullptr(42) == false );
  
  REQUIRE( is_nullptr(main) == false );
  auto lambda = [] { return true; };
  REQUIRE( is_nullptr(lambda) == false );
  void _foo();
  auto foo=false ? _foo : 0;
  REQUIRE( !foo );
  REQUIRE( is_nullptr(foo) == true );
}

TEST_CASE("Undoer/undoes", "Tests that Undoer undoes")
{
	bool undone=false;
	{
		auto undo=Undoer([&undone] { undone=true; });
		REQUIRE(undone==false);
	}
	REQUIRE(undone==true);
	undone=false;
	{
		auto undo=Undoer([&undone] { undone=true; });
		undo.dismiss();
		REQUIRE(undone==false);
	}
	REQUIRE(undone==false);
	undone=false;
	{
		void foo();
		auto undo=Undoer(false ? foo : 0);
	}
	REQUIRE(undone==false);
}
