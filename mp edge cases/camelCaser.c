/**
 * extreme_edge_cases
 * CS 241 - Spring 2021
 */
#include "camelCaser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char **camel_caser(const char *input_str) {
    // TODO: Implement me!
    int i;
    int num_sentences = 0;
    char input_c;
    int sentence = 0;
    int restriction = 0;
    int cflag = 1;
    char to_add;
    if (input_str == NULL) {return NULL;}
    for (i = 0; i < (int)strlen(input_str); i++) {
        if (ispunct(input_str[i])) {
            num_sentences++;
        }
    }
    /////////////////////////////////////////////
    char** result = calloc((num_sentences + 1), sizeof(char*));
    result[num_sentences] = NULL;

    const char* countptr = input_str;
    i = 0;
    while(i < num_sentences){
        int length = 0;
        while(!ispunct(*countptr)){
            if(!isspace(*countptr)) {
                length++;
            }
            countptr++;
        }
        result[i] = malloc(length+1);
        result[i][length] = '\0';
        i++;
        countptr++;
    }
    int j = 0;
    int k = 0;
    while ((input_c = input_str[j++])) {
        if (!result[sentence])
            break;
        if (isspace(input_c)) {
            restriction = 1;
        } else if (ispunct(input_c)) {
            restriction = 0;
            cflag = 1;
            sentence++;
            k = 0;
        } else {
            if (isalpha(input_c)) {
                if(!restriction || cflag){
                    to_add = tolower(input_c);
                    restriction = 0;
                }else{
                    to_add = toupper(input_c);
                    restriction = 0;
                }
            } else {
                to_add = input_c;
            }
            cflag = 0;
            result[sentence][k] = to_add;
            k++;
        }
    }
    return result;
}

void destroy(char **result) {

    int i = 0;
    while(result[i] != NULL){
        free(result[i]);
        result[i] = NULL;//set pointer to null
    }
    free(result);
    result = NULL;
    return;
}