/****@<parser.h>::******
 *
 * 
 */

#include<lexer.h>

void mypass(void);
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
void expr(void);
void smpexpr(void);
void term(void);
void fact(void);

void match(int expected);

extern token_t lookahead;
extern FILE *source, *object;
