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
%token BASM BIRT BLES BLOB BURI CALN CAST CAUS CENS CHAN CHAR CHIL CHRA
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
    : Level Delim XRef Tag LnVal
    ;

Level 
    : number 
    ;

XRef 
    : pointer Delim 
    |
    ;

Tag 
    : ABBR
    | ADDR
    | ADR1
    | ADR2
    | ADOP
    | AFN
    | AGE
    | AGNC
    | ALIA
    | ANCE
    | ANUL
    | ASSO
    | AUTH
    | BARM
    | BASM
    | BIRT
    | BLES
    | BLOB
    | BURI
    | CALN
    | CAST
    | CAUS
    | CENS
    | CHAN
    | CHAR
    | CHIL
    | CHRA
    | CITY
    | CONC
    | CONF
    | CONT
    | COPR
    | CORP
    | CREM
    | CTRY
    | DATA 
    | DATE 
    | DEAT
    | DESC
    | DEST
    | DIV
    | DIVF
    | DSCR
    | EDUC
    | EMAIL
    | EMIG
    | ENDL
    | EVEN
    | FAM
    | FAMC
    | FAMF
    | FAMS
    | FCOM
    | _FILE
    | FORM
    | GEDC
    | GIVN
    | GRAD
    | HEAD
    | HUSB
    | IDNO
    | IMMI
    | INDI
    | LANG
    | LEGA
    | MARB
    | MARC
    | MARL
    | MARR
    | MARS
    | MEDI
    | NAME
    | NATI
    | NATU
    | NCHI
    | NICK
    | NMR
    | NOTE 
    | NPFX
    | NSFX
    | OBJE
    | OCCU
    | ORDI
    | ORDN
    | PAGE
    | PEDI
    | PHON
    | PLAC
    | POST
    | PROP
    | PUBL
    | QUAY
    | REFN
    | RELA
    | RELI
    | REPO
    | RESI
    | RESN
    | RETI
    | RFN
    | RIN
    | ROLE
    | SEX
    | SLGC
    | SLGS
    | SOUR
    | SPFX
    | SSN
    | STAE
    | STAT
    | SUBM
    | SUBN
    | SURN
    | TEMP
    | TEXT
    | TIME
    | TITL
    | TRLR
    | TYPE
    | VERS
    | WIFE
    | WILL
    ;

LnVal
    : pointer   
    | content   
    |
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

