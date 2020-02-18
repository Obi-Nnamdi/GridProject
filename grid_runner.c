#include<stdio.h>
#include<stdlib.h>
#include "grid.c"

int main(void)
{
    puts("A graph with length 3 and width 5: ");
    Grid grid = make(3, 5);
    print_grid(grid);
    change_start(&grid, 0, 1);
    change_end(&grid, 4, 2);

    puts("Changing the Start and End Locations");
    print_grid(grid);

    puts("Resizing the Grid");
    resize(&grid, 6, 9);
    print_grid(grid);

    puts("Making Obstacles:");
    rand_obstacles(grid);
    print_grid(grid);

    puts("Finding a Path:");
    find_path(&grid);

    print_grid(grid);

    puts("Finding the Fastest Path:");
    find_fastest_path(&grid);

    puts("Finding all the Paths:");
    printf("The grid has %zu paths, ",find_all_paths(&grid).size);
    
    puts("Destroying the grid and making a new one:");
    destroy_grid(&grid);
    grid = make(4, 4);

    puts("Finding the paths, and finding all of the paths with different obstacles:");
    find_path(&grid);
    find_all_paths(&grid);
    makeObstacle(grid, 3, 2);
    find_path(&grid);
    find_all_paths(&grid);
    find_fastest_path(&grid);
    makeObstacle(grid, 2, 2);
    makeObstacle(grid, 1, 2);
    find_path(&grid);
    find_fastest_path(&grid);

    puts("How printing looks like to the user:");
    user_print(grid);
}