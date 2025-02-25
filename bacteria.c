#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include <omp.h>

// Schedule
// dyinamic
// static
// guided

void swap_ptr(int **p1, int **p2) // used to swap grids between generations
{
    int *tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

int count_neighbors(int x, int y, int *grid, int ROWS, int COLS) // count alive neighbours of (x,y) in grid
{
    int count = 0;
    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx != 0 && dy != 0)
            { // count only neighbours, not current cell
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS)
                {
                    if (grid[nx * COLS + ny] == 1)
                        count++;
                }
            }
        }

    return count;
}

void simulate_bacteria_serial(int *grid, int *new_grid, int ROWS, int COLS, int generations)
{
    for (int gen = 0; gen < generations; gen++)
    {
        // Apply the rules of the simulation
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
            {
                new_grid[i * COLS + j] = 0; // by default no bacterium

                int neighbors = count_neighbors(i, j, grid, ROWS, COLS);

                if (grid[i * COLS + j] == 1)
                { // If current cell has a bacterium
                    if (neighbors == 2 || neighbors == 3)
                        new_grid[i * COLS + j] = 1; // Survives in next generation
                }
                else
                { // If current cell is empty
                    if (neighbors == 3)
                        new_grid[i * COLS + j] = 1; // New bacterium appears here
                }
            }
        // Make new grid to old grid for the next generation
        swap_ptr(&grid, &new_grid);
#ifdef DEBUG
        printf("\nAfter generation %d:\n", gen);
        print_grid_console(grid, ROWS, COLS);
#endif
    }
}

void simulate_bacteria_parallel_V1(int *grid, int *new_grid, int ROWS, int COLS, int generations, int thread_count, int chunk)
{
    for (int gen = 0; gen < generations; gen++)
    {
//                                   thread_count          Schedule  chunk_size
#pragma omp parallel for num_threads(thread_count) schedule(static, chunk)
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
            {
                new_grid[i * COLS + j] = 0; // by default no bacterium

                int neighbors = count_neighbors(i, j, grid, ROWS, COLS);

                if (grid[i * COLS + j] == 1)
                { // If current cell has a bacterium
                    if (neighbors == 2 || neighbors == 3)
                        new_grid[i * COLS + j] = 1; // Survives in next generation
                }
                else
                { // If current cell is empty
                    if (neighbors == 3)
                        new_grid[i * COLS + j] = 1; // New bacterium appears here
                }
            }

        // Make new grid to old grid for the next generation
        swap_ptr(&grid, &new_grid);

#ifdef DEBUG
        printf("\nAfter generation %d:\n", gen);
        print_grid_console(grid, ROWS, COLS);
#endif
    }
}

void simulate_bacteria_parallel_V2(int *grid, int *new_grid, int ROWS, int COLS, int generations, int thread_count)
{
    // Initialize the threads once
    #pragma omp parallel num_threads(thread_count)
    {
        for (int gen = 0; gen < generations; gen++)
        {
            // Only parallelize the inner loop to process each cell
            #pragma omp for
            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    new_grid[i * COLS + j] = 0; // By default, no bacterium

                    int neighbors = count_neighbors(i, j, grid, ROWS, COLS);

                    if (grid[i * COLS + j] == 1)
                    { // If current cell has a bacterium
                        if (neighbors == 2 || neighbors == 3)
                            new_grid[i * COLS + j] = 1; // Survives in next generation
                    }
                    else
                    { // If current cell is empty
                        if (neighbors == 3)
                            new_grid[i * COLS + j] = 1; // New bacterium appears here
                    }
                }
            }

            // Swap the grids for the next generation, ensuring only one thread does this
            #pragma omp single
            swap_ptr(&grid, &new_grid);

            #ifdef DEBUG
            #pragma omp single
            {
                printf("\nAfter generation %d:\n", gen);
                print_grid_console(grid, ROWS, COLS);
            }
            #endif
        }
    } // End of parallel region
}

void simulate_bacteria_parallel_V3(int *grid, int *new_grid, int ROWS, int COLS, int generations, int thread_count)
{
    for (int gen = 0; gen < generations; gen++)
    {
#pragma omp parallel num_threads(thread_count)
        {
            int thread_id = omp_get_thread_num();

            // Calculate the range of rows this thread will handle
            int rows_per_thread = ROWS / thread_count;
            int start_row = thread_id * rows_per_thread;
            int end_row = (thread_id == thread_count - 1) ? ROWS : start_row + rows_per_thread;

            for (int i = start_row; i < end_row; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    new_grid[i * COLS + j] = 0; // by default no bacterium

                    int neighbors = count_neighbors(i, j, grid, ROWS, COLS);

                    if (grid[i * COLS + j] == 1)
                    { // If current cell has a bacterium
                        if (neighbors == 2 || neighbors == 3)
                            new_grid[i * COLS + j] = 1; // Survives in next generation
                    }
                    else
                    { // If current cell is empty
                        if (neighbors == 3)
                            new_grid[i * COLS + j] = 1; // New bacterium appears here
                    }
                }
            }
        }

        // Make new grid to old grid for the next generation
        swap_ptr(&grid, &new_grid);

#ifdef DEBUG
        printf("After generation %d:\n", gen);
        print_grid_console(grid, ROWS, COLS);
#endif
    }
}

void simulate_bacteria_parallel_V4(int *grid, int *new_grid, int ROWS, int COLS, int generations, int thread_count)
{
    for (int gen = 0; gen < generations; gen++)
    {
#pragma omp parallel num_threads(thread_count)
        {
#pragma omp single // Ensure tasks are created by a single thread
            {
                for (int i = 0; i < ROWS; i++)
                {
#pragma omp task firstprivate(i)
                    {
                        for (int j = 0; j < COLS; j++)
                        {
                            new_grid[i * COLS + j] = 0; // by default no bacterium

                            int neighbors = count_neighbors(i, j, grid, ROWS, COLS);

                            if (grid[i * COLS + j] == 1)
                            { // If current cell has a bacterium
                                if (neighbors == 2 || neighbors == 3)
                                    new_grid[i * COLS + j] = 1; // Survives in next generation
                            }
                            else
                            { // If current cell is empty
                                if (neighbors == 3)
                                    new_grid[i * COLS + j] = 1; // New bacterium appears here
                            }
                        }
                    }
                }
            } // End of single section
        } // End of parallel region

        // Make new grid to old grid for the next generation
        swap_ptr(&grid, &new_grid);

#ifdef DEBUG
        printf("\nAfter generation %d:\n", gen);
        print_grid_console(grid, ROWS, COLS);
#endif
    }
}