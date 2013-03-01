/* SymbolMangler
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_MAP_SIZE 50

//#define BOOST_SPIRIT_DEBUG

#include "NiallsCPP11Utilities.hpp"
#include <utility>
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

//! \brief Abstract base class for a symbol demangler
struct SymbolDemangler
{
	virtual ~SymbolDemangler() { }
	virtual bool parse(std::string::const_iterator &first, std::string::const_iterator &last, std::ostream &errout)=0;
};
//! The char is the leading identifier for this type of mangled symbol
typedef StaticTypeRegistry<SymbolDemangler, std::pair<char, std::unique_ptr<SymbolDemangler>(*)(SymbolType &_ret, std::ostream &_err, SymbolTypeDict &_typedict)>> SymbolDemanglerRegistry;

// Some boilerplate for symbol demanglers
template<SymbolTypeType type> struct SymbolTypeType_ { static const SymbolTypeType value=type; };
template<SymbolTypeQualifier type> struct SymbolTypeQualifiers_ { static const SymbolTypeQualifier value=type; };
template<SymbolTypeStorage type> struct SymbolTypeStorage_ { static const SymbolTypeStorage value=type; };
template<class maptype> struct FillMap
{
	typedef maptype type;
	maptype *map;
	FillMap(maptype &_map) : map(&_map) { }
	template<typename T> void operator()(T) const
	{
		using namespace boost;
		(*map)[T::first::value]=std::make_pair<std::string, std::string>(mpl::c_str<typename T::second::first>::value, mpl::c_str<typename T::second::second>::value);
	}
};
template<class maptype> struct FillMapR
{
	typedef maptype type;
	maptype *map;
	FillMapR(maptype &_map) : map(&_map) { }
	template<typename T> void operator()(T) const
	{
		using namespace boost;
		(*map)[mpl::c_str<typename T::second::first>::value]=T::first::value;
	}
};
template<class fillmaptype, class sourcemaptype, typename T=typename fillmaptype::type> T MakeStringMap()
{
	using namespace boost;
	T ret;
	mpl::for_each<sourcemaptype>(fillmaptype(ret));
	return ret;
}

} // namespace
