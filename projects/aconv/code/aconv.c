// *****************************************************************************************
//
// File description:
//
// Author:	Joao Costa
// Purpose:	Text case conversion (upper/lower) from standard input to standard output
//			using Intel AVX Intrinsics.	The input is assumed to be ASCII encoding.
//
//
// *****************************************************************************************


// *****************************************************************************************
//
// Section: Import headers
//
// *****************************************************************************************


// Define POSIX Standard target
#define  _POSIX_C_SOURCE 200809L

// Include Standard headers
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Include master header file for Intel Intrinsics
#include <immintrin.h>

_Static_assert( _POSIX_VERSION == 200809L, "Wrong POSIX version" );

// Include own headers
#include "sysve.h"
#include "aconv.h"




// *****************************************************************************************
//
// Section: Global variables
//
// *****************************************************************************************

#ifdef VECTOR

 // Define constant registers (for one time cost)
 regType		delta;	//	'a'-'A'
 regType		base;	//	'a' -1
 regType		top;	//	'z'
 regType		xor;	//	To use for the Xor

#endif

// *****************************************************************************************
//
// Section: Function definition
//
// *****************************************************************************************


// *****************************************************************************************
//
// Purpose: Display operator help information
// Input:	None
// Output:	None
// Return:	None
//
// *****************************************************************************************

void display_usage( char * progname )
{
 fprintf( stderr, "%s [ %s | %s ]\n", progname, ACONV_SWITCH_LOWERCASE, ACONV_SWITCH_UPPERCASE );
 fprintf( stderr, "Where:\n" );
 fprintf( stderr, "%s : Transform input to lower case\n", ACONV_SWITCH_LOWERCASE );
 fprintf( stderr, "%s : Transform input to upper case\n", ACONV_SWITCH_UPPERCASE );
}

// *****************************************************************************************
//
// Purpose: Display operator help information
// Input:	None
// Output:	None
// Return:	None
//
// *****************************************************************************************

size_t get_vector_size( void )
{
 size_t sz = 0;

      if( __builtin_cpu_supports( ACONV_CPU_AVX512	) > 0 )	sz = ACONV_AVX512_BYTES;
 else if( __builtin_cpu_supports( ACONV_CPU_AVX2	) > 0 )	sz = ACONV_AVX2_BYTES;
 else if( __builtin_cpu_supports( ACONV_CPU_AVX		) > 0 )	sz = ACONV_AVX_BYTES;
 else if( __builtin_cpu_supports( ACONV_CPU_MMX		) > 0 )	sz = ACONV_MMX_BYTES;

 return sz;
}


#ifdef VECTOR


// *****************************************************************************************
//
// Purpose: Display operator help information
// Input:	None
// Output:	None
// Return:	None
//
// *****************************************************************************************

void init( char b, char t )
{
  delta = VSET1		( (char) ACONV_INIT_DELTA 	);
  base  = VSET1		( b  						);
  top   = VSET1  	( t 						);

  // All bits set to 1
  xor	= VSET_ZERO	();
  xor	= VCMPEQ64	( xor, xor );
}



// *****************************************************************************************
//
// Purpose: Convert input line to upper case using Vector extensions
// Input:	Input Line and size of the input line
// Output:	None
// Return:	None
//
// *****************************************************************************************

void to_upper_vector( char * line, size_t lineSize )
{
 regType	data, data1;		// Register holding data derived from the input line
 regType	in_range, amask;	// Bitmasks

  int i=0;
  for( ; i+ACONV_REGSIZE <= lineSize; )
     {
	  // Generate two copies of input data
	  data = VLOADU	( (regType *)	&line[ i ] 	);		// Move line data to register
	  VSTORE( (regType *) &data1,	data		);		// Create a second copy

	  // Test data to find lower case letters and generate a bit mask
	  // For every character that is a lower case, in the register, set corresponding byte in the register mask to 1 / true / found
	  // Otherwise, set corresponding byte mask to 0 / false / not found

	  in_range	= VCMPGT	( data,		base	);			// Test characters > 'a'-1
	  amask		= VCMPGT	( data,		top		);			// Test characters > 'z'
	  amask		= VXOR		( amask,	xor		);			// Invert previous comparison, i.e. test <= 'z'
	  in_range	= VAND		( in_range,	amask	);			// Result of the comparison (data>='a' && data<='z')

	  // On the data copy register, subtract all bytes by the lowercase / uppercase ASCII delta (32)
	  data1		= VSUB		( data1,	delta	);

	  // Select between original and subtracted data depending on comparison mask
	  data		= VBLEND	( data, 	data1,	in_range	);

	  // Copy data back to the line buffer
	  VSTOREU				( (regType *) &line[ i ], data	);

	  i+=ACONV_REGSIZE;
     }


  // Use branchless programming for the remaining of the string characters
  for( ; i < lineSize; ++i )
     {
	  //printf("Left overs. i=%d. char=%c. result:%d\n", i, line[i], (int) line[i]>= 'a' && line[i] <= 'z' );
	  line[i] -= ACONV_INIT_DELTA * (line[i]>= 'a' && line[i] <= 'z' );
     }

}


// *****************************************************************************************
//
// Purpose: Convert input line to lower case using Vector extensions
// Input:	Input Line and size of the input line
// Output:	None
// Return:	None
//
// *****************************************************************************************

void to_lower_vector( char * line, size_t lineSize )
{
  regType	data, data1;		// Register holding data derived from the input line
  regType	in_range, amask;	// Bitmasks

  int i=0;
  for( ; i+ACONV_REGSIZE <= lineSize; )
     {
	  // Generate two copies of input data
	  data	=	VLOADU( (regType *)	&line[ i ] 	);		// Move line data to register
	  VSTORE( (regType *) &data1,	data		);		// Create a second copy

	  // Test data to find lower case letters and generate a bit mask
	  // For every character that is a lower case, in the register, set corresponding byte in the register mask to 1 / true / found
	  // Otherwise, set corresponding byte mask to 0 / false / not found

	  in_range	= VCMPGT	( data,		base	);			// Test characters > 'A'-1
	  amask		= VCMPGT	( data,		top		);			// Test characters > 'Z'
	  amask		= VXOR		( amask,	xor		);			// Invert previous comparison, i.e. test <= 'Z'
	  in_range	= VAND		( in_range,	amask	);			// Result of the comparison (data>='A' && data<='Z')

	  // On the data copy register, add all bytes by the lowercase / uppercase ASCII delta (32)
	  data1		= VADD		( data1,	delta	);

	  // Select between original and subtracted data depending on comparison mask
	  data		= VBLEND	( data, 	data1,	in_range	);

	  // Copy data back to the line buffer
	  VSTOREU				( (regType *) &line[ i ], data	);

	  i+=ACONV_REGSIZE;
     }


  // Use branchless programming for the remaining of the string characters
  for( ; i < lineSize; ++i )
     {
	  //printf("Left overs. i=%d. char=%c. result:%d\n", i, line[i], (int) line[i]>= 'a' && line[i] <= 'z' );
	  line[i] += ACONV_INIT_DELTA * (line[i]>= 'A' && line[i] <= 'Z' );
     }

}

#endif


// *****************************************************************************************
//
// Purpose: Convert an input line to upper case without using SIMD operations
// Input:	None
// Output:	None
// Return:	None
//
// *****************************************************************************************

void to_upper_novector( char * line, size_t lineSize )
{
  for( int i=0; i < lineSize; i++ )
     {
	  //printf("Left overs. i=%d. char=%c. result:%d\n", i, line[i], (int) line[i]>= 'a' && line[i] <= 'z' );
	  line[i] -= ACONV_INIT_DELTA * (line[i]> ACONV_LOWER_BASE && line[i] <= ACONV_LOWER_TOP );
     }
}


// *****************************************************************************************
//
// Purpose: Convert an input line to lower case without using SIMD operations
// Input:	None
// Output:	None
// Return:	None
//
// *****************************************************************************************

void to_lower_novector( char * line, size_t lineSize )
{
  for( int i=0; i < lineSize; i++ )
     {
	  //printf("Left overs. i=%d. char=%c. result:%d\n", i, line[i], (int) line[i]>= 'a' && line[i] <= 'z' );
	  line[i] += ACONV_INIT_DELTA * (line[i]> ACONV_UPPER_BASE && line[i] <= ACONV_UPPER_TOP );
     }
}

// *****************************************************************************************
//
// Purpose: Principal conversion function (to upper case letters)
// Input:	None
// Output:	None
// Return:	None
//
// *****************************************************************************************

void to_upper( void )
{
  size_t	len		= 0;		// Size of the input buffer
  ssize_t	size	= 0;		// Number of characters read
  char	*	p_line	= NULL;		// Pointer to input line

  INIT( (char) ACONV_LOWER_BASE, (char) ACONV_LOWER_TOP );

  while( ( size = getline( &p_line, &len, stdin )) != -1 )
       {
         //rc = to_upper( p_line, (size_t) size, vs, p_delta, p_base, p_top );
	     TO_UPPER( p_line, (size_t) size );
         printf( "%s", p_line );
       }

  if( p_line != NULL ) free( p_line );

  fprintf( stderr, "\n" );

}


// *****************************************************************************************
//
// Purpose: Principal conversion function (to lower case letters)
// Input:	None
// Output:	None
// Return:	None
//
// *****************************************************************************************

void to_lower( void )
{
  size_t	len		= 0;		// Size of the input buffer
  ssize_t	size	= 0;		// Number of characters read
  char	*	p_line	= NULL;		// Pointer to input line

  INIT( (char) ACONV_UPPER_BASE, (char) ACONV_UPPER_TOP );

  while( ( size = getline( &p_line, &len, stdin )) != -1 )
       {
         //rc = to_upper( p_line, (size_t) size, vs, p_delta, p_base, p_top );
	     TO_LOWER( p_line, (size_t) size );
         printf( "%s", p_line );
       }

  if( p_line != NULL ) free( p_line );

  fprintf( stderr, "\n" );

}




// *****************************************************************************************
//
// Purpose: Main
// Input:	Command line arguments
// Output:	None
// Return:	Program status
//
// *****************************************************************************************

int main( int argc, char * argv[] )
{
 if( argc != 2 )
   {
	 display_usage( argv[0] );
	 return 2;
   }

 if( ACONV_REGSIZE != 0 && get_vector_size() < (size_t) ACONV_REGSIZE )
   {
	 fprintf( stderr, "Error: Compiled vector size (%d) is different from available: %ld. Exiting...\n\n", ACONV_REGSIZE, get_vector_size() );
	 return 3;
   }


 if( strcmp( argv[ 1 ], ACONV_SWITCH_LOWERCASE ) == 0 )
	 to_lower();
 else
   {
	 if( strcmp( argv[ 1 ], ACONV_SWITCH_UPPERCASE ) == 0 )
		 to_upper();
	 else
	   {
		 display_usage( argv[0] );
		 return 4;
	   }
   }
}
