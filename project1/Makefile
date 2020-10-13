CC=gcc
FLEX=flex
BISON=bison


.syntax: src/C/flex/bison/syntax.y
	$(BISON) -dtv src/C/flex/bison/syntax.y
.lex: src/lex.l
	$(FLEX) src/lex.l
splc: .syntax .lex
	@if ! test -d "./bin"; then \
		mkdir bin; \
	fi
	$(CC) syntax.tab.c -lfl -ly -o bin/splc
debug: .syntax .lex
	@if ! test -d "./bin"; then \
		mkdir bin; \
	fi
	$(CC) syntax.tab.c -lfl -ly -DDEBUG -o bin/debug
clean:
	@rm -rf bin/
	@rm lex.yy.c syntax.tab.* syntax.output