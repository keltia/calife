/* HTAB = 4 */
/****************************************************************************
 * compiler.h -- Macro definitions used to handle compilers idiosyncrasies.	*
 *--------------------------------------------------------------------------*
 * (c) 2002 Bertrand Petit													*
 *																			*
 * Redistribution and use in source and binary forms, with or without		*
 * modification, are permitted provided that the following conditions		*
 * are met:																	*
 *																			*
 * 1. Redistributions of source code must retain the above copyright		*
 *    notice, this list of conditions and the following disclaimer.			*
 *																			*
 * 2. Redistributions in binary form must reproduce the above				*
 *    copyright notice, this list of conditions and the following			*
 *    disclaimer in the documentation and/or other materials provided		*
 *    with the distribution.												*
 * 																			*
 * 3. Neither the name of the author nor the names of its contributors		*
 *    may be used to endorse or promote products derived from this			*
 *    software without specific prior written permission.					*
 * 																			*
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''		*
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED		*
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A			*
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR		*
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,				*
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT			*
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF			*
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND		*
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,		*
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT		*
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF		*
 * SUCH DAMAGE.																*
 *																			*
 ****************************************************************************/

/*
 * $Name$
 * $Date$
 * $Revision$
 */

#ifndef COMPILER_H
#define COMPILER_H

/****************************************************************************
 * Macros.																	*
 ****************************************************************************/

/* Function attribute declaring that printf-like format string checks shall be
 * performed on arguments at compile time. Format string and argument
 * positions are 1-based.
 *
 * Declaration usage:
 *  _WITH_FORMAT_STRING(1, 2) FormatedLog(const char * Message, ...);
 *
 * Definition usage:
 *  _WITH_FORMAT_STRING(1, 2) FormatedLog(const char * Message, ...) {}
 */
#if defined(__GNUC__) || defined(__clang__)
# define _WITH_FORMAT_STRING(StringPos, ArgsPos) \
	__attribute__((__format__ (__printf__, StringPos, ArgsPos)))
#else /* __GNUC__ || __clang__ */
# define _WITH_FORMAT_STRING(StringPos, ArgsPos)
#endif

/* Function attribute declaring that a function is an exit point.
 *
 * Declaration usage:
 *  _Noreturn void die(void) _NORETURN;
 *
 * Definition usage:
 *  _Noreturn void die(void) {}
 */
#if defined(__clang__)
# define _NORETURN
#elif defined(__GNUC__)
# define _Noreturn
# define _NORETURN __attribute__ ((noreturn))
#else /* ! __GNUC__ and ! __clang__ */
# define _Noreturn
# define _NORETURN
#endif

/* Variable attribute declaring that a variable is known to be unused.
 *
 * Usage in declarations:
 *   int foo(int bar _UNUSED_VARIABLE); // Optional
 *   extern const char *Name _UNUSED_VARIABLE; // Optional
 *
 * Usage in definitions:
 *   int foo(int bar _UNUSED_VARIABLE) {}
 *   const char *Name _UNUSED_VARIABLE = NULL;
 */
#if defined(__GNUC__) || defined(__clang__)
# define _UNUSED_VARIABLE __attribute__((__unused__))
#else /* __GNUC__ || __clang__ */
# define _UNUSED_VARIABLE
#endif

#endif /* COMPILER_H */

/*
 * Local Variables:
 * tab-width: 4
 * End:
 */

/****************************************************************************
 * End of file compiler.h													*
 ****************************************************************************/
