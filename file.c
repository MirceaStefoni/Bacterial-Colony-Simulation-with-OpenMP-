#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file.h"

#define COLOR_RESET "\x1b[0m"
#define CUSTOM_COLOR_1 "\e[38;2;255;0;0m"
#define CUSTOM_COLOR_2 "\e[38;2;255;255;0m"
#define CUSTOM_COLOR_3 "\e[38;2;0;255;0m"

void print_data_file(const char* nume_output, double time)
{
    FILE *file;

    // Open file
    file = fopen(nume_output, "a");
    if (file == NULL)
    {
        fprintf(stderr,"Can't open file!\n");
        exit(EXIT_FAILURE);
    }

    // Append data in file
    fprintf(file, "%g\n", time);


    fclose(file);
}

void printValid(char* nume)
{
    char rezultat[200] = "";

    strcat(rezultat,nume);

    strcat(rezultat," este valid!");

    
    printf(CUSTOM_COLOR_3 "\n%s\n"COLOR_RESET, rezultat);
}

void printInvalid(char* nume)
{

    char rezultat[200] = "";

    strcat(rezultat,nume);

    strcat(rezultat," nu este valid!");

    
    printf(CUSTOM_COLOR_1 "\n%s\n"COLOR_RESET, rezultat);
}

void isValid(int *grid_serial, int *grid_parallel, int ROWS, int COLS, char* nume)
{

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (grid_serial[i * COLS + j] != grid_parallel[i * COLS + j])
            {
                printInvalid(nume);
                return;
            }
        }
    }

    printValid(nume);
    return;
}


void print_grid_console(int * grid, int ROWS, int COLS)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (grid[i * COLS + j])
            {
                printf(CUSTOM_COLOR_2 "X " COLOR_RESET);
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
    }
}

void genereazaNumeFisier(const char *numeInitial, char *numeNou, char *mode, int version_value)
{
    // extragem partea fara extenise
    char baza[200];
    strncpy(baza, numeInitial, strlen(numeInitial) - 4); // eliminam extensia txt
    baza[strlen(numeInitial) - 4] = '\0';

    if (strcmp(mode, "serial") == 0)
    {
        // formateaza noul nume de fisier
        snprintf(numeNou, 215, "%s_%s_out.txt", baza, mode);
        return;
    }

    if (strcmp(mode, "parallel") == 0)
    {
        // formateaza noul nume de fisier
        snprintf(numeNou, 215, "%s_%s_omp_V%d.txt", baza, mode, version_value);
        return;
    }

    perror("Utilizare incorecta a tipului de fisier\n");
    exit(EXIT_FAILURE);
}

void print_grid_file(const char *numeFisier, char *mode, int version_value, int* grid, int ROWS, int COLS)
{
    char numeNou[100];

    genereazaNumeFisier(numeFisier, numeNou, mode, version_value);

    FILE *fisier = fopen(numeNou, "w");
    if (fisier == NULL)
    {
        perror("Eroare la deschiderea fisierului\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (grid[i * COLS + j])
            {
                fprintf(fisier, "X ");
            }
            else
            {
                fprintf(fisier, ". ");
            }
        }
        fprintf(fisier, "\n");
    }

    fclose(fisier);
}

void init_grid(const char *numeFisier, int **grid, int **new_grid, int* ROWS, int *COLS)
{
    FILE *fisier = fopen(numeFisier, "r");
    if (fisier == NULL)
    {
        perror("Eroare la deschiderea fisierului\n");
        exit(EXIT_FAILURE);
    }

    int linii;
    int coloane;

    if (fscanf(fisier, "%d %d", &linii, &coloane) != 2)
    {
        perror("Eroare la fscanf ROWS COLS\n");
        exit(EXIT_FAILURE);
    }

    

    *ROWS = linii;
    *COLS = coloane;

    *grid = (int *)malloc(linii * coloane * sizeof(int));
    if (!(*grid))
    {
        printf("Memory allocation error for grid\n");
        exit(1);
    }
    *new_grid = (int *)malloc(linii * coloane * sizeof(int));
    if (!(*new_grid))
    {
        printf("Memory allocation error for new grid\n");
        exit(1);
    }

    char aux;

    for (int i = 0; i < linii; i++)
    {
        for (int j = 0; j < coloane; j++)
        {

            if (fscanf(fisier, " %c", &aux) != 1)
            {
                fprintf(stderr,"Eroare la fscanf pentru caractere!\n");
                exit(EXIT_FAILURE);
            }

            if (aux == 'X')
            {
                (*grid)[i * coloane + j] = 1;
            }
            else
            {
                (*grid)[i * coloane + j] = 0;
            }
        }
    }

    fclose(fisier);
}