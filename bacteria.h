#ifndef _BACTERIA_H
#define _BACTERIA_H

void swap_ptr(int **p1, int **p2);

int count_neighbors(int x, int y, int *grid, int ROWS, int COLS);

void simulate_bacteria_serial(int *grid, int *new_grid, int ROWS, int COLS, int generations);

void simulate_bacteria_parallel_V1(int *grid, int *new_grid, int ROWS, int COLS, int generations, int thread_count, int chunk);

void simulate_bacteria_parallel_V2(int *grid, int *new_grid, int ROWS, int COLS, int generations, int thread_count);

void simulate_bacteria_parallel_V3(int *grid, int *new_grid, int ROWS, int COLS, int generations, int thread_count);

void simulate_bacteria_parallel_V4(int *grid, int *new_grid, int ROWS, int COLS, int generations, int thread_count);

#endif