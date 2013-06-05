/* NiallsCPP11Utilities
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#include "NiallsCPP11Utilities.hpp"
#include <atomic>

namespace NiallsCPP11Utilities {

namespace Impl {
    // This evil use of zero initialised byte storage to keep a weak_ptr
    // is to work around freeing the static type registry on static deinit of
    // last user which is of course highly unpredictable within the order of deinit
    //
    // It makes the BIG assumption that as weak_ptr's constructor is constexpr
    // that implies the contents are almost certainly pointers which, luckily,
    // means a zero fill init is the same as construction.
    static std::atomic<int> static_type_registry_storage_lock;
    static char static_type_registry_storage[sizeof(std::weak_ptr<ErasedTypeRegistryMapType>)];
	std::shared_ptr<ErasedTypeRegistryMapType> get_static_type_registry_storage()
    {
        std::weak_ptr<ErasedTypeRegistryMapType> *s=(std::weak_ptr<ErasedTypeRegistryMapType> *) static_type_registry_storage;
        // Try atomically locking in the weak_ptr, if successful all is good.
        std::shared_ptr<ErasedTypeRegistryMapType> ret(s->lock());
        if(ret)
            return ret;
        // Lock, construct and weakly link.
		int clear=0;
        while(static_type_registry_storage_lock.compare_exchange_strong(clear, 1));
        auto unlock=Undoer([]{static_type_registry_storage_lock.store(0);});
        *s=ret=std::make_shared<ErasedTypeRegistryMapType>();
        return ret;
    }
}

} // namespace
