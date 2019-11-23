/******@<keywords.h>::*******/

enum {
 BEGIN = 16384,
 PROGRAM, 	//85
 VAR,		//86
 INTEGER,	//87    TYPE 1
 LONG,		//88    TYPE 2
 REAL,		//89    TYPE 3
 DOUBLE,	//90    TYPE 4
 BOOLEAN,	//91    
 CHAR,		//92
 STRING,	//93
 PROCEDURE,	//94
 FUNCTION,	//95
 IF,		//96
 THEN,		//97
 ELSE,		//98
 WHILE,		//99
 DO,
 REPEAT,
 UNTIL,
 END,
 AND,
 DIV,
 OR,
 MOD,
};

int iskeyword(char* name);

