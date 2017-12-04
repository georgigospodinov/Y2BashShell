#include "runcommons.h"

/* Run 'prog' with args. */
int run() {
    
    pid_t childpid = fork();
    if (childpid == 0) {  // child
        
        if (redirect_input(1)) return FAIL;
        char e = '\0';
        char* c = &e;
        if (redirect_output(c)) return FAIL;
        
        args[noa] = NULL;
        execv(args[0], args);
        printf("Execute failed: %s\n", args[0]);
        return FAIL;
        
    } // else parent
    else if (childpid < 0) {
        int er = errno;
        printf("%s\n", strerror(er));
        return SUCCESS;
    }
    
    int ws;
    waitpid(childpid, &ws, 0);
    return SUCCESS;
    
}

int main(void) {

    init();
    // Read input.
    int run_failed;
    while ( (line_length=getline(&buff, &n, input)) != -1) {
        
        tokenallocation();
        splittokens();
        noa = parse();
        run_failed = run();
        clean();
          // Child should not do this loop.
        if (run_failed) break;
        
    }

    free(buff);
    fclose(input);
    return 0;

}

