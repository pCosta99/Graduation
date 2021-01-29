#ifndef ___FILE_H___
#define ___FILE_H___

int estadotofile(ESTADO e);

ESTADO get_estado_from_file(ESTADO e);

ESTADO get_user_from_file (ESTADO e);

void write_user_to_file (ESTADO e);

void copy_estados(char *source, char *destiny); 

#endif