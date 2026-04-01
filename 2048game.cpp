#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

vector<vector<int>> board(4, vector<int>(4, 0));
vector<char> cmd = {'w', 'a', 's', 'd', 'q', 'W', 'A', 'S', 'D', 'Q'};
int act=0, points=0, overgoal=0, playing=1;

//輸出遊戲盤面
void print() {
    for (int i=0; i<4; i++) {
        cout << "|";
        for (int j=0; j<4; j++) {
            board[i][j] ? (cout << board[i][j]) : (cout << " ");
            cout << "\t|";
        }
        cout << "\n";
    }
}

//隨機產生數字2或4（依照自己之前的遊戲經驗，出現2的機率比4大，因此目前假設出現2和4的的機率比為4:1）
void newBlock() {
    vector<int> init = {2,2,2,2,4};
    vector<pair<int,int>> empty_coord;
    //檢查是否有空白格子
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (board[i][j] == 0) empty_coord.push_back({i,j});
        }
    }
    if (empty_coord.empty()) return;
    
    int pos_index = rand() % empty_coord.size();
    int x = empty_coord[pos_index].first;
    int y = empty_coord[pos_index].second;
    board[x][y] = init[rand() % 5];
}

//檢查是否還能移動，用於偵測遊戲進度
bool movable() {
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            if (board[i][j] == 0) return true;
            if (j<3 && board[i][j]==board[i][j+1]) return true;
            if (i<3 && board[i][j]==board[i+1][j]) return true;
        }
    }
    return false;
}

//依照指令移動
bool move(char c) {
    vector<vector<int>> last = board;   //用於檢查移動方向是否有效

    //向上
    if (c == 'w') {
        for (int j=0; j<4; j++) {
            vector<int> temp(4, 0); //將原本該排數字抽出獨立處理
            int k=0, combined=0;
            for (int i=0; i<4; i++) {
                if (board[i][j] != 0) { //取出非0數字
                    if (k>0 && !combined && temp[k-1]==board[i][j]) {   //檢查是否達合併條件
                        temp[k-1] *= 2; //合併
                        points += temp[k-1];    
                        combined = 1;   //標記已經合併，以免下一個數字相同而重複合併
                    } else {
                        temp[k++] = board[i][j];
                        combined = 0;
                    }
                }
            }
            for (int i=0; i<4; i++) board[i][j] = temp[i];  //寫回borad
        }
    }

    //向左
    if (c == 'a') {
        for (int i=0; i<4; i++) {
            vector<int> temp(4, 0);
            int k=0, combined=0;
            for (int j=0; j<4; j++) {
                if (board[i][j] != 0) {
                    if (k>0 && !combined && temp[k-1]==board[i][j]) {
                        temp[k-1] *= 2;
                        points += temp[k-1];
                        combined = 1;
                    } else {
                        temp[k++] = board[i][j];
                        combined = 0;
                    }
                }
            }
            for (int j=0; j<4; j++) board[i][j] = temp[j];
        }
    }

    //向下
    if (c == 's') {
        for (int j=0; j<4; j++) {
            vector<int> temp(4, 0);
            int k=0, combined=0;
            for (int i=3; i>=0; i--) {
                if (board[i][j] != 0) {
                    if (k>0 && !combined && temp[k-1]==board[i][j]) {
                        temp[k-1] *= 2;
                        points += temp[k-1];
                        combined = 1;
                    } else {
                        temp[k++] = board[i][j];
                        combined = 0;
                    }
                }
            }
            for (int i=0; i<4; i++) board[3-i][j] = temp[i];
        }
    }

    //向右
    if (c == 'd') {
        for (int i=0; i<4; i++) {
            vector<int> temp(4, 0);
            int k=0, combined=0;
            for (int j=3; j>=0; j--) {
                if (board[i][j] != 0) {
                    if (k>0 && !combined && temp[k-1]==board[i][j]) {
                        temp[k-1] *= 2;
                        points += temp[k-1];
                        combined = 1;
                    } else {
                        temp[k++] = board[i][j];
                        combined = 0;
                    }
                }
            }
            for (int j=0; j<4; j++) board[i][3-j] = temp[j];
        }
    }
    
    return (last != board);
}

//檢查是否已經合併出2048
bool achieve() {
    for (const auto & row : board) {
        for (int block : row)
            if (block == 2048) return true;
    }
    return false;
}

int main() {
    //初始化遊戲盤面
    srand(time(0));
    newBlock();
    newBlock();
    
    while (playing) {
        if (!movable()) break;

        //在達標時詢問玩家是否繼續
        if (!overgoal && achieve()) {
            system("cls");
            cout << "You Win!\nYou got " << points << " points in " << act << " moves.\n";
            char confirm;
            cout << "Keep Going? (Press y/n): ";
            confirm = _getch();
            if (tolower(confirm) == 'n') break;
            overgoal = 1;
        }
        
        //每一次的移動提示訊息
        system("cls");
        cout << "Moves: " << act << "\tPoints: " << points << "\n";
        print();
        cout << "Tap WASD to move or tap Q to quit: ";

        //偵測移動方向
        int input = _getch();
        char c = ' ';

        if (input == 224) {
            input = _getch();
            if (input == 72) c = 'w';
            if (input == 75) c = 'a';
            if (input == 80) c = 's';
            if (input == 77) c = 'd'; 
        } else c = (tolower((char)input));

        if (c == 'q') break;
        if (move(c)) {
            act++;
            newBlock();
        }

    }
    system("cls");
    cout << "GAME OVER!\nYou got " << points << " points in " << act << " moves.\n";
    print();
    system("pause");
}
