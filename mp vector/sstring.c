/**
 * vector
 * CS 241 - Spring 2021
 */
#include "sstring.h"
#include "vector.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <string.h>

struct sstring {
    // Anything you want
    vector *vec;
};

sstring *cstr_to_sstring(const char *input) {
    // your code goes here
    sstring * out = calloc(1,sizeof(sstring));
    out->vec = char_vector_create();
    const char * p = input;
    while (*p) {
        vector_push_back(out->vec,(void *) p);
        p++;
    }
    return out;
}

char *sstring_to_cstr(sstring *input) {
    // your code goes here
    char * out = calloc(vector_size(input->vec)+1, sizeof(char));
    size_t i;
    for (i = 0; i < vector_size(input->vec); i++) {
      out[i] = *((char *) vector_get(input->vec, i));
    }
    return out;
}

int sstring_append(sstring *this, sstring *addition) {
    // your code goes here
    vector_reserve(this->vec,vector_size(this->vec)+vector_size(addition->vec));
    size_t i;
    for (i = 0; i< vector_size(addition->vec); i++) {
        vector_push_back(this->vec,((char *) vector_get(addition->vec, i)));
    }
    return vector_size(this->vec);
}

vector *sstring_split(sstring *this, char delimiter) {
    
    vector * toreturn = string_vector_create();
    sstring * str = calloc(1,sizeof(sstring));
    str->vec = char_vector_create();
    size_t i;
    for (i = 0; i < vector_size(this->vec); i++) {
        char * bla = vector_get(this->vec, i);
        if (*bla == delimiter) {
            vector_push_back(toreturn, sstring_to_cstr(str));
            vector_clear(str->vec);
            //free(toadd);
        } else {
            vector_push_back(str->vec, bla);
        }
    }
    vector_push_back(toreturn,sstring_to_cstr(str));
    sstring_destroy(str);
    return toreturn;
}

int sstring_substitute(sstring *this, size_t offset, char *target,
                       char *substitution) {
    // your code goes here
    char *pointer = target;
    int sign = 1;
    for (size_t i = offset; i < vector_size(this->vec); i++) {
      char *curr = vector_get(this->vec, i);
      if (*curr == *pointer && (vector_size(this->vec)-i) >= strlen(target)) {
        int j = i;
        while ( *pointer ) {
          if ( *((char *) vector_get(this->vec, j)) != *pointer ) {
            sign = 0;
            pointer = target;
            break;
          }
          j+=1;
          pointer+=1;
        }
        if ( sign ) {
          for (size_t n = 0; n < strlen(target); n++) {
            vector_erase(this->vec, i);
          }
          for (size_t n = 0; n < strlen(substitution); n++) {
            if (i == vector_size(this->vec)) {
              vector_push_back(this->vec, substitution+n);
              i+=1;
            } else{
              vector_insert(this->vec, i, substitution+n);
              i+=1;
            }
          }
          return 0;
        }
        sign = 1;
      }
    }
    return -1;
}

char *sstring_slice(sstring *this, int start, int end) {
    // your code goes here
    sstring * str = malloc(sizeof(sstring));
    str->vec = char_vector_create();
    for (int i = start; i < end; i++) {
        //sstring_append(this, cstr_to_sstring(vector_get(this->vec, i)));
        vector_push_back(str->vec,vector_get(this->vec, i));
    }
    return sstring_to_cstr(str);
}

void sstring_destroy(sstring *this) {
    // your code goes here
    vector_destroy(this->vec);
    //free(this);
}


