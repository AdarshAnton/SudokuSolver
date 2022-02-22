#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int matrix[36][36],size;
// #define size 16

// int matrix[16][16] = {
//     {1,0,0,2,3,4,0,0,12,0,6,0,0,0,7,0},
//     {0,0,8,0,0,0,7,0,0,3,0,0,9,10,6,11},
//     {0,12,0,0,10,0,0,1,0,13,0,11,0,0,14,0},
//     {3,0,0,15,2,0,0,14,0,0,0,9,0,0,12,0},
//     {13,0,0,0,8,0,0,10,0,12,2,0,1,15,0,0},
//     {0,11,7,6,0,0,0,16,0,0,0,15,0,0,5,13},
//     {0,0,0,10,0,5,15,0,0,4,0,8,0,0,11,0},
//     {16,0,0,5,9,12,0,0,1,0,0,0,0,0,8,0},
//     {0,2,0,0,0,0,0,13,0,0,12,5,8,0,0,3},
//     {0,13,0,0,15,0,3,0,0,14,8,0,16,0,0,0},
//     {5,8,0,0,1,0,0,0,2,0,0,0,13,9,15,0},
//     {0,0,12,4,0,6,16,0,13,0,0,7,0,0,0,5},
//     {0,3,0,0,12,0,0,0,6,0,0,4,11,0,0,16},
//     {0,7,0,0,16,0,5,0,14,0,0,1,0,0,2,0},
//     {11,1,15,9,0,0,13,0,0,2,0,0,0,14,0,0},
//     {0,14,0,0,0,11,0,2,0,0,13,3,5,0,0,12}
// };

void read_grid_from_file(int size, char *ip_file, int grid[36][36]) {
	FILE *fp;
	int i, j;
	fp = fopen(ip_file, "r");
	for (i=0; i<size; i++) 
		for (j=0; j<size; j++) {
			fscanf(fp, "%d", &grid[i][j]);
	}
} 

void print_sudoku()
{
    int i,j;
    for(i=0;i<size;i++)
    {
        for(j=0;j<size;j++)
        {
            printf("%d\t",matrix[i][j]);
        }
        printf("\n\n");
    }
}

int number_unassigned(int *row, int *col)
{
    int num_unassign = 0;
    int i,j;
    for(i=0;i<size;i++)
    {
        for(j=0;j<size;j++)
        {

            if(matrix[i][j] == 0)
            {

                *row = i;
                *col = j;

                num_unassign = 1;
                return num_unassign;
            }
        }
    }
    return num_unassign;
}


int is_safe(int n, int r, int c)
{
    int i,j;
    // int *ans=-1;
    //checking in row
    for(i=0;i<size;i++)
    {

        if(matrix[r][i] == n){
            // *ans=0;
            return 0;
            }
    }

    for(i=0;i<size;i++)
    {
        
        if(matrix[i][c] == n){
            // *ans=0;
            return 0;
            }
    }

    int row_start = (r/3)*3;
    int col_start = (c/3)*3;
    for(i=row_start;i<row_start+3;i++)
    {
        for(j=col_start;j<col_start+3;j++)
        {
            if(matrix[i][j]==n){
            // *ans=0;
            return 0;
            }
        }
    }
    
    // *ans=1;
    return 1;
            
}

int solve_sudoku()
{
    int row;
    int col;

    if(number_unassigned(&row, &col) == 0)
        return 1;
    int n,i;
    //number between 1 to 9
    for(i=1;i<=size;i++)
    {
        pthread_t pid[size+1];
        if(!pthread_create(&pid[i],NULL,(void *)is_safe(i, row, col),NULL) && is_safe(i, row, col))
        {
            matrix[row][col] = i;

            if(solve_sudoku())
                return 1;

            matrix[row][col]=0;
        }
        pthread_join(&pid[i],NULL);
    }
    return 0;
}

// int main()
// {
//     if (solve_sudoku())
//         print_sudoku();
//     else
//         printf("No solution\n");
//     return 0;
// }

int main(int argc, char *argv[]) {
	int i, j;
	
	if (argc != 3) {
		printf("Usage: ./sudoku.out grid_size inputfile");
		exit(-1);
	}
	
	size = atoi(argv[1]);
	read_grid_from_file(size, argv[2], matrix);
	
	/* Do your thing here */
	if (solve_sudoku())
        print_sudoku();
    else
        printf("No solution\n");
    return 0;
    
	// print_grid(size, matrix);
}
