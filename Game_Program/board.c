/* 
char board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
char mark;
int choice, player;

// 커서 이동 함수
void gotoxy(int x, int y) {
    COORD pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 전체 화면 설정
void FullScreen() {
    HWND hwnd = GetConsoleWindow();
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
    ShowWindow(hwnd, SW_MAXIMIZE);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, &coord);
}

// 화려한 보드판 출력
int Board() {
    system("cls");

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int start_x = (width - 40) / 2;
    int start_y = (height - 15) / 2;

    // 타이틀 배경 효과
    gotoxy(start_x - 4, start_y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); // 밝은 하늘색
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    gotoxy(start_x - 4, start_y + 1);
    printf("┃      CYBER TIC-TAC-TOE : HUMAN vs AI       ┃");
    gotoxy(start_x - 4, start_y + 2);
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

    // 플레이어 정보
    gotoxy(start_x + 2, start_y + 4);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // 노란색
    printf("Player (O)");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    printf(" vs ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); // 보라색
    printf("Computer (X)");

    // 보드판 그리기 (Double Line Box)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    int board_x = start_x + 8;
    int board_y = start_y + 6;

    for (int i = 0; i < 3; i++) {
        gotoxy(board_x, board_y + (i * 2));
        for (int j = 0; j < 3; j++) {
            // 마크 색상 구분
            if (board[2 - i][j] == 'O') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            else if (board[2 - i][j] == 'X') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); // 빈 칸은 회색

            printf("  %c  ", board[2 - i][j]);

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); // 구분선은 다시 하늘색
            if (j < 2) printf("┃");
        }
        if (i < 2) {
            gotoxy(board_x, board_y + (i * 2) + 1);
            printf("━━━━━╋━━━━━╋━━━━━");
        }
    }

    gotoxy(start_x, start_y + 13);
    return 0;
}

// 승리 조건 (기존과 동일)
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
    return (count == 0) ? 0 : -1;
}

// 스마트 AI (기존과 동일)
int SmartMove(char m) {
    for (int i = 1; i <= 9; i++) {
        int r = (i - 1) / 3, c = (i - 1) % 3;
        if (board[r][c] != 'O' && board[r][c] != 'X') {
            char backup = board[r][c]; board[r][c] = m;
            if (CheckVic() == 1) { board[r][c] = backup; return i; }
            board[r][c] = backup;
        }
    }
    return 0;
}

int main() {
    FullScreen();
    srand((unsigned int)time(NULL));
    player = 1;
    int status = -1;

    do {
        Board();
        player = (player % 2) ? 1 : 2;
        mark = (player == 1) ? 'O' : 'X';

        if (player == 1) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            printf("  ▶ Your Turn (1~9): ");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            if (scanf_s("%d", &choice) != 1) {
                while (getchar() != '\n');
                choice = -1;
            }
        }
        else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            printf("  ■ AI Thinking... ");
            Sleep(800);
            int sm = SmartMove('X');
            if (sm == 0) sm = SmartMove('O');
            if (sm == 0 && board[1][1] == '5') sm = 5;
            if (sm == 0) {
                do {
                    sm = (rand() % 9) + 1;
                } while (board[(sm - 1) / 3][(sm - 1) % 3] == 'O' || board[(sm - 1) / 3][(sm - 1) % 3] == 'X');
            }
            choice = sm;
        }

        int r = (choice - 1) / 3, c = (choice - 1) % 3;
        if (choice >= 1 && choice <= 9 && board[r][c] != 'O' && board[r][c] != 'X') {
            board[r][c] = mark;
            status = CheckVic();
            player++;
        }
    } while (status == -1);

    Board();
    gotoxy(((GetSystemMetrics(SM_CXSCREEN) / 10)), 25); // 결과창 위치 대략 조정
    if (status == 1) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (--player == 1) ? 14 : 13);
        printf("\n\n      %s WIN!!!", (player == 1) ? "PLAYER" : "COMPUTER");
    }
    else {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        printf("\n\n      DRAW GAME!");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    _getch();
    return 0;
} 
*/