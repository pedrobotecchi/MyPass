/***@parser.c>::***/

/*******************************************************************************
 * Author: Prof. Eraldo Pereira Marinho, Ph.D.
 * Co-Authors:
 * 			Bruna F. Lamim
 * 			Guilherme M. Caes
 * 			Pedro H. Botecchi
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

/* Include Session */
#include<tokens.h>
#include<lexer.h>
#include<keyword.h>
#include<parser.h>
#include<symtab.h>
#include<default.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<code.h>
#include<reg.h>

token_t lookahead;
char lexeme[MAXIDLEN+1];
int linenumber;

int labelProcFunc = 1;		/* Control Procedures and Function Label */
int labelControl = 1;		/* Control Labels number, EX : L1:,L2:,... */

void mypass(void){
	if (lookahead == PROGRAM){
		match(PROGRAM); match(ID); match('(');
		match(ID); match(','); match(ID); match(')');
		match(';');
		}
	declscope();
	procdecl();
	stmblock();
	match('.');
}

/**************************************************************************************
declscope -> {VAR varlst ':' vartype ';' } 
***************************************************************************************/
/***/
	int symtab_initial = 0;			/* Used to count the vars inserted */
	int symtab_final = 0; 			/* Used to count the vars inserted, this allows us to send types for symtab */
/***/
	int lex_level = 0;				/* Used to control the Lexel Level from a variable */
/***/

void declscope(void)
{
	if(lookahead == VAR)
	{
		match(VAR);
	
		while((lookahead == ID) || (lookahead == VAR)){
			/** a variable must be registered here *****/
			if(lookahead == VAR)
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
		//printf("\n%s inserido na posicao %d", lexeme, symtab_descriptor);
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
/* This function do the type attribution */
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
		case STRING:
			/***/
			//printf("Entrou aqui para as posicoes %d e %d", symtab_initial,symtab_final);
			symtab_type_range(7);
			/***/
			match(STRING);
			break;
		default:
			printf("ERROR TYPE - %s SEEN WHILE EXPECTED (integer/long/real/double/char/string/boolean)", lexeme);
	}
}

/**************************************************************************************
procdecl -> { PROCEDURE ID parmdef ';' declscope stmblock |
              FUNCTION  ID parmdef ':' vartype ';' declscope stmblock }
***************************************************************************************/
int symtab_procfunc = 0;		/* Flag used to say if it's a function or not */
void procdecl(void)
{
	int isfunc;
	while(lookahead == PROCEDURE || lookahead == FUNCTION){
		lex_level++;		/* Increment the Lexic Level because it's a function or procedure */
		//printf("LEX LEVEL : %d",lex_level);
		if(lookahead == FUNCTION)
			isfunc=1;
		match(lookahead);
		/* This part checks the procedure or function name insertion in Symtab */
			if(symtab_lookup(lexeme)){
				/*** function and procedure already declared ***/
				fatalerrorcount++;
			} else {
				/*** appends new function or procedure in the table ***/	
				symtab_append(lexeme);
				//printf("\n%s inserido na posicao %d", lexeme, symtab_descriptor);
			}
		
		match(ID);
		parmdef();
		if(isfunc){
			symtab_procfunc = 1;	/* Set up the flag to insert back in Symtab */
			match(':');
			vartype();
			symtab_procfunc = 0;	/* Return to 0 */
		}
		match(';');
		declscope();
		procdecl();
		stmblock();
	}
}

/*************************************************************************************
  parmdef -> ['(' [VAR] varlst ':' vartype {',' [VAR] varlst ':' vartype } ')']			// Se tiver VAR, o parametro é passado por referencia
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
	/* Remove all vars in current lex level */
	symtab_remove_LEXVAR();
	lex_level--;		/* Decrement the Lexic Level before an END */
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

int typeCheck = 0;		/* Initial value to pass when verifying types */
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
			fact(typeCheck);
	}
}
/******************************************************************************
ifstm -> IF expr THEN stmt [ ELSE stmt ]
******************************************************************************/
void ifstmt()
{
	int label;

	match(IF);
	expr(typeCheck);

		/**AS.CODE**/
		code_cmp_imm(EAX,"0");
		code_false(label = labelControl++);
		/**AS.CODE**/

	match(THEN);
	stmt();
	//printf("Saiu do stmt com lexeme : %s ",lexeme);
	if(lookahead == ELSE){
		match(ELSE);
			/**AS.CODE**/	
			code_goto(labelControl);
			code_loop_label(label);
			label = labelControl++;
			/**AS.CODE**/
		stmt();
	}
}

/******************************************************************************
whilestm -> WHILE expr DO stmt
******************************************************************************/
void whilestm()
{
	int labelwhile,labelend;
		/**AS.CODE**/
		code_loop_label(labelwhile = labelControl++);
		/**AS.CODE**/
	match(WHILE);
	expr(typeCheck);
		/**AS.CODE**/
		code_cmp_imm(EAX,"0");
		code_false(labelend = labelControl++);
		/**AS.CODE**/
	match(DO);
	stmt();
		/**AS.CODE**/
		code_goto(labelwhile);
		code_loop_label(labelend);
		/**AS.CODE**/
}

/******************************************************************************
repstm -> REPEAT stmlst UNTIL expr
******************************************************************************/
void repstm()
{
	int labelRept, labelend;
		/**AS.CODE**/
		code_loop_label(labelRept = labelControl++);
		/**AS.CODE**/
	match(REPEAT);
	stmlst();
	match(UNTIL);
	expr(typeCheck);
		/**AS.CODE**/
		code_cmp_imm(EAX,"0");
		code_false(labelend = labelControl++);
		code_goto(labelRept);
		code_loop_label(labelend);
		/**AS.CODE**/
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
	int labelT = labelControl++;
	int labelF = labelControl++;

	switch(lookahead){
		case '<':	
				/**AS.CODE**/
				code_prologue_relop();
				code_jl(labelT);
				code_epilogue_relop(labelT,labelF);
				/**AS.CODE**/
			return '<';
		case '>':
				/**AS.CODE**/
				code_prologue_relop();
				code_jg(labelT);
				code_epilogue_relop(labelT,labelF);
				/**AS.CODE**/
			return '>';
		case '=':
				/**AS.CODE**/
				code_prologue_relop();
				code_je(labelT);
				code_epilogue_relop(labelT,labelF);
				/**AS.CODE**/
			return '=';
		case NEQ:	
				/**AS.CODE**/
				code_prologue_relop();
				code_jne(labelT);
				code_epilogue_relop(labelT,labelF);
				/**AS.CODE**/	
			return NEQ;
		case LEQ:
				/**AS.CODE**/	
				code_prologue_relop();
				code_jle(labelT);
				code_epilogue_relop(labelT,labelF);
				/**AS.CODE**/
			return LEQ;
		case GEQ:	
				/**AS.CODE**/	
				code_prologue_relop();
				code_jge(labelT);
				code_epilogue_relop(labelT,labelF);
				/**AS.CODE**/
			return GEQ;
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
			/**AS.CODE**/
			code_push(EAX);
			/**AS.CODE**/
		match(lookahead);
		t2 = smpexpr(max(t1,p_type));
		//printf("Comparando tipos : t1 - %d, t2 - %d",t1,t2);
		//goto _expr;
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
		case '+':
				/**AS.CODE**/
				code_pop(EBX);
				code_add(EAX,EBX);
				/**AS.CODE**/	
			return '+';
		case '-':	
				/**AS.CODE**/
				code_mov(EBX,EAX);
				code_pop(EAX);
				code_sub(EAX,EBX);
				/**AS.CODE**/
			return '-';
		case OR:	
				/**AS.CODE**/
				code_pop(EBX),code_or(EAX,EBX);
				/**AS.CODE**/
			return OR;
		default:	return 0;
	}
}

type_t smpexpr(type_t p_type)
{
	int oplus;
	/***/
	type_t acctype = 0;
	/***/

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
		case '*':
				/**AS.CODE**/	
				code_pop(EBX);
				code_imull(EBX);
				/**AS.CODE**/
			return '*';
		case '/':
				/**AS.CODE**/	
				code_mov(EBX,EAX);
				code_pop(EAX);
				code_idiv(EBX);
				/**AS.CODE**/
			return '/';
		case DIV:
				/**AS.CODE**/	
				code_pop(EBX);
				code_imull(EBX);
				/**AS.CODE**/
			return DIV;
		case MOD:
				/**AS.CODE**/	
				code_mov(EBX,EAX);
				code_pop(EBX);
				code_mod(EBX);
				/**AS.CODE**/
			return MOD;
		case AND:
				/**AS.CODE**/	
				code_pop(EBX);
				code_and(EAX,EBX);
				/**AS.CODE**/
			return AND;
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
	//printf("!!!! %d !!!!",acctype);
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
ID = [A-Za-z][A-Za-z0-9_]*
******************************************************************************/

#define MAX_SIZE_INT 2147483647 
#define MAX_SIZE_LONG
#define MAX_SIZE_FLOAT 1e24

type_t fact(type_t p_type)
{
	/***/
	int var_descr; 
	type_t acctype = 0;
	type_t old_acctype;
	/***/
	
	switch(lookahead){
		case ID:
			/***/
			var_descr = symtab_lookup(lexeme);
			acctype = symtab[var_descr].typedescriptor;
			/***/
			//printf("\n_LEXEMA %s, com tipo %d_\n",lexeme,acctype);	
			match(ID);
			if(lookahead == ASGN){
				match(ASGN);
				//printf("Comparing type %d for %s",acctype, lexeme);
				if(isCompat(acctype, old_acctype = expr(p_type)))		/* Check if the types match */
					acctype = max(acctype,old_acctype);		 					
				else
					printf("LINE %d - TYPE ERROR - EXPRESSION TYPE NOT MATCH",linenumber); // IMPLEMENTAR MENSAGEM DE ERRO com ENUM?
			} else if (lookahead == '('){
				match('('); exprlst(); match(')');
			}
			
			break;
		case UINT: 
			/* This part checks if the Value in UINT ir a LONG OR INTEGER number */
			if(atof(lexeme) <= MAX_SIZE_INT) {  // CONFIRMAR O CAST
				//printf("ptype : %d",p_type);
				if(isCompat(1, p_type))	acctype = max(1,p_type);		
				else printf("LINE %d - TYPE ERROR - EXPRESSION TYPE NOT MATCH.",linenumber); // IMPLEMENTAR MENSAGEM DE ERRO com ENUM?
			
				//printf("& ptype : %d &",acctype);
			}
			else {
				//printf("ptype : %d\n", p_type);
				if(isCompat(2, p_type))	{
					acctype = max(2,p_type);
					symtab[var_descr].typedescriptor = 2;
					acctype = max(2,p_type);
				}
			}
			match(UINT);
			break;
		case FLT:
			/* This part checks if the FLOAT Is a REAL ou DOUBLE number */									
			if(strtod(lexeme,NULL) <= MAX_SIZE_FLOAT) {    
				if(isCompat(3, p_type))	acctype = max(3,p_type);		
				else printf("LINE %d - TYPE ERROR - EXPRESSION TYPE NOT MATCH.",linenumber); 
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
			//printf("@@@ %d @@@", p_type);
			match(')');
			break;	
	}	
	return max(acctype, p_type);
}
/* This function checks compatibility of two types passed */
int isCompat(type_t t1, type_t t2){
	if(t1 == t2) return 1;
	else
	{
		if(t1==0)
			return 1;
		else if(t1 <= 4 && t2 > 4)
			return 0;
		else if(t2<=4 && t1 > 4)
			return 0;
		else
		{
			if(t1>=0 && t1<=4)
			{
				if(t2>=0 && t2<=4)
					return 1;
			}
		}
	}
	return 0;
}

// ** This function matches an token and read the next when it's the expected
// ** Otherwise it's print an error
void match(int expected){
	if(lookahead == expected)
	{
		lookahead = gettoken(source);
		//fprintf(object, "lookahead : %d, lexeme : %s\n", lookahead,lexeme);		
	}		// ** If I got what i wanted, get next token
	else{
		if(lookahead == ';')
			fprintf(stderr, "ERROR FOUND in line : - %d ; unrecognized",linenumber,lookahead, expected);	// ** Else, error showing what i wanted
		else if(expected == END)
			fprintf(stderr, "ERROR FOUND in line : - %d missing END",linenumber,lookahead, expected);	// ** Else, error showing what i wanted
		else if(expected == THEN)
			fprintf(stderr, "ERROR FOUND in line : - %d, IF without THEN",linenumber,lookahead, expected);	// ** Else, error showing what i wanted
		else if(expected == ';')
			fprintf(stderr, "ERROR FOUND in line : - %d missing ;",linenumber,lookahead, expected);	// ** Else, error showing what i wanted
		else if(expected == ')')
			fprintf(stderr, "ERROR FOUND in line : - %d, expression missing ')' ;",linenumber,lookahead, expected);	// ** Else, error showing what i wanted
		else
			fprintf(stderr, "LINE : - %d -  %d seen while %d expected\n",linenumber,lookahead, expected);	// ** Else, error showing what i wanted
		exit(-5);
	}	
}

/* Function to print SymTab, just for debug purpouses */
void symtab_print()
{
	int j = 1;
	printf("\n\n::::: SYMTAB :::::\n");
	for(j = symtab_descriptor-1;j>0;j--)
	{
		printf("\nSYMTAB LINE %d :\n\tName : %s\n\tType: %d\n\tLEX_LEVEL : %d",j,symtab[j].name,symtab[j].typedescriptor,symtab[j].lexlevel);
	}
}