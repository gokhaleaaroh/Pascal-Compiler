#include "./lexer.c"
#include <stdio.h>

static char* typenames[] = {"ID", "STR_LITERAL", "PLUS", "MINUS", "TIMES", "DIVIDE", "EQ", "LT", "GT", "LE", "GE", "NE", "OPEN_BRACK", "CLOSE_BRACK", "DOT", "COMMA", "OPEN_PAREN", "CLOSE_PAREN", "COLON", "ASSIGN", "POINTER", "AT", "CHARACTER", "DEC", "BIN", "HEX", "OCTAL", "REAL", "LS", "RS", "PE", "ME", "TE", "DE", "RANGE", "ABSOLUTE", "FILE", "OBJECT", "STRING", "AND", "FOR", "OF", "THEN", "ARRAY", "FUNCTION", "OPERATOR", "TO", "ASM", "GOTO", "OR", "TYPE", "BEGIN", "IF", "PACKED", "UNIT", "CASE", "IMPLEMENTATION", "PROCEDURE", "UNTIL", "CONST", "IN", "PROGRAM", "USES", "CONSTRUCTOR", "INHERITED", "RECORD", "VAR", "DESTRUCTOR", "INLINE", "REINTRODUCE", "WHILE", "DIV", "INTERFACE", "REPEAT", "WITH", "DO", "LABEL", "SELF", "XOR", "DOWNTO", "MOD", "SET", "ELSE", "NIL", "SHL", "END", "NOT", "SHR"};


void print_token(Token *tk) {
    if (tk == NULL) { return; }
    printf("Type: %s, Lexeme: ", typenames[tk->type]);
    if (tk->type == TOKEN_ID && tk->type == TOKEN_STR_LITERAL) { printf("%s", tk->value.text); }
    else if (tk->type == TOKEN_DEC) { printf("%ld", tk->value.int_val); } 
    else if (tk->type == TOKEN_CHARACTER) { printf("%c", tk->value.character); } 
    else { printf("N/A"); }
}

int main() {

    char* bruh = "#97";
    char* bruh1 = "122341 + 234";

    Token* example = next_token(bruh);
    Token* example1 = next_token(bruh1);

    if (example != NULL) {
	printf("char: %c\n", example->value.character);
    }

    if (example1 != NULL) {
	printf("second number: %ld\n", example1->value.int_val);
    }

    return 0;  
}
