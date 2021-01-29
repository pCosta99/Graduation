%option noyywrap yylineno

%{
	#include "y.tab.h"
    char* c;
%}

alphanum     [a-zA-Z0-9]

alpha        [a-zA-Z]

digit        [0-9]

otherchar    ([!"]|[$&'\(\)\*\+,-\.\/]|[:;\<\>\=\?]|[\[\]\^]|`|[{\|}\~])

pointer      @{alphanum}({alpha}|{digit}|{otherchar}|\ |#)*@ 

%x INPOINTER
%x INCONTENT

%%

^\t*                    { ; }

(0|[1-9][0-9]?)        { yylval.vnumber = atoi(yytext) ; return number;  }

{pointer}               { yylval.vword = strdup(yytext) ; return pointer; }

\                       { return delim; }

BIRT                    { return BIRT; }

BURI                    { return BURI; }

CONC                    { BEGIN INCONTENT ; return CONC; }

CONT                    { BEGIN INCONTENT ; return CONT; }

DEAT                    { return DEAT; }

FAMC                    { return FAMC; }

NAME                    { BEGIN INCONTENT; return NAME; }

INDI                    { return INDI; }

<INCONTENT>.*           { yylval.vword = strdup(yytext); return content; }
<INCONTENT>\n           { BEGIN 0; }


[\n\r\t]                { ; }

.                       { return ERRO; }

%%

 //<IN>\ *{pointer}\ *     {yylval.vword = strdup(yytext) ; return pointer; }
 // <IN>.*                  {yylval.vword = strdup(yytext) ; return content;}
 // <IN>\n                  { BEGIN 0; }
  //ABBR   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ABBR; }
  //
  //ADDR   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ADDR; }
  //
  //ADR1   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ADR1; }
  //
  //ADR2   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ADR2; }
  //
  //ADOP   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ADOP; }
  //
  //AFN    { BEGIN IN ; yylval.vword = strdup(yytext) ; return AFN;  }
  //
  //AGE    { BEGIN IN ; yylval.vword = strdup(yytext) ; return AGE;  }
  //
  //AGNC   { BEGIN IN ; yylval.vword = strdup(yytext) ; return AGNC; }  
  //
  //ALIA   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ALIA; }
  //
  //ANCE   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ANCE; }
  //
  //ANUL   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ANUL; }
  //
  //ASSO   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ASSO; }  
  //
  //AUTH   { BEGIN IN ; yylval.vword = strdup(yytext) ; return AUTH; }
  //
  //BARM   { BEGIN IN ; yylval.vword = strdup(yytext) ; return BARM; }
  //
  //BASM   { BEGIN IN ; yylval.vword = strdup(yytext) ; return BASM; }
  //
  //BIRT   { BEGIN IN ; yylval.vword = strdup(yytext) ; return BIRT; }
  //
  //BLES   { BEGIN IN ; yylval.vword = strdup(yytext) ; return BLES; }
  //
  //BLOB   { BEGIN IN ; yylval.vword = strdup(yytext) ; return BLOB; }
  //
  //BURI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return BURI; }
  //
  //CALN   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CALN; }
  //
  //CAST   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CAST; }
  //
  //CAUS   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CAUS; }
  //
  //CENS   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CENS; }
  //
  //CHAN   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CHAN; }
  //
  //CHAR   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CHAR; }
  //
  //CHIL   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CHIL; }
  //
  //CHRA   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CHRA; }
  //
  //CITY   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CITY; }
  //
  //CONC   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CONC; }
  //
  //CONF   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CONF; }
  //
  //CONT   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CONT; }
  //
  //COPR   { BEGIN IN ; yylval.vword = strdup(yytext) ; return COPR; }
  //
  //CORP   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CORP; }
  //
  //CREM   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CREM; }
  //
  //CTRY   { BEGIN IN ; yylval.vword = strdup(yytext) ; return CTRY; }
  //
  //DATA   { BEGIN IN ; yylval.vword = strdup(yytext) ; return DATA; }
  //
  //DATE   { BEGIN IN ; yylval.vword = strdup(yytext) ; return DATE; }
  //
  //DEAT   { BEGIN IN ; yylval.vword = strdup(yytext) ; return DEAT; }
  //
  //DESC   { BEGIN IN ; yylval.vword = strdup(yytext) ; return DESC; }
  //
  //DEST   { BEGIN IN ; yylval.vword = strdup(yytext) ; return DEST; }
  //
  //DIV    { BEGIN IN ; yylval.vword = strdup(yytext) ; return DIV;  }
  //
  //DIVF   { BEGIN IN ; yylval.vword = strdup(yytext) ; return DIVF; }
  //
  //DSCR   { BEGIN IN ; yylval.vword = strdup(yytext) ; return DSCR; }
  //
  //EDUC   { BEGIN IN ; yylval.vword = strdup(yytext) ; return EDUC; }
  //
  //EMAIL  { BEGIN IN ; yylval.vword = strdup(yytext) ; return EMAIL; }
  //
  //EMIG   { BEGIN IN ; yylval.vword = strdup(yytext) ; return EMIG; }
  //
  //ENDL   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ENDL; }
  //
  //EVEN   { BEGIN IN ; yylval.vword = strdup(yytext) ; return EVEN; }
  //
  //FAM    { BEGIN IN ; yylval.vword = strdup(yytext) ; return FAM;  }
  //
  //FAMC   { BEGIN IN ; yylval.vword = strdup(yytext) ; return FAMC; }
  //
  //FAMF   { BEGIN IN ; yylval.vword = strdup(yytext) ; return FAMF; }
  //
  //FAMS   { BEGIN IN ; yylval.vword = strdup(yytext) ; return FAMS; }
  //
  //FCOM   { BEGIN IN ; yylval.vword = strdup(yytext) ; return FCOM; }
  //
  //FILE   { BEGIN IN ; yylval.vword = strdup(yytext) ; return _FILE; }
  //
  //FORM   { BEGIN IN ; yylval.vword = strdup(yytext) ; return FORM; }
  //
  //GEDC   { BEGIN IN ; yylval.vword = strdup(yytext) ; return GEDC; }
  //
  //GIVN   { BEGIN IN ; yylval.vword = strdup(yytext) ; return GIVN; }
  //
  //GRAD   { BEGIN IN ; yylval.vword = strdup(yytext) ; return GRAD; }
  //
  //HEAD   { BEGIN IN ; yylval.vword = strdup(yytext) ; return HEAD; }
  //
  //HUSB   { BEGIN IN ; yylval.vword = strdup(yytext) ; return HUSB; }
  //
  //IDNO   { BEGIN IN ; yylval.vword = strdup(yytext) ; return IDNO; }
  //
  //IMMI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return IMMI; }
  //
  //INDI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return INDI; }
  //
  //LANG   { BEGIN IN ; yylval.vword = strdup(yytext) ; return LANG; }
  //
  //LEGA   { BEGIN IN ; yylval.vword = strdup(yytext) ; return LEGA; }
  //
  //MARB   { BEGIN IN ; yylval.vword = strdup(yytext) ; return MARB; }
  //
  //MARC   { BEGIN IN ; yylval.vword = strdup(yytext) ; return MARC; }
  //
  //MARL   { BEGIN IN ; yylval.vword = strdup(yytext) ; return MARL; }
  //
  //MARR   { BEGIN IN ; yylval.vword = strdup(yytext) ; return MARR; }
  //
  //MARS   { BEGIN IN ; yylval.vword = strdup(yytext) ; return MARS; }
  //
  //MEDI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return MEDI; }
  //
  //NAME   { BEGIN IN ; yylval.vword = strdup(yytext) ; return NAME; }
  //
  //NATI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return NATI; }
  //
  //NATU   { BEGIN IN ; yylval.vword = strdup(yytext) ; return NATU; }
  //
  //NCHI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return NCHI; }
  //
  //NICK   { BEGIN IN ; yylval.vword = strdup(yytext) ; return NICK; }
  //
  //NMR    { BEGIN IN ; yylval.vword = strdup(yytext) ; return NMR;  }
  //
  //NOTE   { BEGIN IN ; yylval.vword = strdup(yytext) ; return NOTE; }
  //
  //NPFX   { BEGIN IN ; yylval.vword = strdup(yytext) ; return NPFX; }
  //
  //NSFX   { BEGIN IN ; yylval.vword = strdup(yytext) ; return NSFX; }
  //
  //OBJE   { BEGIN IN ; yylval.vword = strdup(yytext) ; return OBJE; }
  //
  //OCCU   { BEGIN IN ; yylval.vword = strdup(yytext) ; return OCCU; }
  //
  //ORDI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ORDI; }
  //
  //ORDN   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ORDN; }
  //
  //PAGE   { BEGIN IN ; yylval.vword = strdup(yytext) ; return PAGE; }
  //
  //PEDI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return PEDI; }
  //
  //PHON   { BEGIN IN ; yylval.vword = strdup(yytext) ; return PHON; }
  //
  //PLAC   { BEGIN IN ; yylval.vword = strdup(yytext) ; return PLAC; }
  //
  //POST   { BEGIN IN ; yylval.vword = strdup(yytext) ; return POST; }
  //
  //PROP   { BEGIN IN ; yylval.vword = strdup(yytext) ; return PROP; }
  //
  //PUBL   { BEGIN IN ; yylval.vword = strdup(yytext) ; return PUBL; }
  //
  //QUAY   { BEGIN IN ; yylval.vword = strdup(yytext) ; return QUAY; }
  //
  //REFN   { BEGIN IN ; yylval.vword = strdup(yytext) ; return REFN; }
  //
  //RELA   { BEGIN IN ; yylval.vword = strdup(yytext) ; return RELA; }
  //
  //RELI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return RELI; }
  //
  //REPO   { BEGIN IN ; yylval.vword = strdup(yytext) ; return REPO; }
  //
  //RESI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return RESI; }
  //
  //RESN   { BEGIN IN ; yylval.vword = strdup(yytext) ; return RESN; }
  //
  //RETI   { BEGIN IN ; yylval.vword = strdup(yytext) ; return RETI; }
  //
  //RFN    { BEGIN IN ; yylval.vword = strdup(yytext) ; return RFN;  }
  //
  //RIN    { BEGIN IN ; yylval.vword = strdup(yytext) ; return RIN;  }
  //
  //ROLE   { BEGIN IN ; yylval.vword = strdup(yytext) ; return ROLE; }
  //
  //SEX    { BEGIN IN ; yylval.vword = strdup(yytext) ; return SEX;  }
  //
  //SLGC   { BEGIN IN ; yylval.vword = strdup(yytext) ; return SLGC; }
  //
  //SLGS   { BEGIN IN ; yylval.vword = strdup(yytext) ; return SLGS; }
  //
  //SOUR   { BEGIN IN ; yylval.vword = strdup(yytext) ; return SOUR; }
  //
  //SPFX   { BEGIN IN ; yylval.vword = strdup(yytext) ; return SPFX; }
  //
  //SSN    { BEGIN IN ; yylval.vword = strdup(yytext) ; return SSN;  }
  //
  //STAE   { BEGIN IN ; yylval.vword = strdup(yytext) ; return STAE; }
  //
  //STAT   { BEGIN IN ; yylval.vword = strdup(yytext) ; return STAT; }
  //
  //SUBM   { BEGIN IN ; yylval.vword = strdup(yytext) ; return SUBM; }
  //
  //SUBN   { BEGIN IN ; yylval.vword = strdup(yytext) ; return SUBN; }
  //
  //SURN   { BEGIN IN ; yylval.vword = strdup(yytext) ; return SURN; }
  //
  //TEMP   { BEGIN IN ; yylval.vword = strdup(yytext) ; return TEMP; }
  //
  //TEXT   { BEGIN IN ; yylval.vword = strdup(yytext) ; return TEXT; }
  //
  //TIME   { BEGIN IN ; yylval.vword = strdup(yytext) ; return TIME; }
  //
  //TITL   { BEGIN IN ; yylval.vword = strdup(yytext) ; return TITL; }
  //
  //TRLR   { BEGIN IN ; yylval.vword = strdup(yytext) ; return TRLR; }
  //
  //TYPE   { BEGIN IN ; yylval.vword = strdup(yytext) ; return TYPE; }
  //
  //VERS   { BEGIN IN ; yylval.vword = strdup(yytext) ; return VERS; }
  //
  //WIFE   { BEGIN IN ; yylval.vword = strdup(yytext) ; return WIFE; }
  //
  //WILL   { BEGIN IN ; yylval.vword = strdup(yytext) ; return WILL; }
  //
  //_{alphanum}+            { BEGIN IN ; yylval.vword = strdup(yytext) ; return tag; }
