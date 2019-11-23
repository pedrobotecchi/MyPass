		/*
 * @<lexer.c>::
 */

/*******************************************************
* Author: Eraldo Pereira Marinho, PhD
* Creation Date: Aug 13 19:20:59 -03 2019
********************************************************/

/*
 * Módulo para implementação de métodos de varredura léxica
 * *************************************************
 */

#include <stdio.h>
#include <ctype.h>
#include <tokens.h>
#include <lexer.h>
#include <keyword.h>
#include <main.h>
#include <string.h>

#define HIST_SIZE 100

int i = 0;

/*
 * @ skipspaces:: 
 */
int linenumber = 1;
void
skipspaces(FILE * tape)
{
    int            head;

    while (isspace(head = getc(tape)))
    	{
		if(head == '\n')
			linenumber++;
	}
    ungetc(head, tape);
}

void skipcomments(FILE* tape){
	int head;
	skipspaces(tape);
_skipspaces:
	if((head = getc(tape)) == '{'){
		while( (head = getc(tape)) != '}')	
			if( head == EOF)	return;
		goto _skipspaces;	
	}
	ungetc(head, tape);
	skipspaces(tape);
}

/*
 * @ isID:: 
 */
char lexeme[MAXIDLEN+1];
int
isID(FILE * tape)
{

    int i =0;
    token_t token;

    lexeme[i] = getc(tape);
    if (isalpha(lexeme[i])) {

    	i++;

        while ((i<MAXIDLEN-1) && ((isalnum(lexeme[i] = getc(tape))) || lexeme[i] == '_')) 	i++;
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;

		if(token = iskeyword(lexeme)) 	return token;
		
		return ID;
	}

    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

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
int isASGN(FILE *tape)
{
	lexeme[i] = getc(tape);

	if (lexeme[i] == ':') {
		i++;
		if ( (lexeme[i] = getc(tape)) == '=' ) {
			i++;
			lexeme[i] = 0;
			return ASGN;
		}
		ungetc(lexeme[i], tape);
		ungetc(':', tape);
		
		i--;
		return 0;
	}
	ungetc(lexeme[i], tape);
	return 0;
}

/*
 * lexer to parser interface: @ gettoken:: 
 */

token_t isRELOP(FILE* tape){
	
	lexeme[2] = lexeme[1] = 0;
	switch(lexeme[0] = getc(tape)){
		case'<':
			switch(lexeme[1] =getc(tape)){
				case '=':
					return LEQ;
				case '>':
					return NEQ;
			}
			ungetc(lexeme[1], tape);
			lexeme[1] = 0;
			return lexeme[0];
		case'>':
			if((lexeme[1] = getc(tape)) == '=')	return GEQ;
			ungetc(lexeme[1], tape);
			lexeme[1] = 0;
			return lexeme[0];
	}
	ungetc(lexeme[0], tape);
	return 0;
}

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

    lexeme[1] = 0;

	printf("LEXEME : %s",lexeme);
    /*
     * return default token, say an ASCII value 
     */
    return getc(source);
}





