#ifndef _FILE_H
#define _FILE_H

void print_data_file(const char* nume_output, double time);

void printValid(char* nume);

void printInvalid(char *nume);

void isValid(int *grid_serial, int *grid_parallel, int ROWS, int COLS, char* nume);

void genereazaNumeFisier(const char *numeInitial, char *numeNou, char *mode, int version_value);

void print_grid_file(const char *numeFisier, char *mode, int version_value, int* grid, int ROWS, int COLS);

void print_grid_console(int * grid, int ROWS, int COLS);

void init_grid(const char *numeFisier, int **grid, int **new_grid, int* ROWS, int *COLS);
#endif