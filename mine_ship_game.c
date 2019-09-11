

//------------------------------------
//	Include Libraries 
//------------------------------------
#include "mine_ship_game.h"

//------------------------------------
//	01. FUNCTION create_new_game
//------------------------------------
game* create_new_game(char* p1, char* p2, int board_size, int percentage_mines) {
	//1. We declare the variable to be returned -> The struct game to be played.
	game* res = NULL;

	//2. We create memory space for it with malloc.
	res = (game*)malloc(1 * sizeof(game));

	//3. We set the mode based on the names of the players.
	// Human vs. Human --> mode = 1
	// Human vs. Computer --> mode = 2
	// Computer vs. Human --> mode = 3
	// Computer vs. Computer --> mode = 4
	int mode = 0;
	if (strcmp(p1, "Computer") == 0)
		mode = mode + 2;
	if (strcmp(p2, "Computer") == 0)
		mode = mode + 1;
	(*res).mode = mode;

	//4. We set the status to be 1; (i.e., player1 moves first)
	(*res).status = 1;

	//5. We initialise the board_size
	(*res).board_size = board_size;

	//6. We initialise num_mines
	float num_mines = (board_size * board_size) * (percentage_mines / 100.0);
	(*res).num_mines = (int) num_mines;
	
	//7. We initialise p1_name.
	(*res).p1_name = (char*)malloc((strlen(p1) + 1) * sizeof(char));
	strcpy((*res).p1_name, p1);

	//8. We initialise p2_name.
	(*res).p2_name = (char*)malloc((strlen(p1) + 1) * sizeof(char));
	strcpy((*res).p2_name, p2);

	//9. We initialise p1_mines_uncovered.
	(*res).p1_mines_uncovered = (*res).num_mines;

	//10. We initialise p1_mines_uncovered.
	(*res).p2_mines_uncovered = (*res).num_mines;

	//11. We initialise p1_board

	//11.1. We make room for the array pointers
	(*res).p1_board = (board_cell**) malloc(board_size * sizeof(board_cell*));

	//11.2. We make room for the matrix, and adjust the pointers
	(*res).p1_board[0] = (board_cell*)malloc((board_size * board_size) * sizeof(board_cell));
	for (int i = 0; i < board_size; i++)
		(*res).p1_board[i] = (*res).p1_board[0] + (board_size * i);

	//11.3. We initialise the cells
	for (int i = 0; i < board_size; i++) {
		for (int j = 0; j < board_size; j++) {
			(*res).p1_board[i][j].value = 0;
			(*res).p1_board[i][j].visible = False;
		}
	}

	//12. We initialise p2_board

	//12.1. We make room for the array pointers
	(*res).p2_board = (board_cell**) malloc(board_size * sizeof(board_cell*));

	//12.2. We make room for the matrix, and adjust the pointers
	(*res).p2_board[0] = (board_cell*)malloc((board_size * board_size) * sizeof(board_cell));
	for (int i = 0; i < board_size; i++)
		(*res).p2_board[i] = (*res).p2_board[0] + (board_size * i);

	//12.3. We initialise the cells
	for (int i = 0; i < board_size; i++) {
		for (int j = 0; j < board_size; j++) {
			(*res).p2_board[i][j].value = 0;
			(*res).p2_board[i][j].visible = False;
		}
	}

	//13. We return res
	return res;
}

//--------------------------------------------------
// 02. gen_num
//--------------------------------------------------
int gen_num(int lb, int ub) {
	//1. We create the variable to be returned -> The random value being generated
	int res = 0;

	//2. We assign num to a random value in the desired range
	res = (rand() % (ub - lb)) + lb;

	//3. We return the output variable
	return res;
}

//--------------------------------------------------
// 03. FUNCTION my_getchar
//--------------------------------------------------
char my_get_char() {
	//1. We create the variable to be returned -> The char typed on keyboard by the user
	char res = ' ';

	//2. We create an extra variable to consume any other characters entered until a return is pressed
	boolean line_consumed = False;
	char dummy_char = ' ';

	//3. We consume the first char entered, i.e., the one we are interested at
	res = getchar();

	//4. While still there are remaining characters
	while (line_consumed == False) {
		//4.1. We consume the next character
		dummy_char = getchar();

		//4.2. If the new character is the end of line one, we can ensure we have consumed the entire line.
		if (dummy_char == '\n')
			line_consumed = True;
	}

	//5. We return res
	return res;
}

//-------------------------------------
//	04. FUNCTION place_mines_on_board
//-------------------------------------
void place_mines_on_board(board_cell** board, int board_size, int num_mines) {
	// 1. While there are more mines to be generated
	int count = 0;
	while (count < num_mines){
		// 1.1. We randomly select a cell (row and column)
		int rowNum = gen_num(0, board_size);
		int columnNum = gen_num(0, board_size);
		// 1.2. If the cell is not already occupied (with value != 9)
        if ((*(&board[rowNum][columnNum])).value != 9){
			// 1.2.1. We occupy it
			(*(&board[rowNum][columnNum])).value = 9;
			// 1.2.2. We increase by 1 all its non-mine neighbours

			// 1.2.2.1. Left Up neighbour
			if (rowNum-1 >= 0) {
                if (columnNum - 1 >= 0 && (*(&board[rowNum - 1][columnNum - 1])).value != 9)
                    (*(&board[rowNum - 1][columnNum - 1])).value++;
                // 1.2.2.2. Up neighbour
                if ((*(&board[rowNum - 1][columnNum])).value != 9) 
					(*(&board[rowNum - 1][columnNum])).value++;
                // 1.2.2.3. Right Up neighbour
                if (columnNum + 1 < board_size && (*(&board[rowNum - 1][columnNum + 1])).value != 9)
					(*(&board[rowNum - 1][columnNum + 1])).value++;
            }
			// 1.2.2.4. Left neighbour
            if (columnNum-1 >= 0 && (*(&board[rowNum][columnNum-1])).value != 9) 
				(*(&board[rowNum][columnNum-1])).value++;
			// 1.2.2.5. Right neighbour
            if (columnNum+1 < board_size && (*(&board[rowNum][columnNum+1])).value != 9) 
				(*(&board[rowNum][columnNum+1])).value++;

            if (rowNum+1 < board_size) {
                // 1.2.2.6. Left Bottom neighbour
                if (columnNum - 1 >= 0 && (*(&board[rowNum + 1][columnNum - 1])).value != 9)
					(*(&board[rowNum + 1][columnNum - 1])).value++;
                // 1.2.2.7. Bottom neighbour
                if ((*(&board[rowNum + 1][columnNum])).value != 9) 
					(*(&board[rowNum + 1][columnNum])).value++;
                // 1.2.2.8. Right Bottom neighbour
                if (columnNum + 1 < board_size && (*(&board[rowNum + 1][columnNum + 1])).value != 9)
					(*(&board[rowNum + 1][columnNum + 1])).value++;
            }
			// 1.2.3. We decrease num_mines
			num_mines--;
		}
	}
}

//----------------------------------------
//	05. FUNCTION display_board_content
//----------------------------------------
void display_board_content(board_cell** board, int board_size, int mines_uncovered) {
	//1. We print the flag of board content
    printf("\t\t---------------\n\t\t BOARD CONTENT \n\t\t---------------\n");
	// 2. We print the column names, using the value 'A' as reference
	// 2.1. We print a row of lines
    printf("\n\t\t|---|---|---|");
	// 2.2. We print a row with the column names
    char letter = 'A';
    printf("\n\t\t|");
    for (int i = 0; i < board_size; i++) {
        printf(" %c |", letter);
        letter++;
    }
	// 2.3. We print a row of lines
    printf("\n\t\t|---|---|---|");
	//3. We print the rows of the board, using 1 as reference
    int row = 1;
	//3.1. We traverse each row
    for (int j = 0; j < board_size; j++) {
        //3.1.1. We print the row_name
        printf("\n%d\t\t|", row);
        //3.1.2. We traverse each column
        for (int k = 0; k < board_size; k++) {
            //3.1.2.1. If visible, we print each cell content:
            if ((*(&board[row - 1][k])).visible == True) {
                // We print '*' for a position with a mine
                if ((*(&board[row - 1][k])).value == 9)
                    printf(" * |");
                // We print the board_cell value (number of surrounding mine neighbours) for a position with no mine.
                else
                    printf(" %d |", (*(&board[row - 1][k])).value);
            }
            //3.1.2.2. If non-visible, we print ' '
            else
                printf("   |");

        }
        //2.2. Print first a row of dash lines
        printf("\n\t\t|---|---|---|");
        row++;
        //2.3. We increase the column name
    }

	//4. We print the number of mines uncovered
	printf("\nMines Uncovered = %d ", mines_uncovered);

}

//---------------------------------------
//	06. FUNCTION user_get_movement_index
//---------------------------------------
int user_get_movement_index(int status, int board_size, board_cell** p1_board, board_cell** p2_board) {
	//1. We create the variable to be returned -> Position in the board the user wants to uncover
	int res = -1;

	//2. While we have not received a valid position.
    while (res == -1) {
        //2.1. We print a message asking for a new movement.
        printf("\nPlease do your next movement\n");
        //2.2. We collect them from the keyboard
        printf("Enter a row number: ");
        char num = my_get_char();
        printf("\nEnter a column letter: ");
        char letter = my_get_char();

        int iRow = num - '1';
        int iCol = letter - 'A';

        //2.3. If the position is not valid, we inform the user
        if (iRow < 0 || iRow > (board_size-1) || iCol < 0 || iCol >(board_size - 1))
		//if (num < '0' || num > ('0' + board_size) || letter < 'A' || letter > (''))
            printf("This position does not exist, please try again.");
        //2.4. If the position is valid
        else {
            if (status == 1) {
                //2.4.1. If the position is occupied (visible == True), we inform the user
                if ((*(&p2_board[iRow][iCol])).visible == True)
                    printf("This position is already occupied, please try again.");
                //2.4.2. If the position was not occupied (visible == False), we return the position board[row][column] in integer format
                else {
                    res = (iRow * board_size) + iCol;
                }
            }
            else if (status == 2){
                if ((*(&p1_board[iRow][iCol])).visible == True)
                    printf("This position is already occupied, please try again.");
                else {
                    res = (iRow * board_size) + iCol;
                }
            }
        }
    }
	//3. We return res
	return res;
}

//-------------------------------------------
//	07. FUNCTION computer_get_movement_index
//-------------------------------------------
int computer_get_movement_index(int status, int board_size, board_cell** p1_board, board_cell** p2_board) {
	//1. We create the variable to be returned -> Position in the board the computer wants to uncover
	int res = -1;

	//2. While we have not received a valid position.
    while (res == -1) {
        //2.1. We generate row and column randomly
        int row = gen_num(0, board_size);
        int column = gen_num(0, board_size);
        //2.2. If the position was not occupied (visible == False), we return the position board[row][column] in integer format
        if (status == 1) {
            if ((*(&p2_board[row][column])).visible == False)
                res = (row * board_size) + column;
        }
        else if (status == 2){
            if ((*(&p1_board[row][column].visible)) == False)
                res = (row * board_size) + column;
        }
    }
	//3. We return res
	return res;
}

//---------------------------------------
//	08. FUNCTION get_next_movement_index
//---------------------------------------
int get_next_movement_index(int status, char* p1_name, char* p2_name, int board_size, board_cell** p1_board, board_cell** p2_board) {
	//1. We create the variable to be returned -> Position in the board the user or the computer wants to uncover
	int res = -1;

	// 2. Depending on status, we decide who moves next

	// 2.1. If status is 1, then we check p1_name to determine whether the player must move as a user or as a computer
    if (status == 1) {
        printf("%s has to do the next move", p1_name);
		if (strcmp(p1_name, "Computer") != 0) {
			// 2.1.1. One way or another, we collect the movement made by the user
			res = user_get_movement_index(status, board_size, p1_board, p2_board);
		}
		else {
			res = computer_get_movement_index(status, board_size, p1_board, p2_board);
		}
    }
    // 2.2. If status is 2, then we check p2_name to determine whether the player must move as a user or as a computer
    else if (status == 2){
        printf("%s has to do the next move", p2_name);
		if (strcmp(p2_name, "Computer") != 0) {
			// 2.2.1. One way or another, we collect the movement made by the user
			res = user_get_movement_index(status, board_size, p1_board, p2_board);
		}
		else {
			res = computer_get_movement_index(status, board_size, p1_board, p2_board);
		}
    }
	// 3. We return res
	return res;
}

//------------------------------------
//	09. FUNCTION process_movement
//------------------------------------
void process_movement(int position_to_uncover, int* status, int board_size, board_cell** p1_board, board_cell** p2_board,
		              int* p1_mines_uncovered, int* p2_mines_uncovered) {

	// 1. We get the concrete row and column from position_to_uncover
    int row = position_to_uncover / board_size;
    int column = position_to_uncover % board_size;
	// 2. Depending on the status, we uncover the position in one player or another

	// 2.1. If status == 1
    if (*status == 1) {
        //2.1.1. We uncover the position in the p2_board
        (*(&p2_board[row][column])).visible = True;
        //2.1.2. If the position was a mine, we decrease the number of mines to hit
        if ((*(&p2_board[row][column])).value == 9)
            *p1_mines_uncovered = *p1_mines_uncovered - 1;
        //2.1.3. If the number of p1_mines_uncovered is 0, p1_name has won the game and we update status accordingly
        if (*p1_mines_uncovered == 0)
            *status = 3;
    }
	// 2.2. If status == 2
    else if (*status == 2) {
        //2.2.1. We uncover the position in the p1_board
        (*(&p1_board[row][column])).visible = True;
        //2.1.2. If the position was a mine, we decrease the number of mines to hit
        if ((*(&p1_board[row][column])).value == 9)
            *p2_mines_uncovered = *p2_mines_uncovered - 1;
        //2.2.3. If the number of p2_mines_uncovered is 0, p2_name has won the game and we update status accordingly
        if (*p2_mines_uncovered == 0)
            *status = 4;
    }
}

//------------------------------------
//	10. FUNCTION play_game
//------------------------------------
void play_game(char* p1, char* p2, int board_size, int percentage_mines) {
	//1. We create a variable pointer game* g and call to the function 
	//create_new_game so as to initialise it.
	game* g = create_new_game(p1, p2, board_size, percentage_mines);

	//2. We initialise the mines in p1_board and p2_board
	place_mines_on_board((*g).p1_board, (*g).board_size, (*g).num_mines);
	place_mines_on_board((*g).p2_board, (*g).board_size, (*g).num_mines);

	//3. Game loop: While the game is on (i.e., Game Status = 1 or Game Status = 2)
	while ((*g).status < 3) {

		//3.1. We display_board_content before the move
		if ((*g).status == 1) {
			printf("\nGame is on. Player %s moves next.\n\n", (*g).p1_name);
			display_board_content((*g).p2_board, (*g).board_size, (*g).p1_mines_uncovered);
		}
		else {
			printf("\nGame is on. Player %s moves next.\n\n", (*g).p2_name);
			display_board_content((*g).p1_board, (*g).board_size, (*g).p2_mines_uncovered);
		}

		//3.2. We get and process the next movement
		int mov = get_next_movement_index((*g).status, (*g).p1_name, (*g).p2_name, (*g).board_size, (*g).p1_board, (*g).p2_board);
		process_movement(mov, &(*g).status, (*g).board_size, (*g).p1_board, (*g).p2_board, &(*g).p1_mines_uncovered, &(*g).p2_mines_uncovered);

		//3.3. We display_board_content after the move

		if (((*g).status == 1) || ((*g).status == 3)) {
			printf("\nGame is on. Player %s had just moved.\n\n", (*g).p1_name);
			display_board_content((*g).p2_board, (*g).board_size, (*g).p1_mines_uncovered);
			printf("Press a key to continue\n");
			my_get_char();
		}
		else {
			printf("\nGame is on. Player %s had just moved.\n\n", (*g).p2_name);
			display_board_content((*g).p1_board, (*g).board_size, (*g).p2_mines_uncovered);
			printf("Press a key to continue\n");
			my_get_char();
		}

		//3.4. If status is < 3, we swap players
		if ((*g).status < 3) {
            if ((*g).status == 1)
                (*g).status = 2;
            else
                (*g).status = 1;
        }
	}

	//4. We display the end of the game
	if ((*g).status == 3) {
		printf("\n\n\n------------------------------\nPLAYER %s HAS WON!\n------------------------------\n", (*g).p1_name);
		display_board_content((*g).p2_board, (*g).board_size, (*g).p1_mines_uncovered);
	}
	else {
		printf("\n\n\n------------------------------\nPLAYER %s HAS WON!\n------------------------------\n", (*g).p2_name);
		display_board_content((*g).p1_board, (*g).board_size, (*g).p2_mines_uncovered);
	}
}


