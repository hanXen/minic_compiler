#include <stdio.h>

#define LABEL_SIZE 10
#define TABLE_SIZE 100

struct SymbolTable 
{
    char name[LABEL_SIZE];
    int typeSpecifier;
    int typeQualifier;
    int base;
    int offset;
    int width;
    int initialValue;
};

typedef enum 
{
	NONE_SPEC, VOID_SPEC, INT_SPEC, CHAR_SPEC, FLOAT_SPEC
} typeSpecifier;

typedef enum 
{
	NONE_QUAL, CONST_QUAL, VAR_QUAL, FUNC_QUAL, PARAM_QUAL
} typeQualifier;



struct SymbolTable symbolTable[TABLE_SIZE];
int st_top;
int base, offset; 

void initSymbolTable()
{
    base = 1;
    offset = 1;
    st_top = 0;
}

int insert(char *name, int typeSpecifier, int typeQualifier,
        int base, int offset, int width, int initialValue)
{
    struct SymbolTable *stptr = &symbolTable[st_top];
    strcpy(stptr->name, name);
    stptr->typeSpecifier = typeSpecifier;
    stptr->typeQualifier = typeQualifier;
    stptr->base = base;
    stptr->offset = offset;
    stptr->width = width;
    stptr->initialValue = initialValue;
   
    st_top ++;
    return st_top;
}

int lookup(char *name)
{
    int i;
    for(i=0; i<st_top; i++) 
        if(!strcmp(name, symbolTable[i].name)) 
            return i;
    return -1;
}


