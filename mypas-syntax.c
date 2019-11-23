/***@<mypas-syntax.c>::***/

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
RELOP = "< | <> | <= | = | >= | >"

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
