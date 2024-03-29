/******@<$pref>::>*****/

#define MAXSYMTAB 0x1000000 

/***/extern int symtab_initial, symtab_final; /***/

typedef struct __symtab__ {
	char name[MAXIDLEN];
	int  typedescriptor;
} SYMTAB;

extern SYMTAB symtab[];

extern int symtab_descriptor;

extern int symtab_initial, symtab_final;

/* symtab_lookip looks up for a given symbol and, if exists returns
 * the current symbol descriptor */
int symtab_lookup(char* symbol);

/* symtab_append does append a given symbol just defined in
 * declarative scope */
int symtab_append(char* symbol);


int symtab_type_range(int type_descriptor);

