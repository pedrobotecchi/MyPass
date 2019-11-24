/****@<parser.h>::******
 *
 * 
 */

#include<lexer.h>

typedef int type_t;

void mypass(void);
void symtab_print(void);
void declscope(void);
void varlst(void);
void vartype(void);
void parmdef(void);
void procdecl(void);
void stmblock(void);
void stmlst(void);
void exprlst(void);
void ifstmt(void);
void whilestm(void);
void repstm(void);
void stmt(void);
type_t expr(type_t);
type_t smpexpr(type_t);
type_t term(type_t);
type_t fact(type_t);
int isCompat(type_t t1, type_t t2);
void match(int expected);

extern token_t lookahead;
extern FILE *source, *object;
