#include "MiniC.tbl"                 /* Mini C table for appendix A */
//#define  NO_RULES    97            /* number of rules */
//#define  GOAL_RULE  (NO_RULES+1)   /* accept rule */
//#define  NO_SYMBOLS  85            /* number of grammar symbols */
//#define  NO_STATES  153            /* number of states */
#define  PS_SIZE    100              /* size of parsing stack */

typedef struct nodeType {
        struct tokenType token;            // ÅäÅ« Á¾·ù
        enum {terminal, nonterm} noderep;  // ³ëµåÀÇ Á¾·ù
        struct nodeType * son;             // ¿ÞÂÊ ¸µÅ©
        struct nodeType * brother;         // ¿À¸¥ÂÊ ¸µÅ©
		struct nodeType * father;		   // »óÀ§ ¸µÅ©
} Node;

void semantic(int);
void printToken(struct tokenType token);
void dumpStack();
void errorRecovery();

Node *buildNode(struct tokenType token);
Node *buildTree(int nodeNumber, int rhsLength);
int meaningfulToken(struct tokenType token);

enum nodeNumber {
   ACTUAL_PARAM, ADD,         ADD_ASSIGN,   ARRAY_VAR,   ASSIGN_OP,  
   BREAK_ST,     CALL,        CASE_ST,      CHAR_LITERAL,CHAR_TYPE,  
   COMPOUND_ST,  CONSTANT_EXP,CONST_TYPE,   CONTINUE_ST, DCL,  
   DCL_ITEM,     DCL_LIST,    DCL_SPEC,     DEFAULT_ST,  DIV,  
   DIV_ASSIGN,   EQ,          ERROR,        EXP_ST,      FLOAT_LITERAL,  
   FLOAT_TYPE,   FORMAL_PARA, FOR_ST,       FUNC_DEF,    FUNC_HEAD,  
   GE,           GT,          IDENT,        IF_ELSE_ST,  IF_ST,  
   INDEX,        INIT_PART,   INT_LITERAL,  INT_TYPE,    LE,  
   LOGICAL_AND,  LOGICAL_NOT, LOGICAL_OR,   LT,          MODIFY_PART,  
   MOD_ASSIGN,   MUL,         MUL_ASSIGN,   NE,          PARAM_DCL,  
   POST_DEC,     POST_INC,    PRE_DEC,      PRE_INC,     PROGRAM,  
   REMAINDER,    RETURN_ST,   SIMPLE_VAR,   STAT_LIST,   SUB,  
   SUB_ASSIGN,   SWITCH_ST,   TEST_PART,    UNARY_MINUS, VOID_TYPE,  
   WHILE_ST
};

char *nodeName[] = {
   "ACTUAL_PARAM", "ADD",         "ADD_ASSIGN",   "ARRAY_VAR",   "ASSIGN_OP",  
   "BREAK_ST",     "CALL",        "CASE_ST",      "CHAR_LITERAL","CHAR_TYPE",  
   "COMPOUND_ST",  "CONSTANT_EXP","CONST_TYPE",   "CONTINUE_ST", "DCL",  
   "DCL_ITEM",     "DCL_LIST",   "DCL_SPEC",     "DEFAULT_ST",  "DIV",  
   "DIV_ASSIGN",   "EQ",          "ERROR",        "EXP_ST",      "FLOAT_LITERAL",  
   "FLOAT_TYPE",   "FORMAL_PARA", "FOR_ST",       "FUNC_DEF",    "FUNC_HEAD",  
   "GE",           "GT",          "IDENT",        "IF_ELSE_ST",  "IF_ST",  
   "INDEX",        "INIT_PART",   "INT_LITERAL",  "INT_TYPE",    "LE",  
   "LOGICAL_AND",  "LOGICAL_NOT", "LOGICAL_OR",   "LT",          "MODIFY_PART",  
   "MOD_ASSIGN",   "MUL",         "MUL_ASSIGN",   "NE",          "PARAM_DCL",  
   "POST_DEC",     "POST_INC",    "PRE_DEC",      "PRE_INC",     "PROGRAM",  
   "REMAINDER",    "RETURN_ST",   "SIMPLE_VAR",   "STAT_LIST",   "SUB",  
   "SUB_ASSIGN",   "SWITCH_ST",   "TEST_PART",    "UNARY_MINUS", "VOID_TYPE",
   "WHILE_ST"
};

int ruleName[] = {
 /* 0            1            2            3           4           */
	  0,           PROGRAM,     0,           0,          0,
 /* 5            6            7            8           9           */
	  0,           FUNC_DEF,    FUNC_HEAD,   DCL_SPEC,   0,
 /* 10           11           12           13          14          */
	  0,           0,           0,           CONST_TYPE, INT_TYPE,
 /* 15           16           17           18          19          */
	  CHAR_TYPE,   FLOAT_TYPE,  VOID_TYPE,   0,          FORMAL_PARA,
 /* 20           21           22           23          24          */
    0,           0,           0,           0,          PARAM_DCL,
 /* 25           26           27           28          29          */
    COMPOUND_ST, DCL_LIST,    DCL_LIST,    0,          0,
 /* 30           31           32           33          34          */
    DCL,         0,           0,           DCL_ITEM,   DCL_ITEM,
 /* 35           36           37           38          39          */
    SIMPLE_VAR,  ARRAY_VAR,   0,           0,          STAT_LIST,
 /* 40           41           42           43          44          */
    0,           0,           0,           0,          0,
 /* 45           46           47           48          49          */
    0,           0,           0,           0,          0,
 /* 50           51           52           53          54          */
    0,           EXP_ST,      0,           0,          CASE_ST,
 /* 55           56           57           58          59          */
    DEFAULT_ST,  IF_ST,       IF_ELSE_ST,  SWITCH_ST,  WHILE_ST,
 /* 60           61           62           63          64          */
    FOR_ST,      INIT_PART,   TEST_PART,   MODIFY_PART,RETURN_ST,
 /* 65           66           67           68          69          */
    CONTINUE_ST, BREAK_ST,    0,           0,          ASSIGN_OP,
 /* 70           71           72           73          74          */
    ADD_ASSIGN,  SUB_ASSIGN,  MUL_ASSIGN,  DIV_ASSIGN, MOD_ASSIGN,
 /* 75           76           77           78          79          */
    0,           LOGICAL_OR,  0,           LOGICAL_AND,0,
 /* 80           81           82           83          84          */
    EQ,          NE,          0,           GT,         LT,
 /* 85           86           87           88          89          */
    GE,          LE,          0,           ADD,        SUB,
 /* 90           91           92           93          94          */
    0,           MUL,         DIV,         REMAINDER,  0,
 /* 95           96           97           98          99          */
	  UNARY_MINUS, LOGICAL_NOT, PRE_INC,     PRE_DEC,    0,   
 /* 100          101          102          103         104         */
    INDEX,       CALL,        POST_INC,    POST_DEC,   0,
 /* 105          106          107          108         109         */
    0,           ACTUAL_PARAM,0,           0,          0,
 /* 110          111          112          113         114         */
    0,           0,           CONSTANT_EXP,0,          0,
 /* 115                                                         */
    0           
};

int sp;                               // stack pointer
int stateStack[PS_SIZE];              // state stack
int symbolStack[PS_SIZE];             // symbol stack
Node *valueStack[PS_SIZE];            // value stack

Node *parser()
{ 
  extern int parsingTable[NO_STATES][NO_SYMBOLS+1];
  extern int leftSymbol[NO_RULES+1], rightLength[NO_RULES+1];
  int entry, ruleNumber, lhs;
  int currentState;
	struct tokenType token;
	Node *ptr;

	sp = 0; stateStack[sp] = 0;  // initial state
  token = scanner();
  while (1) {
    currentState = stateStack[sp];
    entry = parsingTable[currentState][token.number];
    if (entry > 0)                          /* shift action */
    { 
			sp++;
			if (sp > PS_SIZE) {
					printf("critical compiler error: parsing stack overflow");
					exit(1);
			}
			symbolStack[sp] = token.number;
			stateStack[sp] = entry;
			valueStack[sp] = meaningfulToken(token) ? buildNode(token) : NULL;
      token = scanner();
    }
    else if (entry < 0)                   /* reduce action */
    { 
      ruleNumber = -entry;
      if (ruleNumber == GOAL_RULE) /* accept action */
      { 
//      printf(" *** valid source ***\n");
        return valueStack[sp-1];
      }
//    semantic(ruleNumber);
			ptr = buildTree(ruleName[ruleNumber], rightLength[ruleNumber]);
      sp = sp - rightLength[ruleNumber];
      lhs = leftSymbol[ruleNumber];
      currentState = parsingTable[stateStack[sp]][lhs];
			sp++;
			symbolStack[sp] = lhs;
			stateStack[sp] = currentState;
			valueStack[sp] = ptr;
    }
    else                               /* error action */
    { 
      printf(" === error in source ===\n");
			printf("Current Token : ");
			printToken(token);
			dumpStack();
      errorRecovery();
			token = scanner();
    }
  } /* while (1) */
} /* parser */

void semantic(int n)
{
  printf("reduced rule number = %d\n", n);
}

void dumpStack()
{
	int i, start;

	if (sp > 10) start = sp - 10;
	else start = 0;
	
	printf("\n *** dump state stack :");
	for (i=start; i <= sp; ++i)
		printf(" %d", stateStack[i]);

	printf("\n *** dump symbol stack :");
	for (i=start; i <= sp; ++i)
		printf(" %d", symbolStack[i]);
	printf("\n");
}

void printToken(struct tokenType token)
{
   if (token.number == tident || token.number == tcharliteral)
	   printf("%s", token.value.id);
   else if (token.number == tintliteral)
	   printf("%d", token.value.num);
   else if (token.number == tfloatliteral)
     printf("%.*f", token.value.fnum.len ,token.value.fnum.num);
   else 
	   printf("%s", tokenName[token.number]);
}

void errorRecovery()
{
	struct tokenType tok;
	int parenthesisCount, braceCount;
	int i;

	// step 1: skip to the semicolon
	parenthesisCount = braceCount = 0;
	while (1) {
		tok = scanner();
    //printf("error token: %d\n",tok.number);
		if (tok.number == teof) exit(1);
		if (tok.number == tlparen) parenthesisCount++;
		   else if (tok.number == trparen) parenthesisCount--;
		if (tok.number == tlbrace) braceCount++;
		   else if (tok.number == trbrace) braceCount--;
		if ((tok.number==tsemicolon) && (parenthesisCount<=0) && (braceCount<=0))
			break;
	}

	// step 2: adjust state stack
	for (i=sp; i>=0; i--) {
		// statement_list ->  statement_list .  statement
		if (stateStack[i] == 39) break;	 // second statement part

		// statement_list ->  .  statement
		// statement_list ->  .  statement_list statement
		if (stateStack[i] == 26) break;	 // first statement part

		// declaration_list ->  declaration_list .  declaration
		if (stateStack[i] == 27) break;  // second internal dcl

		// declaration_list ->  .  declaration
		// declaration_list ->  .  declaration_list declaration
		if (stateStack[i] == 19) break;  // internal declaration

		// external declaration
		// external_dcl ->  .  declaration
		if (stateStack[i] == 2) break;	// after first external dcl
		if (stateStack[i] == 0) break;	// first external declaration
	}
	sp = i;
}

int meaningfulToken(struct tokenType token)
{
	  if ((token.number == tident) || (token.number == tintliteral) || token.number == tfloatliteral || token.number == tcharliteral)
		  return 1;
	  else return 0;
}

Node *buildNode(struct tokenType token)
{
       Node *ptr;
       ptr = (Node *) malloc(sizeof(Node));
       if (!ptr) { printf("malloc error in buildNode()\n");
                   exit(1);
       }
       ptr->token = token;
       ptr->noderep = terminal;
       ptr->son = ptr->brother = NULL;
       return ptr;
}

Node *buildTree(int nodeNumber, int rhsLength)
{
	   int i, j, start;
       Node *first, *ptr;

       i = sp-rhsLength + 1;
       /* step 1: find a first index with node in value stack */
       while (i <= sp && valueStack[i] == NULL) i++;
       if (!nodeNumber && i > sp) return NULL;
       start = i;
       /* step 2: linking brothers */
       while (i <= sp-1) {
          j = i + 1;
          while (j <= sp && valueStack[j] == NULL) j++;
          if (j <= sp) {
               ptr = valueStack[i];
               while (ptr->brother) ptr = ptr->brother;
               ptr->brother=valueStack[j];
          }
          i = j;
       }
       first = (start > sp) ? NULL : valueStack[start];
       /* step 3: making subtree root and linking son */
       if (nodeNumber) {
          ptr = (Node *) malloc(sizeof(Node));
          if (!ptr) { printf("malloc error in buildTree()\n");
                      exit(1);
          }
          ptr->token.number = nodeNumber;
          ptr->noderep = nonterm;
          ptr->son = first;
          ptr->brother = NULL;
          return ptr;
       }
       else return first; 
}
