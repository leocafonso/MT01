/*
 * xio_file.h	- device driver for file-type devices
 *   			- works with avr-gcc stdio library
 *
 * Part of TinyG project
 *
 * Copyright (c) 2011 - 2013 Alden S. Hart Jr.
 *
 * This file ("the software") is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2 as published by the
 * Free Software Foundation. You should have received a copy of the GNU General Public
 * License, version 2 along with the software.  If not, see <http://www.gnu.org/licenses/>.
 *
 * THE SOFTWARE IS DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT WITHOUT ANY
 * WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/*--- How to setup and use program memory "files" ----

  Setup a memory file (OK, it's really just a string)
  should be declared as so:

	const char g0_test1[] PROGMEM= "\
	g0 x10 y20 z30\n\
	g0 x0 y21 z-34.2";

	Line 1 is the initial declaration of the array (string) in program memory
	Line 2 is a continuation line. 
		- Must end with a newline and a continuation backslash
		- (alternately can use a semicolon instead of \n is XIO_SEMICOLONS is set)
		- Each line will be read as a single line of text using fgets()
	Line 3 is the terminating line. Note the closing quote and semicolon.


  Initialize: xio_pgm_init() must be called first. See the routine for options.

  Open the file: xio_pgm_open() is called, like so:
	xio_pgm_open(PGMFILE(&g0_test1));	// simple linear motion test

	PGMFILE does the right cast. If someone more familiar with all this 
	can explain why PSTR doesn't work I'd be grateful.

  Read a line of text. Example from parsers.c
	if (fgets(textbuf, BUF_LEN, srcin) == NULL) {	
		printf_P(PSTR("\r\nEnd of file encountered\r\n"));
		clearerr(srcin);
		srcin = stdin;
		tg_prompt();
		return;
	}
*/

#ifndef xio_SPIFFS_h
#define xio_SPIFFS_h

#define PGMFILE (const PROGMEM char *)		// extends pgmspace.h

/* 
 * FILE DEVICE CONFIGS 
 */
#include "spiffs.h"

// file-type device control struct
typedef struct xioSPIFILE {
	spiffs gSPIFFS;
	spiffs_file f;
} xioSPIFFS_t;

/* 
 * FILE DEVICE FUNCTION PROTOTYPES
 */
void xio_init_spiffs();
FILE * xio_open_spiffs(const uint8_t dev, const char *addr, const flags_t flags);
int xio_gets_spiffs(xioDev_t *d, char *buf, const int size);		// read string from program memory
int xio_getc_spiffs(FILE *stream);									// get a character from PROGMEM
int xio_putc_spiffs(const char c, FILE *stream);
void xio_close_spiffs (xioDev_t *d);
// SD Card functions

#endif