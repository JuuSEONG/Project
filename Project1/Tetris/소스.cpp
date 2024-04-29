#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

// 공백 = 0,  테두리 = 1, 블럭 = 2, 쌓인 블럭 = 3, 블럭이 쌓이는 밑바닥 = 4

#define TABLE_X 20 //테트리스 X축
#define TABLE_Y 38 //테트리스 Y축
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

// 커서 숨기기 0, 보이기 1
void CursorView(char show) {
    HANDLE hConsole;;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

//콘솔 커서 위치 이동
void gotoxy(int x, int y) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//일자 블럭
const int block1[4][4][4] = {
        {
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {2, 2, 2, 2},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 2, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {2, 2, 2, 2},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 2, 0}
        },
};

//네모 블럭
const int block2[4][4][4] = {
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {0, 2, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {0, 2, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {0, 2, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {0, 2, 2, 0},
                        {0, 0, 0, 0}
        },
};

//갈고리 블럭
const int block3[4][4][4] = {
        {
                        {0, 2, 0, 0},
                        {0, 2, 0, 0},
                        {0, 2, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 2},
                        {0, 2, 0, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 2, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 0, 2, 0},
                        {2, 2, 2, 0},
                        {0, 0, 0, 0}
        },
};

//지그재그 블럭
const int block4[4][4][4] = {
        {
                        {0, 0, 0, 0},
                        {0, 2, 0, 0},
                        {0, 2, 2, 0},
                        {0, 0, 2, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {2, 2, 0, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 0, 0},
                        {0, 2, 2, 0},
                        {0, 0, 2, 0}
        },
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 0},
                        {2, 2, 0, 0},
                        {0, 0, 0, 0}
        },
};

//ㅗ 블럭
const int block5[4][4][4] = {
        {
                        {0, 0, 0, 0},
                        {0, 2, 2, 2},
                        {0, 0, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 2, 0},
                        {0, 2, 2, 0},
                        {0, 0, 2, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 2, 0},
                        {0, 2, 2, 2},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0}
        },
        {
                        {0, 0, 2, 0},
                        {0, 0, 2, 2},
                        {0, 0, 2, 0},
                        {0, 0, 0, 0}
        },
};