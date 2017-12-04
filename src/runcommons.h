#ifndef RUNCOMMONS_HEADER_GUARD
#define RUNCOMMONS_HEADER_GUARD

#include "sp.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

// Values returned by some function calls.
#define FAIL 1
#define SUCCESS 0

int sio, soo;  // Standard In/Out Original-s.
int sib, sob;  // Standard In/Out Backup-s.
FILE* input;  // Stream from stdin for main program.

void init();
int redirect_input(int close_stdin);
int redirect_output(char* temp);

#endif /* RUNCOMMONS_HEADER_GUARD */

