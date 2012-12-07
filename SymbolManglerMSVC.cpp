/* SymbolMangler
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#include "SymbolMangler.hpp"

namespace NiallsCPP11Utilities {

using namespace std;
using namespace boost;

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

mpl::pair<SymbolTypeType_<SymbolTypeType::Class>,				mpl::pair<mpl::string<'V'>::type,							mpl::string<'@@'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Union>,				mpl::pair<mpl::string<'T'>::type,							mpl::string<'@@'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Struct>,				mpl::pair<mpl::string<'U'>::type,							mpl::string<'@@'>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::Namespace>,			mpl::pair<mpl::string<'V'>::type,							mpl::string<'@@'>::type>::type>::type,
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

	static rule<string::const_iterator, string()> msvc_identifier=+(~char_("@?")) | ("?A0" > +char_("x0-9a-f"));

	struct msvc_simpletype : symbols<char, SymbolTypeType>
	{
		msvc_simpletype()
		{
			name("msvc_simpletype");
			mpl::for_each<SymbolTypeTypeManglingMap_MSVC>(FillMapR<symbols<char, SymbolTypeType>>(this));
		}
	};
	struct msvc_memberfunctiontype : symbols<char, SymbolTypeType>
	{
		msvc_memberfunctiontype()
		{
			name("msvc_memberfunctiontype");
			add
				("A", SymbolTypeType::MemberFunctionPrivate)		("B", SymbolTypeType::MemberFunctionPrivate)
				("C", SymbolTypeType::StaticMemberFunctionPrivate)	("D", SymbolTypeType::StaticMemberFunctionPrivate)
				("E", SymbolTypeType::VirtualMemberFunctionPrivate)	("F", SymbolTypeType::VirtualMemberFunctionPrivate)
				("I", SymbolTypeType::MemberFunctionProtected)		("J", SymbolTypeType::MemberFunctionProtected)
				("K", SymbolTypeType::StaticMemberFunctionProtected)("L", SymbolTypeType::StaticMemberFunctionProtected)
				("M", SymbolTypeType::VirtualMemberFunctionProtected)("N", SymbolTypeType::VirtualMemberFunctionProtected)
				("Q", SymbolTypeType::MemberFunction)				("R", SymbolTypeType::MemberFunction)
				("S", SymbolTypeType::StaticMemberFunction)			("T", SymbolTypeType::StaticMemberFunction)
				("U", SymbolTypeType::VirtualMemberFunction)		("V", SymbolTypeType::VirtualMemberFunction)
				;
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
	struct msvc_operator : symbols<char, string>
	{
		msvc_operator()
		{
			name("msvc_operator");
			add
				("?0", "%1")
				("?1", "~%1")
				("?A", "operator[]")
				("?R", "operator()")
				("?C", "operator->")
				("?E", "operator++")
				("?F", "operator--")
				("?2", "operator new")
				("?_U", "operator new[]")
				("?3", "operator delete")
				("?_V", "operator delete[]")
				("?D", "operator*")
				("?I", "operator&")
				("?H", "operator+")
				("?G", "operator-")
				("?7", "operator!")
				("?S", "operator~")
				("?J", "operator->*")
				("?D", "operator*")
				("?K", "operator/")
				("?L", "operator%")
				("?H", "operator+")
				("?G", "operator-")
				("?6", "operator<<")
				("?5", "operator>>")
				("?M", "operator<")
				("?O", "operator>")
				("?N", "operator<=")
				("?P", "operator>=")
				("?8", "operator==")
				("?9", "operator!=")
				("?I", "operator&")
				("?U", "operator|")
				("?T", "operator^")
				("?V", "operator&&")
				("?W", "operator||")
				("?4", "operator=")
				("?X", "operator*=")
				("?_0", "operator/=")
				("?_1", "operator%=")
				("?Y", "operator+=")
				("?Z", "operator-=")
				("?_3", "operator<<=")
				("?_2", "operator>>=")
				("?_4", "operator&=")
				("?_5", "operator|=")
				("?_6", "operator^=")
				("?Q", "operator,")
				("?B", "operator")
				("?_7", "__vtable")
				;
		}
	};
	// FIXME: This parser can't cope with unsigned long long constants. We need to be more intelligent here.
	template<typename iterator> struct msvc_constant : grammar<iterator, long long()>
	{
		ostream &err;
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
		msvc_constant(ostream &_err) : msvc_constant::base_type(start), err(_err)
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
				err << boost::phoenix::val("Parsing error in constant: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\""));
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
	template<typename iterator> struct msvc_type : grammar<iterator, const SymbolType *(), locals<SymbolType>>
	{
		SymbolTypeDict &typedict;
		ostream &err;
		void reset(SymbolType &a) const
		{
			a.storage=SymbolTypeStorage::None;
			a.qualifier=SymbolTypeQualifier::None;
		}
		void qualifier_writer(SymbolType &a, SymbolTypeQualifier qualifier) const
		{
			if(a.qualifier==SymbolTypeQualifier::None)
			{
				a.qualifier=qualifier;
				a.indirectioncount=0;
			}
			a.indirectioncount+=(qualifier>=SymbolTypeQualifier::Pointer);
		}
		void simpletype_writer(SymbolType &a, SymbolTypeType type) const
		{
			a.type=type;
		}
		void identifier_writer(SymbolType &a, const string &i) const
		{
			a.name=i;
		}
		void finish(const SymbolType *&val, SymbolType &a) const
		{
			string i(a.prettyText());
			SymbolTypeDict::const_iterator dt=typedict.find(i);
			if(dt==typedict.end())
			{
				auto _dt=typedict.emplace(make_pair(i, a));
				dt=_dt.first;
			}
			val=&dt->second;
		}
		bool hasIdentifier(SymbolType &a) const
		{
			return a.type>=SymbolTypeType::Namespace && a.type<=SymbolTypeType::Enum;
		}
		msvc_type(ostream &_err, SymbolTypeDict &_typedict) : msvc_type::base_type(start), err(_err), typedict(_typedict)
		{
			start = eps [ boost::phoenix::bind(&msvc_type::reset, this, _a)]
				>> *qualifier [ boost::phoenix::bind(&msvc_type::qualifier_writer, this, _a, _1)]
				>> *("?" > storage)
				>> simpletype [ boost::phoenix::bind(&msvc_type::simpletype_writer, this, _a, _1)]
				>> (eps(boost::phoenix::bind(&msvc_type::hasIdentifier, this, _a) == false) | msvc_identifier [ boost::phoenix::bind(&msvc_type::identifier_writer, this, _a, _1)])
				>> eps [ boost::phoenix::bind(&msvc_type::finish, this, _val, _a) ];
			BOOST_SPIRIT_DEBUG_NODE(start);
			start.name("msvc_type");
			on_error<boost::spirit::qi::fail, iterator>(start,
				err << boost::phoenix::val("Parsing error in type: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\""));
		}

		rule<iterator, const SymbolType *(), locals<SymbolType>> start;
		msvc_qualifier qualifier;
		msvc_storage storage;
		msvc_simpletype simpletype;
	};
	// This grammar is for a MSVC mangled global variable or static member variable
	template<typename iterator> struct msvc_variable : grammar<iterator>
	{
		SymbolType &ret;
		ostream &err;
		SymbolTypeDict &typedict;
		void global() const { ret.qualifier=SymbolTypeQualifier::None; ret.storage=SymbolTypeStorage::None; }
		void static_() const { ret.qualifier=SymbolTypeQualifier::None; ret.storage=SymbolTypeStorage::Static; }
		void type_writer(const SymbolType *i) const { ret.type=i->type; }
		void return_type_writer(const SymbolType *i) const { ret.returns=i; }
		void parameter_type_writer(const SymbolType *i) const { ret.func_params.push_back(i); }
		void storage_writer(SymbolTypeStorage i) const { ret.storage=static_cast<SymbolTypeStorage>(static_cast<int>(ret.storage)+static_cast<int>(i)); }
		msvc_variable(SymbolType &_ret, ostream &_err, SymbolTypeDict &_typedict) : msvc_variable::base_type(start), ret(_ret), err(_err), typedict(_typedict), type(_err, _typedict)
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
				err << boost::phoenix::val("Parsing error in variable: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\""));
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
		ostream &err;
		SymbolTypeDict &typedict;
		void reset() const { ret.qualifier=SymbolTypeQualifier::None; ret.storage=SymbolTypeStorage::None; ret.type=SymbolTypeType::Function; }
		void return_type_writer(const SymbolType *i) const { ret.returns=i; }
		void storage_writer(SymbolTypeStorage i) const { ret.storage=i; }
		void parameter_type_writer(const SymbolType *i) const { ret.func_params.push_back(i); }
		void pop_last_parameter() const { ret.func_params.pop_back(); }
		msvc_function(SymbolType &_ret, ostream &_err, SymbolTypeDict &_typedict) : msvc_function::base_type(start), ret(_ret), err(_err), typedict(_typedict), type(_err, _typedict)
		{
			delimiter=lit("@") - "@@";
			start = (lit('Y')/*near*/ | lit('Z')/*far*/) [ boost::phoenix::bind(&msvc_function::reset, this) ]
			> char_ /* calling convention */
			> -("?" > storageclass [ boost::phoenix::bind(&msvc_function::storage_writer, this, _1) ]) /* optional storage class */
			> type [ boost::phoenix::bind(&msvc_function::return_type_writer, this, _1) ]
			> +type [ boost::phoenix::bind(&msvc_function::parameter_type_writer, this, _1) ]
			> ("@Z" | eps [ boost::phoenix::bind(&msvc_function::pop_last_parameter, this) ]);
			BOOST_SPIRIT_DEBUG_NODE(start);
			start.name("msvc_function");
			on_error<boost::spirit::qi::fail, iterator>(start,
				err << boost::phoenix::val("Parsing error in function: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\""));
		}

		rule<iterator> start;
		rule<iterator> delimiter;
		msvc_type<iterator> type;
		msvc_storage storageclass;
	};
	/* This grammar is for a MSVC mangled function
		<protection>[<const>]<calling conv>[<stor ret>]   <return type>[<parameter type>...]<term>Z
		<A-V       >[<A-D>  ]<A|E|G       >[<?A|?B|?C|?D>]<MangledToSymbolTypeType...>      <@>Z
	*/
	template<typename iterator> struct msvc_memberfunction : grammar<iterator>
	{
		SymbolType &ret;
		ostream &err;
		SymbolTypeDict &typedict;
		void type_writer(SymbolTypeType i) const { ret.qualifier=SymbolTypeQualifier::None; ret.storage=SymbolTypeStorage::None; ret.type=i; }
		void storage_writer(SymbolTypeStorage i) const { ret.storage=i; }
		void return_type_writer(const SymbolType *i) const { ret.returns=i; }
		void return_self_type_writer() const { ret.returns=ret.dependents.back(); }
		void parameter_type_writer(const SymbolType *i) const { ret.func_params.push_back(i); }
		void pop_last_parameter() const { ret.func_params.pop_back(); }
		msvc_memberfunction(SymbolType &_ret, ostream &_err, SymbolTypeDict &_typedict) : msvc_memberfunction::base_type(start), ret(_ret), err(_err), typedict(_typedict), type(_err, _typedict)
		{
			delimiter=lit("@") - "@@";
			start = memberfunctiontype [ boost::phoenix::bind(&msvc_memberfunction::type_writer, this, _1) ]
			> storageclass [ boost::phoenix::bind(&msvc_memberfunction::storage_writer, this, _1) ]
			> char_ // calling convention
			> (delimiter [ boost::phoenix::bind(&msvc_memberfunction::return_self_type_writer, this) ] // '@' = return type of self
				| type [ boost::phoenix::bind(&msvc_memberfunction::return_type_writer, this, _1) ])
			> +type [ boost::phoenix::bind(&msvc_memberfunction::parameter_type_writer, this, _1) ]
			> ("@Z" | eps [ boost::phoenix::bind(&msvc_memberfunction::pop_last_parameter, this) ]);
			BOOST_SPIRIT_DEBUG_NODE(start);
			start.name("msvc_memberfunction");
			on_error<boost::spirit::qi::fail, iterator>(start,
				err << boost::phoenix::val("Parsing error in memberfunction: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\""));
		}

		rule<iterator> start;
		rule<iterator> delimiter;
		msvc_memberfunctiontype memberfunctiontype;
		msvc_type<iterator> type;
		msvc_storage storageclass;
	};
	// This grammar is for a MSVC mangled identifier
	template<typename iterator> struct msvc_name : grammar<iterator>
	{
		SymbolType &ret;
		ostream &err;
		SymbolTypeDict &typedict;
		void name_writer(const string &i) const { ret.name=i; }
		void replace_operator_name(const string &i) const
		{
			// For constructor and destructors, we need to patch in the right name during dependents
			auto idx=ret.name.find("%1");
			if(idx!=(size_t)-1)
				ret.name.replace(ret.name.begin()+idx, ret.name.begin()+idx+2, i);
		}
		void dependent_writer(const string &_i) const
		{
			string i("class "+_i);
			SymbolTypeDict::const_iterator dt=typedict.find(i);
			if(dt==typedict.end())
			{
				auto _dt=typedict.emplace(make_pair(i, SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Class, _i)));
				dt=_dt.first;
			}
			if(ret.dependents.empty()) replace_operator_name(_i);
			ret.dependents.push_front(&dt->second);
		}
		// These work by spreading the building of a templated type over multiple calls using local variables _a and _b
		// We accumulate template parameters into _a and accumulate mangled symbolness into _b
		void begin_template_dependent_writer(SymbolType &a, const string &_i) const
		{
			a=SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Class, _i);
		}
		void add_template_dependent_dependent_writer(SymbolType &a, const string &_i) const
		{
			string i("class "+_i);
			SymbolTypeDict::const_iterator dt=typedict.find(i);
			if(dt==typedict.end())
			{
				auto _dt=typedict.emplace(make_pair(i, SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Class, _i)));
				dt=_dt.first;
			}
			a.dependents.push_front(&dt->second);
		}
		void add_template_constant_dependent_writer(SymbolType &a, long long constant) const
		{
			string i("__c"+to_string(constant));
			SymbolTypeDict::const_iterator dt=typedict.find(i);
			if(dt==typedict.end())
			{
				auto _dt=typedict.emplace(make_pair(i, SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Constant, to_string(constant))));
				dt=_dt.first;
			}
			a.templ_params.push_back(&dt->second);
		}
		void add_template_type_dependent_writer(SymbolType &a, const SymbolType *type) const
		{
			a.templ_params.push_back(type);
		}
		void finish_template_dependent_writer(SymbolType &a) const
		{
			string b=a.prettyText();
			SymbolTypeDict::const_iterator dt=typedict.find(b);
			if(dt==typedict.end())
			{
				auto _dt=typedict.emplace(make_pair(b, a));
				dt=_dt.first;
			}
			if(ret.dependents.empty()) replace_operator_name(dt->second.name);
			ret.dependents.push_front(&dt->second);
		}
		msvc_name(SymbolType &_ret, ostream &_err, SymbolTypeDict &_typedict) : msvc_name::base_type(start), ret(_ret), err(_err), typedict(_typedict), type(_err, _typedict), constant(_err)
		{
			delimiter=lit("@") - "@@";
			template_dependent_identifier=msvc_identifier;
			template_dependent_identifier.name("template_dependent_identifier");
			dependent_identifier=msvc_identifier;
			dependent_identifier.name("dependent_identifier");
			template_dependent_sequence=("?$" > template_dependent_identifier [ boost::phoenix::bind(&msvc_name::begin_template_dependent_writer, this, _a, _1) ])
					>> *(delimiter > (/* FIXME: missing template recurse here */
						dependent_identifier [ boost::phoenix::bind(&msvc_name::add_template_dependent_dependent_writer, this, _a, _1) ]))
				>> "@@" >>
					+(( "$" > char_("0-9") > constant [ boost::phoenix::bind(&msvc_name::add_template_constant_dependent_writer, this, _a, _1) ])
						| type [ boost::phoenix::bind(&msvc_name::add_template_type_dependent_writer, this, _a, _1) ])
					>> eps [ boost::phoenix::bind(&msvc_name::finish_template_dependent_writer, this, _a) ];
			dependent_sequence=dependent_identifier [ boost::phoenix::bind(&msvc_name::dependent_writer, this, _1) ];

			start = ((operator_ [ boost::phoenix::bind(&msvc_name::name_writer, this, _1) ] > (template_dependent_sequence | dependent_sequence))
					| msvc_identifier [ boost::phoenix::bind(&msvc_name::name_writer, this, _1) ])
				>> *(delimiter > (template_dependent_sequence | dependent_sequence));
			BOOST_SPIRIT_DEBUG_NODE(start);
			start.name("msvc_name");
			on_error<boost::spirit::qi::fail, iterator>(start,
				err << boost::phoenix::val("Parsing error in name: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\""));
		}

		rule<iterator> start;
		rule<iterator> delimiter;
		rule<iterator, string()> template_dependent_identifier, dependent_identifier;
		rule<iterator, locals<SymbolType>> template_dependent_sequence;
		rule<iterator> dependent_sequence;
		msvc_operator operator_;
		msvc_type<iterator> type;
		msvc_constant<iterator> constant;
	};
	template<typename iterator> struct msvc_symbol : SymbolDemangler, grammar<iterator>
	{
		SymbolType &ret;
		ostream &err;
		SymbolTypeDict &typedict;
		void vtable_storage_writer(SymbolTypeStorage i) const { ret.qualifier=SymbolTypeQualifier::None; ret.type=SymbolTypeType::VTable; ret.storage=i; }
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
		msvc_symbol(SymbolType &_ret, ostream &_err, SymbolTypeDict &_typedict) : msvc_symbol::base_type(start), ret(_ret), err(_err), typedict(_typedict), name(ret, _err, _typedict), variable(ret, _err, _typedict), function(ret, _err, _typedict), memberfunction(ret, _err, _typedict)
		{
			vtable="6" > storageclass [ boost::phoenix::bind(&msvc_symbol::vtable_storage_writer, this, _1) ];
			start="?" >> name >> "@@" >> (variable | function | memberfunction | vtable);
			BOOST_SPIRIT_DEBUG_NODE(start);
			on_error<boost::spirit::qi::fail, iterator>(start,
				err << boost::phoenix::val("Parsing error in symbol: Expected ") << _4 << boost::phoenix::val(" here: \"")
					<< boost::phoenix::construct<string>(_3, _2) << boost::phoenix::val("\""));
		}
		virtual bool parse(std::string::const_iterator &first, std::string::const_iterator &last, std::ostream &errout)
		{
			return qi::parse(first, last, *this, errout);
		}

		rule<iterator> start;
		msvc_name<iterator> name;
		msvc_variable<iterator> variable;
		msvc_function<iterator> function;
		msvc_memberfunction<iterator> memberfunction;
		rule<iterator> vtable;
		msvc_storage storageclass;
	};
} // namespace

static auto reg=NiallsCPP11Utilities::AutoDataRegistration<SymbolDemanglerRegistry>(make_pair('?', [](SymbolType &_ret, std::ostream &_err, SymbolTypeDict &_typedict) { return std::unique_ptr<SymbolDemangler>(new qi::msvc_symbol<string::const_iterator>(_ret, _err, _typedict)); }));

} // namespace

