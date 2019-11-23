CFLAGS=-g -I$(INCLUDE)

INCLUDE=./include/
mypass: main.o lexer.o parser.o keywords.o 
	$(CC) -o $@ $^

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~

