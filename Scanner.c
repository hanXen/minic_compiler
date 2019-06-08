#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int superLetter(char ch);
int superLetterOrDigit(char ch);
int getNumber(char firstCharacter);
float getRealNumber();
int hexValue(char ch);
void lexicalError(int n);

#define NO_KEYWORD 15
#define ID_LENGTH 12
#define MAXLEN 99

FILE *sourceFile = NULL;
int flag;
int flen;

enum tsymbol { tnull=-1,
  /* 0                  1                  2                  3 */
    tnot,              tnotequ,           tremainder,        tremAssign,   
  /* 4                  5                  6                  7 */           
    tcharliteral,      tfloatliteral,     tident,            tintliteral,
  /* 8                  9                 10                 11 */    
    tand,              tlparen,           trparen,           tmul,
  /* 12                13                 14                 15 */                  
    tmulAssign,        tplus,             tinc,              taddAssign,
  /* 16                17                 18                 19 */                  
    tcomma,            tminus,            tdec,              tsubAssign,
  /* 20                21                 22                 23 */                  
    tdiv,              tdivAssign,        tcolon,            tsemicolon,
  /* 24                25                 26                 27 */                  
    tless,             tlesse,            tassign,           tequal,
  /* 28                29                 30                 31 */                  
    tgreat,            tgreate,           tlbracket,         trbracket,
  /* 32                33                 34                 35 */                                 
    teof,              tbreak,            tcase,             tchar, 
  /* 36                37                 38                 39 */                             
    tconst,            tcontinue,         tdefault,          telse, 
  /* 40                41                 42                 43 */                             
    tfloat,            tfor,              tif,               tint,
  /* 44                45                 46                 47 */                               
    treturn,           tswitch,           tvoid,             twhile,
  /* 48                49                 50                    */                             
    tlbrace,           tor,               trbrace           	
};

char *tokenName[] = {
  /* 0                  1                  2                  3 */
    "!",               "!=",              "%",               "%=",   
  /* 4                  5                  6                  7 */           
    "%char_literal",   "%float_literal",  "%ident",          "%int_literal",
  /* 8                  9                 10                 11 */    
    "&&",              "(",               ")",               "*", 
  /* 12                13                 14                 15 */                  
    "*=",              "+",               "++",              "+=",
  /* 16                17                 18                 19 */                  
    ",",               "-",               "--",              "-=",
  /* 20                21                 22                 23 */                  
    "/",               "/=",              ":",               ";",
  /* 24                25                 26                 27 */                  
    "<",               "<=",              "=",               "==",
  /* 28                29                 30                 31 */                  
    ">",               ">=",              "[",               "]",
  /* 32                33                 34                 35 */                                 
    "eof",             "break",           "case",            "char",
  /* 36                37                 38                 39 */                             
    "const",           "continue",        "default",         "else", 
  /* 40                41                 42                 43 */                             
    "float",           "for",             "if",              "int",
  /* 44                45                 46                 47 */                               
    "return",          "switch",          "void",            "while",
  /* 48                49                 50                    */                             
    "{",               "||",              "}"               

};

char *keyword[NO_KEYWORD] = { 
    "break",  "case",    "char",    "const",    "continue",    "default",    "else",    
    "float",    "for",    "if",    "int",    "return",  "switch",    "void",
    "while"
};

enum tsymbol tnum[NO_KEYWORD] = {
    tbreak,    tcase,     tchar,     tconst,     tcontinue,   tdefault,     telse,
    tfloat,	   tfor,		 tif,  tint,	 treturn,		tswitch,	   tvoid,
    twhile
};

struct fliteral {
  float num;
  int len;
};

struct tokenType {
	int number;
	union {
		char id[ID_LENGTH];
		int num;
		struct fliteral fnum;
	} value;
};

struct tokenType scanner()
{
 struct tokenType token;
 int i, index;
 char ch, id[ID_LENGTH];
 
token.number = tnull;
do {
     while (isspace(ch = fgetc(sourceFile))) ;	// state 1: skip blanks
     if (superLetter(ch)) { // identifier or keyword
       i = 0;
       do {
            if (i < ID_LENGTH) id[i++] = ch;
            ch = fgetc(sourceFile);
       } while (superLetterOrDigit(ch));
	   if (i >= ID_LENGTH) lexicalError(1);
       id[i] = '\0';
       ungetc(ch, sourceFile);  //  retract
       // find the identifier in the keyword table
	   for (index=0; index < NO_KEYWORD; index++)
		   if (!strcmp(id, keyword[index])) break;
	   if (index < NO_KEYWORD)    // found, keyword exit
         token.number = tnum[index];
       else {                     // not found, identifier exit
			  token.number = tident;
			  strcpy(token.value.id, id);
       }
     }  // end of identifier or keyword
     else if (isdigit(ch)) {  // number
            flag = 0;
            flen = 0;
            token.number = tintliteral;
            token.value.num = getNumber(ch);
            if(flag == 1) {
              token.number = tfloatliteral;
            	token.value.fnum.num = (float)token.value.num + getRealNumber() ;
              token.value.fnum.len = flen;
            }
          }
     else switch (ch) {  // special character
            case '/':
                      ch = fgetc(sourceFile);
                      if (ch == '*')			// text comment
						  do {
							  while (ch != '*') ch = fgetc(sourceFile);
							  ch = fgetc(sourceFile);
						  } while (ch != '/');
                      else if (ch == '/')		// line comment
						  while (fgetc(sourceFile) != '\n') ;
                      else if (ch == '=')  token.number = tdivAssign;
                      else { token.number = tdiv;
                             ungetc(ch, sourceFile); // retract
					  }
                      break;
            case '!':
                      ch = fgetc(sourceFile);
                      if (ch == '=')  token.number = tnotequ;
                        else { token.number = tnot;
                               ungetc(ch, sourceFile); // retract
                             }
                      break;
            case '%': 
                      ch = fgetc(sourceFile);
                      if (ch == '=') {
                        token.number = tremAssign;
                      }
                      else {
                        token.number = tremainder;
                        ungetc(ch, sourceFile);
                      }
                      break;
            case '&':
                      ch = fgetc(sourceFile);
                      if (ch == '&')  token.number = tand;
						          else { lexicalError(2);
                               ungetc(ch, sourceFile);  // retract
                        }
                      break;
            case '*':
                      ch = fgetc(sourceFile);
                      if (ch == '=')  token.number = tmulAssign;
                        else { token.number = tmul;
                               ungetc(ch, sourceFile);  // retract
                             }
                      break;
            case '+':
                      ch = fgetc(sourceFile);
                      if (ch == '+')  token.number = tinc;
                        else if (ch == '=') token.number = taddAssign;
                           else { token.number = tplus;
                                  ungetc(ch, sourceFile);  // retract
                                }
                      break;
            case '-':
                      ch = fgetc(sourceFile);
                      if (ch == '-')  token.number = tdec;
                         else if (ch == '=') token.number = tsubAssign;
                              else { token.number = tminus;
                                     ungetc(ch, sourceFile);  // retract
							  }
                      break;
            case '<':
                      ch = fgetc(sourceFile);
                      if (ch == '=') token.number = tlesse;
                         else { token.number = tless;
                                ungetc(ch, sourceFile);  // retract
						 }
                      break;
            case '=':
                      ch = fgetc(sourceFile);
                      if (ch == '=')  token.number = tequal;
                        else { token.number = tassign;
                               ungetc(ch, sourceFile);  // retract
                             }
                      break;
            case '>':
                      ch = fgetc(sourceFile);
                      if (ch == '=') token.number = tgreate;
                        else { token.number = tgreat;
                               ungetc(ch, sourceFile);  // retract
                             }
                      break;
            case '|':
                      ch = fgetc(sourceFile);
                      if (ch == '|')  token.number = tor;
					  else { lexicalError(3);
                             ungetc(ch, sourceFile);  // retract
                           }
                      break;
            case '\'':
            		  ch = fgetc(sourceFile);
            		  token.number = tcharliteral;
            		  token.value.id[0] = ch;
            		  token.value.id[1] = '\0';
    				      fgetc(sourceFile);
            		  break;

            case '(': token.number = tlparen;         break;
            case ')': token.number = trparen;         break;
            case ',': token.number = tcomma;          break;
            case ':': token.number = tcolon;          break;
            case ';': token.number = tsemicolon;      break;
            case '[': token.number = tlbracket;       break;
            case ']': token.number = trbracket;       break;
            case '{': token.number = tlbrace;         break;
            case '}': token.number = trbrace;         break;
            case EOF: token.number = teof;            break;
			default:  {
						printf("Current character : %c", ch);
						lexicalError(4);
						break;
					  }

          } // switch end
   } while (token.number == tnull);
   return token;
} // end of scanner

void lexicalError(int n)
{
	printf(" *** Lexical Error : ");
	switch (n) {
		case 1: printf("an identifier length must be less than 12.\n");
				break;
		case 2: printf("next character must be &\n");
				break;
		case 3: printf("next character must be |\n");
				break;
		case 4: printf("invalid character\n");
				break;
	}
}

int superLetter(char ch)
{
	if (isalpha(ch) || ch == '_') return 1;
		else return 0;
}

int superLetterOrDigit(char ch)
{
	if (isalnum(ch) || ch == '_') return 1;
		else return 0;
}

int getNumber(char firstCharacter)
{
	int num = 0;
	int value;
	char ch;

	if (firstCharacter == '0') {
		ch = fgetc(sourceFile);
		if ((ch == 'X') || (ch == 'x'))	{		// hexa decimal
			while ((value=hexValue(ch=fgetc(sourceFile))) != -1)
				num = 16*num + value;
		}
		else if ((ch >= '0') && (ch <= '7'))	// octal
				do {
					num = 8*num + (int)(ch - '0');
					ch = fgetc(sourceFile);
				} while ((ch >= '0') && (ch <= '7'));
			 else num = 0;						// zero
	} else {									// decimal
			ch = firstCharacter;
			do {
				num = 10*num + (int)(ch - '0');
				ch = fgetc(sourceFile);
			} while (isdigit(ch));
			if (ch == '.') {
				flag = 1;
				return num;
			}
	}
    ungetc(ch, sourceFile);  /*  retract  */
	return num;
}

float getRealNumber(){
	char ch;
	float fnum = 0.0;
	ch = fgetc(sourceFile);
	do {
		fnum = fnum * 10 + (int)(ch - '0') ;
		flen ++ ;
		ch = fgetc(sourceFile);
	} while(isdigit(ch));
	for(int i = 0; i< flen; i++)
		fnum = fnum / 10;
	ungetc(ch,sourceFile);
	return fnum;
}


int hexValue(char ch)
{
	switch (ch) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			return (ch - '0');
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
			return (ch - 'A' + 10);
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
			return (ch - 'a' + 10);
		default: return -1;
	}
}
