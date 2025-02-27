/*  Metrowerks Standard Library  Version 2.1.2  1997 July  */

/*
 *	uart_console_io.c
 *	
 *		Copyright © 1997 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *	Routines
 *	--------
 *		__read_console
 *		__write_console
 *		__close_console
 *
 *	Implementation
 *	--------------
 *
 *		These routines implement low-level console IO routines using
 *		any UART library which conforms to the MetroTRK UART library
 *		interface.  They are intended to provide basic console IO
 *		services in an embedded environment which lacks OS console
 *		support.
 *
 *
 */

#include "UART.h"
#include "console_io.h"
#include "uart_console_config.h"
#include "stdio.h"

#if __MIPS__
	#include <__board.h>
#endif

static UARTError __init_uart_console(void);


/************************************************************************/
int		__read_console	  (__file_handle handle, unsigned char * buffer, 
							size_t * count, __idle_proc idle_proc)
{
	#pragma unused( handle, idle_proc )

#if __IDT_SIM__ || __PMON__ || __AMC__

	return( __no_io_error );	

#else

	size_t limit;
	UARTError err;

	if (__init_uart_console() != kUARTNoError)
		return( __io_error );

	/*
	** A console read should return when it encounters a line-ending
	** or after it has read the requested number of characters.
	*/

	limit = *count;
	*count = 0;
	err = kUARTNoError;

	while( *count <= limit && err == kUARTNoError )
	{
		err = ReadUART1( (char *)buffer );
		++(*count);
		if (*buffer == '\r')
			break;
		++buffer;
	}

	return( (err == kUARTNoError) ? __no_io_error : __io_error );

#endif
}



/************************************************************************/
int		__write_console	  (__file_handle handle, unsigned char * buffer, 
							size_t * count, __idle_proc idle_proc)
{
	#pragma unused( handle, idle_proc )

#if __IDT_SIM__ || __PMON__
	int i = *count;
	while(i) {
		__putchar(*buffer);
		++buffer; --i;
	}
	*count = 0;
	return( __no_io_error );	

#elif __AMC__
	*count = 0;
	return( __no_io_error );	/* temp! */

#else

	if (__init_uart_console() != kUARTNoError)
		return( __io_error );

	if (WriteUARTN( buffer, *count ) != kUARTNoError)
	{
		*count = 0;
		return( __io_error );
	}
	
	return( __no_io_error );

#endif	
}



/************************************************************************/
int		__close_console	  (__file_handle handle)
{
	#pragma unused( handle )

	return( __no_io_error );
}



/************************************************************************/
UARTError __init_uart_console()
{
#if __IDT_SIM__ || __PMON__ || __AMC__

	return( __no_io_error );

#else

	UARTError err = kUARTNoError;
	static int initialized = 0;
	
	if (initialized == 0)
	{
		err = InitializeUART( UART_CONSOLE_BAUD_RATE );
		
		if (err == kUARTNoError)
			initialized = 1;
	}
	
	return( err );

#endif
}

/*	Change Record
 *	09-Jul-97 SCM	Created
 *	20-Aug-99 SCM	Enhanced __read_console() to return when it receives an EOL.
*/
