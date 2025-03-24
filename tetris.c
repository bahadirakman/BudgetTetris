// BAHADIR AKMAN 31.12.2023
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void printTitle(void){
    printf(" ____  _    _ _____   _____ ______ _______   _______ ______ _______ _____  _____  _____ \n");
    printf("|  _ \\| |  | |  __ \\ / ____|  ____|__   __| |__   __|  ____|__   __|  __ \\|_   _|/ ____|\n");
    printf("| |_) | |  | | |  | | |  __| |__     | |       | |  | |__     | |  | |__) | | | | (___  \n");
    printf("|  _ <| |  | | |  | | | |_ |  __|    | |       | |  |  __|    | |  |  _  /  | |  \\___ \\ \n");
    printf("| |_) | |__| | |__| | |__| | |____   | |       | |  | |____   | |  | | \\ \\ _| |_ ____) |\n");
    printf("|____/ \\____/|_____/ \\_____|______|  |_|       |_|  |______|  |_|  |_|  \\_\\_____|_____/ \n");
}
// print before starting a game
void Prompt1(void){
    printf("__________________________________________________________\n");
    printf("What do you want to do?\n");
    printf("__________________________________________________________\n");
    printf("1: Start a new game\n");
    printf("2: Show the highest score\n");
    printf("3: Exit\n");
    printf("\n");
}
// print after starting a game, every turn
void Prompt2(void){
    printf("__________________________________________________________\n");
    printf("What do you want to do?\n");
    printf("__________________________________________________________\n");
    printf("1: Place\n");
    printf("2: Rotate\n");
    printf("3: Leave the game\n");
    printf("\n");
}
// finds the maximum height of the columns of the board
int maxHeight(int * height, int col){
    int max = 0;
    
    int i;
    for(i = 0; i < col; i++){
        if(height[i] > max){
            max = height[i];
        }
    }
    return max;
}
// create a board and assign ' ' to every element, dynamic memory
void createBoard(char **board, int row, int col){  
    int i, j;
    for(i = 0; i < row; i++){
        board[i] = (char*)malloc(col * sizeof(int));
        
        for (j = 0; j < col; j++){
            board[i][j] = ' ';
        }
    }
}
// print board after every turn
void printBoard(char **board, int row, int col){
    printf("\n");
    printf("Current Board: \n\n");
    
    int r, c;
    for (r = 0; r < row; r++){
        for (c = 0; c < col; c++){
            printf("+---");
        }
        printf("+\n");
        
        for (c = 0; c < col; c++){
            printf("| %c ", board[r][c]);
        }
        printf("|\n");
    }
    for (c = 0; c < col; c++){
        printf("+---");
    }
    printf("+\n");
    // display the column numbers at the bottom of the board
    for (c = 0; c < col; c++){
        if(c >= 9) 
            printf("  %d", c+1);  // two digit numbers
        else
            printf("  %d ", c+1); // one digit numbers
    }
     printf("\n");
}
// display the shape that user will place or rotate
void printShape(char shape[3][3]){
    printf("\nCurrent Shape: \n\n");
    
    int i, j;
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            printf("%c ", shape[i][j]);
        }
        printf("\n");
    }
}
// change the shapeIndex when user asks to rotate the shape
int rotate(int shapeIndex){
    if      (shapeIndex == 0)   return 0;
    else if (shapeIndex == 1)   return 1;      
    else if (shapeIndex == 2)   return 7;     
    else if (shapeIndex == 3)   return 4;      
    else if (shapeIndex == 4)   return 3;
    else if (shapeIndex == 5)   return 8;
    else if (shapeIndex == 6)   return 9;
    else if (shapeIndex == 7)   return 2;
    else if (shapeIndex == 8)   return 5;
    else if (shapeIndex == 9)   return 10;
    else if (shapeIndex == 10)  return 11;
    else if (shapeIndex == 11)  return 6;
    else                        return -1;
}
bool place(char ** board, int row, int col, int * height, char shape[3][3], int shapeIndex, int x_length, int y_length, int x_coo){
    
    bool placed = false;
    int start = height[x_coo-1]; // start checking up from the given x coordinate

    // 8th and 10th shape behave different, add exeptions for them 
    if (shapeIndex == 10){        
        if(start >= 2){                    
            start = height[x_coo-1] - 2; 
        }
        else if(start == 1){
            start = height[x_coo-1] - 1;
        }
    }
    else if(shapeIndex == 8 && start >= 1){
        start = height[x_coo-1] - 1;
    }

    int i;
    // start checking up from the given x coordinate considering the height of the shape
    for (i = row - y_length - start; i >= 0 && !placed; i--){
        
        bool can_place = true;
        start++;
        
        // check if shape can be placed
        int m, k;
        for(m = 0; m < y_length && can_place; m++){
            for(k = 0; k < x_length && can_place; k++){
                if (board[i + m][x_coo-1 + k] == '1' && shape[m][k] == '1'){
                    can_place = false;
                }
            }
        }
        // if it can be placed, place
        if(can_place == true){
            for(m = y_length-1; m >= 0; m--){
                for(k = 0; k < x_length; k++){
                    if (board[i + m][x_coo-1 + k] == ' ' && shape[m][k] == '1'){
                        board[i + m][x_coo-1 + k] = shape[m][k];
                        if (height[x_coo - 1 + k] < start + y_length - m){
                            height[x_coo - 1 + k] = start + y_length - m - 1;
                        }
                    }
                }
            }
            placed = true;
        }
    }
    return placed;
}
// after every placement, check if we can clear a line
int clearLines(char ** board, int * height, int row, int col, int max_height){
    
    int i,j;
    int multiplier = 0; // if multiple lines are cleared, increase the multiplier

    // begin checking every row from max_height to the bottom
    for(i = row - 1; i > row - max_height - 1; i--){
        bool can_clear = true;
        for(j = 0; j < col && can_clear; j++){
            if (board[i][j] == ' '){
                can_clear = false;
            }
        }
        
        if (can_clear){
            for(j = 0; j < col && can_clear; j++){
                board[i][j] = ' '; // if a line can be cleared, change 1's into spaces
            }
            multiplier++;
            // settle the pieces from above to the newly emptied line
            int p, k;
            for (p = i; p > row - max_height - 1; p--){
                if(p == 0){
                    board[p][k] = ' ';
                }
                else{
                    for(k = 0; k < col ; k++){
                        board[p][k] = board[p-1][k];
                    }
                }
            }
            i++;
            // refresh the height values, check from the bottom until a space is found, assign the height
            for(p = 0; p < col; p++){
                height[p] = 0;
            }
            for (p = 0; p < col; p++){
                bool found = false;
                for (k = 0; k < row && !found; k++){
                    if(board[k][p] == '1'){
                        height[p] = row - k;
                        found = true;
                    }
                }
            }
        }
    }
    return multiplier;
}
// if a game ends, free the memory that dynamically allocated
void destroyBoard(char ** board, int * height, int row){
    int i;
     for(i = 0; i < row; i++){
        free(board[i]);
    }
    free(board);
    free(height);
}
int main(void){
    
    // create the shapes and keep them in a 3D array
    char shapes[12][3][3] = {{{'1',' ',' '},
                              {' ',' ',' '},
                              {' ',' ',' '}},
                             {{'1','1',' '},
                              {'1','1',' '},
                              {' ',' ',' '}},
                             {{'1',' ',' '},
                              {'1',' ',' '},
                              {' ',' ',' '}},
                             {{'1',' ',' '},
                              {'1',' ',' '},
                              {'1',' ',' '}},
                             {{'1','1','1'},
                              {' ',' ',' '},
                              {' ',' ',' '}},
                             {{' ','1','1'},
                              {'1','1',' '},
                              {' ',' ',' '}},
                             {{'1',' ',' '},
                              {'1',' ',' '},
                              {'1','1',' '}},
                             {{'1','1',' '},
                              {' ',' ',' '},
                              {' ',' ',' '}},
                             {{'1',' ',' '},
                              {'1','1',' '},
                              {' ','1',' '}},
                             {{'1','1','1'},
                              {'1',' ',' '},
                              {' ',' ',' '}},
                             {{'1','1',' '},
                              {' ','1',' '},
                              {' ','1',' '}},
                             {{' ',' ','1'},
                              {'1','1','1'},
                              {' ',' ',' '}}};
    // shapes = {square1, square2, line1_1, line2_1, line2_2, sShape1, lShape1,    --> default shapes
    //           line1_2, sShape2, lShape2, lShape3, lShape4}                      --> rotated shapes
    int x_length[12] = {1, 2, 1, 1, 3, 3, 2, 2, 2, 3, 2, 3}; // the width of every shape corresponding to their index in 'shapes'
    int y_length[12] = {1, 2, 2, 3, 1, 2, 3, 1, 3, 2, 3, 2}; // the length of every shape corresponding to their index in 'shapes'

    srand(time(0)); // seed
    int high_score = 0; // keeps the highest score until program terminates
    
    printTitle();
    Prompt1();
    int choice; 
    scanf("%d", &choice); // input for the first prompt

    // until user inputs 3 for the first prompt, keep the program running
    while (choice != 3){
        
        // Start the Game
        if(choice == 1){
            int row, col;
            do{
                printf("Please enter the size of the board (row and column should be greater than 6): \n");
                scanf("%d", &row);
                scanf("%d", &col);
            }while(row <= 6 || col <= 6);
            
            printf("\nA NEW GAME HAS STARTED!\n");
            // allocate memory for the board and the height of every column
            char ** board = (char**)malloc (row * col * sizeof(int));
            createBoard(board, row, col);
            int * height = (int*)malloc (col * sizeof(int));
            
            // initialize heights
            int i;
            for(i = 0; i < col; i++){
                height[i] = 0;
            }
            
            bool gameOn = true;   // if game is in play, true
            bool nextMove = true; // checks if a shape is placed and is it time for the next turn
            int shapeIndex;       
            int score = 0;

            while(gameOn){
                
                int choice2;
                if (nextMove){ // if its a new turn, create a new shape
                    shapeIndex = (rand() % 7);
                    printf("__________________________________________________________\n");
                    printf("__________________________________________________________\n");
                    printf("__________________________________________________________\n");
                    printf("Your Score: %d\n", score);
                    printBoard(board, row, col);
                    printShape(shapes[shapeIndex]);
                    
                    Prompt2();
                    scanf("%d", &choice2); // get the input for second prompt
                }

                if (choice2 == 3){ // user ends the game, free the board
                    gameOn = false;
                    printf("THE GAME HAS ENDED!\n"); 
                    destroyBoard(board, height, row);
                }
                else if (choice2 == 2){ // rotate the shape by assigning the corresponding shapeIndex
                    int oldIndex = shapeIndex;
                    shapeIndex = rotate(shapeIndex);
                    printf("__________________________________________________________\n");
                    printf("__________________________________________________________\n");
                    printf("__________________________________________________________\n");
                    if(shapeIndex != -1){ // rotate succeded
                        printf("Your Score: %d\n", score);
                        printBoard(board, row, col);
                        printShape(shapes[shapeIndex]);
                    }
                    else{ // rotate failed
                        printf("ROTATE FAILED!!\n");
                        shapeIndex = oldIndex;
                        printf("Your Score: %d\n", score);
                        printBoard(board, row, col);
                        printShape(shapes[shapeIndex]);
                    }
                    nextMove = false;
                    Prompt2();
                    scanf("%d", &choice2); // get the input for second prompt
                }
                else if (choice2 == 1){ // user decided to place the given shape
                    int x;
                    printf("Enter the X coordinate: ");
                    scanf("%d", &x); // get the x coordinate for the placement
                    printf("\n");

                    // if the given x coordinate is out of bounds or the shape does not fit, give the second prompt again
                    if(x <= 0 || x + x_length[shapeIndex] - 1 > col){ 
                        printf("You can not place the shape at the given coordinate!\n");
                        nextMove = false;
                        Prompt2();
                        scanf("%d", &choice2); // get the input for second prompt
                    }
                    // given x coordinate is appropriate
                    else{
                        bool success;
                        success = place(board, row, col, height, shapes[shapeIndex], shapeIndex, x_length[shapeIndex], y_length[shapeIndex], x);
                        // if the placed shape doesnt fit the board verticaly, the game is lost
                        if(!success){
                            printf("YOU LOST!!!\n");
                            nextMove = false;
                            gameOn = false;
                            destroyBoard(board, height, row);
                        }
                        else{
                            int max = maxHeight(height, col); // get max height
                            int multiplier = clearLines(board, height, row, col, max); // if multiplier is 0, no line is cleared
                            score += multiplier * col; // if several lines are cleared, multiply it with col and add it to score

                            // if the last placed shape's top blocks are on the top-most edge of the board, the game is lost
                            if(max == row){
                                printf("__________________________________________________________\n");
                                printf("__________________________________________________________\n");
                                printf("__________________________________________________________\n");
                                printf("Your Score: %d\n", score);
                                printBoard(board, row, col);
                                
                                printf("YOU LOST!!!\n");
                                nextMove = false;
                                gameOn = false;
                                destroyBoard(board, height, row);
                            }
                            else{
                                nextMove = true; // the shape is placed successfully and the game continues
                            }
                        }
                        // reassign the highest score after every turn if it is exceeded
                        if(score > high_score){
                            high_score = score;
                        }
                    }
                }
                else{
                    nextMove = false;
                    printf("Please enter a valid choice!\n");
                    Prompt2();
                    scanf("%d", &choice2); // get the input for second prompt
                }
            }
        }
        // Show highest score
        else if (choice == 2){
            printf("\n");
            printf("Your highest score is: %d\n", high_score);
        }
        else{
            printf("Please enter a valid choice!\n");
        }
        Prompt1();
        scanf("%d", &choice);
    }
    return 0;
}