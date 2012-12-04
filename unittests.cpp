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

TEST_CASE("SymbolType/works", "Tests that SymbolType works")
{
	auto test1=SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Int);
	cout << test1.prettyText() << endl;
	REQUIRE(test1.prettyText() == "int");
	auto test2=SymbolType(SymbolTypeQualifier::ConstVolatileLValueRef, SymbolTypeType::Struct, "time_t");
	cout << test2.prettyText() << endl;
	REQUIRE(test2.prettyText() == "const volatile struct time_t &");
	auto test3=SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Class, "Foo");
	cout << test3.prettyText() << endl;
	REQUIRE(test3.prettyText() == "class Foo");

	auto test6=SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Constant, "78");
	cout << test6.prettyText() << endl;
	REQUIRE(test6.prettyText() == "78");

	auto test5=SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Struct, "fun");
	test5.dependents.push_back(&test3);
	test5.templ_params.push_back(&test1);
	test5.templ_params.push_back(&test6);
	cout << test5.prettyText() << endl;
	REQUIRE(test5.prettyText() == "struct Foo::fun<int, 78>");

	auto test4=SymbolType(SymbolTypeQualifier::Array, SymbolTypeType::MemberFunctionProtected, "boo");
	test4.returns=&test2;
	test4.dependents.push_back(&test5);
	test4.func_params.push_back(&test1);
	test4.func_params.push_back(&test1);
	cout << test4.prettyText() << endl;
	REQUIRE(test4.prettyText() == "protected const volatile struct time_t & (Foo::fun<int, 78>::*boo [])(int, int)");
}

TEST_CASE("Demangle/msvc", "Tests that the MSVC C++ symbol demangler works")
{
	struct test_symbol { const char *mangled, *demangled; };
	static const test_symbol test_symbols[]={
		{"?alpha@@3HA", "int alpha"},
		{"?myStaticMember@myclass@@2HA", 	"static int myclass::myStaticMember"},
		{"?myconstStaticMember@myclass@@2HB", 	"const int myclass::myconstStaticMember"},
		{"?myvolatileStaticMember@myclass@@2HC", 	"volatile int myclass::myvolatileStaticMember"},
		{"?myfnptr@@3P6AHH@ZA", 	"x myfnptr;"},
		{"?myglobal@@3HA", 	"int myglobal;"},
		{"?myvolatile@@3HC", 	"volatile int myvolatile;"},
		{"?myarray@@3PAHA", 	"int myarray[10];"},
		{"?Fv_PPv@@YAPAPAXXZ", 	"void **Fv_PPv(void)"},
		{"?Fv_Pv@@YAPAXXZ", 	"void *Fv_Pv(void)"},
		{"?FA10_i_i@@YAHQAH@Z", 	"int FA10_i_i(int a[10])"},
		{"?FPi_i@@YAHPAH@Z", 	"int FPi_i(int *a)"},
		{"?Fc_i@@YAHD@Z", 	"int Fc_i(char bar)"},
		{"?Ff_i@@YAHM@Z", 	"int Ff_i(float bar)"},
		{"?Fg_i@@YAHN@Z", 	"int Fg_i(double bar)"},
		{"?Fi_i@@YAHH@Z", 	"int Fi_i(int bar)"},
		{"?Fie_i@@YAHHZZ", 	"int Fie_i(int bar, ...)"},
		{"?Fii_i@@YAHHH@Z", 	"int Fii_i(int bar, int goo)"},
		{"?Fiii_i@@YAHHHH@Z", 	"int Fiii_i(int bar, int goo, int hoo)"},
		{"?Fmxmx_v@@YAXVmyclass@@P6AHH@Z01@Z", 	"void Fmxmx_v(myclass arg1, x arg2, myclass arg3, x arg4)"},
		{"?Fmyclass_v@@YAXVmyclass@@@Z", 	"void Fmyclass_v(myclass m)"},
		{"?Fv_Ci@@YA?BHXZ", 	"const int Fv_Ci(void)"},
		{"?Fv_Lg@@YAOXZ", 	"long double Fv_Lg(void)"},
		{"?Fv_Ri@@YAAAHXZ", 	"int& Fv_Ri(void)"},
		{"?Fv_Sc@@YACXZ", 	"signed char Fv_Sc(void)"},
		{"?Fv_Uc@@YAEXZ", 	"unsigned char Fv_Uc(void)"},
		{"?Fv_Ui@@YAIXZ", 	"unsigned int Fv_Ui(void)"},
		{"?Fv_Ul@@YAKXZ", 	"unsigned long Fv_Ul(void)"},
		{"?Fv_Us@@YAGXZ", 	"unsigned short Fv_Us(void)"},
		{"?Fv_Vi@@YA?CHXZ", 	"volatile int Fv_Vi(void)"},
		{"?Fv_c@@YADXZ", 	"char Fv_c(void)"},
		{"?Fv_f@@YAMXZ", 	"float Fv_f(void)"},
		{"?Fv_g@@YANXZ", 	"double Fv_g(void)"},
		{"?Fv_i@@YAHXZ", 	"int Fv_i(void)"},
		{"?Fv_l@@YAJXZ", 	"long Fv_l(void)"},
		{"?Fv_s@@YAFXZ", 	"short Fv_s(void)"},
		{"?Fv_v@@YAXXZ", 	"void Fv_v(void)"},
		{"?Fv_v_cdecl@@YAXXZ", 	"void __cdecl Fv_v_cdecl(void)"},
		{"?Fv_v_fastcall@@YIXXZ", 	"void __fastcall Fv_v_fastcall(void)"},
		{"?Fv_v_stdcall@@YGXXZ", 	"void __stdcall Fv_v_stdcall(void)"},
		{"?Fx_i@@YAHP6AHH@Z@Z", 	"int Fx_i(x fnptr)"},
		{"?Fxix_i@@YAHP6AHH@ZH0@Z", 	"int Fxix_i(x fnptr, int i, x fnptr3)"},
		{"?Fxx_i@@YAHP6AHH@Z0@Z", 	"int Fxx_i(x fnptr, x fnptr2)"},
		{"?Fxxi_i@@YAHP6AHH@Z00H@Z", 	"int Fxxi_i(x fnptr, x fnptr2, x fnptr3, int i)"},
		{"?Fxxx_i@@YAHP6AHH@Z00@Z", 	"int Fxxx_i(x fnptr, x fnptr2, x fnptr3)"},
		{"?Fxyxy_i@@YAHP6AHH@ZP6AHF@Z01@Z", 	"int Fxyxy_i(x fnptr, y fnptr2, x fnptr3, y fnptr4)"},
		{"??3myclass@@SAXPAX@Z", 	"void myclass::operator delete(void *p)"},
		{"?Fi_i@myclass@@QAEHH@Z", 	"int myclass::Fi_i(int bar)"},
		{"?Fis_i@myclass@@SAHH@Z", 	"static int myclass::Fis_i(int bar)"},
		{"?Fv_v_cdecl@myclass@@QAAXXZ", 	"void __cdecl myclass::Fv_v_cdecl(void)"},
		{"?Fv_v_fastcall@myclass@@QAIXXZ", 	"void __fastcall myclass::Fv_v_fastcall(void)"},
		{"?Fv_v_stdcall@myclass@@QAGXXZ", 	"void __stdcall myclass::Fv_v_stdcall(void)"},
		{"??0myclass@@QAE@H@Z", 	"myclass::myclass(int x)"},
		{"??0myclass@@QAE@XZ", 	"myclass::myclass(void)"},
		{"?Fi_i@nested@myclass@@QAEHH@Z", 	"int myclass::nested::Fi_i(int bar)"},
		{"??0nested@myclass@@QAE@XZ", 	"myclass::nested::nested(void)"},
		{"??1nested@myclass@@QAE@XZ", 	"myclass::nested::~nested()"},
		{"??Hmyclass@@QAE?AV0@H@Z", 	"myclass myclass::operator+(int x)"},
		{"??Emyclass@@QAE?AV0@XZ", 	"myclass myclass::operator++()"},
		{"??Emyclass@@QAE?AV0@H@Z", 	"myclass myclass::operator++(int)"},
		{"??4myclass@@QAEAAV0@ABV0@@Z", 	"myclass& myclass::operator=(const myclass& from)"},
		{"??1myclass@@QAE@XZ", 	"myclass::~myclass()"},
		{"?Fi_i@nested@@QAEHH@Z", 	"int nested::Fi_i(int bar)"},
		{"??0nested@@QAE@XZ", 	"nested::nested(void)"},
		{"??1nested@@QAE@XZ", 	"nested::~nested()"},
		{"??2myclass@@SAPAXI@Z", 	"void* myclass::operator new(size_t size)"},
		{ NULL, NULL }
	};
	for(const test_symbol *i=test_symbols; i->mangled; i++)
	{
		cout << "'" << i->mangled << "' ..." << endl;
		const auto &demangled=Demangle(i->mangled);
		cout << "'" << i->mangled << "' = " << demangled << endl;
	}
}
