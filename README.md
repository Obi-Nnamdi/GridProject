# GridProject
Compile either grid_app.c (for user input), or grid_runner.c (for a pre-generated grid) My project creates a grid, and lets a user interact with it, including placing obstacles, finding paths, or resizing it.
This project has four files: grid_app and grid_runner excecute the program (as stated above). grid.c serves as the implementation for the project, with the major functions and algorithms.
Stacks.c serves as the implmentation of a specific stack that is compatible with the Grid struct.
This project utilizes a pseudo a* algorithm for the find_path function (in grid.c), allowing it to find the shortest path the the goal for non-complex grids.
However, the a* algorithm falters slightly when dealing with more complex grids, where the find_fastest_path algorithm (in grid.c) is better.
This project utilizes a struct to represent a grid, that contains a 2D array of point structs to represent the grid.
This project utilizes the C programming language.
