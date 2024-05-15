#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> //Hacemos uso de la funcion mkdir
#include <sys/types.h>
#include <errno.h>


extern int errno;

void make_folder(char* folder_name);
void make_csv(char *csv_name, float umbral);