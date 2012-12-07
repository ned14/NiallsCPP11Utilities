/* SymbolMangler
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#include "SymbolMangler.hpp"

namespace NiallsCPP11Utilities {

using namespace std;
using namespace boost;

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
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunction>,			mpl::pair<mpl::string<'publ', 'ic:'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunctionProtected>,	mpl::pair<mpl::string<'prot', 'ecte', 'd:'>::type,					mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::StaticMemberFunctionPrivate>,		mpl::pair<mpl::string<'pri', 'vate', ':'>::type,					mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::MemberFunction>,					mpl::pair<mpl::string<'publ', 'ic:'>::type,							mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::MemberFunctionProtected>,			mpl::pair<mpl::string<'prot', 'ecte', 'd:'>::type,					mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::MemberFunctionPrivate>,			mpl::pair<mpl::string<'priv', 'ate', ':'>::type,					mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::VirtualMemberFunction>,			mpl::pair<mpl::string<'publ', 'ic: ', 'virt', 'ual'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::VirtualMemberFunctionProtected>,	mpl::pair<mpl::string<'prot', 'ecte', 'd: v', 'irtu', 'al'>::type,	mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeType_<SymbolTypeType::VirtualMemberFunctionPrivate>,	mpl::pair<mpl::string<'priv', 'ate:', ' vir', 'tual'>::type,			mpl::string<>::type>::type>::type,

mpl::pair<SymbolTypeType_<SymbolTypeType::VTable>,							mpl::pair<mpl::string<'vtab', 'le'>::type,			mpl::string<>::type>::type>::type,

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
mpl::pair<SymbolTypeStorage_<SymbolTypeStorage::StaticVolatile>,			mpl::pair<mpl::string<'stat', 'ic v', 'olat', 'ile'>::type,			mpl::string<>::type>::type>::type,
mpl::pair<SymbolTypeStorage_<SymbolTypeStorage::StaticConstVolatile>,		mpl::pair<mpl::string<'stat', 'ic c', 'onst', ' vol', 'atil', 'e'>::type, mpl::string<>::type>::type>::type
>::type SymbolTypeStorageStringMap;

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
	if(!isFunction)
	{
		if(!sts.first.empty()) ret.append(sts.first).append(" ");
	}
	else
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
	//if(SymbolTypeType::StaticMemberFunction<=type && SymbolTypeType::MemberFunctionPrivate>=type)
	//	ret.append("*");
	ret.append(name);
	if(!templ_params.empty())
	{
		ret.append("<");
		PrintParams(templ_params);
		ret.append(">");
	}
	if(!stq.second.empty())
	{
		if(ret.back()!=' ') ret.append(" ");
		for(int n=0; n<indirectioncount; n++) ret.append(stq.second);
	}
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
		if(!sts.first.empty()) ret.append(" ").append(sts.first);
	}
	if(ret.back()==' ') ret.resize(ret.size()-1);
	return ret;
}

namespace Private
{
	struct SymbolDemangle
	{
		unique_ptr<SymbolTypeDict> default_typedict;
		SymbolTypeDict &typedict;
		SymbolType temp;
		stringstream errstream;
		vector<std::pair<char, std::unique_ptr<SymbolDemangler>>> demanglers;
		std::unordered_map<std::string, SymbolType> parsedSymbols;
		std::unordered_map<std::string, std::pair<SymbolType, std::string>> failedParsedSymbols;
		std::unordered_multimap<std::string, std::string> namespaces;

		SymbolDemangle(SymbolTypeDict &_typedict) : typedict(_typedict) { init(); }
		SymbolDemangle() : default_typedict(new SymbolTypeDict), typedict(*default_typedict) { init(); }
	private:
		void init()
		{
			for(const auto &dm : SymbolDemanglerRegistry())
				demanglers.push_back(make_pair(dm.first, dm.second(temp, errstream, typedict)));
		}
#if !defined(_MSC_VER) || _MSC_VER>1700
		SymbolDemangle &operator=(const SymbolDemangle &) = delete;
#else
		SymbolDemangle &operator=(const SymbolDemangle &);
#endif
	};
}

SymbolDemangle::SymbolDemangle() : p(new Private::SymbolDemangle())
{
}

SymbolDemangle::SymbolDemangle(SymbolTypeDict &typedict) : p(new Private::SymbolDemangle(typedict))
{
}

SymbolDemangle::~SymbolDemangle()
{
	delete p;
	p=nullptr;
}

SymbolTypeDict &SymbolDemangle::typedict() const
{
	return p->typedict;
}

void SymbolDemangle::setTypedict(SymbolTypeDict &typedict)
{
	delete p;
	p=nullptr;
	p=new Private::SymbolDemangle(typedict);
}

void SymbolDemangle::reset()
{
	setTypedict(p->typedict);
}

const std::unordered_map<std::string, SymbolType> &SymbolDemangle::parsedSymbols() const
{
	return p->parsedSymbols;
}

const std::unordered_map<std::string, std::pair<SymbolType, std::string>> &SymbolDemangle::failedParsedSymbols() const
{
	return p->failedParsedSymbols;
}

const std::unordered_multimap<std::string, std::string> &SymbolDemangle::namespaces() const
{
	return p->namespaces;
}

std::pair<const SymbolType *, bool>  SymbolDemangle::demangle(const std::string &mangled)
{
	const SymbolType *ret=nullptr;
	bool success=true, found=false;
	for(const auto &demangler : p->demanglers)
		if(demangler.first==mangled[0])
		{
			found=true;
			p->temp=SymbolType();
			auto clearerror=Undoer([this](){p->errstream.str(string()); p->errstream.clear();});
			auto first=mangled.begin(), last=mangled.end();
			if(demangler.second->parse(first, last, p->errstream))
			{
				auto it=p->parsedSymbols.emplace(make_pair(mangled, p->temp));
				ret=&(it.first)->second;
				clearerror.dismiss();
			}
			else
			{
				auto it=p->failedParsedSymbols.emplace(make_pair(mangled, make_pair(p->temp, p->errstream.str())));
				ret=&(it.first)->second.first;
				success=false;
			}
			string namespace_;
			if(!p->temp.dependents.empty())
			{
				for(const auto &dependent : p->temp.dependents)
					namespace_.append(dependent->name).append("::");
				namespace_.resize(namespace_.size()-2);
			}
			p->namespaces.emplace(make_pair(namespace_, mangled));
			break;
		}
	if(!found)
	{
		// Probably a C symbol, so assume it's a function (we can't tell if it's a variable)
		p->parsedSymbols[mangled]=SymbolType(SymbolTypeQualifier::None, SymbolTypeType::Function, mangled);
	}
	return make_pair(ret, success);
}



} // namespace
