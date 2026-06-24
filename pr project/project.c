#include <stdio.h>

int oboard[19][19];

/* 모드의 모든 칸이 0이 아님을 판단하여 무승부 판단하는 함수 - 정환욱*/
int draw(int board[19][19])
{
    int dr = 1;
    int flag = 0;
    for (int i = 0; i < 19; i++){
        for (int j = 0; j < 19; j++){
            if (board[i][j] == 0){
                dr = 0;
                flag = 1;
                break;
            }
        }
        if (flag == 1) break;
    }
    return dr;
}

/*육목이 아닌 오목인지 판단하는 함수 - 한강현*/
int check_win(int board[19][19], int y, int x, int player) 
{
    int dy[4] = {1, 1, 0, -1};
    int dx[4] = {0, 1, 1, 1};
    for (int t = 0; t < 4; t++)
    {
        int cnt = 1;

        int ry = y, rx = x;
        while (1)
        {
            ry += dy[t];
            rx += dx[t];
            if (ry >= 0 && ry < 19 && rx >= 0 && rx < 19 && board[ry][rx] == player)
                cnt++;
            else
                break;
        }

        int ly = y, lx = x;
        while (1)
        {
            ly -= dy[t];
            lx -= dx[t];
            if (ly >= 0 && ly < 19 && lx >= 0 && lx < 19 && board[ly][lx] == player)
                cnt++;
            else
                break;
        }

        if (cnt == 5) {
            int front = (ry >= 0 && ry < 19 && rx >= 0 && rx < 19 && board[ry][rx] == player);
            int back = (ly >= 0 && ly < 19 && lx >= 0 && lx < 19 && board[ly][lx] == player);
            if (!front && !back) {
                return player;
            }
        }
    }
    return 0;
}

/*삼삼인지 판단하는 함수 - 한강현*/
int double_three(int board[19][19], int y, int x)
{
    int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};

    int open_three_count = 0;

    board[y][x] = 1;

    for (int t = 0; t < 4; t++)
    {
        int cnt = 0;

        for (int j = 1; j < 5; j++)
        {
            int ty = y + dy[t] * j;
            int tx = x + dx[t] * j;

            if (ty < 0 || ty >= 19 || tx < 0 || tx >= 19)
                break;

            if (board[ty][tx] == 1)
                cnt++;

            if (board[ty][tx] == 2)
                break;

            if (cnt == 2)
            {
                int ey = y + dy[t] * (j + 1);
                int ex = x + dx[t] * (j + 1);

                int by = y - dy[t];
                int bx = x - dx[t];

                if (ey >= 0 && ey < 19 && ex >= 0 && ex < 19 &&
                    by >= 0 && by < 19 && bx >= 0 && bx < 19 &&
                    board[ey][ex] == 0 &&
                    board[by][bx] == 0)
                {
                    open_three_count++;
                }

                break;
            }
        }
    }

    board[y][x] = 0;

    if (open_three_count >= 2)
        return 1;

    return 0;
}

/*double_three함수를 호출해 삼삼인지, 범위를 벗어나지 않았는지 이미 둔 수가 어닌지 판단하는 함수 - 박지훈*/
int is_valid(int board[19][19], int y, int x, int player)
{
    if (x < 0 || x >= 19 || y < 0 || y >= 19)
        return 4;

    if (board[y][x] != 0)
        return 2;

    if (player == 1 && double_three(board, y, x))
        return 3;

    return 1;
}

/*바둑판의 현재 상태를 출력하는 함수 - 최시윤*/
void print_board(int board[19][19])
{
    printf("   ");

    for (int i = 0; i < 19; i++)
    {
        if (i == 10)
            printf(" ");
        if (i < 10)
            printf(" %d ", i);
        else
            printf("%d ", i);
    }

    printf("\n");

    for (int i = 0; i < 19; i++)
    {
        if (i < 10)
            printf("%d   ", i);
        else
            printf("%d  ", i);

        for (int j = 0; j < 19; j++)
        {
            if (board[i][j] == 0)
                printf(".  ");
            else if (board[i][j] == 1)
                printf("●  ");
            else if (board[i][j] == 2)
                printf("○  ");
        }
        printf("\n");
    }
}

/*
무승부인지 판단하고, 무승부가 아닐 경우 좌표를 입력받아 유효한 수인지 확인한다.
유효하지 않은 수(이미 둔 수, 삼삼, 범위 초과)일 경우 그 이유를 출력한다.
유효한 수이면 돌을 놓고 보드를 출력한다.
수를 놓은 뒤 승패를 판단하고, 승리한 경우 player? win을 출력한다.
승패가 결정되지 않았을 경우 다음 플레이어에게 차례를 넘긴다.
게임의 전체 진행을 담당하는 함수 - 정환욱
*/
void gameplay(int board[19][19], int player)
{
    if (draw(board) == 1) {
        printf("무승부입니다.\n");
        return;
    }

    int x, y;

    printf("player%d 입력(y x): ", player);
    scanf("%d %d", &y, &x);

    int valid = is_valid(board, y, x, player);

    if (valid == 1)
    {
        board[y][x] = player;

        int result = check_win(board, y, x, player);

        print_board(board);

        if (result == 1)
        {
            printf("player1 win\n");
            return;
        }
        else if (result == 2)
        {
            printf("player2 win\n");
            return;
        }

        if (player == 1)
            gameplay(board, 2);
        else
            gameplay(board, 1);
    }
    else if (valid == 2)
    {
        printf("이미 둔 수입니다.\n");
        gameplay(board, player);
    }
    else if (valid == 3)
    {
        printf("삼삼입니다.\n");
        gameplay(board, player);
    }
    else if (valid == 4)
    {
        printf("범위초과입니다.\n");
        gameplay(board, player);
    }
}
/*print_board 함수와 gameplay함수를 호출한다*/
int main()
{
    print_board(oboard);
    gameplay(oboard, 1);
    return 0;
}