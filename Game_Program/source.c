#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

char board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
char mark;
int choice, player;

// 돌의 위치를 기억하기 위한 구조체와 배열
typedef struct { int r, c; } Position;
Position player_history[2][3];
int stone_count[2] = { 0, 0 };
int mcx, mcy;

void gotoxy(int x, int y) {
    COORD pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 전체 화면으로 전환
void FullScreen() {
    HWND hwnd = GetConsoleWindow();
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
    ShowWindow(hwnd, SW_MAXIMIZE);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode;    // 마우스 입력 활성화
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_MOUSE_INPUT);
    COORD coord;
    SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, &coord);
}

// 보드판 출력
int Board() {
    system("cls");
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int start_x = (width - 45) / 2;
    int start_y = (height - 22) / 2;
    mcx = start_x + 6;
    mcy = start_y + 5;
    // 보드판 UI
    gotoxy(start_x, start_y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    printf("  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    gotoxy(start_x, start_y + 1);
    printf("  ┃            TIC-TAC-TOE           ┃");
    gotoxy(start_x, start_y + 2);
    printf("  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

    for (int i = 0; i < 3; i++) {
        int r = 2 - i;
        for (int line = 0; line < 3; line++) {
            gotoxy(mcx, mcy + (i * 4) + line);
            for (int j = 0; j < 3; j++) {
                int c = j;
                int oldest = 0;
                // 사라질 돌 체크 로직
                for (int p = 0; p < 2; p++) {
                    if (stone_count[p] == 3 && player_history[p][0].r == r && player_history[p][0].c == c) {
                        oldest = 1;
                    }
                }
                if (board[r][c] == 'O')
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), oldest ? 224 : 14);
                else if (board[r][c] == 'X')
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), oldest ? 205 : 13);
                else
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                if (line == 1) printf("    %c   ", board[r][c]);
                else printf("        ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                if (j < 2) printf("┃");
            }
        }
        if (i < 2) {
            gotoxy(mcx, mcy + (i * 4) + 3);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
            printf("━━━━━━━━╋━━━━━━━━╋━━━━━━━━");
        }
    }
    return 0;
}

/* for (int i = 0; i < 3; i++) {
    gotoxy(board_x, board_y + (i * 2));
    for (int j = 0; j < 3; j++) {
        int r = 2 - i;
        int c = j;
        int is_oldest = 0;

        // 다음에 사라질 돌인지 확인 (플레이어와 컴퓨터 모두 확인)
        for (int p = 0; p < 2; p++) {
            if (stone_count[p] == 3 && player_history[p][0].r == r && player_history[p][0].c == c) {
                is_oldest = 1;
            }
        }
        if (board[r][c] == 'O') {
            // 사라질 돌이면 배경색을 넣어서 "깜빡"이는 느낌을 줌
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), is_oldest ? 224 : 14);
        }
        else if (board[r][c] == 'X') {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), is_oldest ? 205 : 13);
        }
        else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        }
        printf("  %c  ", board[r][c]);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        if (j < 2) printf("┃");
    }
    if (i < 2) {
        gotoxy(board_x, board_y + (i * 2) + 1);
        printf("━━━━━╋━━━━━╋━━━━━");
    }
}
gotoxy(start_x, start_y + 13);
return 0;
} */

// 마우스 클릭 또는 키보드 입력
int MouseClick() {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD ir;
    DWORD read;

    while (1) {
        ReadConsoleInput(hInput, &ir, 1, &read);
        if (ir.EventType == MOUSE_EVENT && ir.Event.MouseEvent.dwEventFlags == 0) {
            if (ir.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
                int mx = ir.Event.MouseEvent.dwMousePosition.X;
                int my = ir.Event.MouseEvent.dwMousePosition.Y;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int x_min = mcx + (j * 10);
                        int x_max = x_min + 8;
                        int y_min = mcy + (i * 4);
                        int y_max = y_min + 2;

                        if (mx >= x_min && mx <= x_max && my >= y_min && my <= y_max) {
                            int r = 2 - i;
                            return (r * 3 + j + 1);
                        }
                    }
                }
            }
        }
            if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
                char ch = ir.Event.KeyEvent.uChar.AsciiChar;
                if (ch >= '1' && ch <= '9') return ch - '0';
            }
        }
    }
// 게임 승리 조건
int CheckVic() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return 1;   // 가로 승리 조건
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return 1;   // 세로 승리 조건
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return 1;   // 대각선 승리 조건 
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return 1;

    return -1;
}
// 똑똑한 Computer
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

    int player_wins = 0;   // 플레이어 승리 횟수
    int computer_wins = 0; // 컴퓨터 승리 횟수
    int round = 1;         // 현재 라운드

    // 누군가 2번 이길 때까지 반복
    while (player_wins < 2 && computer_wins < 2) {
        // 매 라운드 시작 전 게임 상태 초기화
        player = 1;
        int status = -1;
        stone_count[0] = 0; stone_count[1] = 0;
        // 보드판 초기화
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = (i * 3 + j + 1) + '0';
            }
        }
        do {
            Board();
            // 상단에 스코어 표시
            gotoxy(mcx, mcy - 2);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            printf("〔ROUND %d〕 PLAYER %d : %d PC", round, player_wins, computer_wins);

            int idx = (player % 2) ? 0 : 1;
            mark = (idx == 0) ? 'O' : 'X';

            if (idx == 0) { // Player Turn
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                gotoxy(mcx - 6, mcy + 13);
                printf("  ▶ PLAYER Turn Click or Choose 1~9 : ");
                choice = MouseClick();
            }
            else { // Computer Turn
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                gotoxy(mcx - 6, mcy + 13);
                printf("  ■ Computer Thinking...          ");
                Sleep(800);
                int sm = SmartMove('X');    // 컴퓨터 공격
                if (sm == 0) sm = SmartMove('O');   // 컴퓨터 방어
                if (sm == 0 && board[1][1] == '5') sm = 5;  // 중앙이 비어있을 때 선점
                if (sm == 0) {  // 랜덤 선택
                    do { sm = (rand() % 9) + 1; } while (board[(sm - 1) / 3][(sm - 1) % 3] == 'O' || board[(sm - 1) / 3][(sm - 1) % 3] == 'X');
                }
                choice = sm;
            }
            
            int r = (choice - 1) / 3, c = (choice - 1) % 3;
            if (choice >= 1 && choice <= 9 && board[r][c] != 'O' && board[r][c] != 'X') {
                if (stone_count[idx] == 3) {
                    int old_r = player_history[idx][0].r;
                    int old_c = player_history[idx][0].c;
                    board[old_r][old_c] = (old_r * 3 + old_c + 1) + '0';
                    player_history[idx][0] = player_history[idx][1];
                    player_history[idx][1] = player_history[idx][2];
                    stone_count[idx]--;
                }
                board[r][c] = mark;
                player_history[idx][stone_count[idx]].r = r;
                player_history[idx][stone_count[idx]].c = c;
                stone_count[idx]++;
                status = CheckVic();
                player++;
            }
        } while (status == -1);

        Board();
        // 라운드 결과 판정
        if (status == 1) {
            if ((player - 1) % 2 != 0) {
                player_wins++;
                gotoxy(mcx + 1, mcy + 13);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                printf("★ PLAYER WON ROUND %d! ★", round);
            }
            else {
                computer_wins++;
                gotoxy(mcx + 1, mcy + 13);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                printf("★ COMPUTER WON ROUND %d! ★", round);
            }
        }
        round++;
        gotoxy(mcx - 1, mcy + 15);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        printf("Press any key for next step...");
        (void)_getch();
    }
    // 최종 승자 발표
    system("cls");
    int final_x = mcx;
    int final_y = mcy;
    gotoxy(final_x, final_y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    printf("===============================");
    gotoxy(final_x, final_y + 2);
    if (player_wins > computer_wins) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        printf("  CONGRATULATIONS! YOU WIN!!!");
    }
    else {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        printf("    GAME OVER! COMPUTER WIN!!!");
    }
    gotoxy(final_x, final_y + 4);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    printf("     FINAL SCORE [ %d : %d ]", player_wins, computer_wins);
    gotoxy(final_x, final_y + 6);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    printf("===============================");
    gotoxy(final_x, final_y + 8);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    printf("   Press any key to exit...");
    (void)_getch();

    return 0;
}

/* int main() {
    FullScreen();
    srand((unsigned int)time(NULL));

	int player_wins = 0;        // 플레이어 승리 횟수
	int computer_wins = 0;  // 컴퓨터 승리 횟수
	int round = 1;                    // 현재 라운드

	while (player_wins < 2 && computer_wins < 2) {
		player = 1;
		int status = -1;
		stone_count[0] = stone_count[1] = 0;

        for (int i = 0; i < 3, i++;) {
			for (int j = 0; j < 3; j++) {
                board[i][j] = (i * 3 + j + 1) + '0';
            }
        }
        // for (int i = 0; i < 3; i++)
        //     for (int j = 0; j < 3; j++)
        //         board[i][j] = (i * 3 + j + 1) + '0';
        // stone_count[0] = stone_count[1] = 0;
        // int result = PlayGame();
        // if (result == 1) {
        //     if ((player - 1) % 2 == 0) {
        //         player_wins++;
        //     }
        //     else {
        //         computer_wins++;
        //     }
        // }
        // 현재 스코어 출력
        // gotoxy(mcx - 6, mcy + 15);
        // SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        // printf(" Score - Player: %d | Computer: %d ", player_wins, computer_wins);
        // gotoxy(mcx - 6, mcy + 17);
        // printf(" Press any key to start next round... ");
        // (void)_getch();
    }

    player = 1;
    int status = -1;

    do {
        Board();
		gotoxy(mcx, mcy - 3);
		SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
		printf("〔     Round %d 〕    PLAYER %d : %d COMPUTER", round, player_wins, computer_wins);

        int idx = (player % 2) ? 0 : 1;
        mark = (idx == 0) ? 'O' : 'X';

        if (idx == 0) {        // Player 차례
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            gotoxy(mcx - 6, mcy + 13);
            printf("  ▶ 「Click Board」 or 「Choose 1~9」 : ");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            choice = MouseClick();

        }
        else {        // Computer 차례
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            gotoxy(mcx - 6, mcy + 13);
            printf("  ■ Computer Thinking... ");
            Sleep(800);
            int sm = SmartMove('X');    // 컴퓨터 공격
            if (sm == 0) sm = SmartMove('O');   // 컴퓨터 방어
            if (sm == 0 && board[1][1] == '5') sm = 5;  // 중앙이 비어있을 때 선점
            // 랜덤 선택
            if (sm == 0) {
                do {sm = (rand() % 9) + 1;}
                while (board[(sm - 1) / 3][(sm - 1) % 3] == 'O' || board[(sm - 1) / 3][(sm - 1) % 3] == 'X');
            }
            choice = sm;
        }

        int r = (choice - 1) / 3, c = (choice - 1) % 3;
        if (choice >= 1 && choice <= 9 && board[r][c] != 'O' && board[r][c] != 'X') {
            // 4번째 뒀을 때 사라지는 메커니즘 추가
            if (stone_count[idx] == 3) {
                // 가장 오래된 수의 위치를 찾아 숫자로 복구
                int old_r = player_history[idx][0].r;
                int old_c = player_history[idx][0].c;
                board[old_r][old_c] = (old_r * 3 + old_c + 1) + '0';

                // 히스토리 한 칸씩 당기기
                player_history[idx][0] = player_history[idx][1];
                player_history[idx][1] = player_history[idx][2];
                stone_count[idx]--;
            }

            // 새로운 수 놓기
            board[r][c] = mark;
            player_history[idx][stone_count[idx]].r = r;
            player_history[idx][stone_count[idx]].c = c;
            stone_count[idx]++;
            status = CheckVic();
            player++;
        }
    } while (status == -1);

    Board();    // 게임 종료 화면 및 결과 출력
    if (status == 1) {
		if ((player - 1) % 2 != 0) {
            player_wins;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            gotoxy(mcx +5 , mcy + 13);
            printf("      ★ PLAYER WIN  %d!  ★      ", round);
        }
        else{
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            gotoxy(mcx + 4, mcy + 13);
            printf("     ★ COMPUTER WIN  %d!  ★     ", round);
        }
    }

    round++;
	gotoxy(mcx + 3, mcy + 15);
    SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE),8);
	printf("     Press any key to continue...     ");
    (void)_getch();
	
	system("cls");
    int final_x = mcx;
	int final_y = mcy;
	gotoxy(final_x, final_y);
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    printf("========================================");
    gotoxy(final_x, final_y + 2);
    if (player_wins > computer_wins) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        printf("   CONGRATULATIONS! PLAYER IS THE CHAMPION!");
    }
    else {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        printf("   GAME OVER! COMPUTER IS THE CHAMPION!");
    }
    gotoxy(final_x, final_y + 4);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    printf("         FINAL SCORE [ %d : %d ]", player_wins, computer_wins);
    gotoxy(final_x, final_y + 6);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    printf("========================================");

    gotoxy(final_x, final_y + 8);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    printf("Press any key to exit...");
    _getch();

    return 0;
}
    /* CONSOLE_SCREEN_BUFFER_INFO csbi;    // 화면 중앙에 결과 출력
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int start_x = (width - 40) / 2;
    int start_y = (height - 15) / 2;

    // 승리자 출력
    gotoxy(start_x, start_y + 13);
    if (status == 1) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((player - 1) % 2 != 0) ? 14 : 13);
        printf("         ★ %s WIN ★", ((player - 1) % 2 != 0) ? "PLAYER" : "COMPUTER");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    gotoxy(start_x, start_y + 15);
    printf("     Press any key to exit...");
    (void)_getch();
    return 0;
} */