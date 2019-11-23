/*****************<main.h>::*************************/
/*	Implemented by:									*/
/*					Bruna F. Lamim					*/
/*					Guilherme M. Cares				*/
/*					Pedro H. Botecchi				*/
/***************************************************/

/* Include Section */
	#include<stdio.h>
	#include<parser.h>

FILE *source, *object;

int main(int argc, char *argv[]){
	
	/* Check if the file was passed */	
	if (argc < 2){
		fprintf(stderr, "%s: missing file name .... Aborting");
		return 0;
	}

	source = stdin;
	object = stdout;

	/* Get the first token */
	lookahead = gettoken(source);

	/* Call the parser */
	mypass();

	return 0;
}
