/* NiallsCPP11Utilities
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#include "NiallsCPP11Utilities.hpp"

#if defined(_MSC_VER) && _MSC_VER<=1700 && !defined(__func__)
#define __func__ __FUNCTION__
#endif

#ifdef EXCEPTION_DISABLESOURCEINFO
#define EXCEPTION_FILE(p) (const char *) 0
#define EXCEPTION_FUNCTION(p) (const char *) 0
#define EXCEPTION_LINE(p) 0
#else
#define EXCEPTION_FILE(p) __FILE__
#define EXCEPTION_FUNCTION(p) __func__
#define EXCEPTION_LINE(p) __LINE__
#endif

namespace NiallsCPP11Utilities
{
#ifdef WIN32
	extern NIALLSCPP11UTILITIES_API void int_throwWinError(const char *file, const char *function, int lineno, unsigned code, const std::string *filename=0);
	extern "C" unsigned __stdcall GetLastError();
#define ERRGWIN(code)				{ NiallsCPP11Utilities::int_throwWinError(EXCEPTION_FILE(0), EXCEPTION_FUNCTION(0), EXCEPTION_LINE(0), code); }
#define ERRGWINFN(code, filename)	{ NiallsCPP11Utilities::int_throwWinError(EXCEPTION_FILE(0), EXCEPTION_FUNCTION(0), EXCEPTION_LINE(0), code, &(filename)); }
#define ERRHWIN(exp)				{ unsigned __errcode=(unsigned)(exp); if(!__errcode) ERRGWIN(GetLastError()); }
#define ERRHWINFN(exp, filename)	{ unsigned __errcode=(unsigned)(exp); if(!__errcode) ERRGWINFN(GetLastError(), filename); }
#endif
}


