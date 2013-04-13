/* std_filesystem.hpp
Gets a std::filesystem implementation from somewhere
(C) 2013 Niall Douglas http://www.nedprod.com/
File Created: Mar 2013
*/

#ifndef STD_FILESYSTEM_FROM_SOMEWHERE
#define STD_FILESYSTEM_FROM_SOMEWHERE
#if __cplusplus > 201103L
#include <filesystem>
#elif defined(HAVE_TR2_FILESYSTEM)
#include <tr2/filesystem>
namespace std { namespace filesystem { using namespace tr2::filesystem; } }
#else
#include "boost/filesystem/convenience.hpp"
namespace std {
	namespace filesystem { using namespace boost::filesystem; using boost::filesystem::path; }
	template<class T> struct hash;
	template<> struct hash<boost::filesystem::path>
	{
		size_t operator()(const boost::filesystem::path& p) const
		{
			return boost::filesystem::hash_value(p);
		}
	};
}
#endif
#endif
