/*****************<main.h>::************************/
#include<stdio.h>
#include<parser.h>
#include<string.h>

FILE *source, *object;

int main(int argc, char *argv[]){
	if (argc < 2){
		fprintf(stderr, "%s: missing file name ....");
		return 0;
	}

	int i=0;
	while(argv[1][i] != '.')	i++;
	if(strcmp(".pas", &argv[1][i]) == 0 || strcmp(".mypas", &argv[1][i]) == 0) 	;
	else{
		printf("extension mypas or pas expected\n");
		return 0;
	}



	source = stdin;
	object = stdout;
	lookahead = gettoken(source);
	mypass();
	return 0;
}
