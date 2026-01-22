#include <stdio.h>

char board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
char mark;
int choice;
int player;

int Board() {                                                                                                           // 보드판 UI
    system("cls");
    printf("\n\n  --- 틱택토 게임 ---  \n\n");
    printf("  Player 1 (O) -  Player 2 (X) \n\n\n");
    printf("       |     |     \n");
    printf("    %c  |  %c  |  %c \n", board[2][0], board[2][1], board[2][2]);   // 7  |  8  |  9
    printf("  _____|_____|_____\n");
    printf("       |     |     \n");
    printf("    %c  |  %c  |  %c \n", board[1][0], board[1][1], board[1][2]);     // 4  |  5  |  6
    printf("  _____|_____|_____\n");
    printf("       |     |     \n");
    printf("    %c  |  %c  |  %c \n", board[0][0], board[0][1], board[0][2]);  // 1  |  2  |  3
    printf("       |     |     \n\n");
}

int CheckVic() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return 1;                                                                                      // 가로, 세로 승리 조건
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return 1;
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) // ↗ 대각선 승리 조건
        return 1;                                                                                           
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) // ↘ 대각선 승리 조건
        return 1;

    int count = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'O' && board[i][j] != 'X') count++;  // 무승부 조건

    if (count == 0) return 0; 
    else return -1;                  // 남은 숫자 확인 및 진행
}

int main() {
    player = 1;
    int status;
    do {
        Board();
        player = (player % 2) ? 1 : 2;
        mark = (player == 1) ? 'O' : 'X';

        printf("  플레이어 %d, 위치 번호를 입력하세요: ", player);
        scanf_s("%d", &choice);

        int row = (choice - 1) / 3;
        int col = (choice - 1) % 3;

        if (choice >= 1 && choice <= 9 && board[row][col] != 'O' && board[row][col] != 'X') {
            board[row][col] = mark;
            status = CheckVic();
            player++;
        }
        else {
            printf("없는 위치입니다. 아무 키나 누르세요.");
            _getch();
        }
    } while (status == -1);

    Board();
    if (status == 1) printf("    Player %d 승리!\n", --player);
    else printf("    무승부입니다!\n");

    return 0;
}