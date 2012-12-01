/* SymbolMangler
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_MAP_SIZE 50

#include "NiallsCPP11Utilities.hpp"
#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/string.hpp>
#include <boost/mpl/for_each.hpp>

#ifdef _MSC_VER
#pragma warning(push,0)
#endif
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct_named.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

BOOST_FUSION_ADAPT_STRUCT(NiallsCPP11Utilities::SymbolType,
	(NiallsCPP11Utilities::SymbolTypeStorage, storage)
	(const NiallsCPP11Utilities::SymbolType *, returns)
	(NiallsCPP11Utilities::SymbolTypeQualifier, qualifier)
	(NiallsCPP11Utilities::SymbolTypeType, type)
	(std::list<const NiallsCPP11Utilities::SymbolType *>, dependents)
	(std::string, name)
	(std::list<const NiallsCPP11Utilities::SymbolType *>, templ_params)
	(std::list<const NiallsCPP11Utilities::SymbolType *>, func_params)
)

namespace NiallsCPP11Utilities {

using namespace std;
using namespace boost;

template<SymbolTypeType type> struct SymbolTypeType_ { static const SymbolTypeType value=type; };
template<SymbolTypeQualifier type> struct SymbolTypeQualifiers_ { static const SymbolTypeQualifier value=type; };

// Format is of pair<SymbolTypeType::X, pair<preamble, postamble>>
typedef mpl::map< 
mpl::pair<SymbolTypeType_<SymbolTypeType::Constant>,			mpl::pair<mpl::string<>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Void>,				mpl::pair<mpl::string<'void'>::type,								mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Bool>,				mpl::pair<mpl::string<'bool'>::type,								mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Char>,				mpl::pair<mpl::string<'char'>::type,								mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::SignedChar>,			mpl::pair<mpl::string<'sign', 'ed c', 'har'>::type,					mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::UnsignedChar>,		mpl::pair<mpl::string<'unsi', 'gned', ' cha', 'r'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::ShortInt>,			mpl::pair<mpl::string<'shor', 't in', 't'>::type,					mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::UnsignedShortInt>,	mpl::pair<mpl::string<'unsi', 'gned', ' sho', 'rt i', 'nt'>::type,	mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Int>,					mpl::pair<mpl::string<'int'>::type,									mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::UnsignedInt>,			mpl::pair<mpl::string<'unsi', 'gned', ' int'>::type,				mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::LongInt>,				mpl::pair<mpl::string<'long', ' int'>::type,						mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::UnsignedLongInt>,		mpl::pair<mpl::string<'unsi', 'gned', ' lon', 'g in', 't'>::type,	mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::LongLong>,			mpl::pair<mpl::string<'long', ' lon', 'g'>::type,					mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::UnsignedLongLong>,	mpl::pair<mpl::string<'unsi', 'gned', ' lon', 'g lo', 'ng'>::type,	mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Wchar_t>,				mpl::pair<mpl::string<'wcha', 'r_t'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Float>,				mpl::pair<mpl::string<'floa', 't'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Double>,				mpl::pair<mpl::string<'doub', 'le'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::LongDouble>,			mpl::pair<mpl::string<'long', ' dou', 'ble'>::type,					mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect64>,				mpl::pair<mpl::string<'__m6', '4'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect128f>,			mpl::pair<mpl::string<'__m1', '28'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect128d>,			mpl::pair<mpl::string<'__m1', '28f'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect128i>,			mpl::pair<mpl::string<'__m1', '28i'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect256f>,			mpl::pair<mpl::string<'__m2', '56'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect256d>,			mpl::pair<mpl::string<'__m2', '56d'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect256i>,			mpl::pair<mpl::string<'__m2', '56i'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Varargs>,				mpl::pair<mpl::string<'...'>::type,									mpl::string<>::type>::type>::type,

mpl::pair<SymbolTypeType_<SymbolTypeType::Namespace>,			mpl::pair<mpl::string<'name', 'spac', 'e'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Union>,				mpl::pair<mpl::string<'unio', 'n'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Struct>,				mpl::pair<mpl::string<'stru', 'ct'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Class>,				mpl::pair<mpl::string<'clas', 's'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Enum>,				mpl::pair<mpl::string<'enum'>::type,								mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::EnumMember>,			mpl::pair<mpl::string<'enum'>::type,								mpl::string<'::'>::type>::type>::type,

mpl::pair<SymbolTypeType_<SymbolTypeType::Function>,						mpl::pair<mpl::string<>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunction>,			mpl::pair<mpl::string<'publ', 'ic'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunctionProtected>,	mpl::pair<mpl::string<'prot', 'ecte', 'd'>::type,					mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunctionPrivate>,		mpl::pair<mpl::string<'pri', 'vate'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::MemberFunction>,					mpl::pair<mpl::string<'publ', 'ic'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::MemberFunctionProtected>,			mpl::pair<mpl::string<'prot', 'ecte', 'd'>::type,					mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::MemberFunctionPrivate>,			mpl::pair<mpl::string<'priv', 'ate'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::VirtualMemberFunction>,			mpl::pair<mpl::string<'publ', 'ic v', 'irt', 'ual'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::VirtualMemberFunctionProtected>,	mpl::pair<mpl::string<'prot', 'ecte', 'd vi', 'rtua', 'l'>::type,	mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::VirtualMemberFunctionPrivate>,	mpl::pair<mpl::string<'priv', 'ate ', 'virt', 'ual'>::type,			mpl::string<>::type>::type>::type,

mpl::pair<SymbolTypeType_<SymbolTypeType::Unknown>,							mpl::pair<mpl::string<'unkn', 'own'>::type,							mpl::string<>::type>::type>::type
>::type SymbolTypeTypeStringMap;

typedef mpl::map< 
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::None>,					mpl::pair<mpl::string<>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::Const>,				mpl::pair<mpl::string<'cons', 't'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::Pointer>,				mpl::pair<mpl::string<>::type,										mpl::string<'*'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstPointer>,			mpl::pair<mpl::string<'cons', 't'>::type,							mpl::string<'*'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::VolatilePointer>,		mpl::pair<mpl::string<'vola', 'tile'>::type,						mpl::string<'*'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstVolatilePointer>,	mpl::pair<mpl::string<'cons', 't vo', 'lati', 'le'>::type,			mpl::string<'*'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerConst>,			mpl::pair<mpl::string<>::type,										mpl::string<'* co', 'nst'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerVolatile>,		mpl::pair<mpl::string<>::type,										mpl::string<'* vo', 'lati', 'le'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerConstVolatile>,	mpl::pair<mpl::string<>::type,										mpl::string<'* co', 'nst ', 'vola', 'tile'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstPointerConst>,	mpl::pair<mpl::string<'cons', 't'>::type,							mpl::string<'cons', 't'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerRestrict>,		mpl::pair<mpl::string<>::type,										mpl::string<'* __', 'rest', 'rict'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::LValueRef>,			mpl::pair<mpl::string<>::type,										mpl::string<'&'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::RValueRef>,			mpl::pair<mpl::string<>::type,										mpl::string<'&&'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstLValueRef>,		mpl::pair<mpl::string<'cons', 't'>::type,							mpl::string<'&'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::VolatileLValueRef>,	mpl::pair<mpl::string<'vola', 'tile'>::type,						mpl::string<'&'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstVolatileLValueRef>,mpl::pair<mpl::string<'cons', 't vo', 'lati', 'le'>::type,			mpl::string<'&'>::type>::type>::type,

mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::Array>,				mpl::pair<mpl::string<>::type,										mpl::string<'[]'>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstArray>,			mpl::pair<mpl::string<'cons', 't'>::type,							mpl::string<'[]'>::type>::type>::type,

mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::Unknown>,				mpl::pair<mpl::string<'unkn', 'own'>::type,							mpl::string<>::type>::type>::type
>::type SymbolTypeQualifierStringMap;

template<class maptype> struct FillMap
{
	typedef maptype type;
	maptype *map;
	FillMap(maptype &_map) : map(&_map) { }
	template<typename T> void operator()(T) const
	{
		(*map)[T::first::value]=make_pair<std::string, std::string>(mpl::c_str<typename T::second::first>::value, mpl::c_str<typename T::second::second>::value);
	}
};
template<class maptype> struct FillMapR
{
	typedef maptype type;
	maptype *map;
	FillMapR(maptype &_map) : map(&_map) { }
	template<typename T> void operator()(T) const
	{
		(*map)[mpl::c_str<typename T::second::first>::value]=T::first::value;
	}
};
template<class fillmaptype, class sourcemaptype, typename T=fillmaptype::type> T MakeStringMap()
{
	T ret;
	mpl::for_each<sourcemaptype>(fillmaptype(ret));
	return ret;
}

static auto SymbolTypeTypeToString=MakeStringMap<FillMap<unordered_map<SymbolTypeType, pair<string, string>>>, SymbolTypeTypeStringMap>();
static auto StringToSymbolTypeType=MakeStringMap<FillMapR<unordered_map<string, SymbolTypeType>>, SymbolTypeTypeStringMap>();
static auto SymbolTypeQualifierToString=MakeStringMap<FillMap<unordered_map<SymbolTypeQualifier, pair<string, string>>>, SymbolTypeQualifierStringMap>();
static auto StringToSymbolTypeQualifer=MakeStringMap<FillMapR<unordered_map<string, SymbolTypeQualifier>>, SymbolTypeQualifierStringMap>();

std::string SymbolType::prettyText(bool withTypeType) const
{	// <qualifiers> [union|struct|class|enum] <name> [*|&] [<qualifiers>]
	const auto &stq=SymbolTypeQualifierToString[qualifier];
	const auto &stt=SymbolTypeTypeToString[type];
	string ret;
	bool appendbracket=false, isFunction=(SymbolTypeType::Function<=type && SymbolTypeType::VirtualMemberFunctionPrivate>=type);
	auto PrintParams=[this, &ret](const std::list<const SymbolType *> &params) {
		bool first=true;
		for(const auto &p : params)
		{
			if(first)
				first=false;
			else ret.append(", ");
			ret.append(p->prettyText());
		}
	};
	if(isFunction)
	{
		ret.append(stt.first).append(" ");
		if(returns)
			ret.append(std::move(returns->prettyText())).append(" ");
		if(SymbolTypeType::Function==type)
		{
			ret.append("(*");
			appendbracket=true;
		}
		else if(SymbolTypeType::MemberFunction<=type && SymbolTypeType::MemberFunctionPrivate>=type)
		{
			ret.append("(");
			appendbracket=true;
		}
	}
	if(!stq.first.empty()) ret.append(stq.first).append(" ");
	if(!stt.first.empty() && !isFunction && withTypeType) ret.append(stt.first).append(" ");
	if(!dependents.empty())
	{
		for(const auto &d : dependents)
			ret.append(std::move(d->prettyText(false))).append("::");
	}
	if(SymbolTypeType::StaticMemberFunction<=type && SymbolTypeType::MemberFunctionPrivate>=type)
		ret.append("*");
	ret.append(name);
	if(!templ_params.empty())
	{
		ret.append("<");
		PrintParams(templ_params);
		ret.append(">");
	}
	if(!stq.second.empty()) ret.append(" ").append(stq.second);
	if(isFunction)
	{
		if(appendbracket) ret.append(")");
		if(func_params.empty())
			ret.append("(void)");
		else
		{
			ret.append("(");
			PrintParams(func_params);
			ret.append(")");
		}
	}
	if(ret.back()==' ') ret.resize(ret.size()-1);
	return ret;
}

typedef mpl::map< 
mpl::pair<SymbolTypeType_<SymbolTypeType::Void>,				mpl::pair<mpl::string<'X'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Bool>,				mpl::pair<mpl::string<'_N'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Char>,				mpl::pair<mpl::string<'D'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::SignedChar>,			mpl::pair<mpl::string<'C'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::UnsignedChar>,		mpl::pair<mpl::string<'E'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::ShortInt>,			mpl::pair<mpl::string<'F'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::UnsignedShortInt>,	mpl::pair<mpl::string<'G'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Int>,					mpl::pair<mpl::string<'H'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::UnsignedInt>,			mpl::pair<mpl::string<'I'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::LongInt>,				mpl::pair<mpl::string<'J'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::UnsignedLongInt>,		mpl::pair<mpl::string<'K'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::LongLong>,			mpl::pair<mpl::string<'_J'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::UnsignedLongLong>,	mpl::pair<mpl::string<'_K'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Wchar_t>,				mpl::pair<mpl::string<'_W'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Float>,				mpl::pair<mpl::string<'M'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Double>,				mpl::pair<mpl::string<'N'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::LongDouble>,			mpl::pair<mpl::string<'O'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect64>,				mpl::pair<mpl::string<'T__m', '64@@'>::type,				mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect128f>,			mpl::pair<mpl::string<'T__m', '128@', '@'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect128d>,			mpl::pair<mpl::string<'T__m', '128d', '@@'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect128i>,			mpl::pair<mpl::string<'T__m', '128i', '@@'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect256f>,			mpl::pair<mpl::string<'T__m', '256@', '@'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect256d>,			mpl::pair<mpl::string<'T__m', '256d', '@@'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Vect256i>,			mpl::pair<mpl::string<'T__m', '256i', '@@'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Varargs>,				mpl::pair<mpl::string<'Z'>::type,							mpl::string<>::type>::type>::type,

mpl::pair<SymbolTypeType_<SymbolTypeType::Namespace>,			mpl::pair<mpl::string<'V'>::type,							mpl::string<'@@'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Union>,				mpl::pair<mpl::string<'T'>::type,							mpl::string<'@@'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Struct>,				mpl::pair<mpl::string<'U'>::type,							mpl::string<'@@'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Class>,				mpl::pair<mpl::string<'V'>::type,							mpl::string<'@@'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Enum>,				mpl::pair<mpl::string<'W4'>::type,							mpl::string<'@@'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::EnumMember>,			mpl::pair<mpl::string<'W4'>::type,							mpl::string<>::type>::type>::type,

mpl::pair<SymbolTypeType_<SymbolTypeType::Function>,						mpl::pair<mpl::string<'P6A'>::type,				mpl::string<'@Z'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunction>,			mpl::pair<mpl::string<'PEQ'>::type,				mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunctionProtected>,	mpl::pair<mpl::string<'PEQ'>::type,				mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunctionPrivate>,		mpl::pair<mpl::string<'PEQ'>::type,				mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::MemberFunction>,					mpl::pair<mpl::string<'P8'>::type,				mpl::string<'@Z'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::MemberFunctionProtected>,			mpl::pair<mpl::string<'P8'>::type,				mpl::string<'@Z'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::MemberFunctionPrivate>,			mpl::pair<mpl::string<'P8'>::type,				mpl::string<'@Z'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::VirtualMemberFunction>,			mpl::pair<mpl::string<'P8'>::type,				mpl::string<'@Z'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::VirtualMemberFunctionProtected>,	mpl::pair<mpl::string<'P8'>::type,				mpl::string<'@Z'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::VirtualMemberFunctionPrivate>,	mpl::pair<mpl::string<'P8'>::type,				mpl::string<'@Z'>::type>::type>::type,

mpl::pair<SymbolTypeType_<SymbolTypeType::Unknown>,							mpl::pair<mpl::string<'unkn', 'own'>::type,		mpl::string<>::type>::type>::type
>::type SymbolTypeTypeManglingMap_MSVC;

typedef mpl::map< 
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::None>,					mpl::pair<mpl::string<>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::Const>,				mpl::pair<mpl::string<>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::Pointer>,				mpl::pair<mpl::string<'PEA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstPointer>,			mpl::pair<mpl::string<'PEB'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::VolatilePointer>,		mpl::pair<mpl::string<'PEC'>::type,						mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstVolatilePointer>,	mpl::pair<mpl::string<'PED'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerConst>,			mpl::pair<mpl::string<'QEA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerVolatile>,		mpl::pair<mpl::string<'REA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerConstVolatile>,	mpl::pair<mpl::string<'SEA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstPointerConst>,	mpl::pair<mpl::string<'QEB'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerRestrict>,		mpl::pair<mpl::string<'PEIA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::LValueRef>,			mpl::pair<mpl::string<'AEA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::RValueRef>,			mpl::pair<mpl::string<>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstLValueRef>,		mpl::pair<mpl::string<'AEB'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::VolatileLValueRef>,	mpl::pair<mpl::string<'AEC'>::type,						mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstVolatileLValueRef>,mpl::pair<mpl::string<'AED'>::type,			mpl::string<>::type>::type>::type,

mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::Array>,				mpl::pair<mpl::string<'QEA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstArray>,			mpl::pair<mpl::string<'QEB'>::type,							mpl::string<>::type>::type>::type,

mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::Unknown>,				mpl::pair<mpl::string<'unkn', 'own'>::type,							mpl::string<>::type>::type>::type
>::type SymbolTypeQualifierManglingMap_MSVC;


static auto SymbolTypeTypeToMangled=MakeStringMap<FillMap<unordered_map<SymbolTypeType, pair<string, string>>>, SymbolTypeTypeManglingMap_MSVC>();
static auto SymbolTypeQualifierToMangled=MakeStringMap<FillMap<unordered_map<SymbolTypeQualifier, pair<string, string>>>, SymbolTypeQualifierManglingMap_MSVC>();

template<class outputtype> struct FillMapR<spirit::qi::symbols<char, outputtype>>
{
	typedef spirit::qi::symbols<char, outputtype> type;
	type *map;
	FillMapR(type *_map) : map(_map) { }
	template<typename T> void operator()(T) const
	{
		map->add(mpl::c_str<typename T::second::first>::value, T::first::value);
	}
};
} // namespace
BOOST_FUSION_ADAPT_STRUCT_NAMED(NiallsCPP11Utilities::SymbolType, msvc_variable_parse_format,
	(NiallsCPP11Utilities::SymbolTypeType, type)
	(NiallsCPP11Utilities::SymbolTypeStorage, storage)
)
namespace NiallsCPP11Utilities {
namespace qi {
	using namespace boost::spirit::qi;

	struct msvc_type : symbols<char, SymbolTypeType>
	{
		msvc_type()
		{
			mpl::for_each<SymbolTypeTypeManglingMap_MSVC>(FillMapR<symbols<char, SymbolTypeType>>(this));
		}
	};
	struct msvc_qualifier : symbols<char, SymbolTypeQualifier>
	{
		msvc_qualifier()
		{
			mpl::for_each<SymbolTypeQualifierManglingMap_MSVC>(FillMapR<symbols<char, SymbolTypeQualifier>>(this));
		}
	};
	struct msvc_storage : symbols<char, SymbolTypeStorage>
	{
		msvc_storage()
		{
			add ("A", SymbolTypeStorage::None)("?A", SymbolTypeStorage::None)
				("B", SymbolTypeStorage::Const)("?B", SymbolTypeStorage::Const)
				("C", SymbolTypeStorage::Volatile)("?C", SymbolTypeStorage::Volatile)
				("D", SymbolTypeStorage::ConstVolatile)("?D", SymbolTypeStorage::ConstVolatile)
				;
		}
	};
	// This grammar is for a MSVC mangled global variable or static member variable
	template<typename iterator> struct msvc_variable : grammar<iterator, fusion::adapted::msvc_variable_parse_format()>
	{
		SymbolTypeDict &typedict;
		msvc_variable(SymbolTypeDict &_typedict) : msvc_variable::base_type(start), typedict(_typedict)
		{
			start = (lit('3')/*variable*/ | lit('2')/*static member variable*/)
			> type
			> storageclass;
		}

		rule<iterator, fusion::adapted::msvc_variable_parse_format()> start;
		msvc_type type;
		msvc_storage storageclass;
	};
	template<typename iterator> struct msvc_symbol : grammar<iterator, SymbolType()>
	{
		SymbolTypeDict &typedict;
		/* The key to Microsoft symbol mangles is the operator '@@' which consists of a preamble
		and a postamble. Immediately following the '@@' operator is:
		Variable:
		3<type><storage class>
		Static member variable:
		2<type><storage class>
		Function:
		<near|far><calling conv>[<stor ret>]   <return type>[<parameter type>...]<term>Z
		<Y   |Z  ><A|E|G       >[<?A|?B|?C|?D>]<MangledToSymbolTypeType...>      <@>Z
		Member Function:
		<protection>[<const>]<calling conv>[<stor ret>]   <return type>[<parameter type>...]<term>Z
		<A-V       >[<A-D>  ]<A|E|G       >[<?A|?B|?C|?D>]<MangledToSymbolTypeType...>      <@>Z
		*/
		msvc_symbol(SymbolTypeDict &_typedict) : msvc_symbol::base_type(symbol), typedict(_typedict), variable(_typedict)
		{
			text%=+char_;
			symbol%=("@@" > variable);
		}

		rule<iterator, SymbolType()> symbol;
		rule<iterator, std::string> text;
		msvc_variable<iterator> variable;
	};
} // namespace

SymbolType Demangle(SymbolTypeDict &typedict, const std::string &mangled)
{
	SymbolType ret;
	switch(mangled[0])
	{
	case '?': // All MSVC mangles start with a ?
		{
			auto grammar=qi::msvc_symbol<decltype(mangled.begin())>(typedict);
			auto first=mangled.begin(), last=mangled.end();
			bool r = qi::parse(first, last, grammar, ret);
			break;
		}
	}
	return ret;
}


} // namespace
