/* SymbolMangler
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_MAP_SIZE 50

#define BOOST_SPIRIT_DEBUG

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
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
//#include <boost/fusion/include/adapt_struct.hpp>
//#include <boost/fusion/include/adapt_struct_named.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#if 0
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
#endif

namespace NiallsCPP11Utilities {

using namespace std;
using namespace boost;

template<SymbolTypeType type> struct SymbolTypeType_ { static const SymbolTypeType value=type; };
template<SymbolTypeQualifier type> struct SymbolTypeQualifiers_ { static const SymbolTypeQualifier value=type; };
template<SymbolTypeStorage type> struct SymbolTypeStorage_ { static const SymbolTypeStorage value=type; };

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

typedef mpl::map< 
mpl::pair<SymbolTypeStorage_<SymbolTypeStorage::None>,						mpl::pair<mpl::string<>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeStorage_<SymbolTypeStorage::Const>,						mpl::pair<mpl::string<'cons', 't'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeStorage_<SymbolTypeStorage::Volatile>,					mpl::pair<mpl::string<'vola', 'tile'>::type,						mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeStorage_<SymbolTypeStorage::ConstVolatile>,				mpl::pair<mpl::string<'cons', 't vo', 'lati', 'le'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeStorage_<SymbolTypeStorage::Static>,					mpl::pair<mpl::string<'stat', 'ic'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeStorage_<SymbolTypeStorage::StaticConst>,				mpl::pair<mpl::string<'stat', 'ic c', 'onst'>::type,				mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeStorage_<SymbolTypeStorage::StaticVolatile>,			mpl::pair<mpl::string<'stat', 'ic v', 'olit', 'ile'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeStorage_<SymbolTypeStorage::StaticConstVolatile>,		mpl::pair<mpl::string<'stat', 'ic c', 'onst', ' vol', 'atil', 'e'>::type, mpl::string<>::type>::type>::type
>::type SymbolTypeStorageStringMap;

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
static auto SymbolTypeQualifierToString=MakeStringMap<FillMap<unordered_map<SymbolTypeQualifier, pair<string, string>>>, SymbolTypeQualifierStringMap>();
static auto SymbolTypeStorageToString=MakeStringMap<FillMap<unordered_map<SymbolTypeStorage, pair<string, string>>>, SymbolTypeStorageStringMap>();

std::string SymbolType::prettyText(bool withTypeType) const
{	// <qualifiers> [union|struct|class|enum] <name> [*|&] [<qualifiers>]
	const auto &stq=SymbolTypeQualifierToString[qualifier];
	const auto &stt=SymbolTypeTypeToString[type];
	const auto &sts=SymbolTypeStorageToString[storage];
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
	if(!sts.first.empty()) ret.append(sts.first).append(" ");
	if(isFunction)
	{
		if(!stt.first.empty()) ret.append(stt.first).append(" ");
		if(returns)
			ret.append(std::move(returns->prettyText())).append(" ");
		else
		{
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
	if(!stq.second.empty()) for(int n=0; n<indirectioncount; n++) ret.append(" ").append(stq.second);
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
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunction>,			mpl::pair<mpl::string<'PQ'>::type,				mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunctionProtected>,	mpl::pair<mpl::string<'PQ'>::type,				mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunctionPrivate>,		mpl::pair<mpl::string<'PQ'>::type,				mpl::string<>::type>::type>::type,
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
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::Pointer>,				mpl::pair<mpl::string<'PA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstPointer>,			mpl::pair<mpl::string<'PB'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::VolatilePointer>,		mpl::pair<mpl::string<'PC'>::type,						mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstVolatilePointer>,	mpl::pair<mpl::string<'PD'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerConst>,			mpl::pair<mpl::string<'QA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerVolatile>,		mpl::pair<mpl::string<'RA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerConstVolatile>,	mpl::pair<mpl::string<'SA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstPointerConst>,	mpl::pair<mpl::string<'QB'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::PointerRestrict>,		mpl::pair<mpl::string<'PIA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::LValueRef>,			mpl::pair<mpl::string<'AA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::RValueRef>,			mpl::pair<mpl::string<>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstLValueRef>,		mpl::pair<mpl::string<'AB'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::VolatileLValueRef>,	mpl::pair<mpl::string<'AC'>::type,						mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstVolatileLValueRef>,mpl::pair<mpl::string<'AD'>::type,			mpl::string<>::type>::type>::type,

mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::Array>,				mpl::pair<mpl::string<'QA'>::type,										mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeQualifiers_<SymbolTypeQualifier::ConstArray>,			mpl::pair<mpl::string<'QB'>::type,							mpl::string<>::type>::type>::type,

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

ostream &operator<<(ostream &s, const SymbolType &type)
{
	s << type.prettyText();
	return s;
}

namespace qi {
	using namespace boost::spirit::qi;
	using std::string;

	struct msvc_simpletype : symbols<char, SymbolTypeType>
	{
		msvc_simpletype()
		{
			name("msvc_simpletype");
			mpl::for_each<SymbolTypeTypeManglingMap_MSVC>(FillMapR<symbols<char, SymbolTypeType>>(this));
		}
	};
	struct msvc_qualifier : symbols<char, SymbolTypeQualifier>
	{
		msvc_qualifier()
		{
			name("msvc_qualifier");
			mpl::for_each<SymbolTypeQualifierManglingMap_MSVC>(FillMapR<symbols<char, SymbolTypeQualifier>>(this));
		}
	};
	struct msvc_storage : symbols<char, SymbolTypeStorage>
	{
		msvc_storage()
		{
			name("msvc_storage");
			add ("A", SymbolTypeStorage::None)("?A", SymbolTypeStorage::None)
				("B", SymbolTypeStorage::Const)("?B", SymbolTypeStorage::Const)
				("C", SymbolTypeStorage::Volatile)("?C", SymbolTypeStorage::Volatile)
				("D", SymbolTypeStorage::ConstVolatile)("?D", SymbolTypeStorage::ConstVolatile)
				;
		}
	};
	// FIXME: This parser can't cope with unsigned long long constants. We need to be more intelligent here.
	template<typename iterator> struct msvc_constant : grammar<iterator, long long()>
	{
		void zero(long long &val) const { val=0; }
		void digit(long long &val, char i, bool neg) const
		{
			val=i-'0'+1;
			if(neg) val=-val;
		}
		void decoder(long long &val, const string &i, bool neg) const
		{	// Hexadecimal, but encoded A-P instead of 0-F
			string v(i);
			for(auto &c : v)
				c=(c-'A'+'0');
			val=stoll(v, 0, 16);
			if(neg) val=-val;
		}
		msvc_constant() : msvc_constant::base_type(start)
		{
			hexencoding=+char_("A-P");
			start = lit("A@") [ boost::phoenix::bind(&msvc_constant::zero, this, _val) ]
				| ( "?" > (char_("0-9") [ boost::phoenix::bind(&msvc_constant::digit, this, _val, _1, true) ]
					| hexencoding [ boost::phoenix::bind(&msvc_constant::decoder, this, _val, _1, true) ]))
				| (char_("0-9") [ boost::phoenix::bind(&msvc_constant::digit, this, _val, _1, false) ]
					| hexencoding [ boost::phoenix::bind(&msvc_constant::decoder, this, _val, _1, false) ]);
			BOOST_SPIRIT_DEBUG_NODE(start);
			start.name("msvc_constant");
			on_error<boost::spirit::qi::fail, iterator>(start,
				cerr << boost::phoenix::val("Parsing error: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\"") << endl);
		}

		rule<iterator, long long()> start;
		rule<iterator, string()> hexencoding;
	};
	/* This grammar is for a MSVC mangled type
	Right now it wraps msvc_simpletype and does no more :)

	FIXME: Currently throws away intermediate qualifiers, so const void **volatile ***const **
	will lose the intermediate bits
	FIXME: Currently makes no attempt at templated types
	*/
	template<typename iterator> struct msvc_type : grammar<iterator, const SymbolType *()>
	{
		SymbolTypeDict &typedict;
		// This is a nasty FIXME, but need to work around const volatile void ***** or worse
		mutable SymbolTypeQualifier qualifier_;
		mutable int qualifier_count;
		void reset() const
		{
			qualifier_=SymbolTypeQualifier::None;
		}
		void qualifier_writer(const SymbolType *&, SymbolTypeQualifier qualifier) const
		{
			if(qualifier_==SymbolTypeQualifier::None)
			{
				qualifier_=qualifier;
				qualifier_count=0;
			}
			qualifier_count+=(qualifier>=SymbolTypeQualifier::Pointer);
		}
		void simpletype_writer(const SymbolType *&val, SymbolTypeType type) const
		{
			string i("_t"+to_string(static_cast<int>(type))+"_"+to_string(static_cast<int>(qualifier_))+"_"+to_string(qualifier_count));
			SymbolTypeDict::const_iterator dt=typedict.find(i);
			if(dt==typedict.end())
			{
				SymbolType v(qualifier_, type);
				v.indirectioncount=qualifier_count;
				auto _dt=typedict.emplace(make_pair(i, v));
				dt=_dt.first;
			}
			val=&dt->second;
		}
		msvc_type(SymbolTypeDict &_typedict) : msvc_type::base_type(start), typedict(_typedict)
		{
			start = eps [ boost::phoenix::bind(&msvc_type::reset, this)]
				>> *qualifier [ boost::phoenix::bind(&msvc_type::qualifier_writer, this, _val, _1)]
				>> simpletype [ boost::phoenix::bind(&msvc_type::simpletype_writer, this, _val, _1)];
			BOOST_SPIRIT_DEBUG_NODE(start);
			start.name("msvc_type");
			on_error<boost::spirit::qi::fail, iterator>(start,
				cerr << boost::phoenix::val("Parsing error: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\"") << endl);
		}

		rule<iterator, const SymbolType *()> start;
		msvc_qualifier qualifier;
		msvc_simpletype simpletype;
	};
	// This grammar is for a MSVC mangled global variable or static member variable
	template<typename iterator> struct msvc_variable : grammar<iterator>
	{
		SymbolType &ret;
		SymbolTypeDict &typedict;
		void global() const { ret.qualifier=SymbolTypeQualifier::None; ret.storage=SymbolTypeStorage::None; }
		void static_() const { ret.qualifier=SymbolTypeQualifier::None; ret.storage=SymbolTypeStorage::Static; }
		void type_writer(const SymbolType *i) const { ret.type=i->type; }
		void return_type_writer(const SymbolType *i) const { ret.returns=i; }
		void parameter_type_writer(const SymbolType *i) const { ret.func_params.push_back(i); }
		void storage_writer(SymbolTypeStorage i) const { ret.storage=static_cast<SymbolTypeStorage>(static_cast<int>(ret.storage)+static_cast<int>(i)); }
		msvc_variable(SymbolType &_ret, SymbolTypeDict &_typedict) : msvc_variable::base_type(start), ret(_ret), typedict(_typedict), type(_typedict)
		{
			delimiter=lit("@") - "@@";
			start = (lit('3')/*variable*/ [ boost::phoenix::bind(&msvc_variable::global, this)]
				| lit('2')/*static member variable*/ [ boost::phoenix::bind(&msvc_variable::static_, this)])
			> type [ boost::phoenix::bind(&msvc_variable::type_writer, this, _1)]
			> (storageclass [ boost::phoenix::bind(&msvc_variable::storage_writer, this, _1) ]
				| (type [ boost::phoenix::bind(&msvc_variable::return_type_writer, this, _1)]
				> +("@Z" | type [ boost::phoenix::bind(&msvc_variable::parameter_type_writer, this, _1)])
				> storageclass [ boost::phoenix::bind(&msvc_variable::storage_writer, this, _1) ]));
			BOOST_SPIRIT_DEBUG_NODE(start);
			start.name("msvc_variable");
			on_error<boost::spirit::qi::fail, iterator>(start,
				cerr << boost::phoenix::val("Parsing error: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\"") << endl);
		}

		rule<iterator> start;
		rule<iterator> delimiter;
		msvc_type<iterator> type;
		msvc_storage storageclass;
	};
	/* This grammar is for a MSVC mangled function
		<near|far><calling conv>[<stor ret>]   <return type>[<parameter type>...]<term>Z
		<Y   |Z  ><A|E|G       >[<?A|?B|?C|?D>]<MangledToSymbolTypeType...>      <@>Z
	*/
	template<typename iterator> struct msvc_function : grammar<iterator>
	{
		SymbolType &ret;
		SymbolTypeDict &typedict;
		void reset() const { ret.qualifier=SymbolTypeQualifier::None; ret.storage=SymbolTypeStorage::None; ret.type=SymbolTypeType::Function; }
		void return_type_writer(const SymbolType *i) const { ret.returns=i; }
		void storage_writer(SymbolTypeStorage i) const { ret.storage=i; }
		void parameter_type_writer(const SymbolType *i) const { ret.func_params.push_back(i); }
		void pop_last_parameter() const { ret.func_params.pop_back(); }
		msvc_function(SymbolType &_ret, SymbolTypeDict &_typedict) : msvc_function::base_type(start), ret(_ret), typedict(_typedict), type(_typedict)
		{
			delimiter=lit("@") - "@@";
			start = (lit('Y')/*near*/ | lit('Z')/*far*/) [ boost::phoenix::bind(&msvc_function::reset, this) ]
			> char_ /* calling convention */
			> -storageclass [ boost::phoenix::bind(&msvc_function::storage_writer, this, _1) ] /* optional storage class */
			> type [ boost::phoenix::bind(&msvc_function::return_type_writer, this, _1) ]
			> +type [ boost::phoenix::bind(&msvc_function::parameter_type_writer, this, _1) ]
			> ("@Z" | eps [ boost::phoenix::bind(&msvc_function::pop_last_parameter, this) ]);
			BOOST_SPIRIT_DEBUG_NODE(start);
			start.name("msvc_function");
			on_error<boost::spirit::qi::fail, iterator>(start,
				cerr << boost::phoenix::val("Parsing error: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\"") << endl);
		}

		rule<iterator> start;
		rule<iterator> delimiter;
		msvc_type<iterator> type;
		msvc_storage storageclass;
	};
	// This grammar is for a MSVC mangled identifier
	template<typename iterator> struct msvc_name : grammar<iterator, locals<SymbolType, string>>
	{
		SymbolType &ret;
		SymbolTypeDict &typedict;
		void name_writer(const string &i) const { ret.name=i; }
		void dependent_writer(const string &i) const
		{
			SymbolTypeDict::const_iterator dt=typedict.find(i);
			if(dt==typedict.end())
			{
				auto _dt=typedict.emplace(make_pair(i, SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Namespace, i)));
				dt=_dt.first;
			}
			ret.dependents.push_back(&dt->second);
		}
		// These work by spreading the building of a templated type over multiple calls using local variables _a and _b
		// We accumulate template parameters into _a and accumulate mangled symbolness into _b
		void begin_template_dependent_writer(SymbolType &a, string &b, const string &i) const
		{
			a=SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Class, i);
			b=i;
		}
		void add_template_constant_dependent_writer(SymbolType &a, string &b, long long constant) const
		{
			string i("_c"+to_string(constant));
			SymbolTypeDict::const_iterator dt=typedict.find(i);
			if(dt==typedict.end())
			{
				auto _dt=typedict.emplace(make_pair(i, SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Constant, to_string(constant))));
				dt=_dt.first;
			}
			a.templ_params.push_back(&dt->second);
			b.append(i);
		}
		void add_template_type_dependent_writer(SymbolType &a, string &b, const SymbolType *type) const
		{
			string i("_t"+to_string(static_cast<int>(type->type)));
			a.templ_params.push_back(type);
			b.append(i);
		}
		void finish_template_dependent_writer(SymbolType &a, string &b) const
		{
			SymbolTypeDict::const_iterator dt=typedict.find(b);
			if(dt==typedict.end())
			{
				auto _dt=typedict.emplace(make_pair(b, a));
				dt=_dt.first;
			}
			ret.dependents.push_back(&dt->second);
		}
		msvc_name(SymbolType &_ret, SymbolTypeDict &_typedict) : msvc_name::base_type(start), ret(_ret), typedict(_typedict), type(_typedict)
		{
			delimiter=lit("@") - "@@";
			identifier=+(char_ - '@');
			identifier.name("identifier");
			template_dependent_identifier=+(char_ - '@');
			template_dependent_identifier.name("template_dependent_identifier");
			dependent_identifier=+(char_ - '@');
			dependent_identifier.name("dependent_identifier");
			start = identifier [ boost::phoenix::bind(&msvc_name::name_writer, this, _1) ] >> *(
				(("@?$" > template_dependent_identifier [ boost::phoenix::bind(&msvc_name::begin_template_dependent_writer, this, _a, _b, _1) ])
					> delimiter > +(( "$0" > constant [ boost::phoenix::bind(&msvc_name::add_template_constant_dependent_writer, this, _a, _b, _1) ])
						| type [ boost::phoenix::bind(&msvc_name::add_template_type_dependent_writer, this, _a, _b, _1) ])
					>> eps [ boost::phoenix::bind(&msvc_name::finish_template_dependent_writer, this, _a, _b) ])
				| (delimiter > dependent_identifier [ boost::phoenix::bind(&msvc_name::dependent_writer, this, _1) ]))
				;
			BOOST_SPIRIT_DEBUG_NODE(start);
			start.name("msvc_name");
			on_error<boost::spirit::qi::fail, iterator>(start,
				cerr << boost::phoenix::val("Parsing error: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\"") << endl);
		}

		rule<iterator, locals<SymbolType, string>> start;
		rule<iterator> delimiter;
		rule<iterator, string()> identifier, template_dependent_identifier, dependent_identifier;
		msvc_type<iterator> type;
		msvc_constant<iterator> constant;
	};
	template<typename iterator> struct msvc_symbol : grammar<iterator>
	{
		SymbolType &ret;
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
		msvc_symbol(SymbolType &_ret, SymbolTypeDict &_typedict) : msvc_symbol::base_type(start), ret(_ret), typedict(_typedict), name(ret, _typedict), variable(ret, _typedict), function(ret, _typedict)
		{
			start="?" >> name >> "@@" >> (variable | function);
			BOOST_SPIRIT_DEBUG_NODE(start);
			on_error<boost::spirit::qi::fail, iterator>(start,
				cerr << boost::phoenix::val("Parsing error: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\"") << endl);
		}

		rule<iterator> start;
		msvc_name<iterator> name;
		msvc_variable<iterator> variable;
		msvc_function<iterator> function;
	};
} // namespace

std::pair<std::vector<SymbolType>, std::vector<SymbolType>> Demangle(SymbolTypeDict &typedict, const std::vector<std::string> &mangleds)
{
	SymbolType ret;
	auto symbolmsvc=qi::msvc_symbol<std::string::const_iterator>(ret, typedict);
	vector<SymbolType> parsed, failed;
	parsed.reserve(mangleds.size());
	failed.reserve(mangleds.size());
	for(const auto &mangled : mangleds)
	{
		switch(mangled[0])
		{
		case '?': // All MSVC mangles start with a ?
			{
				ret=move(SymbolType());
				auto first=mangled.begin(), last=mangled.end();
				if(qi::parse(first, last, symbolmsvc))
					parsed.push_back(move(ret));
				else
					failed.push_back(move(ret));
				break;
			}
		case '_': // All Itanium (GCC) mangles start with a _
			{
				// TODO: Needs implementing
				SymbolType ret;
				ret.name=mangled;
				failed.push_back(move(ret));
				break;
			}
		default: // Probably a C symbol, so assume it's a function (we can't tell if it's a variable)
			{
				SymbolType ret(SymbolTypeQualifier::None, SymbolTypeType::Function, mangled);
				parsed.push_back(move(ret));
				break;
			}
		}
	}
	parsed.reserve(parsed.size());
	failed.reserve(failed.size());
	return make_pair(move(parsed), move(failed));
}


} // namespace
