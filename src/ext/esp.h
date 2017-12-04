#ifndef HEADER_GUARD_ESP
#define HEADER_GUARD_ESP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 1024/2 + 1
#define String char*

String tokens[MAX_TOKENS];
int nt;  // Number of tokens.
String buff;  // Buffer for line read.
size_t n;  // Used by getline().
ssize_t line_length;
ssize_t max_tokens;
int noa;  // Number Of Arguments.

String inf;  // Stdin redirection
String outf;  // Stdout redirection
String args[MAX_TOKENS];

String unparsed;
/* 0 - not piping
 * 1 - write to pfd[0]
 * 2 - read from pfd[0]
 * 3 - read from pfd[0], write to pfd[1]
 * 4 - read from pfd[1]
 * 5 - read from pfd[1], write to pfd[0]
 */
int piping;
int pfd[2][2];
int splitstring();
int parse ();
void clean();
void tokenallocation();
void splittokens();

#endif /* HEADER_GUARD_ESP */

