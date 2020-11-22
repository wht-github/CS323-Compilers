CC := g++ # this is the main compiler
# $@ is a macro that refers to the target
# $< is a macro that refers to the first dependency
# $^ is a macro that refers to all dependencies
# % is a macro to make a pattern that we want to watch in both the target and the dependency
CFLAGS := -g -Wall -Wextra -Wpedantic -std=c++17 


PARINC := -I ./include
PARBUILDDIR := ./build
PARSRCDIR := ./src

# Make the parser
splc : $(PARBUILDDIR)/main.o  $(PARBUILDDIR)/parser.tab.o $(PARBUILDDIR)/lex.yy.o 
	@echo "Linking..."
	@echo " mkdir -p bin"; mkdir -p bin
	@echo " $(CC) $^ -o $@"; $(CC) $^ -o bin/$@

$(PARBUILDDIR)/%.yy.o : $(PARSRCDIR)/%.yy.cpp
	@echo "Building lex..."
	@echo " mkdir -p $(PARBUILDDIR)"; mkdir -p $(PARBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<

$(PARBUILDDIR)/parser.tab.o : $(PARSRCDIR)/parser.tab.cpp
	@echo "Building yacc..."
	@echo " mkdir -p $(PARBUILDDIR)"; mkdir -p $(PARBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<

$(PARBUILDDIR)/%.o : $(PARSRCDIR)/%.cpp
	@echo "Building sources..."
	@echo " mkdir -p $(PARBUILDDIR)"; mkdir -p $(PARBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<

$(PARSRCDIR)/lex.yy.cpp : $(PARSRCDIR)/lex.l
	@echo "Creating lex..."
	@echo " flex -o $@  $<"; flex -o $@  $<

$(PARSRCDIR)/parser.tab.cpp $(PARSRCDIR)/parser.tab.hpp : $(PARSRCDIR)/parser.y
	@echo "Creating yacc..."
	@echo " bison -v -d $< -o $(PARSRCDIR)/parser.tab.cpp"; bison -v -d $< -o $(PARSRCDIR)/parser.tab.cpp

clean :
	@echo "Cleaning..."
	rm -rf bin
	rm -rf $(PARBUILDDIR)
	rm -f $(PARSRCDIR)/lex.yy.cpp
	rm -f $(PARSRCDIR)/parser.tab.cpp
	rm -f $(PARSRCDIR)/parser.tab.hpp
	rm -f $(PARSRCDIR)/parser.output

tests: 
	./runtest.sh bin/splc test
	

.PHONY : clean