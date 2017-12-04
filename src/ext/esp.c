#include "esp.h"

/* Split a source String at spaces.
 * Do not split if the space is within "".
 * Put the words in the buffer.
 * Return the number of words.
 */
int splitstring() {
    String str = unparsed;
    int flag_do_split = 1;  // control whether there was "
    int substr_count = 0;  // number of substrings so far
    int curr_size = 0;  // the size of the current substring
    while (*str != '\0') {
        switch (*str) {
            case '\"':
                flag_do_split = flag_do_split^1;
                break;
            case ';':  // A program has been entered. A new one follows.
                if (flag_do_split) goto afterwhile;
            case '|':  // A program has been entered. A new one follows.
                if (flag_do_split) {
                    piping++;  // Set the piping flag.
                    goto afterwhile;
                }
            case ' ':
                if (flag_do_split) {
                    tokens[substr_count++][curr_size] = '\0';
                    curr_size = 0;
                    break;  // break switch
                }
                //otherwise treat it is another character:
            default: tokens[substr_count][curr_size++] = *str;
        }
        str++;
    }
    
    afterwhile:
    /* NOTE2:
     * First read NOTE1. (in 'splittokens')
     * So the while stops at the first '\0'.
     * This guarantees that unparsed will never point to a random string.
     */
    unparsed = str+1;
    if (*unparsed == ' ') unparsed++;  // Skip a leading space of next command.
    // End of string character for last token.
    tokens[substr_count++][curr_size] = '\0';
    
    return substr_count;
}

/* Parse the tokens.
 * Determine program, input file, output file, and arguments.
 */
int parse () {
    
    // The first item is the program to run.
    strcpy(args[0], tokens[0]);
    int type = 0;  // 0 - argument, 1 - input_file, 2 - output_file
    int control = 0;  // 0=only args, 1=inf, 2=outf, 3=inf+outf
    int argc = 1;
    for (int i = 1; i < nt; i++) {
        if (strcmp(tokens[i], ">") == 0) type = 2;
        else if (strcmp(tokens[i], "<") == 0) type = 1;
        else {
            if (type == 0) strcpy(args[argc++], tokens[i]);
            else if (type == 1) {
                strcpy(inf, tokens[i]);
                type = 0;
                if (control == 2 || control == 0) control++;
            }
            else {
                strcpy(outf, tokens[i]);
                type = 0;
                if (control == 1) control = 3;
                if (control == 0) control = 2;
            }
        }
    }
    switch (control) {
        case 0:
            *inf = '\0';
        case 1:
            *outf = '\0';
            break;
        case 2:
            *inf = '\0';
            break;
        default: break;
    }
    return argc;
}

/* Memory freeing and stream flushing. */
void clean() {
    
    fflush(stdout);
    if (inf != NULL) free(inf);
    if (outf != NULL) free(outf);
    
    for (int i = 0; i < max_tokens; i++) {
        free(tokens[i]);
        free(args[i]);
    }
    
}

/* Allocate space for each token. */
void tokenallocation() {
    
    /* NOTE 1:
     * The two characters of the buffer are '\n\0'.
     * The next line makes them '\0\0'.
     * goto NOTE2.
     */
    
    unparsed[--line_length] = '\0';  // ignore the NEWLINE character
    max_tokens = line_length/2 +1;
    for (int i = 0; i < max_tokens; i++)
        tokens[i] = calloc(line_length, sizeof(char));

}

/* Split the buffer into tokens and put the arguments in args. */
void splittokens() {
    
    nt = splitstring();
    int prog_length = strlen(tokens[0]);
    inf = calloc(line_length-prog_length+1, sizeof(char));
    outf = calloc(line_length-prog_length+1, sizeof(char));
    
    args[0] = calloc(prog_length+1, sizeof(char));
    for (int i = 1; i < max_tokens; i++)
        args[i] = calloc(line_length-prog_length, sizeof(char));

}