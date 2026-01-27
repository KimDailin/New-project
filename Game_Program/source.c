#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

char board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
char mark;
int choice;
int player;

// 보드판 UI 출력
int Board() {
    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    printf("\n\n  --- 틱택토 게임 《사람 VS 컴퓨터》 ---  \n\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 223);
    printf("  Player (O) - 컴퓨터 (X)  \n\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    for (int i = 2; i >= 0; i--) {
        printf("       |     |       \n");
        printf("    %c  |  %c  |  %c    \n", board[i][0], board[i][1], board[i][2]);
        printf("  _____|_____|_____  \n");
    }
    return 0;
}

// 승리 조건 체크
int CheckVic() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return 1;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return 1;
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return 1;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return 1;

    int count = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 'O' && board[i][j] != 'X') count++;

    if (count == 0) return 0; // 무승부
    else return -1; // 진행 중
}

int SmartMove(char m) {
    for (int i = 1; i <= 9; i++) {
        int r = (i - 1) / 3;
        int c = (i - 1) % 3;
        if (board[r][c] != 'O' && board[r][c] != 'X') {
            char backup = board[r][c];
            board[r][c] = m;
            if (CheckVic() == 1) {
                board[r][c] = backup;
                return i;
            }
            board[r][c] = backup;
        }
    }
    return 0;
}

int main() {
    srand((unsigned int)time(NULL));
    player = 1;
    int status = -1;

    do {
        Board();
        player = (player % 2) ? 1 : 2;
        mark = (player == 1) ? 'O' : 'X';
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

        if (player == 1) {
            printf("  Your Turn. (1~9) :  ");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            if (scanf_s("%d", &choice) != 1) { // 숫자 이외 입력 방지
                while (getchar() != '\n');
                choice = -1;
            }
        }
        else {
            printf("  Computer Thinking... \n");
            Sleep(550);
            // 똑똑한 컴퓨터
            int smartchoice = 0;
            // 컴퓨터 이길 곳이 있다면 둔다.
            smartchoice = SmartMove('X');

            // 상대방이 이길 곳이 있다면 막는다.
            if (smartchoice == 0) smartchoice = SmartMove('O');

            // 보드판에서 5번이 비어있다면 차지한다.
            if (smartchoice == 0 && board[1][1] == '5') smartchoice = 5;

            // 빈 공간 중 랜덤하게 선택한다.
            if (smartchoice == 0) {
                do {
                    smartchoice = (rand() % 9) + 1;
                } while (board[(smartchoice - 1) / 3][(smartchoice - 1) % 3] == 'O' || board[(smartchoice - 1) / 3][(smartchoice - 1) % 3] == 'X');
            }
            choice = smartchoice;
        }

        int row = (choice - 1) / 3;
        int col = (choice - 1) % 3;

        if (choice >= 1 && choice <= 9 && board[row][col] != 'O' && board[row][col] != 'X') {
            board[row][col] = mark;
            status = CheckVic();
            if (player == 2) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                printf("  컴퓨터가 %d번을 선택했습니다.", choice);
                Sleep(700);
            }
            player++;
        }
        else {
            if (player == 1) {
                printf("  [!] 잘못된 위치입니다. 다시 입력하세요.");
                Sleep(700);
            }
        }
    } while (status == -1);

    Board();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    if (status == 1) {
        if (--player == 1) printf("\n   ☆ 당신의 승리입니다! ☆\n");
        else printf("\n   ★ 컴퓨터가 승리했습니다! ★\n");
    }
    else printf("\n   무승부입니다!\n");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    return 0;
}