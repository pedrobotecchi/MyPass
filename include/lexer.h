/****@<lexer.h>::******
 *
 *
 * 
 */

#pragma once

#define MAXIDLEN 256
typedef int token_t;

extern char lexeme[MAXIDLEN+1];
extern int linenumber;

#include<stdio.h>
token_t gettoken(FILE* source);
token_t isRELOP(FILE* tape);
