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

 mypas -> [ PROGRAM ID (ID',' ID) ';' ] declscope stmblock '.'
 input, output: standard buffer handlers

 */

#include<tokens.h>
#include<lexer.h>
#include<keyword.h>
#include<parser.h>

token_t lookahead;
char lexeme[MAXIDLEN+1];

/***************************************************************
 mypas -> [ PROGRAM ID (ID',' ID) ';' ] declscope stmblock '.'
 input, output: standard buffer handlers
***************************************************************/

void mypass(void){
	
	if (lookahead == PROGRAM){
		match(PROGRAM); match(ID); match('(');
		match(ID); match(','); match(ID); match(')');
		match(';');
		}
	declscope();
	stmblock();
}

/**************************************************************************************
declscope -> {VAR varlst ':' vartype ';'  VAR varlst ':' vartype ';' } procdecl
***************************************************************************************/
void declscope(void)
{
	while(lookahead == VAR){
		match(VAR);
		varlst();
		match(':');
		vartype();
		match(';');
	}
	procdecl();
}

/**************************************************************************************
varlst -> ID { ',' ID }
***************************************************************************************/
void varlst(void)
{
_varlst:
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
			match(INTEGER);
			break;
		case LONG:
			match(LONG);
			break;
		case REAL:
			match(REAL);
			break;
		case DOUBLE:
			match(DOUBLE);
			break;
		case BOOLEAN:
			match(BOOLEAN);
			break;
		case CHAR:
			match(CHAR);
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
	int isfunc = 0;
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
		switch(lookahead) {
			case ';' :
				match(';');
				break;
			default '.' :
				match('.');
		}
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
		varlst();
		match(':');
		vartype();
		if(lookahead == ',')
		{
			match(',');
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
	expr();
	match(THEN);
	stmt();
	//printf("\nlookahead: %d, %s* \n", lookahead,lexeme);
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
	expr();
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
	expr();
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

void expr()
{
	int relop;
_expr:
	smpexpr();
	if(relop = isOREL()){
		match(relop);
		goto _expr;
	}
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

void smpexpr()
{
	int oplus;
_term:
	if((oplus = lookahead) == '+')			match('+');
	else if((oplus = lookahead) == '-')		match('-');

	term();
	if(oplus= isOPLUS()){
		match(oplus);
		goto _term;
	}
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

void term()
{
	int otimes;
_fact:
	fact();
	if(otimes = isOTIMES()){
		match(otimes);
		goto _fact;
	}
}

void exprlst(void){
	/**********************************
	 * exprlst -> expr {',' expr }	 	
	 * ********************************/
_expr:
	expr();
	if(lookahead == ','){
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
void fact()
{
	switch(lookahead){
		case ID:
			match(ID);
			if(lookahead == ASGN){
				match(ASGN);
				expr();
				if(lookahead == ';') match(';');			
			} else if (lookahead == '('){
				match('('); exprlst(); match(')');
			}
			
			break;
		case UINT: 
			match(UINT);
			break;
		case FLT:
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
			expr();
			match(')');	
	}
}

#include<stdlib.h>
// ** This function mathces an token and read the next when it's the expected
// ** Otherwise it's print an error
void match(int expected){
		//printf("-");
		//printf("%- s -", lexeme);		
		//printf("-");
	if(lookahead == expected)
	{
		lookahead = gettoken(source);
	}		// ** If I got what i wanted, get next token
	else{
		fprintf(stderr, "%d seen while %d expected\n", lookahead, expected);	// ** Else, error showing what i wanted
		fprintf(stderr, "%c seen while %c expected\n", lookahead, expected);	// ** Else, error showing what i wanted
		exit(0);	
	}	
} 

