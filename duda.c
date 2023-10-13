#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define STACK_CAP 100
int pos = 0;
long long stack[STACK_CAP] = {0};
int stackptr = 0;


const char* tokentypedict[] = {
    "INTEGER",
    "PLUS",
    "MINUS",
    "TIMES",
    "DIV",
    "POW"
    "GREATER_THAN",
    "LESS_THAN",
    "EQUALS",
    "DIFFERENT",
    "SPACE", 
    "STDOUT_PRINT"
}; 

typedef enum {
    INTEGER,
    PLUS,
    MINUS,
    TIMES,
    DIV,
    POW,
    GREATER_THAN,
    LESS_THAN,
    EQUALS,
    DIFFERENT,
    SPACE,
    STDOUT_PRINT 
} TokenType;


typedef struct Token {
    int pos;
    char *value;
    TokenType type;
} Token;


bool strisdigit(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return false;
        }
    }
    return true;
}

bool strisspace(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            return false;
        }
    }
    return true;
}



Token* lexer(char *curr) {    

    Token *token = (Token *) malloc(sizeof(Token));
    token->value = (char *) malloc(sizeof(char) * strlen(curr));
    token->pos = pos;
    strcpy(token->value, curr);

    if(strisdigit(curr)) {
        token->type = INTEGER;
    }else if(strcmp(curr, "+") == 0) {
        token->type = PLUS;
    }else if(strcmp(curr , "-") == 0) {
        token->type = MINUS;
    }else if(strcmp(curr , "*") == 0) {
        token->type = TIMES;
    }else if(strcmp(curr , "^") == 0) {
        token->type = POW;
    }else if(strcmp(curr , "/") == 0) {
        token->type = DIV;
    }else if(strisspace(curr)) {
        token->type = SPACE;
    }else if(strcmp(curr, ">") == 0) {
        token->type = GREATER_THAN;
    }else if(strcmp(curr, "<") == 0) {
        token->type = LESS_THAN;
    }else if(strcmp(curr, "==") == 0) {
        token->type = EQUALS;
    }else if(strcmp(curr, "!=") == 0) {
        token->type = DIFFERENT;
    }else if (strcmp(curr , ":") == 0){
        token->type = STDOUT_PRINT;
    }else {
        assert(false && "Syntax error at %d" && pos);
    }
    
    return token;
} 

void eval(Token *token) {

    if(token->type == INTEGER) {
        stack[stackptr] = atoi(token->value);
        stackptr++;

    }else if(token->type == PLUS) {
        
        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];
 
        stackptr -= 2;
        stack[stackptr] = a + b;
        stackptr++;

    }else if(token->type == MINUS) {

        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        stackptr -= 2;
        stack[stackptr] = a - b;
        stackptr++;

    }else if(token->type == DIV) {

        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        stackptr -= 2;
        stack[stackptr] = a / b;
        stackptr++;

    }else if(token->type == TIMES) {
        
        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        stackptr -= 2;
        stack[stackptr] = a * b;
        stackptr++;
    }else if(token->type == POW) {
        
        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        long int result = 1;

        for(int i = 0; i < b; ++i) {
            result *= a;
        }

        stackptr -= 2;
        stack[stackptr] = result;
        stackptr++;

    }else if(token->type == GREATER_THAN) {


        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        stackptr -= 2;
        stack[stackptr] = a > b;
        stackptr++;
    
    
    }else if(token->type == LESS_THAN) {
        
        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        stackptr -= 2;
        stack[stackptr] = a < b;
        stackptr++;
    
    }else if(token->type == EQUALS) {
        
        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        stackptr -= 2;
        stack[stackptr] = a == b;
        stackptr++;

    }else if(token->type == DIFFERENT) {
      
        int a = stack[stackptr - 2];
        int b = stack[stackptr - 1];

        stackptr -= 2;
        stack[stackptr] = a != b;
        stackptr++;
    
    }else if (token->type == STDOUT_PRINT) {
        printf("%lld\n", stack[stackptr - 1]);
    }
    
}

char getnextchar(FILE *file) {
    if(fseek(file, pos, SEEK_SET) != 0) {
        printf("[LEXER error] internal error on reading .dudal file");
        fclose(file);
        return -1;
    }
    pos++;
    return fgetc(file);
}


void getcompletedigit(FILE *file, char *buffer){

    char next = getnextchar(file);
    size_t bufferptr = 1;

    while (isdigit( next ) ) {
        buffer[bufferptr] = next;
        bufferptr++;
        next = getnextchar(file);
    }

}

void walker(FILE *file) {

    char curr;
    do {
   
        curr = getnextchar(file);
        if(curr == EOF) break; 

        char *buffer = malloc(sizeof(char) * 100);
        size_t bufferptr = 0;
        buffer[bufferptr] = curr;
        bufferptr++;

        if(isdigit(curr)) {
            getcompletedigit(file, buffer);
        }

        if(curr == '=') {
            char next = getnextchar(file);
            if(next == '=') {
                buffer[bufferptr++] = next;
            } 
        }

        if(curr == '!') {
            char next = getnextchar(file);
            if(next == '=') {
                buffer[bufferptr++] = next;
            } 
        }

        Token *t = lexer(buffer);
                
        eval(t);
        
        free(t->value);
        free(t);
        free(buffer);
    
    } while ( curr != EOF );    

}


void usage() {
    puts("[USAGE]: ./dudac filename.duda");
}

int main(int argc, char **argv) {

    // assert(argc >= 2);

    FILE *file = fopen("test.dudal", "r");

    if(file == NULL) {
        printf("[ERROR]: Filename \"%s\" not found\n\n", argv[1]);
        usage();
        exit(-1);
    }

    walker(file);
    fclose(file);

    return 0;
}