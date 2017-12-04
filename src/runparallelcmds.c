#include "runcommons.h"
#include <sys/stat.h>

// Number of processes to run in parallel.
int cpus;
// Pointers to filenames for temporary output.
String* tempouts;
// Statuses of the pointers.
typedef enum {FREE, BUSY, PRINT_READY, USE_AS_COUNTER} status;
status* tempouts_statuses;
// Index of file to print.
int file_ind = 0;
// A map of files to children that print to them.
pid_t* filemap;

/* Advance the file index, accounting for cpus. */
void advfileind() {
    file_ind++;
    if (file_ind == cpus) file_ind = 0;
}

/* Generate filenames for temporary output. */
void gentemps() {
    
    tempouts = calloc(cpus, sizeof(String));
    tempouts_statuses = calloc(cpus, sizeof(status));
    filemap = calloc(cpus, sizeof(pid_t));
    
    /* The first two characters are the path. ("t/")
     * The next two are the name.
     * So the filename is a number (00-99).
     * The fifth and last character is for '\0'.
     */
    int tfpl = 5;  // Temp File Path Length
    // Make the directory.
    struct stat my_dir_t = {0};
    if (stat("./t", &my_dir_t) == -1) mkdir("./t", 0777);
    for (int i = 0; i < cpus; i++ ) {
        tempouts[i] = calloc(tfpl, sizeof(char));
        snprintf(tempouts[i], tfpl, "t/%d", i);
        tempouts_statuses[i] = FREE;
    }
    
}

/* Free filenames for temporary output. */
void freetemps() {
    for (int i = 0; i < cpus; i++ ) free(tempouts[i]);
    free(tempouts);
    free(tempouts_statuses);
    free(filemap);
}

/* Returns the index of the next free temporary file. -1 if no such is found. */
int getfreetemp() {
    for (int i = 0; i < cpus; i++ ) if (tempouts_statuses[i] == FREE) return i;
    return -1;
}

/* Find the index of the file this child wrote to. -1 if no such child. */
int findindex(pid_t child) {
    for (int i = 0; i < cpus; i++) if (filemap[i] == child) return i;
    return -1;
}

/* Print consecutively the contents of files that are PRINT_READY. */
void printfiles() {
    ssize_t ll;
    size_t m;
    String b = NULL;
    FILE* tmp;
    
    while (tempouts_statuses[file_ind] == FREE) {
        advfileind();
        if (file_ind == 0) break;
    }
    
    // If the file to print is PRINT_READY,
    while (tempouts_statuses[file_ind] == PRINT_READY) {
        // open it,
        tmp = fopen(tempouts[file_ind], "r");
        if (tmp == NULL) break;
        
        // print it,
        while ((ll=getline(&b, &m, tmp)) != -1) printf("%s", b);
        
        // set it to FREE
        tempouts_statuses[file_ind] = FREE;
        advfileind();
        fclose(tmp);
    }
    
    free(b);
    
}

/* Looks for a zombie child and calls printfiles. */
void cleanfiles() {
    
    // Check for a zombie, get the file it printed to.
    int fi = findindex(waitpid(-1, NULL, WNOHANG));
    if (fi != -1) {
        // If the associated file has no contents,
        if (tempouts_statuses[fi] == USE_AS_COUNTER)
            // just free it.
            tempouts_statuses[fi] = FREE;

        // Else, prepare for printing.
        else tempouts_statuses[fi] = PRINT_READY;
    }
    
    printfiles();
    
}

/* Run 'prog' with args. */
int run() {
    
    // Index of free file for output.
    int index = -1;
    // Wait for a free temporary.
    while((index=getfreetemp()) == -1) cleanfiles();
    // If an output file is not specified,
    if (*outf == '\0') tempouts_statuses[index] = BUSY;
    else tempouts_statuses[index] = USE_AS_COUNTER;
    
    pid_t childpid = fork();
    if (childpid == 0) {  // child
        
        if (redirect_input(0)) return FAIL;
        if (redirect_output(tempouts[index])) return FAIL;
        
        args[noa] = NULL;
        execv(args[0], args);
        
        // All lines below are executed if execv fails.
        FILE* temp_stdout = fdopen(sob, "w");
        fprintf(temp_stdout, "Execute failed: %s\n", args[0]);
        fclose(temp_stdout);
        return FAIL;
        
    } // else parent
    else if (childpid < 0) {
        int er = errno;
        printf("%s\n", strerror(er));
        return SUCCESS;
    }
    
    // Bind child to file
    if (index > -1) filemap[index] = childpid;
    return SUCCESS;
    
}

int main(int argc, char** argv) {

    if (argc != 1 && argc != 3) {
        printf("Usage: runparallelcmds\n");
        printf("Or: runparallelcmds -j <number of cpus to run on>\n");
        exit(1);
    }
    
    if (argc == 3) cpus = atoi(argv[2]);
    if (cpus < 1) cpus = 1;
    gentemps();
    init();
    // Read input.
    int run_result = 0;
    while ( (line_length=getline(&buff, &n, input)) != -1) {
        
        tokenallocation();
        splittokens();
        noa = parse();
        run_result = run();
        clean();
        // Child should exit ASAP.
        if (run_result == FAIL) goto cleanups;
        
    }
    
    int fi;
    int ws;
    // Wait for all children, marking their files.
    while((fi = findindex(wait(&ws)))!=-1) {
        int es = WEXITSTATUS(ws);
        // Success and written to, prepare to print.
        if (es == SUCCESS && tempouts_statuses[fi] == BUSY)
            tempouts_statuses[fi] = PRINT_READY;
        
        // Else, free.
        else tempouts_statuses[fi] = FREE;
    }
    printfiles();
    
    cleanups:
    free(buff);
    fclose(input);
    freetemps();
    return run_result;

}
