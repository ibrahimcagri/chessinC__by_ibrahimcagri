/*A two-player chess game. The board will be drawn using Unicode symbols.
Each row and column will be indicated with its row and column label. The 
board should have a checkerboard pattern and each chess piece should be 
made out of unicode characters (e.g., white king: u2654). There will be 
a chess timer printed below the chess board and the player to make the 
next move will be indicated. In order to make a move, the player will 
enter the current row and column label of the piece and the target row 
and column label he/she wants to move that piece to. For example, A3A4 
will move the piece from position A3 to position A4. Game status will be
indicated below the board with a message. Invalid moves will be indicated
with "Invalid move!" message. Checks will be indicated with "Check!" message
and checkmate will be indicated with a "Checkmate!" message. The game state
will be saved by pressing the "S" button at any time. At the game entry,
a menu will be displayed with options to start a new game, to load a 
previously saved game or to exit the game. If the user selects to load a
previously saved game, the menu should show previously saved games with 
their date and time. For example "10052020_12:39pm". If the user selects 
one of the saved games, the game will start and continue from the point 
the user left off last time for that particular saved game. The user should
be able to exit to the main menu at any time by pressing M key where a 
confirmation message will be displayed and if the player confirms, the game
state will automatically be saved and the game will exit to the main menu.*/


/*
Tasks==>
+1:Create the board
+2:Create the pieces
+3:Assign the pieces to the board
?+4:Create checker functions for Check and Checkmate.
+5:Invalid move check function.
+6:Input function that takes a3a4 as an input
7:Game History Log; Game Saver function; Undo function for debugging
+8:Main Menu with "New Game" and "Saved Game" and "Exit Game"
9:Create a timer and a printout that tells who the next player is
+10:Implement chess rules --> Pawn promotion (+), Castling (+),  
*/



#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include <stdlib.h>


    char col_name[8] = {'a','b','c','d','e','f','g','h'}; // array for board column names

    int original_board[8][8] = { //positive for white, negative for black
    //1: pawn, 2: knight, 3:bishop, 4:queen, 5:king 
        -1,-2,-3,-4,-5,-3,-2,-1,
        -6,-6,-6,-6,-6,-6,-6,-6,
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
        6, 6, 6, 6, 6, 6 ,6, 6,
        1 ,2, 3, 4, 5, 3 ,2, 1 
    };    

    int board[8][8] = { //positive for white, negative for black
    //1: pawn, 2: knight, 3:bishop, 4:queen, 5:king 
        -1,-2,-3,-4,-5,-3,-2,-1,
        -6,-6,-6,-6,-6,-6,-6,-6,
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
        6, 6, 6, 6, 6, 6 ,6, 6,
        1 ,2, 3, 4, 5, 3 ,2, 1 
    };

    int game_isnt_finished = 1; //1 = means that the game is still continuing; 0 = means game has ended
    

    void menu(void); //open Menu
    void end_game(); // terminates game
    void load_new_game(); // resets the board and opens a new game
    void load_prev_game(); //updates the board and the game resumes from where it left off
    int player; //switches between white - black
    int did_bR1_move = 0; //check for black long castle.
    int did_bR2_move = 0; //check for black short castle.
    int did_wR1_move = 0; //check for white long castle.
    int did_wR2_move = 0; //check for white short castle.
    int did_wK_move = 0; //check for both short and long white castle.
    int did_bK_move = 0; //check for both short and long black castle. 
    int get_move_info(char*); //takes the 4 character long string and makes the move
    int rule_validity(int, int, int, int); //checks if the move is valid
    void piece_move(int, int, int, int); //from input moves the piece accordingly
    int wP_move(int, int, int, int); //white pawn move
    int bP_move(int, int, int, int); //black pawn
    int N_move(int, int, int, int); //knight move
    int R_move(int, int, int, int); //rook move
    int B_move(int, int, int, int); //bishop move
    int K_move(int, int, int, int); //king move
    int castling_validity(int, int, int, int); //check for castling situation
    int is_king_safe(); //check for checks and tell if the move isnt valid
    void castling(int, int, int, int); //castling if castle requirements are satisfied
    void pawn_promotion(int, int); //promotes pawn if reached other side
    void clrscr(); //clears screen



int main(void) {

    	printf("\n"
		" Hi, Enjoy our game, This Chess Game was written by IBRAHIM C. KUTLU and DORUK ERGUN.\n\n"
		" Press '1' to start a New Game - \n"
		" Press '2' Load one of your previous games - \n"
		" Press '0' Exit the Game - \n"
        " Your Input :  ");

        int key;

        scanf("\n%d", &key);
        if (key==0){
            end_game();
            return 0;
        }

        switch(key){
            case 1:
            printf("\nA new game has been initalized!!!\n\n");
            load_new_game();
            break;

            case 2:
            printf("\nA previously created game has been opened!!!\n\n");
            load_prev_game();
            break;

            default:
            //main();
            break;
        }

    player = 1;
    char move[4];

    while (game_isnt_finished)
    {   //printf("\e[1;1H\e[2J");   clear screen for linux (not recommended)
        //clrscr();       clear screen for windows (not recommended)

        printf("   ");
        for (int i=0; i<8;i++)
            printf("  %c ",col_name[i]);
        printf("\n\n");
        for (int row = 0,p=8;row<8;row++,p--)
        {
            printf(" %d  ",p);
            for (int col = 0;col<8;col++)
            { 
                switch (board[row][col]) //writes the board to the output console
                {
                    case 0:
                        printf(" .  ");
                        break;
                    case -1:
                        printf(" bR ");
                        break;
                    case -2:
                        printf(" bN ");
                        break;
                    case -3:
                        printf(" bB ");
                        break;
                    case -4:
                        printf(" bQ ");
                        break;
                    case -5:
                        printf(" bK ");
                        break;
                    case -6:
                        printf(" bP ");
                        break;
                    case 1:
                        printf(" wR ");
                        break;
                    case 2:
                        printf(" wN ");
                        break;
                    case 3:
                        printf(" wB ");
                        break;
                    case 4:
                        printf(" wQ ");
                        break;
                    case 5:
                        printf(" wK ");
                        break;
                    case 6:
                        printf(" wP ");
                        break;
                    default:
                        break;
                }
            }
            printf("\n");
        }


        if (player)
        {
            printf("\n Write 'menu' to save your game and exit to Main Menu \n"
                    " White's turn\n"
                    " Enter your move - ");
            scanf("%s",move);
            if(move == "menu")
            {
                menu();
            }

            if (get_move_info(move))
            {
                if (player == -1) {
                    player = 1;
                }
                else {
                    player = 0;
                }
            }
            else 
            {
                printf(" INVALID MOVE BY WHITE, please enter a valid move !!! \n\n");
            }
        }
        else
        {
        printf("\n Write 'menu' to save your game and exit to Main Menu \n"
                " Black's turn\n"
                " Enter your move - ");
        scanf("%s",move);
        }

            if (get_move_info(move))
            {
                if (player == -1){
                    player = 0;
                }
                else{
                    player = 1;
                }
            }
            else {
                printf(" INVALID MOVE BY BLACK, please enter a valid move !!! \n\n");
            }

    }
}


int get_move_info(char *point)
{
    char move[4];
    int len,start[2],end[2];
    for(len = 0; *point ;point++ , len++ )
        move[len] = *point;
    if ((len == 4) && isalpha(move[0]) && isalpha(move[2]) && isdigit(move[1]) && isdigit(move[3]) )
    {
        switch(move [0])
        {
            case 'a':
                start[1]=0;
                break;
            case 'b':
                start[1]=1;
                break;
            case 'c':
                start[1]=2;
                break;
            case 'd':
                start[1]=3;
                break;
            case 'e':
                start[1]=4;
                break;
            case 'f':
                start[1]=5;
                break;
            case 'g':
                start[1]=6;
                break;
            case 'h':
                start[1]=7;
                break;
            default:
                return 0;
        }
        start[0] = ('9' - move[1]) - 1;
        end[0]= ('9' - move[3]) - 1;
        if (start[0]>8 || end[0]>8)
            return 0;
        switch(move [2])
        {
            case 'a':
                end[1]=0;
                break;
            case 'b':
                end[1]=1;
                break;
            case 'c':
                end[1]=2;
                break;
            case 'd':
                end[1]=3;
                break;
            case 'e':
                end[1]=4;
                break;
            case 'f':
                end[1]=5;
                break;
            case 'g':
                end[1]=6;
                break;
            case 'h':
                end[1]=7;
                break;
            default:
                return 0;
        }
        printf("\n\n Last move: Row %d, Col %d to Row %d, Col %d (", 8-start[0], start[1]+1, 8-end[0], end[1]+1);
        printf(" %c%d to %c%d ) \n\n", col_name[start[1]], 8-start[0], col_name[end[1]], 8-end[0]);
        if (rule_validity(start[0],start[1],end[0],end[1]))
        {
            piece_move(start[0],start[1],end[0],end[1]);
            return 1;
        }

    }
    return 0;
}
int rule_validity(int r1,int c1,int r2,int c2) //checks for the wanted move's validity for example pawn promotion.
{
    int pawn = board[r1][c1];
    if ((player && pawn>0) || ((player == 0 )&& pawn<0))
    {
        switch (pawn)
        {
            case 6:
                if (wP_move(r1,c1,r2,c2))
                {
                    if (r2 == 0)
                        pawn_promotion(r1,c1);
                    return 1;
                }
                return 0;
                break;
            case -6:
                if (bP_move(r1,c1,r2,c2))
                {
                    if (r2 == 7)
                        pawn_promotion(r1,c1);
                    return 1;
                }
                return 0;
                break;
            case 5:
            case -5:
                if (K_move(r1,c1,r2,c2))
                {
                    if (player)
                        did_wK_move = 1;
                    else
                        did_bK_move = 1;
                    return 1;
                }
                return 0;
                break;
            case 3:
            case -3:
                if (B_move(r1,c1,r2,c2))
                {
                    return 1;
                }
                return 0;
                break;
            case 2:
            case -2:
                if (N_move(r1,c1,r2,c2))
                {
                    return 1;
                }
                return 0;
                break;
            case 1:
            case -1:
                if (R_move(r1,c1,r2,c2))
                {
                    if (r1 == 0 && c1 == 0)
                        did_bR1_move = 1;
                    if (r1 == 0 && c1 == 7)
                        did_bR2_move = 1;
                    if (r1 == 7 && c1 == 0)
                        did_wR1_move = 1;
                    if (r1 == 7 && c1 == 7)
                        did_wR2_move = 1;
                    return 1;
                }
                return 0;
                break;
            case 4:
            case -4:
                if (R_move(r1,c1,r2,c2) || B_move(r1,c1,r2,c2))
                {
                                        return 1;
                }
                return 0;
                break;
        }
        return 1;
    }
        return 0;
}

void piece_move(int r1,int c1,int r2,int c2) //moves the piece, if a side's King is in check warns the player
{
    int temp;
    temp = board[r2][c2];
    board[r2][c2] = board[r1][c1];
    board[r1][c1] = 0;
    if (!is_king_safe())
    {
        board[r1][c1] = board[r2][c2];
        board[r2][c2] = temp;
        printf("\nInvalid move, %s's king in check !\n",player?"Black":"White");
        player = -1;
    }
}
int wP_move(int r1,int c1,int r2,int c2)
{
    int res;
    if (c1 == c2 )
    {
        if ((r1 == 6) && (board[r2][c2] == 0) && (board[r1 - 1][c2] == 0) &&  r2> 3 )
            res = 1;
        else if ((r2+1) == r1 && (board[r2][c2] == 0))
            res = 1;
        else
            res = 0;
    }
    else if ((board[r2][c2]<0) && (r2 == (r1 - 1)) && (c2 == (c1 - 1) ||  c2 == (c1+1)))
        res = 1;
    else
        res = 0;
    return res;

}
int bP_move(int r1,int c1,int r2,int c2)
{
    int res;
    if (c1 == c2 )
    {
        if ((r1 == 1) && (board[r2][c2] == 0) && (board[r1 + 1][c2] == 0) &&  r2<4 )
            res = 1;
        else if ((r2-1) == r1 && (board[r2][c2] == 0))
            res = 1;
        else
            res = 0;
    }
    else if ((board[r2][c2]>0) && (r2 == (r1 + 1)) && (c2 == (c1 - 1) ||  c2 == (c1+1)))
        res = 1;
    else
        res = 0;
    return res;

}
int N_move(int r1,int c1,int r2,int c2)
{
    int res;
    if (player)
    {
        if (board[r2][c2]>0)
            return 0;
    }
    else
    {
        if (board[r2][c2]<0)
            return 0;
    }
    if (((r2 == (r1+2)) || (r2 == (r1-2))) && ((c2 == (c1+1)) || (c2 == (c1-1))))
        res = 1;
    else if (((c2 == (c1+2)) || (c2 == (c1-2))) && ((r2 == (r1+1)) || (r2 == (r1-1))))
    {
        res = 1;
    }
    else
        res = 0;
    return res;
}
int R_move(int r1,int c1,int r2,int c2)
{
    int res = 1;
    if (player)
    {
        if (board[r2][c2]>0)
            return 0;
    }
    else
    {
        if (board[r2][c2]<0)
            return 0;
    }
    if (r1 == r2)
    {
        if (c1>c2)
        {
            for(int i = (c1-1);i>c2;i--)
            {
                if (board[r2][i] == 0)
                    continue;
                res = 0;
                break;
            }
        }
        else if (c2>c1)
        {
            for(int i = (c1+1);i<c2;i++)
            {
                if (board[r2][i] == 0)
                    continue;
                res = 0;
                break;
            }
        }
        else
            return 0;
    }
    else if(c1 == c2)
    {
       if (r1>r2)
        {
            for(int i = (r1-1);i>r2;i--)
            {
                if (board[i][c2] == 0)
                    continue;
                res = 0;
                break;
            }
        }
        else if (r2>r1)
        {
            for(int i = (r1+1);i<r2;i++)
            {
                if (board[i][c2] == 0)
                    continue;
                res = 0;
                break;
            }
        }
        else
            return 0;
    }
    else
        return 0;
    return res;
}
int B_move(int r1,int c1,int r2,int c2)
{
    int res = 1;
    if (player)
    {
        if (board[r2][c2]>0)
            return 0;
    }
    else
    {
        if (board[r2][c2]<0)
            return 0;
    }
    if (r2>r1 && c2>c1)
    {
            if ((r2 - r1) == (c2 -c1))
            {
                for(int i = r1+1,j = c1+1;i<r2;i++,j++)
                {
                    if (board[i][j] == 0)
                        continue;
                    res = 0;
                    break;
                }
            }
        else
            res = 0;
    }
    else if(r2>r1 && c2<c1)
    {
        if ((r2 - r1) == (c1 - c2))
        {
            for(int i = r1+1,j = c1-1;i<r2;i++,j--)
            {
                if (board[i][j] == 0)
                    continue;
                res = 0;
                break;
            }
        }
        else
            res = 0;
    }
    else if(r1>r2 && c2>c1)
    {
        if ((r1 - r2) == (c2 - c1))
        {
            for(int i = r1-1,j = c1+1;j<c2;i--,j++)
            {
                if (board[i][j] == 0)
                    continue;
                res = 0;
                break;
            }
        }
        else
            res = 0;
    }
    else if(r2<r1 && c2<c1)
    {
        if ((r1 - r2) == (c1 - c2))
        {
            for(int i = r1-1,j = c1-1;i<r2;i--,j--)
            {
                if (board[i][j] == 0)
                    continue;
                res = 0;
                break;
            }
        }
        else
            res = 0;
    }
    else
        res = 0;
    return res;
}
int K_move(int r1,int c1,int r2,int c2)
{
    int res = 0;
    if (r2 == r1 && !(c2 == c1 + 1 || c2 == c1 - 1))
        if (castling_validity(r1,c1,r2,c2))
        {
            castling(r1,c1,r2,c2);
            return 1;
        }

    if (player)
    {
        if (board[r2][c2]>0)
            return 0;
    }
    else
    {
        if (board[r2][c2]<0)
            return 0;
    }
    if ((r2 == r1 + 1 || r2 == r1 -1) &&  (c2 == c1 + 1 || c2 == c1 -1))
        res =  1;
    else if ((r2 == r1) && (c2 == c1 + 1 || c2 == c1 - 1) )
        res = 1;
    else if ((c2 == c1) && (r2 == r1 + 1 || r2 == r1 - 1) )
        res = 1;
    else
        res = 0;
    return res;
}
int castling_validity(int r1,int c1,int r2,int c2) //castling
{
    int res = 0;
    if (r1 == 0 && c1 == 4)
    {
        if (did_bK_move)
            return 0;
        if (c2 == 6 && !did_bR2_move)
        {
            for (int i = c1+1;i<c2;i++)
            {
                if (board[r1][i] == 0)
                    res = 1;
                    continue;
                return 0;
            }
        }
        else if (c2 == 2 && !did_bR1_move)
        {
            for (int i = c1-1;i>c2;i--)
            {
                if (board[r1][i] == 0)
                    res = 1;
                    continue;
                return 0;
            }
        }
    }
    else if (r1 == 7 && c1 == 4)
    {
        if (did_wK_move)
            return 0;
        if (c2 == 6 && !did_wR2_move)
        {
            for (int i = c1+1;i<c2;i++)
            {
                if (board[r1][i] == 0)
                    res = 1;
                    continue;
                return 0;
            }
        }
        else if (c2 == 2 && !did_wR1_move)
        {
            for (int i = c1-1;i>c2;i--)
            {
                if (board[r1][i] == 0)
                    res = 1;
                    continue;
                return 0;
            }
        }
    }
    return res;
}
void castling(int r1,int c1,int r2,int c2)
{
    if (r1 == 0)
    {
        if (c2 == 6)
        {
            board[0][7] = 0;
            board[0][5] = -1;
        }
        if (c2 == 2)
        {
            board[0][0] = 0;
            board[0][3] = -1;
        }
    }
    else
    {
        if (c2 == 6)
        {
            board[7][7] = 0;
            board[7][5] = 1;
        }
        if (c2 == 2)
        {
            board[7][0] = 0;
            board[7][3] = 1;
        }
    }
}
void pawn_promotion(int r1, int c1)
{
    char ch;
    printf("Which pawn do you want to promote?\n ");
    if (player)
    {
        printf("Enter Q for Queen\n\tH for knight\n\tR for Rook\n\tB for Bishop");
        fflush(stdin);
        scanf(" %c",&ch);
        switch (ch)
        {
            case 'Q':
                   board[r1][c1] = 4;
                   break;
            case 'H':
                board[r1][c1] = 2;
                break;
            case 'R':
                board[r1][c1] = 1;
                break;
            case 'B':
                board[r1][c1] = 3;
                break;
            default:
                printf("Invalid input!\nPlease enter a valid input\n");
                pawn_promotion(r1,c1);
                break;
        }
    }
    else
    {
        printf("Enter q for Queen\n\th for knight\n\tr for Rook\n\tb for Bishop");
        fflush(stdin);
        scanf(" %c",&ch);
        switch (ch)
        {
            case 'q':
                board[r1][c1] = -4;
                break;
            case 'h':
                board[r1][c1] = -2;
                break;
            case 'r':
                board[r1][c1] = -1;
                break;
            case 'b':
                board[r1][c1] = -3;
                break;
            default:
                printf("Invalid input!\nPlease enter a valid input\n");
                pawn_promotion(r1,c1);
                break;
        }
    }
}
int is_king_safe()
{
    int r,c;
    for(int i = 0;i<8;i++)
    {
        for(int j = 0;j<8;j++)
        {
            if ((board[i][j] == 5 && player) || (board[i][j] == -5 & !player) )
            {
                r = i;
                c = j;
                break;
            }
        }
    }
    if (player)
    {
        player = 0;
        for(int i = 0;i<8;i++)
            {
                for(int j = 0;j<8;j++)
                {
                    if (board[i][j]>=0)
                        continue;
                    else
                    {
                        if (rule_validity(i,j,r,c))
                        {
                            return 0;
                        }
                    }
                }
            }
        player = 1;
    }
    else
        {
            player = 1;
        for(int i = 0;i<8;i++)
            {
                for(int j = 0;j<8;j++)
                {
                    if (board[i][j]<=0)
                        continue;
                    else
                    {
                        if (rule_validity(i,j,r,c))
                            return 0;
                    }
                }
            }
            player = 0;
        }
        return 1;
        printf("Check!!!");
}














void menu(void){

    FILE *fPtr = NULL;    
    fopen("chess_games.txt", "wb");
    fwrite(board, sizeof(int), sizeof(board), fPtr);
    fclose(fPtr);

/*  Original Version
     int written = 0;
     FILE *f = fopen("chessgames.txt", "wb");
     written = fwrite(board, sizeof(int), sizeof(board), f);
     if (written == 0) {
     printf("Error during saving/writing the game to file !!");}
     fclose(f); */


    char choice;
	printf("Press 'e' to end and save your current game\n"
           "Press 'n' to save your game and start a new game"
           "Press 'p' to save your game and load a previous game");

	scanf("%c", &choice);

	switch (choice)
	{
    case 'e': //Quit Game
		end_game();
        break;
	case 'n': //Start New Game
		load_new_game();
        break;
	case 'p': //Open Prev. Game
		load_prev_game();
        break;
	default:
		break;
	}
}

void load_new_game(){
    board[8][8] = original_board[8][8];
    printf("Current Game: (new) Game 1 \n\n");
}

void end_game(void){
    printf("Game terminated!!!\n"
           "See you later!");
    game_isnt_finished = 0; //game is finished!
}

void load_prev_game(void){ //change the board accordingly, also see prev games.
    //read from file
    printf("Current Game: (continued) _Name_Of_prevGame \n");
}

void clrscr(){
    system("cls||clear");
}