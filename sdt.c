void printNode(Node *pt, int indent);
void printTree(Node *pt, int indent);

void printTree(Node *pt, int indent)
{
  Node *p = pt;
  while (p != NULL) {
    printNode(p, indent);
    if (p->noderep == nonterm) printTree(p->son, indent+5);
      p = p->brother;
  }
}

void printNode(Node *pt, int indent)
{
  extern FILE * astFile;
	int i;

  for (i=1; i<=indent; i++) fprintf(astFile," ");
  if (pt->noderep == terminal) {
	  if (pt->token.number == tident)
      fprintf(astFile," Terminal: %s", pt->token.value.id);
	  else if (pt->token.number == tintliteral)
      fprintf(astFile," Terminal: %d", pt->token.value.num);
    else if (pt->token.number == tfloatliteral)
      fprintf(astFile," Terminal: %.*f", pt->token.value.fnum.len ,pt->token.value.fnum.num);
    else if (pt->token.number == tcharliteral)
      fprintf(astFile," Terminal: %s", pt->token.value.id);
	 }
  else { // nonterminal node
    i = (int) (pt->token.number);
    fprintf(astFile," Nonterminal: %s", nodeName[i]);
  }
	fprintf(astFile, "\n");
} 
