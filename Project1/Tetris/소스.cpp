#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

// https://seokjin2.tistory.com/35 <= 참고 블로그
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

//블럭 부모 클래스 (??)
class Block {
protected:
    int shape[4][4][4]; //shape[rotate][y][x] 왜 y 먼저?
    int x;
    int y;
    int rotationCount;  //shape[0][y][x], shape[1][y][x] ...
public:
    int getShape(int rotationCount, int y, int x) {
        return shape[rotationCount][y][x];
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    int getRotationCount() {
        return rotationCount;
    }
    void down() {   //블럭 한 칸 아래 이동
        y++; 
    }
    void left() {   //블럭 한 칸 왼쪽 이동   
        x--;
    }
    void right() {  //블럭 한 칸 오른쪽 이동
        x++;
    }
    void rotate() { //블럭 회전
        rotationCount = (rotationCount + 1) % 4; // 나머지 0, 1, 2, 3,으로 회전 표현
    }
    void setX(int x) { //여기서부턴 왜 하는지 아예 모르겠다.
        this->x = x;
    }
    void setY(int y) {
        this->y = y;
    }
    void setRotationCount(int r) {
        this->rotationCount = r;
    }
    void setShape(int r, int y, int x, int value) {
        this->shape[r][y][x] = value;
    }
};

//1번 블럭 클래스
class Block1 : public Block {
public:
    Block1() {
        x = TABLE_X / 2 - 3; //초기 생성 맨 위 중앙
        y = 1;
        rotationCount = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    shape[i][j][k] = block1[i][j][k]; //블럭 객체 정보 저장
                }
            }
        }
    }
};

//2번 블럭 클래스
class Block2 : public Block {
public:
    Block2() {
        x = TABLE_X / 2 - 3; 
        y = 1;
        rotationCount = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    shape[i][j][k] = block2[i][j][k]; 
                }
            }
        }
    }
};

//3번 블럭 클래스
class Block3 : public Block {
public:
    Block3() {
        x = TABLE_X / 2 - 3;
        y = 1;
        rotationCount = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    shape[i][j][k] = block3[i][j][k];
                }
            }
        }
    }
};

//4번 블럭 클래스
class Block4 : public Block {
public:
    Block4() {
        x = TABLE_X / 2 - 3;
        y = 1;
        rotationCount = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    shape[i][j][k] = block4[i][j][k];
                }
            }
        }
    }
};

//5번 블럭 클래스
class Block5 : public Block {
public:
    Block5() {
        x = TABLE_X / 2 - 3;
        y = 1;
        rotationCount = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    shape[i][j][k] = block5[i][j][k];
                }
            }
        }
    }
};

//메인 메뉴 클래스
class MainMenu {
public:
    MainMenu() {
        cout << "TETRIS";
        cout << "아무키나 누르면 시작";
        getchar(); //아무 키 입력 기다림
        system("cls"); //콘솔창 클리어
    }
};

//블럭, table 백업용 클래스 (??)
class backup {
public:
    //블럭 백업
    static void updateBlock(Block* source, Block& backupBlock) {
        backupBlock.setX(source->getX());
        backupBlock.setY(source->getY());
        backupBlock.setRotationCount(source->getRotationCount());
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    backupBlock.setShape(i, j, k, source->getShape(i, j, k));
                }
            }
        }
    }
    //테이블 백업
    static void updateTable(vector<vector<int> >& source, vector<vector<int>  >& backupTable) {
        backupTable.resize(source.size(), vector<int>(source.size()));
        copy(source.begin(), source.end(), backupTable.begin());
    }
};

//테트리스 게임 테이블 클래스
class GameTable {
private:
    int x;
    int y;
    Block* blockObject;
    vector<vector<int> > table; //블럭 정보 저장시 table[y][x]
public:
    GameTable(int x, int y) { //테트리스 판 뼈대 생성
        blockObject = nullptr;
        this->x = x;
        this->y = y;
        for (int i = 0; i < y; i++) {
            vector<int> temp;
            for (int j = 0; j < x; j++) {
                temp.push_back(0);
            }
            table.push_back(temp);
        }
        //가장 자리 뼈대 색칠
        for (int i = 0; i < x; i++) {
            table[0][i] = 1;
            table[y - 1][i] = 1;
        }
        for (int i = 1; i < y; i++) {
            table[i][0] = 1;
            table[i][x - 1] = 1;
        }
        //밑바닥 감지용
        for (int i = 1; i < x - 1; i++) {
            table[y - 1][i] = 4;
        }
    }
    //게임판 그리는 함수
    void DrawGameTable() {
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                if (table[i][j] == 1 || table[i][j] == 4) cout << "▦";
                else if (table[i][j] == 2 || table[i][j] == 3) cout << "■";
                else cout << "  "; // 두 칸 띄우기
            }
            cout << "\n";
        }
    }
};