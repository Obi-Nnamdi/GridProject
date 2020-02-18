#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "grid.c"

void obstacles(Grid grid);
void change_extremes(GridPtr grid);
void find_paths(Grid grid);
void resize_grid(GridPtr grid);
bool point_exists(Grid grid, int x, int y);
int main (void)
{
    int grid_length = -1;
    int grid_width = -1;
    while(grid_length < 0 && grid_width < 0)
    {
        printf("Enter grid length (> 0): ");
        scanf("%d", &grid_length);
        printf("Enter grid width (> 0): ");
        scanf("%d", &grid_width);
    }
    Grid grid = make(grid_length, grid_width);
    while (true)
    {
        int input;
        printf("You can:\n1. Make an Obstacle | 2. Reset All Obstacles | 3. Change the Start or End | 4. Find Paths | 5. Resize the Grid | 6. Print the Grid | 7. Quit: ");
        scanf("%d", &input);
        switch (input)
        {
        case 1:
            obstacles(grid);
            break;
        case 2:
            resetObstacles(grid);
            break;
        case 3:
            change_extremes(&grid);
            break;
        case 4:
            find_paths(grid);
            break;
        case 5:
            resize_grid(&grid);
            break;
        case 6:
            user_print(grid);
            break;
        
        default:
            return 0;
            break;
        }

    }
}

void obstacles(Grid grid)
{
    int input = -1;
    int x = -1;
    int y = -1;
    int lowX = -1;
    int lowY = -1; 
    int highX = -1; 
    int highY = -1;
    printf("You can:\n1. Make one obstacle | 2. Make a range of obstacles | 3. Randomize Obstacles | 4. Go back: ");
    scanf("%d", &input);
    switch (input)
    {
    case 1:
        while (!point_exists(grid, x, y))
        {
            printf("X value: ");
            scanf("%d", &x);
            printf("Y value: ");
            scanf("%d", &y);

        }
        makeObstacle(grid, x, y);
        break;

    case 2:
        while (!(point_exists(grid, lowX, lowY) && point_exists(grid, highX - 1, highY - 1) && highX > lowX && highY > lowY))
        {
            printf("Low X value: ");
            scanf("%d", &lowX);
            printf("High X value: ");
            scanf("%d", &highX);
            printf("Low Y value: ");
            scanf("%d", &lowY);
            printf("High Y value: ");
            scanf("%d", &highY);
        }
        makeObstacleRange(grid, lowX, highX, lowY, highY);
        break;
    case 3:
        rand_obstacles(grid);
        break;
    
    default:
        return;
        break;
    }
}

void change_extremes(GridPtr grid)
{
    int input = -1;
    int x = -1;
    int y = -1;
    printf("You can:\n1. Change the Start Coordinates | 2. Change the End Coordinates | 3. Go Back: ");
    scanf("%d", &input);
    switch (input)
    {
    case 1:
        while(!point_exists(*grid, x, y))
        {
            printf("Start X value: ");
            scanf("%d", &x);
            printf("Start Y value: ");
            scanf("%d", &y);
        }
        change_start(grid, x, y);
        break;
    case 2:
        while(!point_exists(*grid, x, y))
        {
            printf("End X value: ");
            scanf("%d", &x);
            printf("End Y value: ");
            scanf("%d", &y);
        }
        change_end(grid, x, y);
        break;
    default:
        break;
    }

}

void find_paths(Grid grid)
{
    int input = -1;
    printf("You can:\n1. Find a Path (quick) | 2. Find the Fastest Path (slower) | 3. Find all Paths (slow) | 4. Go Back: ");
    scanf("%d", &input);
    switch (input)
    {
    case 1:
        find_path(&grid);
        break;
    case 2:
        find_fastest_path(&grid);
        break;
    case 3:
        printf("There are %zu possible paths for this grid\n", find_all_paths(&grid).size);
        break;
    default:
        break;
    }
}

void resize_grid(GridPtr grid)
{
    int length = -1;
    int width = -1;
    while(length < 0 && width < 0)
    {
        printf("New Grid Length: ");
        scanf("%d", &length);
        printf("New Grid Width: ");
        scanf("%d", &width);
    }
    resize(grid, length, width);
}

bool point_exists(Grid grid, int x, int y)
{
    return x >= 0 && x < (int)grid.width && y >= 0 && y < (int)grid.length;
}