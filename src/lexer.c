#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef enum {
    TOKEN_ID,          // generic Identifiers
    TOKEN_STR_LITERAL, // 'stuff'
    
    // symbols
    TOKEN_PLUS,        // +
    TOKEN_MINUS,       // -
    TOKEN_TIMES,       // *
    TOKEN_DIVIDE,      // /
    TOKEN_EQ,          // =
    TOKEN_LT,          // <
    TOKEN_GT,          // >
    TOKEN_LE,          // <=
    TOKEN_GE,          // >=
    TOKEN_NE,          // <>
    TOKEN_OPEN_BRACK,  // [
    TOKEN_CLOSE_BRACK, // ]
    TOKEN_DOT,         // .
    TOKEN_COMMA,       // ,
    TOKEN_OPEN_PAREN,  // (
    TOKEN_CLOSE_PAREN, // )
    TOKEN_COLON,       // :
    TOKEN_ASSIGN,      // :=
    TOKEN_POINTER,     // ^
    TOKEN_AT,          // @
    TOKEN_CHARACTER,   // #num or 'singlechar'
    TOKEN_DEC,         // decnum
    TOKEN_BIN,         // %binnum
    TOKEN_HEX,         // $hexnum
    TOKEN_OCTAL,       // &octnum
    TOKEN_REAL,        // 12345.999E20
    TOKEN_LS,          // <<
    TOKEN_RS,          // >>
    TOKEN_PE,          // +=
    TOKEN_ME,          // -=
    TOKEN_TE,          // *=
    TOKEN_DE,          // /=
    TOKEN_RANGE,       // ..
    
    // keywords
    TOKEN_ABSOLUTE,
    TOKEN_FILE,
    TOKEN_OBJECT,
    TOKEN_STRING,
    TOKEN_AND,
    TOKEN_FOR,
    TOKEN_OF,
    TOKEN_THEN,
    TOKEN_ARRAY,
    TOKEN_FUNCTION,
    TOKEN_OPERATOR,
    TOKEN_TO,
    TOKEN_ASM,
    TOKEN_GOTO,
    TOKEN_OR,
    TOKEN_TYPE,
    TOKEN_BEGIN,
    TOKEN_IF,
    TOKEN_PACKED,
    TOKEN_UNIT,
    TOKEN_CASE,
    TOKEN_IMPLEMENTATION,
    TOKEN_PROCEDURE,
    TOKEN_UNTIL,
    TOKEN_CONST,
    TOKEN_IN,
    TOKEN_PROGRAM,
    TOKEN_USES,
    TOKEN_CONSTRUCTOR,
    TOKEN_INHERITED,
    TOKEN_RECORD,
    TOKEN_VAR,
    TOKEN_DESTRUCTOR,
    TOKEN_INLINE,
    TOKEN_REINTRODUCE,
    TOKEN_WHILE,
    TOKEN_DIV,
    TOKEN_INTERFACE,
    TOKEN_REPEAT,
    TOKEN_WITH,
    TOKEN_DO,
    TOKEN_LABEL,
    TOKEN_SELF,
    TOKEN_XOR,
    TOKEN_DOWNTO,
    TOKEN_MOD,
    TOKEN_SET,
    TOKEN_ELSE,
    TOKEN_NIL,
    TOKEN_SHL,
    TOKEN_END,
    TOKEN_NOT,
    TOKEN_SHR
} TokenType;

typedef struct {
    union {
	long int_val;
	double float_val;
	char *text;
	char character;
    } value;
    TokenType type;
} Token;

Token *try_ops(char *input) {
    Token *ret_tok = NULL;
    
    if (input[0] == '+') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->value.text = NULL;
	if (input[1] == '=') {
	    ret_tok->type = TOKEN_PE;
	} else {
	    ret_tok->type = TOKEN_PLUS;
	}
    }
    
    else if (input[0] == '-') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->value.text = NULL;
	if (input[1] == '=') {
	    ret_tok->type = TOKEN_ME;
	} else {
	    ret_tok->type = TOKEN_MINUS;
	}
    }
    
    else if (input[0] == '*') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->value.text = NULL;
	if (input[1] == '=') {
	    ret_tok->type = TOKEN_TE;
	} else {
	    ret_tok->type = TOKEN_TIMES;
	}
    }
    
    else if (input[0] == '/') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->value.text = NULL;
	if (input[1] == '=') {
	    ret_tok->type = TOKEN_DE;
	} else {
	    ret_tok->type = TOKEN_DIV;
	}
    }
    
    else if (input[0] == '=') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->value.text = NULL;
	ret_tok->type = TOKEN_EQ;
    }
    
    else if (input[0] == '<') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->value.text = NULL;
	if (input[1] == '>') {
	    ret_tok->type = TOKEN_NE;
	} else if (input[1] == '=') {
	    ret_tok->type = TOKEN_LE;
	} else if (input[1] == '<') {
	    ret_tok->type = TOKEN_LS;
	} else {
	    ret_tok->type = TOKEN_LT;
	}
    }
    
    else if (input[0] == '>') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->value.text = NULL;
	if (input[1] == '=') {
	    ret_tok->type = TOKEN_GE;
	} else if (input[1] == '>') {
	    ret_tok->type = TOKEN_RS;
	} else {
	    ret_tok->type = TOKEN_GT;
	}
    }
    
    else if (input[0] == '^') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->value.text = NULL;
	ret_tok->type = TOKEN_POINTER;
    }
    
    else if (input[0] == '@') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->value.text = NULL;
	ret_tok->type = TOKEN_AT;
    }
    
    else if (input[0] == ':' && input[1] == '=') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->value.text = NULL;
	ret_tok->type = TOKEN_ASSIGN;
    }
    
    return ret_tok;
}

Token *try_string_literal(char *input) {
    if (input[0] != '\'') {
	return NULL;
    }
    
    Token *ret_tok;
    
    int length = 0;
    
    while (input[length + 1] != '\0') {
	if (input[length + 1] == '\'') {
	    if (length > 0) {
		char *lexeme = malloc(length + 1);
		memcpy(lexeme, input + 1, length + 1);
		lexeme[length] = '\0';
		ret_tok = malloc(sizeof(Token));
		if (length > 1) {
		    ret_tok->type = TOKEN_STR_LITERAL;
		    ret_tok->value.text = lexeme;
		} else {
		    ret_tok->type = TOKEN_CHARACTER;
		    ret_tok->value.character = lexeme[0];
		}
		return ret_tok;
	    }
	}
	length++;
    }
    
    return NULL;
}

bool is_digit(char c) { return c >= 48 && c <= 57; }

long to_num(char *num, int length) {
    long power = 1;
    long ans = 0;
    
    for (int i = length - 1; i >= 0; i--) {
	ans += power * (num[i] - 48);
	power *= 10;
    }
    
    return ans;
}

Token *try_integer_literal(char *input) {
    if (!is_digit(input[0])) {
	return NULL;
    }
    
    int length = 1;
    
    while (is_digit(input[length])) {
	length++;
    }
    
    Token *ret_tok = malloc(sizeof(Token));
    ret_tok->value.int_val = to_num(input, length);
    ret_tok->type = TOKEN_DEC;
    
    return ret_tok;
}

Token *try_char_ascii(char *input) {
    if (input[0] != '#') {
	return NULL;
    }
    Token *temp_num = try_integer_literal(input + 1);
    
    if (temp_num == NULL) {
	return NULL;
    }
    char c = temp_num->value.int_val;
    Token *ret_tok = malloc(sizeof(Token));
    ret_tok->type = TOKEN_CHARACTER;
    ret_tok->value.character = c;
    free(temp_num);
    return ret_tok;
}

Token *try_delims(char *input) {
    Token *ret_tok = NULL;
    if (input[0] == '[') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_OPEN_BRACK;
    } else if (input[0] == ']') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_CLOSE_BRACK;
    } else if (input[0] == '(') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_OPEN_PAREN;
    } else if (input[0] == ')') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_CLOSE_PAREN;
    } else if (input[0] == '.') {
	ret_tok = malloc(sizeof(Token));
	if (input[1] == '.') {
	    ret_tok->type = TOKEN_RANGE;
	} else {
	    ret_tok->type = TOKEN_DOT;
	}
    } else if (input[0] == ',') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_DOT;
    } else if (input[0] == ':') {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_COLON;
    }
    
    return ret_tok;
}

Token *try_keywords(char *input) {
    Token *ret_tok = NULL;
    
    // handle stuff that starts with a: 4 keywords
    if (input[0] == 'a') {
	if (strncmp(input + 1, "bsolute", 6) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_ABSOLUTE;
	} else if (strncmp(input + 1, "nd", 2) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_AND;
	} else if (strncmp(input + 1, "rray", 4) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_ARRAY;
	} else if (strncmp(input + 1, "sm", 2) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_ASM;
	}
    }
    
    // handle stuff that starts with b: 1 keyword
    if (strncmp(input, "begin", 5) == 0) {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_BEGIN;
    }
    
    // handle stuff that starts with c: 3 keywords
    else if (input[0] == 'c') {
	if (strncmp(input + 1, "ase", 3) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_CASE;
	} else if (strncmp(input + 1, "onst", 4) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    if (strncmp(input + 5, "ructor", 6) == 0) {
		ret_tok->type = TOKEN_CONSTRUCTOR;
	    } else {
		ret_tok->type = TOKEN_CONST;
	    }
	}
    }
    
    // handle stuff that starts with d: 4 keywords
    else if (input[0] == 'd') {
	if (strncmp(input + 1, "estructor", 9) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_DESTRUCTOR;
	}
	
	else if (strncmp(input + 1, "iv", 2) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_DIV;
	}
	
	else if (input[1] == 'o') {
	    ret_tok = malloc(sizeof(Token));
	    if (strncmp(input + 2, "wnto", 4) == 0) {
		ret_tok->type = TOKEN_DOWNTO;
	    } else {
		ret_tok->type = TOKEN_DO;
	    }
	}
    }
    
    // handle stuff that starts with e: 2 keywords
    else if (input[0] == 'e') {
	if (strncmp(input + 1, "lse", 3) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_ELSE;
	}
	
	else if (strncmp(input + 1, "nd", 2) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_END;
	}
    }
    
    // handle stuff that starts with f: 3 keywords
    else if (input[0] == 'f') {
	if (strncmp(input + 1, "ile", 3) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_FILE;
	} else if (strncmp(input + 1, "or", 2) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_FOR;
	} else if (strncmp(input + 1, "unction", 4) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_FUNCTION;
	}
    }
    
    // handle stuff that starts with g: 1 keyword
    else if (strncmp(input, "goto", 4) == 0) {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_GOTO;
    }
    
    // handle stuff that starts with i: 6 keywords
    else if (input[0] == 'i') {
	if (input[1] == 'f') {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_IF;
	} else if (strncmp(input + 1, "mplementation", 13) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_IMPLEMENTATION;
	} else if (input[1] == 'n') {
	    if (strncmp(input + 2, "herited", 7) == 0) {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_INHERITED;
	    } else if (strncmp(input + 2, "line", 4) == 0) {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_INLINE;
	    } else if (strncmp(input + 2, "terface", 7) == 0) {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_INTERFACE;
	    }
	}
    }
    
    // handle stuff that starts with l: 1 keyword
    else if (strncmp(input, "label", 5) == 0) {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_LABEL;
    }
    
    // handle stuff that starts with m: 1 keyword
    else if (strncmp(input, "mod", 3) == 0) {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_MOD;
    }
    
    // handle stuff that starts with n: 2 keywords
    else if (input[0] == 'n') {
	if (strncmp(input + 1, "il", 2) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_NIL;
	} else if (strncmp(input + 1, "ot", 2) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_NOT;
	}
    }
    
    // handle stuff that starts with o: 4 keywords
    else if (input[0] == 'o') {
	if (strncmp(input + 1, "bject", 5) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_OBJECT;
	} else if (input[1] == 'f') {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_OF;
	} else if (strncmp(input + 1, "perator", 7) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_OPERATOR;
	} else if (input[1] == 'r') {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_OR;
	}
    }
    
    // handle stuff that starts with p: 3 keywords
    else if (input[0] == 'p') {
	if (strncmp(input + 1, "acked", 5) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_PACKED;
	} else if (strncmp(input + 1, "ro", 2) == 0) {
	    if (strncmp(input + 3, "cedure", 6) == 0) {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_PROCEDURE;
	    }
	    
	    else if (strncmp(input + 3, "gram", 4) == 0) {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_PROGRAM;
	    }
	}
    }
    
    // handle stuff that starts with r: 3 keywords
    else if (input[0] == 'r' && input[1] == 'e') {
	if (strncmp(input + 2, "cord", 4) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_RECORD;
	} else if (strncmp(input + 2, "introduce", 9) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_REINTRODUCE;
	} else if (strncmp(input + 2, "peat", 4) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_REPEAT;
	}
    }
    
    // handle stuff that starts with s: 5 keywords
    else if (input[0] == 's') {
	if (input[1] == 'e') {
	    if (strncmp(input + 2, "lf", 2) == 0) {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_SELF;
	    } else if (input[2] == 't') {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_SET;
	    }
	}
	
	else if (input[1] == 'h') {
	    if (input[2] == 'l') {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_SHL;
	    } else if (input[2] == 'r') {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_SHR;
	    }
	}
	
	else if (strncmp(input + 1, "tring", 5) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_STRING;
	}
    }
    
    // handle stuff that starts with t: 3 keywords
    else if (input[0] == 't') {
	if (strncmp(input + 1, "hen", 3) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_THEN;
	    
	} else if (input[1] == 'o') {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_TO;
	    
	} else if (strncmp(input + 1, "ype", 3) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_TYPE;
	}
    }
    
    // handle stuff that starts with u: 3 keywords
    else if (input[0] == 'u') {
	if (input[1] == 'n') {
	    if (strncmp(input + 2, "it", 2) == 0) {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_UNIT;
	    } else if (strncmp(input + 2, "til", 3) == 0) {
		ret_tok = malloc(sizeof(Token));
		ret_tok->type = TOKEN_UNTIL;
	    }
	} else if (strncmp(input + 1, "ses", 3) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_USES;
	}
    }
    
    // handle stuff that starts with v: 1 keyword
    else if (strncmp(input, "var", 3) == 0) {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_VAR;
    }
    
    // handle stuff that starts with w: 2 keywords
    else if (input[0] == 'w') {
	if (strncmp(input + 1, "hile", 4) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_WHILE;
	} else if (strncmp(input + 1, "ith", 3) == 0) {
	    ret_tok = malloc(sizeof(Token));
	    ret_tok->type = TOKEN_WITH;
	}
    }
    
    // handle stuff that starts with x: 1 keyword
    else if (strncmp(input, "xor", 3) == 0) {
	ret_tok = malloc(sizeof(Token));
	ret_tok->type = TOKEN_XOR;
    }
    
    return ret_tok;
}

bool is_letter(char c) { return (c >= 65 && c <= 90) || (c >= 97 && c <= 122); }

Token *try_identifier(char *input) {
    if (!is_letter(input[0]) && input[0] != '_') {
	return NULL;
    }
    
    int length = 1;
    
    while (is_digit(input[length]) || is_letter(input[length]) ||
	   input[length] == '_') {
	length++;
    }
    
    Token *ret_tok = malloc(sizeof(Token));
    ret_tok->type = TOKEN_ID;
    ret_tok->value.text = malloc(length + 1);
    memcpy(ret_tok->value.text, input, length);
    ret_tok->value.text[length] = '\0';
    
    return ret_tok;
}

/*
  main lexing function
  returns next valid token in the given string
  returns null if no valid token can be formed from the start of the string
*/
Token *next_token(char *input) {
    
    // let the epic begin
    Token *ret_tok;
    
    Token *(*recognizers[])(char *) = {
	&try_ops,    &try_char_ascii, &try_string_literal, &try_integer_literal,
	&try_delims, &try_keywords,   &try_identifier};
    
    int NUM_RECOGS = 7;
    
    for (int i = 0; i < NUM_RECOGS; i++) {
	ret_tok = (*recognizers[i])(input);
	if (ret_tok) {
	    return ret_tok;
	}
    }
    
    return NULL;
}
