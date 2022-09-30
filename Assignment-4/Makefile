all: 190101003_lexer.l  190101003_parser.y
	flex 190101003_lexer.l
	yacc -d 190101003_parser.y
	g++ y.tab.c lex.yy.c -o parser

clean:
	rm -f parser lex.yy.c y.tab.c y.tab.h tokenizer

