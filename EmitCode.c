/********************************************************
 *         Code emitting functions                      *
 *        (Type & Grammer Extension)                    *
 ********************************************************/

#define numberOfOpcodes 48           // 35 + 4 + 1

typedef enum {
     notop, neg,   incop, decop, dup,  swp, add,  sub,   mult, 
     divop, addf, subf, multf, divopf, modop, andop, orop,  gt,    
     lt,   ge,  le,  eq,   ne, lod,  ldc, ldf, lds, lda,  ldi,   
     ldp,  str, strc, strf,  sti,  ujp,   tjp,  fjp, call,  ret,  
     retv,  chkh,  chkl, nop, proc, endop, bgn,  sym, none
} opcode;

char *mnemonic[numberOfOpcodes] = {
     "notop", "neg",  "inc", "dec",  "dup", "swp",  "add", "sub", 
	 "mult",  "div",  "addf", "subf", "multf", "divopf", "mod", 
	 "and",  "or",  "gt",   "lt",  "ge", "le",    "eq",   "ne",	 
	 "lod",  "ldc", "ldf", "lds", "lda",  "ldi", "ldp",  "str", 
	 "strc", "strf",  "sti",  "ujp", "tjp",  "fjp", "call", "ret", 
	 "retv", "chkh",  "chkl", "nop", "proc", "end", "bgn",  "sym", 
	 "none"
};

void emit0(opcode op)
{
	fprintf (ucodeFile, "           ");
	fprintf (ucodeFile, "%-10s\n", mnemonic[op]);
}

void emit1(opcode op, int num)
{
	fprintf (ucodeFile, "           ");
	fprintf (ucodeFile, "%-10s %5d\n", mnemonic[op], num);
}

void emit2(opcode op, int base, int offset)
{
	fprintf (ucodeFile, "           ");
	fprintf (ucodeFile, "%-10s %5d %5d\n", mnemonic[op], base, offset);
}
void emit3(opcode op, int p1, int p2, int p3)
{
	fprintf (ucodeFile, "           ");
	fprintf (ucodeFile, "%-10s %5d %5d %5d\n", mnemonic[op], p1, p2, p3);
}

void emitLabel(char *label)
{
	int i, noBlanks;

	fprintf (ucodeFile, "%s", label);
	noBlanks = 12-strlen(label);
	for (i=1; i<noBlanks; ++i)
		fprintf (ucodeFile, " ");
	fprintf (ucodeFile, "%-10s\n", mnemonic[nop]);

}

void emitJump(opcode op, char *label)
{
	fprintf (ucodeFile, "           ");
	fprintf (ucodeFile, "%-10s %s\n", mnemonic[op], label);
}

void emitSym(int base, int offset, int size)
{
	fprintf (ucodeFile, "           ");
	fprintf (ucodeFile, "%-10s %5d %5d %5d\n", mnemonic[sym], base, offset, size);
}

void emitFunc(char *label, int base, int offset, int size)
{
	int i, noBlanks;

	fprintf (ucodeFile, "%s", label);
	noBlanks = 12-strlen(label);
	for (i=1; i<noBlanks; ++i)
		fprintf (ucodeFile, " ");
	fprintf (ucodeFile, "%-10s", mnemonic[proc]);
	fprintf (ucodeFile, " ");
	fprintf (ucodeFile, "%5d %5d %5d\n", base, offset, size);
}

void emitChar(opcode op, char *name)
{
	fprintf (ucodeFile, "           ");
	fprintf (ucodeFile, "%-10s %5s\n", mnemonic[op], name);
}

void emitFloat(opcode op, struct fliteral fnum)
{
	fprintf (ucodeFile, "           ");
	fprintf (ucodeFile, "%-10s %5.*f\n", mnemonic[op], fnum.len, fnum.num);
}