#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include "file.h"
#include "bacteria.h"

// Color defines

#define COLOR_RESET "\x1b[0m"
#define CUSTOM_COLOR_1 "\e[38;2;255;0;0m"
#define CUSTOM_COLOR_2 "\e[38;2;255;255;0m"

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf(CUSTOM_COLOR_1 "\nUtilizare incorecta !!!\n\n" COLOR_RESET);
        printf(CUSTOM_COLOR_1 "\nUtilizare corecta: " COLOR_RESET "./nume_program nume_fisier_intrare numar_generatii numar_thread\n\n" COLOR_RESET);
        exit(-1);
    }

    int ROWS; // size of grid
    int COLS;

    int *grid_serial; // the grid and next generation grid
    int *new_grid;

    int *grid_parallel_1;
    int *grid_parallel_2;
    int *grid_parallel_3;
    int *grid_parallel_4;

    int chunk = 10;



    char *fisier_intrare = argv[1];
    int generations = atoi(argv[2]); // number of simulated generations
    int thread_count = atoi(argv[3]); // number of threads


    double start_time;
    double end_time;

    double time_serial;
    double time_parallel_V1;
    double time_parallel_V2;
    double time_parallel_V3;
    double time_parallel_V4;



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                              SERIAL                                                    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////


    init_grid(fisier_intrare, &grid_serial, &new_grid, &ROWS, &COLS);

    start_time = omp_get_wtime();

    simulate_bacteria_serial(grid_serial, new_grid, ROWS, COLS, generations);

    end_time = omp_get_wtime();

    time_serial = end_time - start_time;

    print_grid_file(fisier_intrare, "serial", 0, grid_serial, ROWS, COLS);

    free(new_grid);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                            PARALLEL  V1                                                //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////


    init_grid(fisier_intrare, &grid_parallel_1, &new_grid, &ROWS, &COLS);

    start_time = omp_get_wtime();

    simulate_bacteria_parallel_V1(grid_parallel_1, new_grid, ROWS, COLS, generations, thread_count, chunk);

    end_time = omp_get_wtime();

    time_parallel_V1 = end_time - start_time;

    print_grid_file(fisier_intrare, "parallel", 1, grid_parallel_1, ROWS, COLS);

    free(new_grid);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                            PARALLEL  V2                                                //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////


    init_grid(fisier_intrare, &grid_parallel_2, &new_grid, &ROWS, &COLS);

    start_time = omp_get_wtime();

    simulate_bacteria_parallel_V2(grid_parallel_2, new_grid, ROWS, COLS, generations, thread_count);

    end_time = omp_get_wtime();

    time_parallel_V2 = end_time - start_time;

    print_grid_file(fisier_intrare, "parallel", 2, grid_parallel_2, ROWS, COLS);

    free(new_grid);



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                            PARALLEL   V3                                               //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    init_grid(fisier_intrare, &grid_parallel_3, &new_grid, &ROWS, &COLS);

    start_time = omp_get_wtime();

    simulate_bacteria_parallel_V3(grid_parallel_3, new_grid, ROWS, COLS, generations, thread_count);

    end_time = omp_get_wtime();

    time_parallel_V3 = end_time - start_time;

    print_grid_file(fisier_intrare, "parallel", 3, grid_parallel_3, ROWS, COLS);

    free(new_grid);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                            PARALLEL   V4                                               //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////


    init_grid(fisier_intrare, &grid_parallel_4, &new_grid, &ROWS, &COLS);

    start_time = omp_get_wtime();

    simulate_bacteria_parallel_V4(grid_parallel_4, new_grid, ROWS, COLS, generations, thread_count);

    end_time = omp_get_wtime();

    time_parallel_V4 = end_time - start_time;

    print_grid_file(fisier_intrare, "parallel", 4, grid_parallel_4, ROWS, COLS);

    free(new_grid);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                            VALIDATION                                                  //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////


    isValid(grid_serial, grid_parallel_1, ROWS, COLS, "Parallel V1");
    isValid(grid_serial, grid_parallel_2, ROWS, COLS, "Parallel V2");
    isValid(grid_serial, grid_parallel_3, ROWS, COLS, "Parallel V3");
    isValid(grid_serial, grid_parallel_4, ROWS, COLS, "Parallel V4");

    printf("\nSerial: %g\n",time_serial);
    printf("Parallel V1: %g\n",time_parallel_V1);
    printf("Parallel V2: %g\n",time_parallel_V2);
    printf("Parallel V3: %g\n",time_parallel_V3);
    printf("Parallel V4: %g\n\n",time_parallel_V4);

    free(grid_serial);
    free(grid_parallel_1);
    free(grid_parallel_2);
    free(grid_parallel_3);
    free(grid_parallel_4);

/*

    print_data_file("data_serial.txt",time_serial);
    print_data_file("data_parallel_V1.txt",time_parallel_V1);
    print_data_file("data_parallel_V2.txt",time_parallel_V2);
    print_data_file("data_parallel_V3.txt",time_parallel_V3);
    print_data_file("data_parallel_V4.txt",time_parallel_V4);

*/

    return 0;
}

// compile -DDEBUG