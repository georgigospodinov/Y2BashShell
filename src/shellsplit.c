#include "sp.h"

int main() {

    // Read input.
    buff = NULL;
    line_length = getline(&buff, &n, stdin);
    tokenallocation();
    splittokens();
    
    // Parse and print.
    noa = parse();
    
    printf("Run program \"%s\"", args[0]);
    if (noa == 1) printf(". ");
    else if (noa == 2) printf(" with argument ");
    else printf(" with arguments ");
    for (int i = 1; i < noa-1; i++) printf("\"%s\" and ", args[i]);
    if (noa > 1) printf("\"%s\". ", args[noa-1]);
    if (strlen(inf)) printf("Read the input from file \"%s\". ", inf);
    if (strlen(outf)) printf("Write the output to file \"%s\".", outf);
    printf("\n");
    
    clean();
    free(buff);
    return 0;
}
