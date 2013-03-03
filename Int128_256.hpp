/* Int128_256.hpp
(C) 2013 Niall Douglas http://www.nedprod.com
Created: Feb 2013

AVX2 accelerated 256 bit integer implementation. Falls back to SSE2/NEON 128 bit integers, or even char[32] if needed.
*/

#ifndef NIALLSINT256_H
#define NIALLSINT256_H

/*! \file Int128_256.hpp
\brief Provides the Int128 and Int256 hardware accelerated types
*/

#include "NiallsCPP11Utilities.hpp"
#include <cstring>
#include <exception>
#include <string>

/*! \def HAVE_M256
\brief Turns on support for the __m256i hardware accelerated type
*/
#ifndef HAVE_M256
#ifdef _MSC_VER
#if _M_IX86_FP>=3 || defined(__AVX2__)
#define HAVE_M256 1
#include <immintrin.h>
#endif
#elif defined(__GNUC__)
#if defined(__AVX2__)
#define HAVE_M256 1
#include <immintrin.h>
#endif
#endif
#endif

/*! \def HAVE_M128
\brief Turns on support for the __m128i hardware accelerated type
*/
#ifndef HAVE_M128
#ifdef _MSC_VER
#if _M_IX86_FP>=2 || defined(_M_AMD64) || _M_ARM_FP>=40
#define HAVE_M128 1
#include <emmintrin.h>
#endif
#elif defined(__GNUC__)
#if defined(__SSE2__) || defined(__ARM_NEON__)
#define HAVE_M128 1
#include <emmintrin.h>
#endif
#endif
#endif

#ifndef HAVE_M128
#define HAVE_M128 0
#endif
#ifndef HAVE_M256
#define HAVE_M256 0
#endif

namespace NiallsCPP11Utilities {

/*! \class Int128
\brief Declares a 128 bit SSE2/NEON compliant container. WILL throw exception if initialised unaligned.
*/
class NIALLSCPP11UTILITIES_API TYPEALIGNMENT(16) Int128
{
	union
	{
		char asBytes[16];
		unsigned long long asLongLongs[2];
		size_t asSize_t;
#if HAVE_M128
		__m128i asM128;
#endif
	} mydata;
	void int_testAlignment() const
	{
#ifndef NDEBUG
		if(((size_t)this) & 15) throw std::runtime_error("This object must be aligned to 16 bytes in memory!");
#endif
	}
	static char int_tohex(int x)
	{
		return (char)((x>9) ? 'a'+x-10 : '0'+x);
	}
public:
	//! Constructs an empty hash
	Int128() { int_testAlignment(); memset(this, 0, sizeof(*this)); }
#if HAVE_M128
	Int128(const Int128 &o) { int_testAlignment(); mydata.asM128=_mm_load_si128(&o.mydata.asM128); }
	Int128 &operator=(const Int128 &o) { mydata.asM128=_mm_load_si128(&o.mydata.asM128); return *this; }
	explicit Int128(const char *bytes) { int_testAlignment(); mydata.asM128=_mm_loadu_si128((const __m128i *) bytes); }
	bool operator==(const Int128 &o) const
	{
		__m128i result;
		result=_mm_cmpeq_epi32(mydata.asM128, o.mydata.asM128);
		unsigned r=_mm_movemask_epi8(result);
		return r==0xffff;
	}
	bool operator>(const Int128 &o) const
	{
		__m128i result[2];
		result[0]=_mm_cmpgt_epi32(mydata.asM128, o.mydata.asM128);
		result[1]=_mm_cmpgt_epi32(o.mydata.asM128, mydata.asM128);
		unsigned r[2];
		r[0]=_mm_movemask_epi8(result[0]);
		r[1]=_mm_movemask_epi8(result[1]);
		return r[0]>r[1];
	}
#else
	//! Copies another hash
	Int128(const Int128 &o) { int_testAlignment(); memcpy(mydata.asBytes, o.mydata.asBytes, sizeof(mydata.asBytes)); }
	//! Copies another hash
	Int128 &operator=(const Int128 &o) { memcpy(mydata.asBytes, o.mydata.asBytes, sizeof(mydata.asBytes)); return *this; }
	//! Constructs a hash from unaligned data
	explicit Int128(const char *bytes) { int_testAlignment(); memcpy(mydata.asBytes, bytes, sizeof(mydata.asBytes)); }
	bool operator==(const Int128 &o) const { return !memcmp(mydata.asBytes, o.mydata.asBytes, sizeof(mydata.asBytes)); }
	bool operator>(const Int128 &o) const { return memcmp(mydata.asBytes, o.mydata.asBytes, sizeof(mydata.asBytes))>0; }
#endif
	Int128(Int128 &&o) { *this=o; }
	Int128 &operator=(Int128 &&o) { *this=o; return *this; }
	bool operator!=(const Int128 &o) const { return !(*this==o); }
	bool operator<(const Int128 &o) const { return o>*this; }
	bool operator>=(const Int128 &o) const { return !(o>*this); }
	bool operator<=(const Int128 &o) const { return !(*this>o); }
	//! Returns the hash as bytes
	const char *asBytes() const { return mydata.asBytes; }
	//! Returns the hash as long longs
	const unsigned long long *asLongLongs() const { return mydata.asLongLongs; }
	//! Returns the front of the hash as a size_t
	size_t asSize_t() const { return mydata.asSize_t; }
	//! Returns the hash as a 64 character hexadecimal string
	std::string asHexString() const
	{
		std::string ret(32, '0');
		for(int i=0; i<16; i++)
		{
			unsigned char c=mydata.asBytes[i];
			ret[i*2]=int_tohex(c>>4);
			ret[i*2+1]=int_tohex(c&15);
		}
		return ret;
	}
};

/*! \class Int256
\brief Declares a 256 bit AVX2/SSE2/NEON compliant container. WILL throw exception if initialised unaligned.
*/
class NIALLSCPP11UTILITIES_API TYPEALIGNMENT(32) Int256
{
	union
	{
		char asBytes[32];
		unsigned long long asLongLongs[4];
		size_t asSize_t;
#if HAVE_M128
		__m128i asM128s[2];
#endif
#if HAVE_M256
		__m256i asM256;
#endif
	} mydata;
	void int_testAlignment() const
	{
#ifndef NDEBUG
		if(((size_t)this) & 31) throw std::runtime_error("This object must be aligned to 32 bytes in memory!");
#endif
	}
	static char int_tohex(int x)
	{
		return (char)((x>9) ? 'a'+x-10 : '0'+x);
	}
public:
	//! Constructs an empty hash
	Int256() { int_testAlignment(); memset(this, 0, sizeof(*this)); }
#if HAVE_M256
	Int256(const Int256 &o) { int_testAlignment(); mydata.asM256=_mm256_load_si256(&o.mydata.asM256); }
	Int256 &operator=(const Int256 &o) { mydata.asM256=_mm256_load_si256(&o.mydata.asM256); return *this; }
	explicit Int256(const char *bytes) { int_testAlignment(); mydata.asM256=_mm256_loadu_si256((const __m256i *) bytes); }
	bool operator==(const Int256 &o) const
	{
		__m256i result=_mm256_cmpeq_epi64(mydata.asM256, o.mydata.asM256);
		return !(~_mm256_movemask_epi8(result));
	}
	bool operator>(const Int256 &o) const
	{
		__m256i result[2];
		result[0]=_mm256_cmpgt_epi64(mydata.asM256, o.mydata.asM256);
		result[1]=_mm256_cmpgt_epi64(o.mydata.asM256, mydata.asM256);
		unsigned r[2];
		r[0]=_mm256_movemask_epi8(result[0]);
		r[1]=_mm256_movemask_epi8(result[1]);
		return r[0]>r[1];
	}
#elif HAVE_M128
	Int256(const Int256 &o) { int_testAlignment(); mydata.asM128s[0]=_mm_load_si128(&o.mydata.asM128s[0]); mydata.asM128s[1]=_mm_load_si128(&o.mydata.asM128s[1]); }
	Int256 &operator=(const Int256 &o) { mydata.asM128s[0]=_mm_load_si128(&o.mydata.asM128s[0]); mydata.asM128s[1]=_mm_load_si128(&o.mydata.asM128s[1]); return *this; }
	explicit Int256(const char *bytes) { int_testAlignment(); mydata.asM128s[0]=_mm_loadu_si128((const __m128i *) bytes); mydata.asM128s[1]=_mm_loadu_si128((const __m128i *)(bytes+16)); }
	bool operator==(const Int256 &o) const
	{
		__m128i result[2];
		result[0]=_mm_cmpeq_epi32(mydata.asM128s[0], o.mydata.asM128s[0]);
		result[1]=_mm_cmpeq_epi32(mydata.asM128s[1], o.mydata.asM128s[1]);
		unsigned r=_mm_movemask_epi8(result[0]);
		r|=_mm_movemask_epi8(result[1])<<16;
		return !(~r);
	}
	bool operator>(const Int256 &o) const
	{
		__m128i result[4];
		result[0]=_mm_cmpgt_epi32(mydata.asM128s[0], o.mydata.asM128s[0]);
		result[1]=_mm_cmpgt_epi32(mydata.asM128s[1], o.mydata.asM128s[1]);
		result[2]=_mm_cmpgt_epi32(o.mydata.asM128s[0], mydata.asM128s[0]);
		result[3]=_mm_cmpgt_epi32(o.mydata.asM128s[1], mydata.asM128s[1]);
		unsigned r[2];
		r[0]=_mm_movemask_epi8(result[0])|(_mm_movemask_epi8(result[1])<<16);
		r[1]=_mm_movemask_epi8(result[2])|(_mm_movemask_epi8(result[3])<<16);
		return r[0]>r[1];
	}
#else
	//! Copies another hash
	Int256(const Int256 &o) { int_testAlignment(); memcpy(mydata.asBytes, o.mydata.asBytes, sizeof(mydata.asBytes)); }
	//! Copies another hash
	Int256 &operator=(const Int256 &o) { memcpy(mydata.asBytes, o.mydata.asBytes, sizeof(mydata.asBytes)); return *this; }
	//! Constructs a hash from unaligned data
	explicit Int256(const char *bytes) { int_testAlignment(); memcpy(mydata.asBytes, bytes, sizeof(mydata.asBytes)); }
	bool operator==(const Int256 &o) const { return !memcmp(mydata.asBytes, o.mydata.asBytes, sizeof(mydata.asBytes)); }
	bool operator>(const Int256 &o) const { return memcmp(mydata.asBytes, o.mydata.asBytes, sizeof(mydata.asBytes))>0; }
#endif
	Int256(Int256 &&o) { *this=o; }
	Int256 &operator=(Int256 &&o) { *this=o; return *this; }
	bool operator!=(const Int256 &o) const { return !(*this==o); }
	bool operator<(const Int256 &o) const { return o>*this; }
	bool operator>=(const Int256 &o) const { return !(o>*this); }
	bool operator<=(const Int256 &o) const { return !(*this>o); }
	//! Returns the hash as bytes
	const char *asBytes() const { return mydata.asBytes; }
	//! Returns the hash as long longs
	const unsigned long long *asLongLongs() const { return mydata.asLongLongs; }
	//! Returns the front of the hash as a size_t
	size_t asSize_t() const { return mydata.asSize_t; }
	//! Returns the hash as a 64 character hexadecimal string
	std::string asHexString() const
	{
		std::string ret(64, '0');
		for(int i=0; i<32; i++)
		{
			unsigned char c=mydata.asBytes[i];
			ret[i*2]=int_tohex(c>>4);
			ret[i*2+1]=int_tohex(c&15);
		}
		return ret;
	}
};

/*! \class Hash128
\brief Provides a 128 bit hash

To use this you must compile Int128_256.cpp.
*/
class NIALLSCPP11UTILITIES_API Hash128 : public Int128
{
public:
	//! Adds data to this hash.
	void AddTo(const char *data, size_t length);
};

/*! \class Hash256
\brief Provides a 256 bit hash

To use this you must compile Int128_256.cpp.
*/
class NIALLSCPP11UTILITIES_API Hash256 : public Int256
{
public:
	//! Adds data to this hash.
	void AddTo(const char *data, size_t length);
};

} //namespace

namespace std
{
	//! Defines a hash for a Int128 (simply truncates)
	template<> struct hash<NiallsCPP11Utilities::Int128>
	{
		size_t operator()(const NiallsCPP11Utilities::Int128 &v) const
		{
			return v.asSize_t();
		}
	};
	//! Defines a hash for a Hash256 (simply truncates)
	template<> struct hash<NiallsCPP11Utilities::Int256>
	{
		size_t operator()(const NiallsCPP11Utilities::Int256 &v) const
		{
			return v.asSize_t();
		}
	};
	//! Stop the default std::vector<> doing unaligned storage
	template<> class vector<NiallsCPP11Utilities::Int256, allocator<NiallsCPP11Utilities::Int256>> : public vector<NiallsCPP11Utilities::Int256, NiallsCPP11Utilities::aligned_allocator<NiallsCPP11Utilities::Int256>>
	{
		typedef vector<NiallsCPP11Utilities::Int256, NiallsCPP11Utilities::aligned_allocator<NiallsCPP11Utilities::Int256>> Base;
	public:
		explicit vector (const allocator_type& alloc = allocator_type()) : Base(alloc) { }
		explicit vector (size_type n) : Base(n) { }
		vector (size_type n, const value_type& val,
			const allocator_type& alloc = allocator_type()) : Base(n, val, alloc) { }
		template <class InputIterator> vector (InputIterator first, InputIterator last,
			const allocator_type& alloc = allocator_type()) : Base(first, last, alloc) { }
		vector (const vector& x) : Base(x) { }
		//vector (const vector& x, const allocator_type& alloc) : Base(x, alloc) { }
		vector (vector&& x) : Base(std::move(x)) { }
		//vector (vector&& x, const allocator_type& alloc) : Base(std::move(x), alloc) { }
#if defined(_MSC_VER) && _MSC_VER>1700
		vector (initializer_list<value_type> il, const allocator_type& alloc = allocator_type()) : Base(il, alloc) { }
#endif
	};
}

#endif
