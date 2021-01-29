%{
    #include <stdio.h>
    extern int yylex();
    extern int yylineno;
    extern char* yytext;
    int yyerror();
%}

%union{
    int vnumber;
    char valpha;
    char* vword;
}

%token ERRO tag number pointer delim string terminator  content
%token ABBR ADDR ADR1 ADR2 ADOP AFN AGE AGNC ALIA ANCE ANUL ASSO AUTH BARM
%token BASM BIRT BLES BLOB BURI CALN CAST CAUS CENS CHAN CHAR CHIL CHRA CHR
%token CITY CONC CONF CONT COPR CORP CREM CTRY DATA DATE DEAT DESC DEST
%token DIV DIVF DSCR EDUC EMAIL EMIG ENDL EVEN FAM FAMC FAMF FAMS FCOM
%token _FILE FORM GEDC GIVN GRAD HEAD HUSB IDNO IMMI INDI LANG LEGA MARB
%token MARC MARL MARR MARS MEDI NAME NATI NATU NCHI NICK NMR NOTE NPFX
%token NSFX OBJE OCCU ORDI ORDN PAGE PEDI PHON PLAC POST PROP PUBL QUAY
%token REFN RELA RELI REPO RESI RESN RETI RFN RIN ROLE SEX SLGC SLGS SOUR
%token SPFX SSN STAE STAT SUBM SUBN SURN TEMP TEXT TIME TITL TRLR TYPE
%token VERS WIFE WILL

%type <vnumber> number  
%type <vword> pointer string content

%%

GedCom 
    : GedCom GedComLine  {printf("ok!\n");}
    | GedComLine         {printf("ok1\n");} 
    ;

GedComLine 
    : Level Delim Tag 
    ;

Level 
    : number 
    ;

Tag
    : EventTag
    | ContentTag
    ;

EventTag
    : BIRT 
    | BURI
    | CHR 
    | DEAT
    ;

ContentTag 
    : pointer Delim INDI 
    | FAMC Delim pointer 
    | ADDR content 
    | CONC content  {printf("%s\n",$2);} 
    | CONT content 
    | NAME content
    ;

Delim 
    : delim 
    ;

%%

int main(){
    yyparse();
    return 0;
}

int yyerror(){
    printf("Erro na linha: %d, com o texto: %s\n", yylineno, yytext);
    return 0;
}

