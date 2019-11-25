/******@<keywords.h>::*******/
enum {
 BEGIN = 16384,
 PROGRAM, 
 VAR,
 INTEGER, // tipo 1
 LONG,	  // tipo 2
 REAL,    // tipo 3
 DOUBLE,  // tipo 4
 BOOLEAN, // tipo 5
 CHAR,	  // tipo 6
 STRING,  // tipo 7
 PROCEDURE,
 FUNCTION,
 IF,
 THEN,
 ELSE,
 WHILE,
 DO,
 REPEAT,
 UNTIL,
 END,
};

int iskeyword(char* name);

