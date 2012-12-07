/* NiallsCPP11Utilities
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "NiallsCPP11Utilities.hpp"
#include <stdio.h>
#include <fstream>

extern "C" char* __cdecl __unDName(char* buffer, const char* mangled, int buflen,
                      void *(*memget)(size_t), void (*memfree)(void *),
                      unsigned short int flags);


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
  CHECK( is_nullptr(nullptr) == true );
  CHECK( is_nullptr((void *) 0) == true );
  CHECK( is_nullptr(NULL) == true );
  CHECK( is_nullptr(0) == true );
  CHECK( is_nullptr((void *) 42) == false );
  CHECK( is_nullptr(42) == false );
  
  CHECK( is_nullptr(main) == false );
  auto lambda = [] { return true; };
  CHECK( is_nullptr(lambda) == false );
  auto foo=false ? _foo : 0;
  CHECK( !foo );
  CHECK( is_nullptr(foo) == true );
}

TEST_CASE("Undoer/undoes", "Tests that Undoer undoes")
{
	bool undone=false;
	{
		auto undo=Undoer([&undone] { undone=true; });
		CHECK(undone==false);
	}
	CHECK(undone==true);
	undone=false;
	{
		auto undo=Undoer([&undone] { undone=true; });
		undo.dismiss();
		CHECK(undone==false);
	}
	CHECK(undone==false);
	undone=false;
	{
		auto undo=Undoer(false ? _foo : 0);
	}
	CHECK(undone==false);
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
	CHECK(l[0]==5);
	CHECK(l[1]==6);
	CHECK(l[2]==7);
	l.clear();
	UnregisterData<MakeablesRegistry>(5);
	l.resize(MakeablesRegistry().size());
	copy(MakeablesRegistry().cbegin(), MakeablesRegistry().cend(), l.begin());
	CHECK(l.size()==2);
	CHECK(l[0]==6);
	CHECK(l[1]==7);
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
	CHECK(test1.prettyText() == "int");
	auto test2=SymbolType(SymbolTypeQualifier::ConstVolatileLValueRef, SymbolTypeType::Struct, "time_t");
	cout << test2.prettyText() << endl;
	CHECK(test2.prettyText() == "const volatile struct time_t &");
	auto test3=SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Class, "Foo");
	cout << test3.prettyText() << endl;
	CHECK(test3.prettyText() == "class Foo");

	auto test6=SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Constant, "78");
	cout << test6.prettyText() << endl;
	CHECK(test6.prettyText() == "78");

	auto test5=SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Struct, "fun");
	test5.dependents.push_back(&test3);
	test5.templ_params.push_back(&test1);
	test5.templ_params.push_back(&test6);
	cout << test5.prettyText() << endl;
	CHECK(test5.prettyText() == "struct Foo::fun<int, 78>");

	auto test4=SymbolType(SymbolTypeQualifier::Array, SymbolTypeType::MemberFunctionProtected, "boo");
	test4.returns=&test2;
	test4.dependents.push_back(&test5);
	test4.func_params.push_back(&test1);
	test4.func_params.push_back(&test1);
	cout << test4.prettyText() << endl;
	CHECK(test4.prettyText() == "protected: const volatile struct time_t & (Foo::fun<int, 78>::*boo [])(int, int)");
}

TEST_CASE("Demangle/msvc", "Tests that the MSVC C++ symbol demangler works")
{
	struct test_symbol { const char *const mangled, *demangled; };
	static test_symbol test_symbols[]={
		{"?alpha@@3HA", "int alpha"},
		{"?myStaticMember@myclass@@2HA", 	"public: static int myclass::myStaticMember"},
		{"?myconstStaticMember@myclass@@2HB", 	"public: static const int myclass::myconstStaticMember"},
		{"?myvolatileStaticMember@myclass@@2HC", 	"public: static volatile int myclass::myvolatileStaticMember"},
		{"?myfnptr@@3P6AHH@ZA", 	"int myfnptr(int)"},
		{"?myglobal@@3HA", 	"int myglobal"},
		{"?myvolatile@@3HC", 	"volatile int myvolatile"},
		{"?myarray@@3PAHA", 	"int * myarray"},
		{"?Fv_PPv@@YAPAPAXXZ", 	"void ** Fv_PPv(void)"},
		{"?Fv_Pv@@YAPAXXZ", 	"void * Fv_Pv(void)"},
		{"?FA10_i_i@@YAHQAH@Z", 	"int FA10_i_i(int * const"},
		{"?FPi_i@@YAHPAH@Z", 	"int FPi_i(int *)"},
		{"?Fc_i@@YAHD@Z", 	"int Fc_i(char)"},
		{"?Ff_i@@YAHM@Z", 	"int Ff_i(float)"},
		{"?Fg_i@@YAHN@Z", 	"int Fg_i(double)"},
		{"?Fi_i@@YAHH@Z", 	"int Fi_i(int)"},
		{"?Fie_i@@YAHHZZ", 	"int Fie_i(int, ...)"},
		{"?Fii_i@@YAHHH@Z", 	"int Fii_i(int, int)"},
		{"?Fiii_i@@YAHHHH@Z", 	"int Fiii_i(int, int, int)"},
		{"?Fmxmx_v@@YAXVmyclass@@P6AHH@Z01@Z", 	"void Fmxmx_v(class myclass, int (*)(int), class myclass, int (*)(int))"},
		{"?Fmyclass_v@@YAXVmyclass@@@Z", 	"void Fmyclass_v(class myclass)"},
		{"?Fv_Ci@@YA?BHXZ", 	"const int Fv_Ci(void)"},
		{"?Fv_Lg@@YAOXZ", 	"long double Fv_Lg(void)"},
		{"?Fv_Ri@@YAAAHXZ", 	"int& Fv_Ri(void)"}, // Can't handle storage class not specified when combined with lvalueref 'AA' (need a special case retry)
		{"?Fv_Sc@@YACXZ", 	"signed char Fv_Sc(void)"},
		{"?Fv_Uc@@YAEXZ", 	"unsigned char Fv_Uc(void)"},
		{"?Fv_Ui@@YAIXZ", 	"unsigned int Fv_Ui(void)"},
		{"?Fv_Ul@@YAKXZ", 	"unsigned long int Fv_Ul(void)"},
		{"?Fv_Us@@YAGXZ", 	"unsigned short int Fv_Us(void)"},
		{"?Fv_Vi@@YA?CHXZ", 	"volatile int Fv_Vi(void)"},
		{"?Fv_c@@YADXZ", 	"char Fv_c(void)"},
		{"?Fv_f@@YAMXZ", 	"float Fv_f(void)"},
		{"?Fv_g@@YANXZ", 	"double Fv_g(void)"},
		{"?Fv_i@@YAHXZ", 	"int Fv_i(void)"},
		{"?Fv_l@@YAJXZ", 	"long int Fv_l(void)"},
		{"?Fv_s@@YAFXZ", 	"short int Fv_s(void)"},
		{"?Fv_v@@YAXXZ", 	"void Fv_v(void)"},
		//{"?Fv_v_cdecl@@YAXXZ", 	"void __cdecl Fv_v_cdecl(void)"},
		//{"?Fv_v_fastcall@@YIXXZ", 	"void __fastcall Fv_v_fastcall(void)"},
		//{"?Fv_v_stdcall@@YGXXZ", 	"void __stdcall Fv_v_stdcall(void)"},
		{"?Fx_i@@YAHP6AHH@Z@Z", 	"int Fx_i(int (*)(int))"},
		{"?Fxix_i@@YAHP6AHH@ZH0@Z", 	"int Fxix_i(int (*)(int), int, int (*)(int))"},
		{"?Fxx_i@@YAHP6AHH@Z0@Z", 	"int Fxx_i(int (*)(int), int (*)(int))"},
		{"?Fxxi_i@@YAHP6AHH@Z00H@Z", 	"int Fxxi_i(int (*)(int), int (*)(int), int (*)(int), int)"},
		{"?Fxxx_i@@YAHP6AHH@Z00@Z", 	"int Fxxx_i(int (*)(int), int (*)(int), int (*)(int))"},
		{"?Fxyxy_i@@YAHP6AHH@ZP6AHF@Z01@Z", 	"int Fxyxy_i(int (*)(int), int (*)(short int), int (*)(int), int (*)(short int))"},
		{"??3myclass@@SAXPAX@Z", 	"public: static void myclass::operator delete(void *)"},
		{"?Fi_i@myclass@@QAEHH@Z", 	"public: int myclass::Fi_i(int)"},
		{"?Fis_i@myclass@@SAHH@Z", 	"public: static int myclass::Fis_i(int)"},
		//{"?Fv_v_cdecl@myclass@@QAAXXZ", 	"void __cdecl myclass::Fv_v_cdecl(void)"},
		//{"?Fv_v_fastcall@myclass@@QAIXXZ", 	"void __fastcall myclass::Fv_v_fastcall(void)"},
		//{"?Fv_v_stdcall@myclass@@QAGXXZ", 	"void __stdcall myclass::Fv_v_stdcall(void)"},
		{"??0myclass@@QAE@H@Z", 	"public: myclass::myclass(int)"},
		{"??0myclass@@QAE@XZ", 	"public: myclass::myclass(void)"},
		{"?Fi_i@nested@myclass@@QAEHH@Z", 	"public: int myclass::nested::Fi_i(int)"},
		{"??0nested@myclass@@QAE@XZ", 	"public: myclass::nested::nested(void)"},
		{"??1nested@myclass@@QAE@XZ", 	"public: myclass::nested::~nested(void)"},
		{"??Hmyclass@@QAE?AV0@H@Z", 	"public: class myclass myclass::operator+(int)"},
		{"??Emyclass@@QAE?AV0@XZ", 	"public: class myclass myclass::operator++(void)"},
		{"??Emyclass@@QAE?AV0@H@Z", 	"public: class myclass myclass::operator++(int)"},
		{"??4myclass@@QAEAAV0@ABV0@@Z", 	"public: class myclass & myclass::operator=(class const myclass &)"},
		{"??1myclass@@QAE@XZ", 	"public: myclass::~myclass()"},
		{"?Fi_i@nested@@QAEHH@Z", 	"public: int nested::Fi_i(int)"},
		{"??0nested@@QAE@XZ", 	"public: nested::nested(void)"},
		{"??1nested@@QAE@XZ", 	"public: nested::~nested(void)"},
		{"??2myclass@@SAPAXI@Z", 	"public: static void * myclass::operator new(unsigned int)"},

		{"??4BatteryChargingState@device@bb@@QAEAAV012@ABV012@@Z", "public: class bb::device::BatteryChargingState & bb::device::BatteryChargingState::operator=(class bb::device::BatteryChargingState const &)"},
		{"?__unique_247@?A0xb1a90387@__component_export__@device@bb@@3QBDB", "char const * const bb::device::__component_export__::A0xb1a90387::__unique_247"},
		{"?staticMetaObject@BatteryInfo@device@bb@@2UQMetaObject@@B", "public: static struct QMetaObject const bb::device::BatteryInfo::staticMetaObject"},
		{"??4?$exported_component@VBatteryInfo@device@bb@@$1?__unique_247@?A0xb1a90387@__component_export__@23@3QBDB$0PH@@__component_export_machinery__@@QAEAAU01@ABU01@@Z", "public: struct __component_export_machinery__::exported_component<class bb::device::BatteryInfo,&char const * const bb::device::__component_export__::A0xb1a90387::__unique_247,247> & __component_export_machinery__::exported_component<class bb::device::BatteryInfo,&char const * const bb::device::__component_export__::A0xb1a90387::__unique_247,247>::operator=(struct __component_export_machinery__::exported_component<class bb::device::BatteryInfo,&char const * const bb::device::__component_export__::A0xb1a90387::__unique_247,247> const &)"},

		{ NULL, NULL }
	};
#if 0
	auto dump=ofstream("demangled.txt");
	for(test_symbol *i=test_symbols; i->mangled; i++)
	{
		//i->demangled=msvc_demangle(NULL, i->mangled, 0);
		i->demangled=__unDName(NULL, i->mangled, 0, malloc, free, 0);
		dump << "{\"" << i->mangled << "\", \"" << i->demangled << "\"}," << endl;
	}
#else
	for(test_symbol *i=test_symbols; i->mangled; i++)
	{
		const auto &demangled=Demangle(i->mangled, nothrow);
		CHECK(demangled.first==std::string(i->demangled));
	}
#endif
}
