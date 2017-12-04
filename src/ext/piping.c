#include "eredirect.h"

/* Open a new pipe. */
void openpipe() {
    
    // If opening a pipe fails, do not pipe.
    if (piping == 1 && pipe(pfd[0])) piping = 0;
    else if (piping == 3 && pipe(pfd[1])) piping = 0;
    else if (piping == 5 && pipe(pfd[0])) piping = 0;
    
}

/* Close the read end of a pipe for a child. */
void closechildend() {
    
    if (piping == 1) close(pfd[0][0]);
    else if (piping == 3) close(pfd[1][0]);
    else if (piping == 5) close(pfd[0][0]);
    
}

/* Close the read/write end of a pipe for the parent.
 * (Future children also do not have access to that end.)
 */
void closeparentend() {
    switch(piping) {
        
        case 1:
            piping++;  // Next child is reading the pipe.
            close(pfd[0][1]);  // Close pipe 0 write end.
            break;
            
        case 2:
            piping = 0;  // No more piping.
            close(pfd[0][0]);  // Close pipe 0 read end.
            break;
            
        case 3:
            piping++;
            close(pfd[1][1]);  // Close pipe 1 write end.
            close(pfd[0][0]);  // Close pipe 0 read end.
            break;
        
        case 4:
            piping = 0;
            close(pfd[1][0]);  // Close pipe 1 read end.
            break;
            
        case 5:
            piping = 2;
            close(pfd[0][1]);  // Close pipe 0 write end.
            close(pfd[1][0]);  // Close pipe 1 read end.
    }
}