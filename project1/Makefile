CC=g++
FLEX=flex
splc:
	@mkdir bin
	$(FLEX) lex.l
	$(CC) lex.yy.c -lfl -o ./bin/splc
	@chmod +x ./bin/splc 
clean:
	@rm -rf bin/
.PHONY: splc