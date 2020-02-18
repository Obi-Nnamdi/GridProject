#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "stacks.c"
#include<time.h>

typedef struct point
{
    bool obstacle, passed;
} Point;

typedef Point * PointPtr;

typedef struct grid
{
    PointPtr * plane;
    size_t length, width;
    size_t startX, startY;
    size_t endX, endY;
} Grid;

typedef Grid * GridPtr;

typedef struct grid_stack //has its own struct bc it needs to keep track of all of the vars
{
    Grid *arr;
    size_t capacity;
    size_t size;
} GridStack;

typedef GridStack *GridStackPtr;

Grid pop_grid(GridStackPtr stack);
void push_grid(GridStackPtr stack, Grid datum);
void destroy_stack(GridStackPtr stack);
void print_stack(GridStack stack); //prints from first in to last in
GridStack create_stack(void);

Grid make (const unsigned int length, const unsigned int width); //makes the grid
void refresh (GridPtr grid); //used in the find_path function to reset every obstacle's passed function
void resetObstacles (Grid grid); //used to reset every point's obstacle variable
void makeObstacle (Grid grid, size_t x, size_t y); //makes a point an obstacle
void makeObstacleRange(Grid grid, size_t startX, size_t endX, size_t startY, size_t endY); //makes a range of points an obstacle
void change_start(GridPtr grid, size_t x, size_t y); //changes the start pos
void change_end (GridPtr grid, size_t x, size_t y);
void find_path(GridPtr grid);
void find_fastest_path(GridPtr grid);
int paths(GridPtr grid);
void resize(GridPtr grid, size_t new_length, size_t new_width);
void print_grid (Grid grid);
void destroy_grid(GridPtr grid);
void rand_obstacles(Grid grid);
GridStack find_all_paths(GridPtr grid);
Grid dup_grid(GridPtr grid);
int count_passed(Grid grid);
void user_print(Grid grid);


Grid make (const unsigned int length, const unsigned int width)
{
    Grid grid = {NULL, length, width, 0, 0, width-1, length-1};
    grid.plane = malloc(sizeof(Point*) * length); //had a challenge mallocing the grid at first
    for (size_t i = 0; i < length; ++i)
    {
        grid.plane[i] = malloc(sizeof(Point) * width);
        for (size_t j = 0; j < width; ++j)
        {
            grid.plane[i][j].obstacle = false;
            grid.plane[i][j].passed = false;
        }
    }
    return grid;
}

void refresh (GridPtr grid)
{
    for (size_t i = 0; i < grid->length; ++i)
        for (size_t j = 0; j < grid->width; ++j)
            grid->plane[i][j].passed = false;
}

void makeObstacle (Grid grid, size_t x, size_t y)
{
    if (!(y == grid.startY && x == grid.startX) && !(y== grid.endY && x == grid.endX) && !grid.plane[y][x].obstacle)
        grid.plane[y][x].obstacle = true;
    else 
        grid.plane[y][x].obstacle = false;
}

void makeObstacleRange(Grid grid, size_t startX, size_t endX, size_t startY, size_t endY)
{
    for (size_t i = startY; i < endY; ++i)
        for (size_t j = startX; j < endX; ++j)
        {
            makeObstacle(grid, j, i);
        }
}

void resetObstacles (Grid grid)
{
    for (size_t i = 0; i < grid.length; ++i)
        for (size_t j = 0; j < grid.width; ++j)
            grid.plane[i][j].obstacle = false;
}

void change_start(GridPtr grid, size_t x, size_t y)
{
        grid->startX = x;
        grid->startY = y;
        if (grid->plane[y][x].obstacle)
            grid->plane[y][x].obstacle = false;
    
}

void change_end(GridPtr grid, size_t x, size_t y)
{
    grid->endX = x;
    grid->endY = y;

    if (grid->plane[y][x].obstacle)
        grid->plane[y][x].obstacle = false;
}

void print_grid(Grid grid)
{
    for (size_t i = 0; i < grid.length; ++i)
    {
        for (size_t j = 0; j < grid.width; ++j)
        {
            if (grid.startX == j && grid.startY == i)
                printf("s ");
            else if (grid.endX == j && grid.endY == i)
                printf("e ");
            else if (grid.plane[i][j].obstacle)
                printf("x ");
            else if (grid.plane[i][j].passed)
                printf(". ");
            else
                printf("- ");
        }
        puts("");
    }
    printf("Start Point: (%zu, %zu)\n", grid.startX, grid.startY);
    printf("End Point: (%zu, %zu)\n", grid.endX, grid.endY);
}

void user_print(Grid grid)
{
    for (int i = grid.length-1; i >= 0; i--)
    {
        for (size_t j = 0; j < grid.width; ++j)
        {
            if (grid.startX == j && (int)grid.startY == i)
                printf("s ");
            else if (grid.endX == j && (int)grid.endY == i)
                printf("e ");
            else if (grid.plane[i][j].obstacle)
                printf("x ");
            else if (grid.plane[i][j].passed)
                printf(". ");
            else
                printf("- ");
        }
        puts("");
    }
    printf("Start Point: (%zu, %zu)\n", grid.startX, grid.startY);
    printf("End Point: (%zu, %zu)\n", grid.endX, grid.endY);
}

void destroy_grid(GridPtr grid)
{
    for (size_t i = 0; i < grid->length; ++i)
        free(grid->plane[i]);
    free(grid->plane);
    grid->plane = NULL;
    grid->length = 0; grid->width = 0;
    grid->endX = 0; grid->endY = 0;   
    grid->startX = 0; grid->startY = 0;
}

void resize(GridPtr grid, size_t new_length, size_t new_width)
{
    void move_extremes_if_needed(GridPtr grid, size_t length, size_t width);
    move_extremes_if_needed(grid, new_length, new_width);

    for (size_t i = 0; i < grid->length; ++i)
        grid->plane[i] = realloc(grid->plane[i], new_width * sizeof(Point*));
    grid->plane = realloc(grid->plane, new_length * sizeof(Point*));

    for(size_t i = 0; i < grid->length; ++i)
        for (size_t j = grid->width; j < new_width; ++j)
        {
            grid->plane[i][j].obstacle = false;
            grid->plane[i][j].passed = false;
        }

    for(size_t i = grid->length; i < new_length; ++i)
    {
        grid->plane[i] = malloc(new_width * sizeof(Point*));
        for (size_t j = 0; j < new_width; ++j)
        {
            grid->plane[i][j].obstacle = false;
            grid->plane[i][j].passed = false;
        }
    }

    grid->length = new_length;
    grid->width = new_width;
}

void move_extremes_if_needed(GridPtr grid, size_t length, size_t width) //covers downsizing grid
{
    if (grid->startX >= width)
        grid->startX = width-1;
    if (grid->startY >= length)
        grid->startY = length - 1;
    if (grid->endX >= width)
        grid->endX = width-1;
    if (grid->endY >= length)
        grid->endY = length - 1;
    if (grid->startX == grid->endX && grid->startY == grid->endY)
    {
        grid->startX = 0;
        grid->startY = 0;
    }
    grid->plane[grid->endY][grid->endX].obstacle = false;
    grid->plane[grid->startY][grid->startX].obstacle = false;
}

void rand_obstacles (Grid grid)
{
    srand(time(NULL));
    for (size_t i = 0; i < grid.length; ++i)
        for (size_t j = 0; j < grid.width; ++j)
            if (rand() % 2)
                makeObstacle(grid, j, i);
}

void find_path(GridPtr grid)
{
    bool pref_path(GridPtr grid, size_t * x, size_t * y);
    bool nopref_path(GridPtr grid, size_t * x, size_t * y);
    size_t x = grid->startX, y = grid->startY;
    while (!(x == grid->endX && y == grid->endY))
    {
        grid->plane[y][x].passed = true;
        if (!pref_path(grid, &x, &y))
        {
            if (!nopref_path(grid, &x, &y))
            {
                puts("No Path Found");
                refresh(grid);
                return;
            }

        }
    }
    user_print(*grid);
    refresh(grid);
}

void find_fastest_path(GridPtr grid)
{
    Grid best = make(1,1);
    int paths = 0;
    int best_path = 0;
    Stack positions = create();
    push(&positions, grid->startX);
    push(&positions, grid->startY);
    GridStack grids = create_stack();
    push_grid(&grids, *grid);

    while (positions.size > 0)
    {
        int y = pop(&positions);
        int x = pop(&positions);
        Grid current_grid = pop_grid(&grids);
        current_grid.plane[y][x].passed = true;

        if (paths > 0 && count_passed(current_grid) > best_path)
        {
            continue;
        }

        if (x == (int)grid->endX && y == (int)grid->endY)
        {
            best = current_grid;
            best_path = count_passed(best);
            paths++;
            continue;
        }
        
        if (x != (int)grid->width-1 && !grid->plane[y][x + 1].obstacle && !current_grid.plane[y][x + 1].passed)
        {
            push(&positions, x + 1);
            push(&positions, y);
            push_grid(&grids, current_grid);
        }
        
        if (x != 0 && !grid->plane[y][x - 1].obstacle && !(current_grid.plane[y][x - 1].passed))
        {
            push(&positions, x - 1);
            push(&positions, y);
            push_grid(&grids, current_grid);
        }

        if (y != (int)grid->length-1 && !grid->plane[y + 1][x].obstacle && !current_grid.plane[y + 1][x].passed)
        {
            push(&positions, x);
            push(&positions, y + 1);
            push_grid(&grids, current_grid);
        }

        if (y != 0 && !grid->plane[y - 1][x].obstacle && !current_grid.plane[y - 1][x].passed)
        {
            push(&positions, x);
            push(&positions, y - 1);
            push_grid(&grids, current_grid);
        }
    }
    if (count_passed(best) != 0)
        user_print(best);
    else
    {
        puts("No path found.");
    }
}

int count_passed(Grid grid)
{
    int passed = 0;
    for (size_t i = 0; i < grid.length; ++i)
        for (size_t j = 0; j < grid.width; ++j)
            if (grid.plane[i][j].passed)
                passed++;
    return passed; 
}

bool pref_path(GridPtr grid, size_t *  x, size_t * y)
{
    if (*x < grid->endX && *x != grid->width-1 && !grid->plane[*y][*x+1].obstacle && !grid->plane[*y][*x+1].passed)
        //if you need to go right, and the point exists and it's not forbidden
        {
            ++(*x);
            return true;
        }
        else if (*x > grid->endX && *x != 0 && !grid->plane[*y][*x-1].obstacle && !grid->plane[*y][*x-1].passed)
        ////if you need to go left, and the point exists and it's not forbidden
        {
            --(*x);
            return true;
        }
        else if (*y <= grid->endY && *y != grid->length-1 && !grid->plane[*y+1][*x].obstacle && !grid->plane[*y+1][*x].passed)
        {
            ++(*y);
            return true;
        }
        else if (*y >= grid->endY && *y != 0 && !grid->plane[*y-1][*x].obstacle && !grid->plane[*y-1][*x].passed)
        {
            --(*y);
            return true;
        }
        
        return false;
}

bool nopref_path(GridPtr grid, size_t * x, size_t * y)
{
    if (*x != grid->width-1 && !grid->plane[*y][*x+1].obstacle && !grid->plane[*y][*x+1].passed)
        //if you need to go right, and the point exists and it's not forbidden
        {
            ++(*x);
            return true;
        }
        else if (*x != 0 && !grid->plane[*y][*x-1].obstacle && !grid->plane[*y][*x-1].passed)
        ////if you need to go left, and the point exists and it's not forbidden
        {
            --(*x);
            return true;
        }
        else if (*y != grid->length-1 && !grid->plane[*y+1][*x].obstacle && !grid->plane[*y+1][*x].passed)
        {
            ++(*y);
            return true;
        }
        else if (*y != 0 && !grid->plane[*y-1][*x].obstacle && !grid->plane[*y-1][*x].passed)
        {
            --(*y);
            return true;
        }
        
        return false;   
}

GridStack find_all_paths(GridPtr grid)
{
    int paths = 0;
    Stack positions = create();
    push(&positions, grid->startX);
    push(&positions, grid->startY);
    GridStack grids = create_stack();
    push_grid(&grids, *grid);
    GridStack successful = create_stack();

    while (positions.size > 0)
    {
        size_t y = pop(&positions);
        size_t x = pop(&positions);
        Grid current_grid = pop_grid(&grids);
        current_grid.plane[y][x].passed = true;

        if (x == grid->endX && y == grid->endY)
        {
            push_grid(&successful, current_grid);
            paths++;
            continue;
        }
        
        if (x != grid->width-1 && !grid->plane[y][x + 1].obstacle && !current_grid.plane[y][x + 1].passed)
        {
            push(&positions, x + 1);
            push(&positions, y);
            push_grid(&grids, current_grid);
        }
        
        if (x != 0 && !grid->plane[y][x - 1].obstacle && !(current_grid.plane[y][x - 1].passed))
        {
            push(&positions, x - 1);
            push(&positions, y);
            push_grid(&grids, current_grid);
        }

        if (y != grid->length-1 && !grid->plane[y + 1][x].obstacle && !current_grid.plane[y + 1][x].passed)
        {
            push(&positions, x);
            push(&positions, y + 1);
            push_grid(&grids, current_grid);
        }

        if (y != 0 && !grid->plane[y - 1][x].obstacle && !current_grid.plane[y - 1][x].passed)
        {
            push(&positions, x);
            push(&positions, y - 1);
            push_grid(&grids, current_grid);
        }
    }

    return successful;
}

Grid dup_grid(GridPtr grid)
{
    Grid new_grid = make(grid->length, grid->width);
    change_start(&new_grid, grid->startX, grid->startY);
    change_end(&new_grid, grid->endX, grid->endY);
    for (size_t i = 0; i < grid->length; i++)
    {
        for(size_t j = 0; j < grid->width; j++)
        {
            if (grid->plane[i][j].obstacle)
                new_grid.plane[i][j].obstacle = true;
            else if (grid->plane[i][j].passed)
                new_grid.plane[i][j].passed = true;
        }
    }
    return new_grid;
}



GridStack create_stack(void)
{
    GridStack stack;
    stack.arr = calloc(INITIAL_CAPACITY, sizeof(Grid));
    stack.capacity = INITIAL_CAPACITY;
    stack.size = 0;

    return stack;
}

void push_grid(GridStackPtr stack, Grid datum)
{
    void resize_grid_stack_if_full(GridStackPtr stack);
    resize_grid_stack_if_full(stack);
    stack->arr[stack->size] = dup_grid(&datum);
    ++stack->size;
}

void resize_grid_stack_if_full(GridStackPtr stack)
{
    if (stack->size == stack->capacity)
    {
        stack->capacity > 0 ? stack -> capacity *= 2 : (stack->capacity = INITIAL_CAPACITY);
        stack->arr = realloc(stack -> arr, stack -> capacity*sizeof(Grid));
    }
}

Grid pop_grid(GridStackPtr stack)
{
    stack->size = stack->size - 1;
    Grid tmp = dup_grid(&stack->arr[stack->size]);
    destroy_grid(&stack->arr[stack->size]);
    return tmp;
}

void destroy_stack (GridStackPtr stack)
{
    free(stack->arr);
    stack->arr = NULL;
    stack->size = 0;
    stack->capacity = 0;
}
