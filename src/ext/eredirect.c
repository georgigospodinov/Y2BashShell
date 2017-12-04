#include "eredirect.h"

/* Initialisations. */
void init() {
    
    sio = fileno(stdin);
    soo = fileno(stdout);
    sib = dup(sio);
    sob = dup(soo);
    input = fdopen(sib, "r");
    
}

/* Redirect the input stream. */
int redirect_input(int close_stdin) {
    
    FILE* progin = NULL;
    if (*inf != '\0') {  // If there is an input file specified.
        progin = freopen(inf, "r", stdin);
        if (progin == NULL) {
            printf("Read failed: %s\n", inf);
            return FAIL;
        }
    }
    else if (piping == 2) {
        if (dup2(pfd[0][0], sio) == -1) {
            printf("pipe in failed!\n");
            return FAIL;
        }
    }
    else if (piping == 4) {
        if (dup2(pfd[1][0], sio) == -1) {
            printf("pipe in failed!(4)\n");
            return FAIL;
        }
    }
    else if (close_stdin) fclose(input);  // Else, maybe close stdin.
    
    return SUCCESS;
}

/* Redirect the output stream. */
int redirect_output(char* temp) {
    
    FILE* progout = NULL;
    if (*outf != '\0') {  // If there is an output file specified.
        
        progout = freopen(outf, "w", stdout);
        if (progout == NULL) {
            FILE* temp_stdout = fdopen(sob, "w");
            fprintf(temp_stdout, "Write failed: %s\n", outf);
            fclose(temp_stdout);
            return FAIL;
        }
        
    }
    else if (piping == 1) {
        if (dup2(pfd[0][1], soo) == -1) {
            printf("pipe out failed!\n");
            return FAIL;
        }
    }
    else if (piping == 3) {
        if (dup2(pfd[1][1], soo) == -1) {
            printf("pipe out failed!(3)\n");
            return FAIL;
        }
    }
    else if (piping == 5) {
        if (dup2(pfd[0][1], soo) == -1) {
            printf("pipe out failed!(5)\n");
            return FAIL;
        }
    }
    else if (*temp != '\0') {
        progout = freopen(temp, "w", stdout);
        if (progout == NULL) return FAIL;
    }
    
    return SUCCESS;
    
}
