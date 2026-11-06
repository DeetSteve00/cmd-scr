#include <iostream>
#include <windows.h>
#include <conio.h>
#define WIDTH 132
#define HEIGHT 37

class Display {
    public:
    HANDLE hConsole;
    SMALL_RECT rectWindow;
    CHAR_INFO *srcDisplay;
    Display(const char* ConsoleTitle) {
        hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        srcDisplay = new CHAR_INFO[WIDTH*HEIGHT];
        SetConsoleActiveScreenBuffer(hConsole);
        rectWindow = {0, 0, WIDTH-1, HEIGHT-1};
        SetConsoleTitleA(ConsoleTitle);

        for(int i = 0; i < WIDTH * HEIGHT; i++) {
            srcDisplay[i].Char.AsciiChar = ' ';
            srcDisplay[i].Attributes = 0x0F;
        }
    }

    void RefreshScreen() {
        WriteConsoleOutputA(hConsole, srcDisplay, {WIDTH, HEIGHT}, {0, 0}, &rectWindow);
    }

};

Display game("Test screen :)");
int x = 0, y = 0,
    dx = -2, dy = -1;
int col = 0;
bool fall = false;


void gameLoop() {
    if(_kbhit()) {
        if(_getch()==' ') {fall = !fall;return;}
        x = 0; y = 0;
        dx = -2; dy = -1;
        col = 0;
        for(int i = 0; i < WIDTH * HEIGHT; i++) {
            game.srcDisplay[i].Char.AsciiChar = ' ';
            game.srcDisplay[i].Attributes = 0x0F;
        }
        return;
    }
    
    if(fall) for(int i = WIDTH*(HEIGHT-1); i >= 0; i--) {
        if(game.srcDisplay[i].Char.AsciiChar != '.') continue;
        if(game.srcDisplay[i+WIDTH].Char.AsciiChar == '.') continue;
        game.srcDisplay[i+WIDTH].Char.AsciiChar = '.';
        game.srcDisplay[i+WIDTH].Attributes = game.srcDisplay[i].Attributes;
        game.srcDisplay[i].Char.AsciiChar = ' ';
        game.srcDisplay[i].Attributes = 0x0F;
    }
    
    if(col == 0x00) col += 0x10;
    game.srcDisplay[y * WIDTH + x].Attributes = col;
    game.srcDisplay[y * WIDTH + x+1].Attributes = col + col/0x10;
    game.srcDisplay[y * WIDTH + x+1].Char.AsciiChar = '.';
    
    game.RefreshScreen();
    
    game.srcDisplay[y * WIDTH + x].Attributes = 0x0F;
    game.srcDisplay[y * WIDTH + x+1].Attributes = col/0x10;
    if(x == WIDTH-2 || x == 0) {dx *= -1; col = (col+0x10)%0xF0;}
    if(y == HEIGHT-1 || y == 0) {dy *= -1; col = (col+0x10)%0xF0;}
    x = (x+dx);
    y = (y+dy);
    Sleep(50);
}

int main(void) {
    while(true) gameLoop();
    return 0;
}   