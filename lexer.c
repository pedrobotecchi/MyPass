		/*
 * @<lexer.c>::
 */

/*******************************************************
* Author: Eraldo Pereira Marinho, PhD
  Co-Authores:
			Bruna F. Lamim
			Guilherme M. Caes
			Pedro H. Botecchi
* Creation Date: Aug 13 19:20:59 -03 2019
********************************************************/

/*
 * Módulo para implementação de métodos de varredura léxica
 * *************************************************
 */

/* Include Session */
#include <stdio.h>
#include <ctype.h>
#include <tokens.h>
#include <lexer.h>
#include <keyword.h>
#include <main.h>
#include <string.h>

/* Max size for the index used in lexeme */
#define HIST_SIZE 100

/* Var that controls the index of lexeme */
int i = 0;

/*
 * @ skipspaces:: 
 */
/* This functions checks for spaces in the tape, ignoring them */
int linenumber = 1;
void
skipspaces(FILE * tape)
{
    int            head;

	/* While the head read is a space, just consume the tape */
    while (isspace(head = getc(tape)))
    {
		/* Check if it's a enter, this helps to count the number of lines used  */
		if(head == '\n')
			linenumber++;
	}
    ungetc(head, tape);
}

/* This function skips the comments marked with { } */
void skipcomments(FILE* tape){
	int head;
	skipspaces(tape); 	/* Call skipspaces */
_skipspaces:
	if((head = getc(tape)) == '{'){					/* If the head of the tape is { read it until reach a } */
		while( (head = getc(tape)) != '}')	
			if( head == EOF)	return;				/* Return if the comment ends the file */
			else if (head == '\n') linenumber++;	/* Countinue counting the number of lines */
			
		goto _skipspaces;	
	}
	ungetc(head, tape);		/* If it's not a comment, return the read character */
	skipspaces(tape);
}

/*
 * @ isID:: 
 */
char lexeme[MAXIDLEN+1];		/* Save the value that will be read from the tape */

/* This function checks the tape, reading it's values and return if what was read is a ID or not */
int
isID(FILE * tape)
{

    int i =0;
    token_t token;

    lexeme[i] = getc(tape);
    if (isalpha(lexeme[i])) {		/* If the first character is alphabetic can be an ID */

    	i++;
		/* While the next character read is an alphnumeric character, and i does't surpass MAXIDLEN +1 save the character in lexeme */
        while ((i<MAXIDLEN-1) && ((isalnum(lexeme[i] = getc(tape))) || lexeme[i] == '_')) 	i++;	
        ungetc(lexeme[i], tape);		/* Return the character read that it's not alphanumeric */
        lexeme[i] = 0;
		/* Check if what was read is a lexeme or not, if  */
		if(token = iskeyword(lexeme)) 	return token;
		/* Else return ID */
		return ID;
	}

    ungetc(lexeme[i], tape); 
    lexeme[i] = 0;
    return 0;
}

/*
 * @ isEE:: 
 */
/* This function checks Exponential notation for number*/
int isEE(FILE* tape){

	int j = i;

	if(toupper(lexeme[i] = getc(tape)) == 'E'){		// Checa se veio e ou E na fita

		i++;

		if((lexeme[i] = getc(tape)) == '+' || lexeme[i] == '-')		i++;					// Checa existencia de +|-
		else
		{
			ungetc(lexeme[i], tape);   // Caso nao haja, devolve char
		}													


		if(isdigit(lexeme[i] = getc(tape))){							// Checha se um numero segue (E|e)?(+|-)
			i++;

			while(isdigit(lexeme[i] = getc(tape)) && i < HIST_SIZE)	// Consome digitos que seguem
				i++;												// se houver espaco no buffer

			if(i == HIST_SIZE)		goto BUF_OVERFLOW;				// Numero grande demais para o buffer, caso em
																	// que o lexer devolve tudo

			ungetc(lexeme[i], tape);									// Devolve primeiro nao digito consumido

			lexeme[i] = 0;
			
 			return FLT;						// Retorna TRUE 
		}	
	}
	ungetc(lexeme[i],tape);
	lexeme[i] = 0;
	goto END;

BUF_OVERFLOW:

	for( ; i >= 0; i--)		ungetc(lexeme[i], tape);					// Devolve todos os elementos lidos para a fita
	
END:
	return 0;														// Retorna FALSE
}


/*
 * @ isFloat:: 
 */
/* This function checls if the lexeme is FLOAT or INTEGER */
int isFLOAT(FILE* tape){

	i = 0;

	int token = 0;
	lexeme[i] = getc(tape);

	if(isdigit(lexeme[i])){
		i++;

		token = UINT;											// Primeiro char eh digito, token atualizado para UINT

		if(lexeme[i-1] == '0')
		{
			lexeme[i] = getc(tape);								// Somente o int 0 pode ser ter um 0 a esqueda
			i++;
		}		
		else				while(isdigit(lexeme[i] = getc(tape)))i++	;	// Consome digitos seguintes

		if(lexeme[i] == '.'){												// Checa existencia de . para FLOAT
			i++;
			while(isdigit(lexeme[i] = getc(tape)))i++;					// Consome digitos seguintes e marca
			token = FLT;												// token como FLOAT
		}

		ungetc(lexeme[i], tape);										// Devolve primeiro nao digito consumido

		if(isEE(tape))		token = FLT;								// Consome notacao exponencial, se existir
			
		return token;													// Retorna token identificado	

	} else if (lexeme[i] == '.'){									// Primeiro char eh '.', pode ser um float
		i++;
		if(isdigit(lexeme[i] = getc(tape))){									// Checa se eh seguido por numero
			i++;

			token = FLT;												// Atualiza token para FLOAT
			while(isdigit(lexeme[i] = getc(tape)))i++	;						// Consome digitos 
			ungetc(lexeme[i], tape);											// Devolve primeiro nao digito


			isEE(tape);													// Consome notacao exponencial, se houver

			return token;												// Retorna Float
		}

		ungetc(lexeme[i], tape);										// Caso o ponto nao seja seguido por um numero, eh
		ungetc('.', tape);										// necessario devolver o ultimo char lido mais o pto
		i--;
		return token;											// Nao eh FLOAT nem INT, token = 0
	}

	ungetc(lexeme[i], tape);		
	lexeme[i] = 0;												// Devolve char lido
	return token;												// Retorna token = 0;

}

/*
 *	@isASGN::
 */
/* This function checks the assignment sign */
int isASGN(FILE *tape)
{
	lexeme[i] = getc(tape);				/* Read the tape */

	if (lexeme[i] == ':') {				/* If it's a ':' */
		i++;
		if ( (lexeme[i] = getc(tape)) == '=' ) {		/* Check if the next is a '=' */
			i++;
			lexeme[i] = 0;
			return ASGN;
		}
		ungetc(lexeme[i], tape);		/* Else return the character read */
		ungetc(':', tape);				/* And return : read first */
		
		i--;
		return 0;
	}
	ungetc(lexeme[i], tape);			/* Just return the what was read in the place of ':' */
	return 0;
}

/*
 * @isRELOP:: 
 */
/* This function checks if the lexeme read is a Relational Operation */
token_t isRELOP(FILE* tape){
	
	lexeme[2] = lexeme[1] = 0;				/* Set up lexeme */
	switch(lexeme[0] = getc(tape)){			/* Check which sign lexeme is (<,>) */
		case'<':
			switch(lexeme[1] =getc(tape)){	/* Check the second character to see if it's a combination */
				case '=':
					return LEQ;				/* if it's <= */
				case '>':
					return NEQ;				/*if it's <>*/
			}
			ungetc(lexeme[1], tape);		/* return the second character read and just return < */
			lexeme[1] = 0;
			return lexeme[0];
		case'>':							
			if((lexeme[1] = getc(tape)) == '=')	return GEQ;		/* if it's >=*/
			ungetc(lexeme[1], tape);		/* unget the second */
			lexeme[1] = 0;		
			return lexeme[0];				/* Return just > */
	}
	ungetc(lexeme[0], tape);				/* Return the first character read case the switch failed */
	return 0;
}

/*
 * @ isCHR:: 
 */
/* Function that checks Char patterns */
// Falta tratar casos que requerem a \ como \', \0, \\ ...
token_t isCHR(FILE* tape){

	if((lexeme[0] = getc(tape)) == '\''){
		lexeme[1] = getc(tape);
		if( (lexeme[2] = getc(tape)) == '\''){
			lexeme[3] = 0;
			return CHR;
		}
		ungetc(lexeme[2], tape);
		ungetc(lexeme[1], tape);
	}
	ungetc(lexeme[0], tape);
	return 0;
}

/*
 * @ isSTR:: 
 */
/* Function that cheks string patterns */
// Falta tratar casos que requerem a \ como \', \0, \\ ...
token_t isSTR(FILE* tape){
	
	i = 1;

	if((lexeme[0] = getc(tape)) == '\"'){
		while((lexeme[i] = getc(tape)) != '\"'){
			if(lexeme[i]==EOF)	return EOF;
			i++;
		}
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return STR;
	}

	ungetc(lexeme[0], tape);
	return 0;
}

/*
 * lexer to parser interface : @gettoken:: 
 */
int
gettoken(FILE * source)
{
    int             token;

    /*
     * ignore comments and spaces
     */
    skipcomments(source);

    /*
     * lexical analysers are called hereafter: 
     */

    if (token = isID(source))
        return token;
    if (token = isFLOAT(source))
        return token;
    if (token = isASGN(source))
        return token;
    if (token = isSTR(source))
        return token;
    if (token = isCHR(source))
        return token;
	if (token = isRELOP(source))
		return token;

    lexeme[1] = 0;

	//printf("LEXEME : %s",lexeme);
    /*
     * return default token, say an ASCII value 
     */
    return getc(source);
}





