#include<symtab.h>
#include<string.h>
#include<lexer.h>

SYMTAB symtab[MAXSYMTAB];

/* symtab_lookip looks up for a given symbol and, if exists returns
 * the current symbol descriptor */
int symtab_lookup(char* symbol){
	int stbd; //symtab descriptor
	for(stbd = symtab_descriptor-1; stbd; stbd--){
		if(strcmp(symbol, symtab[stbd].name) == 0)	break;
	}
	return stbd;
}
/* symtab_append does append a given symbol just defined in
 * declarative scope */
int symtab_append(char* symbol){
	strcpy(symtab[symtab_descriptor].name, symbol);
	return symtab_descriptor++;
}

int symtab_descriptor = 1;

int symtab_type_range(int type_descriptor){
	int stbd;
	int count = 0;
	for(stbd=symtab_initial; stbd<symtab_final; stbd++){
		symtab[stbd].typedescriptor = type_descriptor;
		count++;
	}
	return count;
}

