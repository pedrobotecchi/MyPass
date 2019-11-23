/****@<lexer.h>::******
 *
 *
 * 
 */

#pragma once

#define MAXIDLEN 256
typedef int token_t;

extern char lexeme[MAXIDLEN+1];

#include<stdio.h>
token_t gettoken(FILE* source);
token_t isRELOP(FILE* tape);
