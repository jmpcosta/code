// *****************************************************************************************
//
// File description:
//
// Author:	Joao Costa
// Purpose:	Declarations for the Alphabetic conversion application
//
// *****************************************************************************************

#ifndef ACONV_H_
#define ACONV_H_


// *****************************************************************************************
//
// Section: Macros and Constants
//
// *****************************************************************************************

#define ACONV_SWITCH_LOWERCASE	"-a"			// Command line switch to request lower case conversion
#define ACONV_SWITCH_UPPERCASE	"-A"			// Command line switch to request lower case conversion

#define ACONV_INIT_DELTA	32				// Set ASCII table delta between Upper and Lowercase letters (32)

// Lower case range
#define ACONV_LOWER_BASE	96				// 'a' - 1
#define ACONV_LOWER_TOP		122				// 'z'

// Upper case range
#define ACONV_UPPER_BASE	64				// 'A' - 1
#define ACONV_UPPER_TOP		90				// 'Z'


// *****************************************************************************************
//
// Section: Function declarations
//
// *****************************************************************************************

void	to_upper			( void );
void	to_lower			( void );

void	init				( char base, char top 			);
void	display_usage		( char *						);

void	to_upper_vector		( char *, size_t	);
void	to_upper_novector	( char *, size_t	);

void	to_lower_vector		( char *, size_t	);
void	to_lower_novector	( char *, size_t	);


#endif // ACONV_H_
