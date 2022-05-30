struct Move
{
    int row, col;
};
 
int evaluate(char b[3][3]);
int minimax(char board[3][3], int depth, int isMax);
struct Move findBestMove(char board[3][3]);