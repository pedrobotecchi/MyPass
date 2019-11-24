/******@<$pref>::>*****/

#define MAXSYMTAB 0x1000 
#define MAXIDLEN 256

typedef struct __symtab__ {
	char name[MAXIDLEN];
	int  typedescriptor;
	int  lexlevel;
} SYMTAB;

extern int symtab_initial,symtab_final;

extern SYMTAB symtab[];

/* symtab_lookip looks up for a given symbol and, if exists returns
 * the current symbol descriptor */
int symtab_lookup(char* symbol);

/* symtab_append does append a given symbol just defined in
 * declarative scope */
int symtab_append(char* symbol);

extern int symtab_descriptor;

int symtab_type_range(int type_descriptor);

