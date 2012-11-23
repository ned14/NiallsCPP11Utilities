/* NiallsCPP11Utilities
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#ifndef NIALLSCPP11UTILITIES_H
#define NIALLSCPP11UTILITIES_H

/*! \file NiallsCPP11Utilities.hpp
\brief Declares Niall's useful C++ 11 utilities
*/

/*! \mainpage

\warning You'll definitely need a fairly compliant C++ 11 compiler for this library to work

Tested on Visual Studio 2012 Nov CTP (the one with variadic template support)
*/

#include <vector>
#include <memory>
#include <type_traits>
#include <functional>
#include <algorithm>

#if defined(_MSC_VER) && _MSC_VER<=1700 && !defined(noexcept)
#define noexcept throw()
#endif

//! \define DEFINES Defines RETURNS to automatically figure out your return type
#ifndef RETURNS
#define RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }
#endif

//! \namespace NiallsCPP11Utilities Where Niall's useful C++ 11 utilities live
namespace NiallsCPP11Utilities {

namespace Impl {
	template<typename T, bool iscomparable> struct is_nullptr { bool operator()(T c) const noexcept { return c!=nullptr; } };
	template<typename T> struct is_nullptr<T, false> { bool operator()(T c) const noexcept { return true; } };
}
//! Compile-time safe detector of if \em v is nullptr (can cope with non-pointer convertibles)
template<typename T> bool is_nullptr(T &&v) noexcept { return Impl::is_nullptr<T, std::is_trivially_constructible<void *, T>::value>()(std::forward<T>(v)); }

template<typename callable> class UndoerImpl
{
	callable undoer;
	bool dismissed;

#if !defined(_MSC_VER) || _MSC_VER>1700
	UndoerImpl() = delete;
#endif
	UndoerImpl(callable &&c) : undoer(std::move(c)), dismissed(false) { }
public:
	template<typename callable> friend UndoerImpl<callable> &&Undoer(callable &&c);
	~UndoerImpl() { if(!dismissed && !is_nullptr(undoer)) undoer(); }
	//! Dismisses the Undoer
	void dismiss(bool d=true) { dismissed=d; }
	//! Undismisses the Undoer
	void undismiss(bool d=true) { dismissed=!d; }
};


/*! \brief Alexandrescu style rollbacks, a la C++ 11.

Example of usage:
\code
auto resetpos=Undoer([&s]() { s.seekg(0, std::ios::beg); });
...
resetpos.dismiss();
\endcode
*/
template<typename callable> UndoerImpl<callable> &&Undoer(callable &&c)
{
	return std::move(UndoerImpl<callable>(std::move(c)));
}

namespace Impl {
	template<class _registry, class _type, class _containertype> struct TypeRegistryStorage
	{
		typedef _registry registry;
		typedef _type type;
		typedef _containertype containertype;
		static containertype *&registryStorage()
		{
			static containertype *_registryStorage;
			return _registryStorage;
		}
		static void RegisterData(type &&c)
		{
			if(!registryStorage()) registryStorage()=new containertype;
			registryStorage()->push_back(std::move(c));
		}
		static void UnregisterData(type &&c)
		{
			auto r=registryStorage();
			// Quick optimisation for tail pop to avoid a search
			if(*r->rbegin()==c)
				r->erase(--r->end());
			else
				r->erase(std::remove(r->begin(), r->end(), std::forward<type>(c)), r->end());
			if(r->empty())
			{
				delete r;
				registryStorage()=nullptr;
			}
		}
	};
}
/*! \brief An iterable, statically stored registry of items associated with a type

Only one of these ever exists in the process, so you can always iterate like this:
\code
typedef TypeRegistry<Foo, std::unique_ptr<Foo>(*)()> MakeablesRegistry;
for(auto n : TypeRegistry<MakeablesRegistry>())
   ...
\endcode

\sa NiallsCPP11Utilities::RegisterData(), NiallsCPP11Utilities::AutoDataRegistration()
*/
template<class _registry, class _type, class _containertype=std::vector<_type>> struct TypeRegistry
{
private:
	_containertype &__me() { return *Impl::TypeRegistryStorage<_registry, _type, _containertype>::registryStorage(); }
	const _containertype &__me() const { return *Impl::TypeRegistryStorage<_registry, _type, _containertype>::registryStorage(); }
public:
	operator _containertype &() { return __me(); }
	operator const _containertype &() const { return __me(); }
	typename _containertype::iterator &&begin() { return std::move(__me().begin()); }
	typename _containertype::iterator &&end() { return std::move(__me().end()); }
};

namespace Impl {
	template<class typeregistry> struct RegisterDataImpl;
	template<class _registry, class _type, class _containertype> struct RegisterDataImpl<TypeRegistry<_registry, _type, _containertype>>
	{
		typedef _registry registry;
		typedef _type type;
		typedef _containertype containertype;
		static void Do(_type &&v)
		{
			Impl::TypeRegistryStorage<_registry, _type, _containertype>::RegisterData(std::forward<_type>(v));
		}
	};
	template<class typeregistry> struct UnregisterDataImpl;
	template<class _registry, class _type, class _containertype> struct UnregisterDataImpl<TypeRegistry<_registry, _type, _containertype>>
	{
		typedef _registry registry;
		typedef _type type;
		typedef _containertype containertype;
		static void Do(_type &&v)
		{
			Impl::TypeRegistryStorage<_registry, _type, _containertype>::UnregisterData(std::forward<_type>(v));
		}
	};
}
//! Registers a piece of data with the specified type registry
template<class typeregistry> void RegisterData(typename Impl::RegisterDataImpl<typeregistry>::type &&v)
{
	Impl::RegisterDataImpl<typeregistry>::Do(std::forward<typename Impl::RegisterDataImpl<typeregistry>::type>(v));
}
//! Unregisters a piece of data with the specified type registry
template<class typeregistry> void UnregisterData(typename Impl::UnregisterDataImpl<typeregistry>::type &&v)
{
	Impl::UnregisterDataImpl<typeregistry>::Do(std::forward<typename Impl::UnregisterDataImpl<typeregistry>::type>(v));
}

namespace Impl {
	template<class _typeregistry> struct DataRegistration;
	template<class _registry, class _type, class _containertype> struct DataRegistration<TypeRegistry<_registry, _type, _containertype>>
	{
		typedef TypeRegistry<_registry, _type, _containertype> _typeregistry;
		DataRegistration(_type &&_c) : c(std::move(_c)) { RegisterData<_typeregistry>(std::forward<_type>(c)); }
		~DataRegistration() { UnregisterData<_typeregistry>(std::forward<_type>(c)); }
	private:
		_type c;
	};
}
/*! \brief Auto registers a data item with a type registry. Typically used at static init/deinit time.

Per DLL object:
\code
typedef TypeRegistry<Foo, std::unique_ptr<Foo>(*)()> MakeablesRegistry;
static auto reg=AutoDataRegistration<MakeablesRegistry>(&Goo::Make);
\endcode
This registers the Goo::Make callable with the registry MakeablesRegistry during DLL load. It also unregisters during DLL unload.

You now have a registry of static Make() methods associated with type MakeablesRegistry. To iterate:
\code
for(auto n : TypeRegistry<MakeablesRegistry>())
   ...
\endcode
*/
template<class _typeregistry, class _type> Impl::DataRegistration<_typeregistry> AutoDataRegistration(_type &&c)
{
	return Impl::DataRegistration<_typeregistry>(std::move(c));
}

} // namespace

#endif
