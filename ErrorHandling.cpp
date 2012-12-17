/* NiallsCPP11Utilities
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#define _CRT_SECURE_NO_WARNINGS

#include "ErrorHandling.hpp"
#include <locale>
#include <codecvt>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

namespace NiallsCPP11Utilities {

using namespace std;

void int_throwWinError(const char *file, const char *function, int lineno, unsigned code, const std::string *filename)
{
	DWORD len;
	TCHAR buffer[1024];
	len=FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, code, 0, buffer, sizeof(buffer)/sizeof(TCHAR), 0);
	// Remove annoying CRLF at end of message sometimes
	while(10==buffer[len-1])
	{
		buffer[len-1]=0;
		len--;
		if(13==buffer[len-1])
		{
			buffer[len-1]=0;
			len--;
		}
	}
	wstring_convert<codecvt_utf8_utf16<TCHAR>, TCHAR> UTF16toUTF8;
	string errstr(UTF16toUTF8.to_bytes(buffer, buffer+len));
	errstr.append(" ("+to_string(code)+") in '"+string(file)+"':"+string(function)+":"+to_string(lineno));
	if(ERROR_FILE_NOT_FOUND==code || ERROR_PATH_NOT_FOUND==code)
	{
		errstr="File '"+*filename+"' not found [Host OS Error: "+errstr+"]";
		throw ios_base::failure(errstr);
	}
	else if(ERROR_ACCESS_DENIED==code || ERROR_EA_ACCESS_DENIED==code)
	{
		errstr="Access to '"+*filename+"' denied [Host OS Error: "+errstr+"]";
		throw ios_base::failure(errstr);
	}
	else if(ERROR_NO_DATA==code || ERROR_BROKEN_PIPE==code || ERROR_PIPE_NOT_CONNECTED==code || ERROR_PIPE_LISTENING==code)
	{
		throw ios_base::failure(errstr);
	}
	else
	{
		throw ios_base::failure(errstr);
	}
}

} // namespace

#endif

namespace NiallsCPP11Utilities {

using namespace std;

void int_throwOSError(const char *file, const char *function, int lineno, int code, const std::string *filename)
{
	/*if(EINTR==code && QThread::current()->isBeingCancelled())
	{	*//* Some POSIX implementation have badly written pthread support which unpredictably returns
		an interrupted system call error rather than actually cancelling the thread. */
		/*fxmessage("WARNING: Your pthread implementation caused an interrupted system call error rather than properly cancelling a thread. You should report this to your libc maintainer!\n");
		QThread::current()->checkForTerminate();
	}*/
	string errstr(strerror(code));
	errstr.append(" ("+to_string(code)+") in '"+string(file)+"':"+string(function)+":"+to_string(lineno));
	if(ENOENT==code || ENOTDIR==code)
	{
		errstr="File '"+*filename+"' not found [Host OS Error: "+errstr+"]";
		throw ios_base::failure(errstr);
	}
	else if(EACCES==code)
	{
		errstr="Access to '"+*filename+"' denied [Host OS Error: "+errstr+"]";
		throw ios_base::failure(errstr);
	}
	else
	{
		throw ios_base::failure(errstr);
	}
}

} // namespace
