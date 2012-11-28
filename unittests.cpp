/* NiallsCPP11Utilities
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "NiallsCPP11Utilities.hpp"
#include <stdio.h>

using namespace NiallsCPP11Utilities;
using namespace std;

static void _foo() { }

int main (int argc, char * const argv[]) {
    int ret=Catch::Main( argc, argv );
	printf("Press Return to exit ...\n");
	getchar();
	return ret;
}

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
		auto undo=Undoer(false ? _foo : 0);
	}
	REQUIRE(undone==false);
}

TEST_CASE("StaticTypeRegistry/works", "Tests that StaticTypeRegistry works")
{
	struct Foo;
	typedef StaticTypeRegistry<Foo, int> MakeablesRegistry;
	RegisterData<MakeablesRegistry>(5);
	RegisterData<MakeablesRegistry>(6);
	RegisterData<MakeablesRegistry>(7);
	std::vector<int> l;
	l.resize(MakeablesRegistry().size());
	copy(MakeablesRegistry().cbegin(), MakeablesRegistry().cend(), l.begin());
	REQUIRE(l[0]==5);
	REQUIRE(l[1]==6);
	REQUIRE(l[2]==7);
	l.clear();
	UnregisterData<MakeablesRegistry>(5);
	l.resize(MakeablesRegistry().size());
	copy(MakeablesRegistry().cbegin(), MakeablesRegistry().cend(), l.begin());
	REQUIRE(l.size()==2);
	REQUIRE(l[0]==6);
	REQUIRE(l[1]==7);
	l.clear();
	cout << TextDump(MakeablesRegistry()) << endl;
}

TEST_CASE("MappedFileInfo/works", "Tests that MappedFileInfo works")
{
	auto mfs=MappedFileInfo::mappedFiles();
	cout << "Mapped files in this process:" << endl;
	cout << TextDump(mfs) << endl;
	cout << endl << "Of these, I (" << hex << (void *) main << ") live in:" << endl;
	cout << TextDump(FromCodePoint(mfs, main)->second);
}
