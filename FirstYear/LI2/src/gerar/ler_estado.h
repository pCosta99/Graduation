#ifndef ___LER_ESTADO_H___
#define ___LER_ESTADO_H___

ESTADO ler_estado();

void get_file_name(char *query_string, char *filename);

void get_action(char *query_string, char *action);

void get_user(char *query_string, char *user);

void inicializar();

void win(ESTADO e);

void choose_user();

ESTADO fileToHist(ESTADO e);

#endif