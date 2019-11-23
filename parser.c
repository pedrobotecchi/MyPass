/***@parser.c>::***/

/*******************************************************************************
 * Author: Prof. Eraldo Pereira Marinho, Ph.D.
 * Local standard date: ter out 29 09:38:03 -03 2019
 ******************************************************************************/

/*******************************************************************************
 Syntax for simplified / modified Pascal, namely MyPas Project

 EBNF Grammar

 mypas: initial nonterminal symbol

 mypas -> [ PROGRAM ID (input ',' output) ';' ] declscope stmblock '.'
 input -> ID
 output -> ID
 input, output: standard buffer handlers

--------------------------------------------------------------------------------
declscope: declarative scope

declscope -> VAR varlst ':' vartype ';' { VAR varlst ':' vartype ';' } procdecl
varlst -> ID { ',' ID }
vartype -> INT | LONG | FLOAT | DOUBLE | BOOLEAN | CHAR | STRING
procdecl -> { PROCEDURE parmdef ';' declscope stmblock |
              FUNCTION  parmdef ':' vartype ';' declscope stmblock }
stmblock -> BEGIN stmtlst END
stmlst -> stmt { ';' stmt }

-------------------------------------------------------------------------------
stmt: statement definitions

stmt -> stmblock | ifstm | whilestm | repstm | fact
ifstm -> IF expr THEN stmt [ ELSE stmt ]
whilestm -> WHILE expr DO stmt
repstm -> REPEAT stmlst UNTIL expr

-------------------------------------------------------------------------------
expr: the top expression case, which can be either simple logic-arithmetic or
      relational

expr -> smpexpr [ RELOP smpexpr ]
RELOP = '<' | NEQ | LEQ | '=' | GEQ | '>'  
NEQ = "<>"
LEQ = "<="
GEQ = ">="

smpexpr -> ['+'|'-'] term { OPLUS term }
OPLUS = " + | - " | OR
term -> fact { OTIMES fact }
OTIMES = " * | / " | DIV | MOD | AND
fact ->   '(' expr ')'
	| NUM
	| CHR
	| STR
	| TRUE
	| FALSE
	| ID [ ":=" expr ]
NUM = UINT | FLT
CHR = \'[0x00-0xFF]\' (ASCII)
STR = \"CHR*\"
ID = [A-Za-z][A-Za-z0-9_]*

Notes:: 
 1. low case variable means nonterminals whereas upper case means tokens, or
 terminal symbols. Of course, explicit ASCII characters are standard terminals;
 2. this is not Pascal, neither C.
*******************************************************************************/

/**** Atividade:
 * 1. Com base na sintaxe acima exposta, implemente um analisador sintático (parser)
 * para cada uma das produções. Observe que os tokens serão tratados no nível léxico
 * mas esses podem ser, em alguns casos, tratados diretamente no parser, na forma
 * de funções predicado.
 *
 * 2. Aumente o analisador léxico para os tokens que sejam mais bem definidos
 * na parte léxica do pré-compilador.
 ****/


/*******************************************************************************
 Syntax for simplified / modified Pascal, namely MyPas Project

 EBNF Grammar

 mypas: initial nonterminal symbol

 mypas -> [ PROGRAM ID (ID',' ID) ';' ] declscope procdecl stmblock '.'
 input, output: standard buffer handlers

 */

#include<tokens.h>
#include<lexer.h>
#include<keyword.h>
#include<parser.h>
#include<symtab.h>

token_t lookahead;
char lexeme[MAXIDLEN+1];

void mypass(void){
	if (lookahead == PROGRAM){
		match(PROGRAM); match(ID); match('(');
		match(ID); match(';'); match(ID); match(')');
		match(';');
		}
	declscope();
	procdecl();
	stmblock();
	match('.');
}

int symtab_initial = 0;
int symtab_final = 0;

/**************************************************************************************
declscope -> {VAR varlst ':' vartype ';' } 
***************************************************************************************/

void declscope(void)
{
	while(lookahead == VAR){
		/** a variable must be registered here *****/
		match(VAR);
		/***/ 
		symtab_initial = symtab_descriptor;
		/***/ 
		varlst();
		/***/ 
		symtab_final = symtab_descriptor;
		/***/ 
		match(':');
		vartype();
		match(';');
	}
}

/**************************************************************************************
varlst -> ID { ',' ID }
***************************************************************************************/
int fatalerrorcount = 0;
void varlst(void)
{
_varlst:
	/**/if(symtab_lookup(lexeme)){
		/*** symbol already declared ***/
		fatalerrorcount++;
	} else {
		/*** appends new symbol in the table ***/	
		symtab_append(lexeme);
	}/**/
	match(ID);
	if(lookahead == ','){
		match(',');
		goto _varlst;
	}
}
/**************************************************************************************
vartype -> INT | LONG | FLOAT | DOUBLE | BOOLEAN | CHAR | STRING
***************************************************************************************/
void vartype(void)
{
	switch(lookahead){
		case INTEGER:
			/***/
			symtab_type_range(1);
			/***/
			match(INTEGER);
			break;
		case LONG:
			/***/
			symtab_type_range(2);
			/***/
			match(LONG);
			break;
		case REAL:
			/***/
			symtab_type_range(3);
			/***/
			match(REAL);
			break;
		case DOUBLE:
			/***/
			symtab_type_range(4);
			/***/
			match(DOUBLE);
			break;
		case BOOLEAN:
			/***/
			symtab_type_range(5);
			/***/
			match(BOOLEAN);
			break;
		case CHAR:
			/***/
			symtab_type_range(6);
			/***/
			match(CHAR);
			break;
		case STR:
			/***/
			symtab_type_range(7);
			/***/
			match(STR);
			break;
		default:
			match(STRING);
	}

}

/**************************************************************************************
procdecl -> { PROCEDURE ID parmdef ';' declscope stmblock |
              FUNCTION  ID parmdef ':' vartype ';' declscope stmblock }
***************************************************************************************/

void procdecl(void)
{
	int isfunc;
	while(lookahead == PROCEDURE || (isfunc = lookahead) == FUNCTION){
		match(lookahead);
		match(ID);
		parmdef();
		if(isfunc){
			match(':');
			vartype();
		}
		match(';');
		declscope();
		stmblock();
	}
}

/*************************************************************************************
  parmdef -> ['(' [VAR] varlst ':' vartype {';' [VAR] varlst ':' vartype } ')']			// Se tiver VAR, o parametro é passado por referencia
**************************************************************************************/ 
void parmdef(void){

	if(lookahead == '(')
	{
		match('(');
_parmdef:
		if(lookahead == VAR)	match(VAR);
		/***/ 
		symtab_initial = symtab_descriptor;
		/***/ 
		varlst();
		/***/ 
		symtab_final = symtab_descriptor;
		/***/ 
		match(':');
		vartype();
		if(lookahead == ';')
		{
			match(';');
			goto _parmdef;
		}
		match(')');
	}
}

/**************************************************************************************
stmblock -> BEGIN stmtlst END
***************************************************************************************/
void stmblock(void)
{
	match(BEGIN);
	stmlst();
	match(END);
}

/**************************************************************************************
stmlst -> stmt { ';' stmt }
***************************************************************************************/
void stmlst(void)
{
_stmt:
	stmt();
	if(lookahead == ';'){
		match(';');
		goto _stmt;
	}
}

type_t type_checker = 0;

/******************************************************************************
stmt: statement definitions
stmt -> stmblock | ifstm | whilestm | repstm | fact
******************************************************************************/
void stmt()
{
	switch(lookahead){
		case BEGIN:		
			stmblock();
			break;
		case IF:
			ifstmt();
			break;
		case WHILE:		
			whilestm();
			break;
		case REPEAT:
			repstm();
			break;
		default:
			fact();
	}
}
/******************************************************************************
ifstm -> IF expr THEN stmt [ ELSE stmt ]
******************************************************************************/
void ifstmt()
{
	match(IF);
	expr(type_checker);
	match(THEN);
	stmt();
	if(lookahead == ELSE){
		match(ELSE);
		stmt();
	}
}

/******************************************************************************
whilestm -> WHILE expr DO stmt
******************************************************************************/
void whilestm()
{
	match(WHILE);
	expr(type_checker);
	match(DO);
	stmt();
}

/******************************************************************************
repstm -> REPEAT stmlst UNTIL expr
******************************************************************************/
void repstm()
{
	match(REPEAT);
	stmlst();
	match(UNTIL);
	expr(type_checker);
}

/******************************************************************************
expr: the top expression case, which can be either simple logic-arithmetic or
      relational

expr -> smpexpr [ RELOP smpexpr ]
RELOP = '<' | NEQ | LEQ | '=' | GEQ | '>'  
NEQ = "<>"
LEQ = "<="
GEQ = ">="
******************************************************************************/
int isOREL()
{
	switch(lookahead){
		case '<':	return '<';
		case '>':	return '>';
		case '=':	return '=';
		case NEQ:	return NEQ;
		case LEQ:	return LEQ;
		case GEQ:	return GEQ;
		default:	return 0;
	}
}

type_t expr(type_t p_type)
{
	/***/
	type_t t1,t2=0;
	/***/
	int relop;
_expr:
	t1 = smpexpr(p_type);
	if(relop = isOREL()){
		match(lookahead);
		t2 = smpexpr(max(t1,p_type));
		if(lookahead!=')')
			goto _expr;
	}
	if(t2) return t2;
	return t1;
	
}
/******************************************************************************
smpexpr -> ['+'|'-'] term { OPLUS term }
OPLUS = " + | - " | OR
******************************************************************************/
int isOPLUS(){
	switch(lookahead){
		case '+':	return '+';
		case '-':	return '-';
		case OR:	return OR;
		default:	return 0;
	}
}

type_t smpexpr(type_t p_type)
{
	int oplus;
	/***/
	type_t acctype = 0;
	/***/
	
	if(isNEG())
		match(lookahead);
_term:
	p_type = max(p_type, acctype);
	if((oplus = lookahead) == '+')		match('+');

	acctype = term(p_type);
	if(oplus= isOPLUS()){
		match(lookahead);
		goto _term;
	}

	return acctype;
}
/******************************************************************************
term -> fact { OTIMES fact }
OTIMES = " * | / " | DIV | MOD | AND
******************************************************************************/
int isOTIMES(){
	switch(lookahead){
		case '*':	return '*';
		case '/':	return '/';
		case DIV:	return DIV;
		case MOD:	return MOD;
		case AND:	return AND;
		default:	return 0;
	}
}

type_t term(type_t p_type)
{
	int otimes;
	
	/***/
	type_t acctype = 0;
	/***/
_fact:
	p_type = max(p_type, acctype);
	acctype = fact(p_type);
	if(otimes = isOTIMES()){
		match(otimes);
		goto _fact;
	}

	return acctype;
}

void exprlst(void){
	/**********************************
	 * exprlst -> expr {',' expr }	 	
	 * ********************************/
_expr:
	expr(0);
	if(lookahead == ';'){
		match(',');
		goto _expr;
	}
}

/******************************************************************************
fact ->   '(' expr ')'
	| NUM
	| CHR
	| STR
	| TRUE
	| FALSE
	| ID [ ":=" expr ]
NUM = UINT | FLT
CHR = \'[0x00-0xFF]\' (ASCII)
STR = \"CHR*\"
ID = [A-Za-z][A-Za-z0-9_]*
******************************************************************************/

#define MAX_SIZE_INT 2147483647 
#define MAX_SIZE_LONG
#define MAX_SIZE_FLOAT 1e24

type_t fact(type_t p_type)
{
	/***/
	int var_descr; 
	type_t acctype;
	type_t old_acctype;
	/***/
	
	switch(lookahead){
		case ID:
			/***/
			var_descr = symtab_lookup(lexeme);
			acctype = symtab[var_descr].typedescriptor;
			/***/	
			match(ID);
			if(lookahead == ASGN){
				match(ASGN);
				if(isCompat(acctype, old_acctype = expr(p_type)))
					acctype = max(acctype,old_acctype);		 					
				else
					printf("FATAL ERROR - EXPRESSION TYPE NOT MATCH. EXPECTED "); // IMPLEMENTAR MENSAGEM DE ERRO com ENUM?
			} else if (lookahead == '('){
				match('('); exprlst(); match(')');
			}
			
			break;
		case UINT: 
			if(atof(lexeme) <= (float)MAX_SIZE_INT) {  // CONFIRMAR O CAST
				if(isCompat(1, p_type))	acctype = max(1,p_type);		
				else printf("FATAL ERROR - EXPRESSION TYPE NOT MATCH. EXPECTED "); // IMPLEMENTAR MENSAGEM DE ERRO com ENUM?
			}
			else {
				if(isCompat(2, p_type))	{
					acctype = max(2,p_type);
					symtab[var_descr].typedescriptor = 2;
					acctype = max(2,p_type);
				}
			}
			match(UINT);
			break;
		case FLT:									// Implementar algo que verifique o tamanho do numero para ver o tipo entre long e double
			if(ato(lexeme) <= (double)MAX_SIZE_FLOAT) {  // CONFIRMAR O CAST ACHAR UM JEITO DE CONVERTER PARA DOUBLE  
				if(isCompat(3, p_type))	acctype = max(3,p_type);		
				else printf("FATAL ERROR - EXPRESSION TYPE NOT MATCH. EXPECTED "); // IMPLEMENTAR MENSAGEM DE ERRO com ENUM?
			}
			else {
				if(isCompat(4, p_type))	{
					acctype = max(4,p_type);
					symtab[var_descr].typedescriptor = 4;
					acctype = max(4,p_type);
				}
			}
			/***/
			acctype = max(3,p_type);
			/***/
			match(FLT);
			break;
		case CHR:
			match(CHR);
			break;
		case STR:
			match(STR);
			break;
		case TRUE:
			match(TRUE);
			break;
		case FALSE:
			match(FALSE);
			break;
		default:
			match('(');
			acctype = expr(p_type);
			match(')');
		return max(acctype, p_type);	
	}
}

int isCompat(type_t t1, type_t t2){
	if(t1 == t2) return 1;
	else
	{
		if(t1>=1 || t1<=4)
		{
			if(t2>=1 || t2<=4)
				return 1;
		}
	}
	return 0;
}

// ** This function mathces an token and read the next when it's the expected
// ** Otherwise it's print an error
void match(int expected){
	if(lookahead == expected)
	{
		lookahead = gettoken(source);
		fprintf(object, "lookahead : %d, lexeme : %s \n", lookahead,lexeme);		
	}		// ** If I got what i wanted, get next token
	else{
		fprintf(stderr, "%d seen while %d expected\n", lookahead, expected);	// ** Else, error showing what i wanted
	}	
} 


















