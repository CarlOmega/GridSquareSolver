#include <stdio.h>
#define MAX_SIZE 10 //maximum inputed grid size

//Structure to hold the grid to easily send and edit/copy a grid
struct GRID{
    int length;
    int grid[MAX_SIZE][MAX_SIZE];
    int rowTotals[MAX_SIZE];
    int columnTotals[MAX_SIZE];
};
//to initialize the grid to no size.
struct GRID NewGRID()
{
    struct GRID newGrid;
    newGrid.length = 0; 
    return newGrid;
}
//reads a text file in the given format and returns the data into the struct GRID (does not error check assumes input will follow standard)
int readGridFile(struct GRID *original, char location[])
{
    FILE *inputfile = NULL;
    int inputBuffer = 0;
    inputfile = fopen(location, "r");
    //Checks if file exsists
	if (inputfile == NULL)
    {
        printf("Unable to open input file.\n");
        fclose(inputfile);
        return 0;
    }
    //loads the size
    if (fscanf(inputfile, "%d", &inputBuffer) == 1)
    {
        original->length = inputBuffer;
        
    }
    //loads all the elements
    for (int i1 = 0; i1 < original->length; i1++)
    {
        for (int i2 = 0; i2 < original->length; i2++)
        {
             if (fscanf(inputfile, "%d", &inputBuffer) == 1)
             {
                 original->grid[i1][i2] = inputBuffer;
             }
        }
    } 
    //loads the totals
    for (int i = 0; i < original->length; i++)
    {
        if (fscanf(inputfile, "%d", &inputBuffer) == 1)
        {
            original->rowTotals[i] = inputBuffer;
        }
    }
    for (int i = 0; i < original->length; i++)
    {
        if (fscanf(inputfile, "%d", &inputBuffer) == 1)
        {
            original->columnTotals[i] = inputBuffer;
        }
    }
    //closes file
    fclose(inputfile);
    return 1;
}
//prints a grid in a nice format
void printGrid(struct GRID gridPrint)
{
    //top bar __________________
    for (int i = 0; i < gridPrint.length; i++)
    {
        printf("____");
    }
    printf("\n");
    //prints elements and row totals at the end
    for (int i1 = 0; i1 < gridPrint.length; i1++)
    {
        for (int i2 = 0; i2 < gridPrint.length; i2++)
        {
            if (gridPrint.grid[i1][i2] != -1)
            {
                printf("|%2d|", gridPrint.grid[i1][i2]);
            }
            else
            {
                printf("|  |");
            }
        }
        printf("%2d", gridPrint.rowTotals[i1]);
        printf("\n");
    }
    //bottom bar _____________________
    for (int i = 0; i < gridPrint.length; i++)
    {
        printf("____");
    }
    printf("\n");
    //column totals
    for (int i = 0; i < gridPrint.length; i++)
    {
        printf("|%2d|", gridPrint.columnTotals[i]);
    }
    printf("\n\n");
}
//tests to see if the grid is solved if not returns 0 otherwise return 1
int testForComplete(struct GRID grid)
{
    int currentRowTotal[MAX_SIZE] = {0};
    int currentColumnTotal[MAX_SIZE] = {0};
    //checks that the elements are not -1 or out of range then stores the totals
    for (int i1 = 0; i1 < grid.length; i1++)
    {
        for (int i2 = 0; i2 < grid.length; i2++)
        {
            if (grid.grid[i1][i2] >= 0 && grid.grid[i1][i2] <= 9)
            {
                currentRowTotal[i1] += grid.grid[i1][i2];
                currentColumnTotal[i2] += grid.grid[i1][i2];
            }
            else 
            {
                return 0;
            }
        }
    }
    //compares totals then returns if not complete
    for (int i = 0; i < grid.length; i++)
    {
        if (currentColumnTotal[i] != grid.columnTotals[i] || currentRowTotal[i] != grid.rowTotals[i])
        {
            return 0;
        }
    }
    return 1;
}
//recursive function that solves and trys numbers then backtracks to problem
int solveGrid(struct GRID *unsolved, int row, int column)
{
    //quick outline on what is happening
    //check if complete
    //check if out of bounds if so restart because no complete
    //check if -1
        //check if can be solved/checked with no other -1s
        //if checked and can do -1 solver but number isnt 0-9 return false
        //try brute force
    //if not -1 move to next one 
    if (testForComplete(*unsolved))
    {
        return 1;
    }
    if (row == unsolved->length)
    {
        if (solveGrid(unsolved, 0, 0)) return 1;
    }
    if (unsolved->grid[row][column] == -1)
    {
        int rowCount = 0;
        int rowTotal = 0;
        int columnCount = 0;
        int columnTotal = 0;
        
        for (int i = 0; i < unsolved->length; i++)
        {
            //making sure it doesnt count current element
            if (i != column)
            {
                if (unsolved->grid[row][i] == -1) rowCount++;
                else rowTotal += unsolved->grid[row][i];
            }
            if (i != row)
            {
                if (unsolved->grid[i][column] == -1) columnCount++;
                else columnTotal += unsolved->grid[i][column];
            }
        }
        // if either them are 0 then can be solved and then checked?
        int rowNumber = (unsolved->rowTotals[row] - rowTotal);
        int columnNumber = (unsolved->columnTotals[column] - columnTotal);
        //subbing in values calculated with current elements and totals
        if (!rowCount && !columnCount)
        {
            if (rowNumber == columnNumber) 
            {
                if (rowNumber > -1 && rowNumber < 10) 
                    unsolved->grid[row][column] = rowNumber;
                else return 0;
                if (solveGrid(unsolved, row, column))
                {
                    return 1;
                }
                unsolved->grid[row][column] = -1;
                return 0;
            }
            else return 0;
        }
        else if (!rowCount)
        {
            if (rowNumber > -1 && rowNumber < 10) 
                unsolved->grid[row][column] = rowNumber;
            else return 0;
            if (solveGrid(unsolved, row, column))
            {
                return 1;
            }
            unsolved->grid[row][column] = -1;
            return 0;
        }
        else if (!columnCount)
        {
            if (columnNumber > -1 && columnNumber < 10) 
                unsolved->grid[row][column] = columnNumber;
            else return 0;
            if (solveGrid(unsolved, row, column))
            {
                return 1;
            }
            unsolved->grid[row][column] = -1;
            return 0;
        }
        //brute forceish method for solving
        for (int number = 9; number >= 0; number--)
        {
            unsolved->grid[row][column] = number;
            if (column == unsolved->length - 1)
            {
                if (solveGrid(unsolved, row + 1, 0)) return 1;
            }
            else 
            {
                if (solveGrid(unsolved, row, column + 1)) return 1;
            }
            unsolved->grid[row][column] = -1;
        }
    }
    else
    {
        if (column == (unsolved->length - 1))
        {
            return (solveGrid(unsolved, row + 1, 0));
        }
        else
        {
            return (solveGrid(unsolved, row, column + 1));
        }
    }
    //failsafe to say not complete 
    return 0;
}

int main(int argc, char **argv)
{
    //starting grid and setting it to newgrid with length 0
    struct GRID original = NewGRID();
    //directory to file
    char location[1000];
    printf("Please enter file name or directory using \n");
    scanf("%s", location);
    //checks if file read properly
	 if (readGridFile(&original, location))
    {
        printGrid(original);
    }
    else
    {
        printf("Error");
        return 0;
    }
    //solve grid and print result
    if (solveGrid(&original, 0, 0))
    {
        printf("Completed\n");
        printGrid(original);
    }
    else
    {
        printf("Fail\n");
        printGrid(original);
    }
    return 0;
}