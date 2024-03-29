 /*
 * @<symtab.c>::
 */

/*******************************************************
* Author: Eraldo Pereira Marinho, PhD
  Co-Authores:
			Bruna F. Lamim
			Guilherme M. Caes
			Pedro H. Botecchi
* Creation Date: Aug 13 19:20:59 -03 2019
********************************************************/
/* Include Session */
#include<symtab.h>
#include<string.h>
#include<lexer.h>
#include<parser.h>

SYMTAB symtab[MAXSYMTAB];  /* Symtab declaration */

/* symtab_lookip looks up for a given symbol and, if exists returns
 * the current symbol descriptor */
int symtab_lookup(char* symbol){
	int swap_lex = lex_level;
	int stbd; //symtab descriptor
	for(stbd = symtab_descriptor-1; stbd; stbd--){
		if(strcmp(symbol, symtab[stbd].name) == 0)
		{
			/* Checks for vars in the same lex level */
			if(symtab[stbd].lexlevel == lex_level)
				break;
		}	
	}
	return stbd;
}
/* symtab_append does append a given symbol just defined in
 * declarative scope */
int symtab_append(char* symbol){
	strcpy(symtab[symtab_descriptor].name, symbol);
	symtab[symtab_descriptor].lexlevel = lex_level;
	return symtab_descriptor++;
}

int symtab_descriptor = 1;

int symtab_type_range(int type_descriptor){
	int stbd;
	int count = 0;
	/* If inserted to put fucntions and procedures in the Symtab, initially type for them is setted in 1 and then changed here for fucntions */
	/* Procedures have type 0 */
	if(symtab_procfunc)
	{
		for(stbd=symtab_final; stbd>0; stbd--){
			if((symtab[stbd].lexlevel == lex_level)&&(symtab[stbd].typedescriptor == 0))
			{
				symtab[stbd].typedescriptor = type_descriptor;
				count++;
			}
		}
	}
	else
	{
		for(stbd=symtab_initial; stbd<symtab_final; stbd++){
			if(symtab[stbd].lexlevel == lex_level)
			{
				symtab[stbd].typedescriptor = type_descriptor;
				count++;
			}
		}
	}
	
	return count;
}

/* Remove all vars in a lex level after a procedure or function */
void symtab_remove_lexVAR(){
	while(symtab[symtab_descriptor-1].lexlevel == lex_level) symtab_descriptor--;
}