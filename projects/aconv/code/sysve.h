// *****************************************************************************************
//
// File description:
//
// Author:	Joao Costa
// Purpose:	Declarations for the Vector extensions
// Info:	https://software.intel.com/sites/landingpage/IntrinsicsGuide/
//
// *****************************************************************************************

#ifndef ACONV_SYSVE_H_
#define ACONV_SYSVE_H_

// Include Standard C headers
#include <limits.h>

// Include master header file for Intel Intrinsics
#include <immintrin.h>

// *****************************************************************************************
//
// Section: Macros and Constants
//
// *****************************************************************************************

#define ACONV_CPU_AVX512		"avx512f"		// INTEL AVX512 Foundation instructions
#define ACONV_CPU_AVX2			"avx2"			// INTEL AVX2
#define ACONV_CPU_AVX			"avx"			// INTEL AVX2
#define ACONV_CPU_MMX			"mmx"			// INTEL MMX

#define ACONV_AVX512_BYTES		64				// Number of bytes in a AVX512 	vector
#define ACONV_AVX2_BYTES		32				// Number of bytes in a AVX2   	vector
#define ACONV_AVX_BYTES			16				// Number of bytes in a AVX    	vector
#define ACONV_MMX_BYTES			8				// Number of bytes in a MMX		vector



// Define the Vector extensions types and respective functions

#ifdef __AVX512F__

 #define VECTOR				512				// Use Vector operations
 #define ACONV_REGSIZE		64				// Number of bytes in 512 bits
 #define regType			__m512i			// AVX512 register type
 #define maskType			__mmask64		// Mask is different in AVX512

 // Macros for Vector operations (alias for concrete functions)

 // Load & Store, align and not
 #define VLOAD(m)			_mm512_load_si512(m)
 #define VLOADU(m)			_mm512_loadu_si512(m)
 #define VSTORE(d,s)		_mm512_store_si512(d,s)
 #define VSTOREU(d,s)		_mm512_storeu_si512(d,s)

 // Set registers
 #define VSET_ZERO			_mm512_setzero_si512
 #define VSET1(a)			_mm512_set1_epi8(a)

 // Add and subtract
 #define VADD(a,b)			_mm512_add_epi8(a,b)
 #define VSUB(a,b)			_mm512_sub_epi8(a,b)

 // Compare macros
 #define VCMPGT(a,b)		_mm512_cmpgt_epi8_mask(a,b)
 #define VCMPEQ(a,b)		_mm512_cmpeq_epi8_mask(a,b)
 #define VCMPEQ64(a,b)		_mm512_cmpeq_epi64_mask(a,b)

 // Relation operations
 #define VAND(a,b)			(a & b)
 #define VOR(a,b)			(a | b)

 // Bit wise operations
 #define VXOR(a,b)			(a ^ b)

 // Selection operations
 #define VBLEND(a,b,m)		_mm512_mask_blend_epi8(m,a,b)

 #define XOR_SETBITS(x)		{x = ULLONG_MAX; }

 // Specific function call
 #define TO_UPPER(x,y)		to_upper_vector(x,y)
 #define TO_LOWER(x,y)		to_lower_vector(x,y)

 #pragma message("Compiling for AVX512")

#elif defined __AVX2__

 #define VECTOR				256				// Use Vector operations
 #define ACONV_REGSIZE		32				// Number of bytes in 256 bits
 #define regType			__m256i			// AVX2 register type
 #define maskType			__m256i			// Mask is the same as the regType in AVX2

 // Macros for Vector operations (alias for concrete functions)

 // Load & Store, align and not
 #define VLOAD(m)			_mm256_load_si256(m)
 #define VLOADU(m)			_mm256_loadu_si256(m)
 #define VSTORE(d,s)		_mm256_store_si256(d,s)
 #define VSTOREU(d,s)		_mm256_storeu_si256(d,s)

 // Set registers
 #define VSET_ZERO			_mm256_setzero_si256
 #define VSET1(a)			_mm256_set1_epi8(a)

 // Add and subtract
 #define VADD(a,b)			_mm256_add_epi8(a,b)
 #define VSUB(a,b)			_mm256_sub_epi8(a,b)

 // Compare macros
 #define VCMPGT(a,b)		_mm256_cmpgt_epi8(a,b)
 #define VCMPEQ(a,b)		_mm256_cmpeq_epi8(a,b)
 #define VCMPEQ64(a,b)		_mm256_cmpeq_epi64(a,b)

 // Relation operations
 #define VAND(a,b)			_mm256_and_si256(a,b)
 #define VOR(a,b)			_mm256_or_si256(a,b)

 // Bit wise operations
 #define VXOR(a,b)			_mm256_xor_si256(a,b)

 #define XOR_SETBITS(x)		{x = VSET_ZERO(); x=VCMPEQ64(x,x); }

 // Selection operations
 #define VBLEND(a,b,m)		_mm256_blendv_epi8(a,b,m)

 // Specific function call
 #define TO_UPPER(x,y)		to_upper_vector(x,y)
 #define TO_LOWER(x,y)		to_lower_vector(x,y)

 #pragma message "Compiling for AVX2"

#elif defined __AVX__

 #define VECTOR				128				// Use Vector operations
 #define ACONV_REGSIZE		16				// Number of bytes in 128 bits
 #define regType			__m128i			// AVX register type
 #define maskType			__m256i			// Mask is the same as the regType in AVX

 // Macros for Vector operations (alias for concrete functions)

 // Load & Store, align and not
 #define VLOAD(m)			_mm_load_si128(m)
 #define VLOADU(m)			_mm_loadu_si128(m)
 #define VSTORE(d,s)		_mm_store_si128(d,s)
 #define VSTOREU(d,s)		_mm_storeu_si128(d,s)

 // Set registers
 #define VSET_ZERO			_mm_setzero_si128
 #define VSET1(a)			_mm_set1_epi8(a)

 // Add and subtract
 #define VADD(a,b)			_mm_add_epi8(a,b)
 #define VSUB(a,b)			_mm_sub_epi8(a,b)

 // Compare macros
 #define VCMPGT(a,b)		_mm_cmpgt_epi8(a,b)
 #define VCMPEQ(a,b)		_mm_cmpeq_epi8(a,b)
 #define VCMPEQ64(a,b)		_mm_cmpeq_epi64(a,b)

 // Relation operations
 #define VAND(a,b)			_mm_and_si128(a,b)
 #define VOR(a,b)			_mm_or_si128(a,b)

 // Bit wise operations
 #define VXOR(a,b)			_mm_xor_si128(a,b)

 #define XOR_SETBITS(x)		{x = VSET_ZERO(); x=VCMPEQ64(x,x); }

 // Selection operations
 #define VBLEND(a,b,m)		_mm_blendv_epi8(a,b,m)		// SSE4.1

 // Specific function call
 #define TO_UPPER(x,y)		to_upper_vector(x,y)
 #define TO_LOWER(x,y)		to_lower_vector(x,y)

 #pragma message("Compiling for AVX")

#else

 #define ACONV_REGSIZE		0

 // Macros for Vector operations (mostly fakes)

 // Specific function call
 #define TO_UPPER(x,y)		to_upper_novector(x,y)
 #define TO_LOWER(x,y)		to_lower_novector(x,y)

 #pragma message("No vector extensions available!")

#endif

#ifdef VECTOR
 #define INIT(x,y)			init(x,y)
#else
 #define INIT(x,y)
#endif


// *****************************************************************************************
//
// Section: Function declarations
//
// *****************************************************************************************

size_t	get_vector_size	( void );

#endif // ACONV_SYSVE_H_
