/* Int128_256.hpp
(C) 2013 Niall Douglas http://www.nedprod.com
Created: Feb 2013

AVX2 accelerated 256 bit integer implementation. Falls back to SSE2/NEON 128 bit integers, or even char[32] if needed.

Literally combines cityhash 128 bit and spookyhash 128 bit to make a 256 bit hash
which isn't fast, but it's the fastest reasonably good 256 bit hash I can make quickly.
*/

#include "Int128_256.hpp"
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif
#include "hashes/cityhash/src/city.cc"
#include "hashes/spookyhash/SpookyV2.cpp"
#ifdef _MSC_VER
#pragma warning(pop)
#endif
//#include <omp.h>

#if ALLOW_UNALIGNED_READS
#error ALLOW_UNALIGNED_READS needs to be zero for ARM compatibility
#endif

namespace NiallsCPP11Utilities {

void Hash128::AddTo(const char *data, size_t length)
{
	uint64 *spookyhash=const_cast<unsigned long long *>(asLongLongs());
	SpookyHash::Hash128(data, length, spookyhash, spookyhash+1);
}

void Hash256::AddTo(const char *data, size_t length)
{
	uint64 *spookyhash=const_cast<unsigned long long *>(asLongLongs());
	uint128 cityhash=*(uint128 *)(asLongLongs()+2);
#pragma omp parallel for if(length>=1024)
	for(int n=0; n<2; n++)
	{
		if(!n)
			SpookyHash::Hash128(data, length, spookyhash, spookyhash+1);
		else
			cityhash=CityHash128WithSeed(data, length, cityhash);
	}
	*(uint128 *)(asLongLongs()+2)=cityhash;
}

} // namespace
