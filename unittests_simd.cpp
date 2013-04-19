/* NiallsCPP11Utilities SIMD unit tests
(C) 2013 Niall Douglas http://www.nedprod.com/
File Created: April 2013
*/

#define CATCH_CONFIG_RUNNER
#include "NiallsCPP11Utilities.hpp"
#include <fstream>
#include "Int128_256.hpp"
#include "catch.hpp"
#if HAVE_M128
#include "hashes/sha256/sha256-sse.c"
#endif
#if HAVE_NEON128
#include "hashes/sha256/sha256-neon.c"
#endif

using namespace NiallsCPP11Utilities;
using namespace std;

int main (int argc, char * const argv[]) {
    int ret=Catch::Main( argc, argv );
#ifdef _MSC_VER
	printf("Press Return to exit ...\n");
	getchar();
#endif
	return ret;
}

TEST_CASE("SHA256/load", "Tests that SIMD load works")
{
	union { uint32_t a[4]; __m128i b; } x;
	x.b=load_epi32(0x12345678, 0xdeadbeef, 0x98765432, 0xfee1baad);
	CHECK( x.a[0] == 0xfee1baad );
	CHECK( x.a[1] == 0x98765432 );
	CHECK( x.a[2] == 0xdeadbeef );
	CHECK( x.a[3] == 0x12345678 );
}

TEST_CASE("SHA256/store", "Tests that SIMD store works")
{
	union { uint32_t a[4]; __m128i b; } x;
	uint32_t c[4];
	x.b=load_epi32(0x12345678, 0xdeadbeef, 0x98765432, 0xfee1baad);
	store_epi32(x.b, &c[0], &c[1], &c[2], &c[3]);
	CHECK( c[0] == 0x12345678 );
	CHECK( c[1] == 0xdeadbeef );
	CHECK( c[2] == 0x98765432 );
	CHECK( c[3] == 0xfee1baad );
}

TEST_CASE("SHA256/shaload", "Tests that SHA LOAD works")
{
	union { uint32_t a[4]; __m128i b; } x;
	uint32_t d[4]={0x12345678, 0xdeadbeef, 0x98765432, 0xfee1baad};
	const __sha256_block_t *blks[4]={ (__sha256_block_t *)(d+0), (__sha256_block_t *)(d+1), (__sha256_block_t *)(d+2), (__sha256_block_t *)(d+3) };
	x.b=LOAD(blks, 0);
	CHECK( x.a[0] == 0xadbae1fe );
	CHECK( x.a[1] == 0x32547698 );
	CHECK( x.a[2] == 0xefbeadde );
	CHECK( x.a[3] == 0x78563412 );
}

TEST_CASE("SHA256/const", "Tests that SHA CONST works")
{
	union { uint32_t a[4]; __m128i b; } x;
	x.b=SHA256_CONST(4);
	CHECK( x.a[0] == 0x3956c25b );
	CHECK( x.a[1] == 0x3956c25b );
	CHECK( x.a[2] == 0x3956c25b );
	CHECK( x.a[3] == 0x3956c25b );
}

TEST_CASE("SHA256/BIGSIGMA0_256", "Tests that BIGSIGMA0_256 works")
{
	union { uint32_t a[4]; __m128i b; } x;
	x.b=load_epi32(0x12345678, 0xdeadbeef, 0x98765432, 0xfee1baad);
	x.b=BIGSIGMA0_256(x.b);
	CHECK( x.a[0] == 0x2c3d2e5d );
	CHECK( x.a[1] == 0xded99cdf );
	CHECK( x.a[2] == 0xb62e25ac );
	CHECK( x.a[3] == 0x66146474 );
}

TEST_CASE("SHA256/BIGSIGMA1_256", "Tests that BIGSIGMA1_256 works")
{
	union { uint32_t a[4]; __m128i b; } x;
	x.b=load_epi32(0x12345678, 0xdeadbeef, 0x98765432, 0xfee1baad);
	x.b=BIGSIGMA1_256(x.b);
	CHECK( x.a[0] == 0x92990c22 );
	CHECK( x.a[1] == 0x7718ced6 );
	CHECK( x.a[2] == 0x345e14a3 );
	CHECK( x.a[3] == 0x3561abda );
}

TEST_CASE("SHA256/SIGMA0_256", "Tests that SIGMA0_256 works")
{
	union { uint32_t a[4]; __m128i b; } x;
	x.b=load_epi32(0x12345678, 0xdeadbeef, 0x98765432, 0xfee1baad);
	x.b=SIGMA0_256(x.b);
	CHECK( x.a[0] == 0x2a8a8b98 );
	CHECK( x.a[1] == 0xe3328033 );
	CHECK( x.a[2] == 0xabd31b0b );
	CHECK( x.a[3] == 0xe7fce6ee );
}

TEST_CASE("SHA256/SIGMA1_256", "Tests that SIGMA1_256 works")
{
	union { uint32_t a[4]; __m128i b; } x;
	x.b=load_epi32(0x12345678, 0xdeadbeef, 0x98765432, 0xfee1baad);
	x.b=SIGMA1_256(x.b);
	CHECK( x.a[0] == 0xea3cf8c2 );
	CHECK( x.a[1] == 0xe0b902a0 );
	CHECK( x.a[2] == 0x689dbfec );
	CHECK( x.a[3] == 0xa1f78649 );
}

TEST_CASE("SHA256/Ch", "Tests that Ch works")
{
	__m128i a=load_epi32(0x12345678, 0xdeadbeef, 0x98765432, 0xfee1baad);
	__m128i b=load_epi32(0xdeadbeef, 0x98765432, 0xfee1baad, 0x12345678);
	__m128i c=load_epi32(0x98765432, 0xfee1baad, 0x12345678, 0xdeadbeef);
	union { uint32_t a[4]; __m128i b; } x;
	x.b=Ch(a, b, c);
	CHECK( x.a[0] == 0x122c166a );
	CHECK( x.a[1] == 0x9a601268 );
	CHECK( x.a[2] == 0xb8641422 );
	CHECK( x.a[3] == 0x9a66166a );
}

TEST_CASE("SHA256/Maj", "Tests that Maj works")
{
	__m128i a=load_epi32(0x12345678, 0xdeadbeef, 0x98765432, 0xfee1baad);
	__m128i b=load_epi32(0xdeadbeef, 0x98765432, 0xfee1baad, 0x12345678);
	__m128i c=load_epi32(0x98765432, 0xfee1baad, 0x12345678, 0xdeadbeef);
	union { uint32_t a[4]; __m128i b; } x;
	x.b=Maj(a, b, c);
	CHECK( x.a[0] == 0xdea5beed );
	CHECK( x.a[1] == 0x9a745638 );
	CHECK( x.a[2] == 0xdee5beaf );
	CHECK( x.a[3] == 0x9a34567a );
}
