/*****@<keywords.c>::********/

#include<keyword.h>

// Mudar letras para minusculo
char *keyword[] = {
	 "begin",
	 "program", 
	 "var",
	 "int",
	 "long",
	 "float",
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

#include<string.h>
int iskeyword(char* name){

	int token;
	for(token = BEGIN; token < END+1; token++){
		if(strcmp(keyword[token - BEGIN], name))	return token;	
	}
	return 0;
}

