/* NiallsCPP11Utilities
(C) 2012 Niall Douglas http://www.nedprod.com/
File Created: Nov 2012
*/

#include "NiallsCPP11Utilities.hpp"
#include "ErrorHandling.hpp"
#include <locale>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <Psapi.h>
#include <tchar.h>
#elif defined(__APPLE__)
  #include <sys/sysctl.h>
  #include <sys/vmmeter.h>
  #include <sys/param.h>
  #include <sys/mount.h>
  #include <mach/mach.h>
  #include <mach/vm_map.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#endif

namespace NiallsCPP11Utilities {

using namespace std;

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

std::map<size_t, MappedFileInfo> MappedFileInfo::mappedFiles()
{
	map<size_t, MappedFileInfo> list;
	MappedFileInfo bi;
#ifdef WIN32
	TCHAR rawbuffer[4096];
	MEMORY_BASIC_INFORMATION mbi;
	HMODULE handles[1024];
	DWORD needed;
	wstring_convert<codecvt_utf8_utf16<TCHAR>, TCHAR> UTF16toUTF8;
	ERRHWIN(EnumProcessModules(GetCurrentProcess(), handles, sizeof(handles), &needed));
	for(DWORD n=0; n<needed/sizeof(HMODULE); n++)
	{
		MODULEINFO mi={0};
		ERRHWIN(GetModuleInformation(GetCurrentProcess(), handles[n], &mi, sizeof(mi)));
		bi.startaddr=(size_t) mi.lpBaseOfDll;
		bi.length=(size_t) mi.SizeOfImage;
		bi.endaddr=bi.startaddr+bi.length;
		VirtualQuery(mi.lpBaseOfDll, &mbi, sizeof(mbi));
		bi.read=(mbi.AllocationProtect & (PAGE_READONLY|PAGE_EXECUTE_READ))!=0;
		bi.write=(mbi.AllocationProtect & (PAGE_READWRITE|PAGE_EXECUTE_READWRITE))!=0;
		if(bi.write) bi.read=true;
		bi.execute=(mbi.AllocationProtect & (PAGE_EXECUTE|PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE_WRITECOPY))!=0;
		if(bi.execute) bi.read=true;
		bi.copyonwrite=(mbi.AllocationProtect & (PAGE_WRITECOPY|PAGE_EXECUTE_WRITECOPY))!=0;
		bi.offset=0;
		ERRHWIN(GetModuleFileNameEx(GetCurrentProcess(), handles[n], rawbuffer, sizeof(rawbuffer)));
		bi.path=UTF16toUTF8.to_bytes(rawbuffer);
		list[bi.startaddr]=bi;
	}
	// Unfortunately the above is only shared libraries and doesn't include mapped sections
	size_t addr=0;
	while(VirtualQuery((LPVOID) addr, &mbi, sizeof(mbi)))
	{
		if(MEM_FREE!=mbi.State)
		{
			rawbuffer[GetMappedFileName(GetCurrentProcess(), mbi.AllocationBase, rawbuffer, sizeof(rawbuffer))]=0;
			if(rawbuffer[0])
			{
				bi.startaddr=addr;
				bi.length=(size_t) mbi.RegionSize;
				bi.endaddr=bi.startaddr+bi.length;
				bi.read=(mbi.AllocationProtect & (PAGE_READONLY|PAGE_EXECUTE_READ))!=0;
				bi.write=(mbi.AllocationProtect & (PAGE_READWRITE|PAGE_EXECUTE_READWRITE))!=0;
				if(bi.write) bi.read=true;
				bi.execute=(mbi.AllocationProtect & (PAGE_EXECUTE|PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE_WRITECOPY))!=0;
				if(bi.execute) bi.read=true;
				bi.copyonwrite=(mbi.AllocationProtect & (PAGE_WRITECOPY|PAGE_EXECUTE_WRITECOPY))!=0;
				bi.offset=0;
				bi.path=UTF16toUTF8.to_bytes(rawbuffer);
				list[bi.startaddr]=bi;
			}
		}
		addr+=mbi.RegionSize;
	}
	// Now we have to fix up any paths with the format \Device\HarddiskVolume* to paths CreateFile() can use
	vector<TCHAR> drives;
	do
	{
		drives.resize(GetLogicalDriveStrings(0, NULL)+1);
	} while(!GetLogicalDriveStrings((DWORD) drives.size(), drives.data()));
	unordered_map<string, string> deviceToDrive;
	for(TCHAR *d=drives.data(), *e=_tcschr(d, 0); *d!=0; d=e+1, e=_tcschr(d, 0))
	{
		e[-1]=0;
		QueryDosDevice(d, rawbuffer, sizeof(rawbuffer)/sizeof(TCHAR));
		string drivespec=UTF16toUTF8.to_bytes(d), device=UTF16toUTF8.to_bytes(rawbuffer);
		deviceToDrive[device]=drivespec;
	}
	for(auto &item : list)
	{
		string &path=item.second.path;
		if(!path.empty())
		{
			string front=path.substr(0, path.find_first_of('\\', path.find_first_of('\\', path.find_first_of('\\')+1)+1));
			if(deviceToDrive.count(front)>0)
				path=path.replace(0, front.size(), deviceToDrive[front]);
		}
	}
#endif
#ifndef WIN32
#ifdef __linux__
	int fh;
	ERRHOS(fh=open("/proc/self/maps", O_RDONLY));
	auto unfh=Undoer([fh] { close(fh); });
	char rawbuffer[16384];
	size_t readed=0, laststartaddr=0;
	while((readed=::read(fh, rawbuffer+readed, sizeof(rawbuffer)-readed)))
	{
		char *end=rawbuffer+readed;
		*end=0;
		for(; *end!=10; --end);
		for(char *ptr=rawbuffer; ptr<end; ptr=strchr(ptr, 10)+1)
		{	// Format is hexstart-_hexend_ rwxp hexofset dd:dd _inode     /path...
			unsigned long startaddr, endaddr, offset;
			char _r, _w, _x, _p;
			int t1, t2;
			unsigned int pid;
			char path[2048];
			// 00c37000-00c39000 r--p 0019f000 fd:04 1050887                            /lib/i386-linux-gnu/libc-2.15.so
			int len=sscanf(ptr, "%lx-%lx %c%c%c%c %lx %x:%x %u %s", &startaddr, &endaddr,
				&_r, &_w, &_x, &_p, &offset, &t1, &t2, &pid, path);
			bi.startaddr=(size_t) startaddr;
			bi.endaddr=(size_t) endaddr;
			bi.length=bi.endaddr-bi.startaddr;
			bi.read=('r'==_r);
			bi.write=('w'==_w);
			bi.execute=('x'==_x);
			bi.copyonwrite=('p'==_p);
			bi.offset=(size_t) offset;
			bi.path=path;
			trim(bi.path);
			if(!list.empty())
			{	// Linux doesn't say RAM sections belong to DLL
				MappedFileInfo &bbi=list[laststartaddr];
				if(bbi.endaddr==bi.startaddr && bi.path.empty()
					&& bi.read==bbi.read && bi.write==bbi.write && bi.execute==bbi.execute && bi.copyonwrite==bbi.copyonwrite)
				{	// Collapse
					bbi.endaddr=bi.endaddr;
					continue;
				}
			}
			list[bi.startaddr]=bi;
			laststartaddr=bi.startaddr;
		}
		end++;
		readed-=end-rawbuffer;
		memmove(rawbuffer, end, readed);
	}
#endif
#if defined(__FreeBSD__)
	FXString procpath=FXString("/proc/%1/map").arg(FXProcess::id());
	// FreeBSD doesn't like incremental reads according to kernel sources
	char rawbuffer[131072 /* WARNING: This value derived by experimentation */ ];
	char *ptr=rawbuffer, *end;
	{
		QFile fh(procpath, QFile::WantLightQFile());
		if(!fh.exists()) // Probably /proc isn't mounted - let FXProcess take care of that
			return list;
		fh.open(IO_ReadOnly);
		FXuval read=fh.readBlock(rawbuffer, sizeof(rawbuffer)-2);
		assert(read>0 && read<sizeof(rawbuffer)-2);
		end=ptr+read; end[0]=10; end[1]=0;
	}
	// Format is 0xstart 0xend resident privresident [0x]obj rwx refcnt shadcnt 0xflags NCOW|COW NNC|NC type path
	// (or from kernel sources) "0x%lx 0x%lx %d %d %p %s%s%s %d %d 0x%x %s %s %s %s\n"
	while(ptr<end)
	{	// sscanf seems to have thread safety issues on FreeBSD :( - so we parse manually
		long start, end, objptr; char r,w,x; int refcnt, shadcnt, flags;
		char cow[8], nc[8], type[32], *sp; bool ok;

		// Start
		assert('0'==ptr[0] && 'x'==ptr[1]);
		if((sp=strchr(ptr, ' '))) *sp=0; assert(sp);
		bi.startaddr=FXString(ptr+2, sp-ptr-2).toULong(&ok, 16); assert(ok);
		if(sp) ptr=sp+1;

		// End
		assert('0'==ptr[0] && 'x'==ptr[1]);
		if((sp=strchr(ptr, ' '))) *sp=0; assert(sp);
		bi.endaddr=FXString(ptr+2, sp-ptr-2).toULong(&ok, 16); assert(ok);
		if(sp) ptr=sp+1;

		bi.length=bi.endaddr-bi.startaddr;
		sp=strchr(ptr, ' '); assert(sp); if(sp) ptr=sp+1;	// resident
		sp=strchr(ptr, ' '); assert(sp); if(sp) ptr=sp+1;	// privresident
		sp=strchr(ptr, ' '); assert(sp); if(sp) ptr=sp+1;	// obj

		// rwx
		bi.read='r'==ptr[0];
		bi.write='w'==ptr[1];
		bi.execute='x'==ptr[2];
		ptr+=4;

		sp=strchr(ptr, ' '); assert(sp); if(sp) ptr=sp+1;	// refcnt
		sp=strchr(ptr, ' '); assert(sp); if(sp) ptr=sp+1;	// shadcnt
		sp=strchr(ptr, ' '); assert(sp); if(sp) ptr=sp+1;	// flags

		// COW
		//fxmessage("Processing %s\n", FXString(ptr, 32).text());
		assert(!strncmp("NCOW", ptr, 4) || !strncmp("COW", ptr, 3));
		bi.copyonwrite='C'==ptr[0];
		sp=strchr(ptr, ' '); assert(sp); if(sp) ptr=sp+1;

		sp=strchr(ptr, ' '); assert(sp); if(sp) ptr=sp+1;	// NNC
		sp=strchr(ptr, ' '); assert(sp); if(sp) ptr=sp+1;	// type
		bi.offset=0;
		char *nl=strchr(ptr, 10);
		if('-'==ptr[0])
			bi.path.truncate(0);
		else
			bi.path.assign(ptr, nl-ptr);
		list.append(bi);
		if(nl)
			ptr=nl+1;
		else
			break;
	}
	assert(end-ptr<2);
#endif
#ifdef __APPLE__
	mach_port_t self=mach_task_self();
	vm_address_t addr=0;
	vm_size_t size=0;
	vm_region_basic_info_data_64_t basicinfo;
	vm_region_top_info_data_t topinfo;
	mach_msg_type_number_t len=VM_REGION_BASIC_INFO_COUNT_64;
	memory_object_name_t object=0;
	while(KERN_SUCCESS==vm_region_64(self, &addr, &size, VM_REGION_BASIC_INFO_64, (vm_region_info_t) &basicinfo, &len, &object))
	{
		if(object)
			mach_port_deallocate(mach_task_self(), object);
		bi.startaddr=addr;
		bi.length=size;
		bi.endaddr=bi.startaddr+bi.length;
		bi.read=(basicinfo.protection & VM_PROT_READ)!=0;
		bi.write=(basicinfo.protection & VM_PROT_WRITE)!=0;
		bi.execute=(basicinfo.protection & VM_PROT_EXECUTE)!=0;
		bi.offset=basicinfo.offset;
		// Now pull extended attrs
		len=VM_REGION_TOP_INFO_COUNT;
		if(KERN_SUCCESS==vm_region_64(self, &addr, &size, VM_REGION_TOP_INFO, (vm_region_info_t) &topinfo, &len, &object))
		{
			dl_info info;
			if(object)
				mach_port_deallocate(mach_task_self(), object);
			bi.copyonwrite=(topinfo.share_mode==SM_COW);
			if(dladdr((void *) bi.startaddr, &info))
				bi.path=info.dli_fname;
			else
			{
				bi.path.truncate(0);
				if(SM_PRIVATE==topinfo.share_mode)
					bi.path="{ Private region (malloc?) }";
				else if(SM_EMPTY==topinfo.share_mode)
					bi.path="{ Empty region }";
				else if(SM_COW==topinfo.share_mode)
					bi.path="{ Memory mapped file }";
				if(topinfo.obj_id)
					bi.path.append(FXString(" [mach obj_id=0x%1]").arg(topinfo.obj_id, 0, 16));
			}
			list.append(bi);
		}
		addr+=size;
		len=VM_REGION_BASIC_INFO_COUNT_64;
	}
#endif
#endif
	return list;
}

} // namespace
