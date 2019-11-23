		/*****************<main.h>::************************/
#include<stdio.h>
#include<parser.h>
#include<string.h>

FILE *source, *object;

int main(int argc, char *argv[]){
	if (argc < 2){
		fprintf(stderr, "%s: missing file name ....", argv[1]);
		return 0;
	}

	int i=0;
	while(argv[1][i] != '.')	i++;
	if(strcmp(".pas", &argv[1][i]) == 0 || strcmp(".mypas", &argv[1][i]) == 0) 	;
	else{
		printf("extension mypas or pas expected\n");
		return 0;
	}

 	source = fopen(argv[1], "r");
	if(source == NULL){
		printf("File Error: Couldn't open %s\n", argv[1]);
		return 0;
	}

	object = stdout;
	lookahead = gettoken(source);
	mypass();
	return 0;
}
