/*****@<keywords.c>::********/

/* Include Session */
#include<keyword.h>
#include<string.h>

char *keyword[] = {
	 "begin",
	 "program", 
	 "var",
	 "integer",
	 "long",
	 "real",
	 "double",
	 "boolean",
	 "char",
	 "string",
	 "procedure",
	 "function",
	 "if",
	 "then",
	 "else",
	 "while",
	 "do",
	 "repeat",
	 "until",
	 "end",
};

/* This function checks if a name is a KeyWord from the list above, if it is return the token, else return 0 */
int iskeyword(char* name){

	int token;
	for(token = BEGIN; token < END+1; token++){
		if(strcmp(keyword[token - BEGIN], name) == 0)	return token;	
	}
	return 0;
}

