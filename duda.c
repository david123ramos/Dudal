#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define STACK_CAP 100

const char* tokentypedict[] = {
    "INTEGER",
    "PLUS",
    "MINUS",
    "TIMES",
    "DIV",
    "SPACE"
}; 

typedef enum {
    INTEGER,
    PLUS,
    MINUS,
    TIMES,
    DIV,
    SPACE, 
} TokenType;


typedef struct Token {
    int pos;
    char value;
    TokenType type;
} Token;


int pos = 0;

Token* lexer(char curr) {    

    Token *token = (Token *) malloc(sizeof(Token));

    token->pos = pos;
    token->value = curr;

    if(isdigit(curr)) {
        token->type = INTEGER;
    }else if(curr == '+') {
        token->type = PLUS;
    }else if(curr == '-') {
        token->type = MINUS;
    }else if(curr == '*') {
        token->type = TIMES;
    }else if(curr == '/') {
        token->type = DIV;
    }else if(isspace(curr)) {
        token->type = SPACE;
    }
    pos++;
    return token;
}

long long stack[STACK_CAP] = {0};
int stackptr = 0;

int char2int(char c) {
    return c - '0';
} 

void eval(Token *token) {

    if(token->type == INTEGER) {
        stack[stackptr] = char2int(token->value);
        stackptr++;

    }else if(token->type == PLUS) {
        
        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];
 
        stackptr -= 2;
        stack[stackptr] = a + b;
        printf("%lld\n", stack[stackptr]);
        stackptr++;

    }else if(token->type == MINUS) {

        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        stackptr -= 2;
        stack[stackptr] = a - b;
        printf("%lld\n", stack[stackptr]);
        stackptr++;

    }else if(token->type == DIV) {

        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        stackptr -= 2;
        stack[stackptr] = a / b;
        printf("%lld\n", stack[stackptr]);
        stackptr++;

    }else if(token->type == TIMES) {
        
        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        stackptr -= 2;
        stack[stackptr] = a * b;
        printf("%lld\n", stack[stackptr]);
        stackptr++;
    }
    
}

void walker(FILE *file) {

    char curr;
    do {
        curr = fgetc(file);
        if(curr == EOF) break;

        Token *t = lexer(curr);
        
        // printf("{ value: '%c', type: '%s', pos: '%d' }\n", t->value, tokentypedict[t->type], t->pos);
        
        eval(t);
        
        free(t);

    } while ( curr != EOF );
}


void usage() {
    puts("[USAGE]: ./dudac filename.duda");
}

int main(int argc, char **argv) {

    assert(argc >= 2);

    FILE *file = fopen(argv[1], "r");

    if(file == NULL) {
        printf("[ERROR]: Filename \"%s\" not found\n\n", argv[1]);
        usage();
        exit(-1);
    }

    walker(file);

    return 0;
}