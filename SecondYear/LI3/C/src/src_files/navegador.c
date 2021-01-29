#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "navegador.h"
#include <sys/ioctl.h>
#include <unistd.h>

#define CUT_OFF 0.25

extern FILE *popen( const char *command, const char *modes);
extern int pclose(FILE *stream);

struct navigator {
    int start;
    int size_to_print_rows;
    int size_to_print_cols;
    GPtrArray* buffer;
};

struct winsize w;

int number_linhas(){
    FILE* f = popen("tput lines", "r");
    int rows;

    fscanf(f, "%d", &rows);

    pclose(f);

    return rows;
}

int number_colunas(){
    FILE* f = popen("tput cols", "r");
    int cols;

    fscanf(f, "%d", &cols);

    pclose(f);

    return cols;
}

/*Incializa o navagador*/
Navigator init_navigator(GPtrArray* buffer){
    Navigator nav = malloc(sizeof(struct navigator));
    nav->start = 0;
    nav->size_to_print_rows = number_linhas();
    nav->size_to_print_cols = number_colunas();
    nav->buffer = buffer;
    return nav;
}

void set_cols_number(Navigator nav, int cols_number){
    nav->size_to_print_cols = cols_number;
}

void destroy_navigator(Navigator nav){
    free(nav);
}

void getPreviousPage(Navigator nav){
    int new_start = nav->start - (nav->size_to_print_rows * nav->size_to_print_cols);
    if(new_start >= 0) nav->start = new_start;
}

void getNextPage(Navigator nav){
    int new_start = nav->start + (nav->size_to_print_cols * nav->size_to_print_rows);
    if(new_start < nav->buffer->len) nav->start = new_start;
}

/*Printa o conteudo*/
void print_content(Navigator nav, int adjust){
    /*int acumulado=0;*/
    int rows;
    if(adjust) {
        int size = ((int) number_colunas() / (strlen(g_ptr_array_index(nav->buffer, 0)) + 1));
        int margin = size * CUT_OFF;
        nav->size_to_print_cols = size - margin;
    }
    int r = nav->size_to_print_cols * nav->size_to_print_rows;
    for(rows = nav->start; rows < nav->start + r && rows < nav->buffer->len; rows++){
        /*acumulado = strlen(g_ptr_array_index(nav->buffer, rows)) + 4;*/
        if(rows && rows % nav->size_to_print_cols == 0) {
            printf("\n");
            /*acumulado=0;*/
        }
        printf("%s\t", (char*) g_ptr_array_index(nav->buffer, rows));
    }
    printf("\n");
}


/*void print_content1(Navigator nav){
    int i=0;
    int t=nav->start;
    int j=nav->size_to_print_rows;
    for(i=t; (i < t+j) && (nav->buffer[i]); i++){
        printf("%s\n",nav->buffer[i]);

    }
    nav->start=t+j;
}*/
/*
int main(){
int i;
char** buffer = calloc(sizeof(char*), 200);
for(i = 0; i < 200; i++){
    char* string = malloc(sizeof(char) * 200);
    sprintf(string, "produto%d", i);
    buffer[i] = string;
}
Navigator nav = init_navigator(buffer);
print_content(nav);
printf("Pressione 'a' para a proxima pagina\n");
printf("Pressione 'd' para a pagina anterior\n");
char c='\0' ;
while(c!='q'){
    scanf("%c",&c);
switch(c){
    case 'a':
    printf("left was pressed\n");
    nav=guarda_estado_tras(nav);
    print_content(nav);
    break;

    case 'd':
    printf("right was pressed\n");
    nav=guarda_estado_frente(nav);
    print_content(nav);
    break;

    default: printf("pressione tecla valida\n");
    break;
    }
    }
    return 0;
}

float f,c,t1,t2,duty,period;
    int r1=8200,r2=68000;
    char heading[100] = "Freq(Hz)     Period     T1(high)     T2(low)     Duty Cycle";
        c= .000001;
        f = 1/(.693*(r1+(2*r2))*c);  freq(hz) = 1/ .693*(r1+(2*r2))*c
        period = 1/f;
        t1 = .693*(r1+r2)*c;
        t2 = .693*r2*c;
        duty = (t1/period)*100;
        printf("%s", heading);
        printf("\n");
    printf("%-12.2f %-10.3f %-11.3f  %-10.3f  %.3f\n", f, period,t1,t2,duty);
    printf("Column1    Column2   Column3\n");
    printf("%7.1lf%11.0lf%10.3lf\n", 100.1, 1221.0, 9348.012);
    printf("%7.1lf%11.0lf%10.3lf\n", 2.3, 211.0, 214.0);
    return 0;  */
