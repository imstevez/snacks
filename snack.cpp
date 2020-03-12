#include <iostream>
#include <unistd.h>
#include <string>
using namespace std;

#include <sys/select.h>
#include <termios.h>
#include <sys/ioctl.h>

bool gameOver;
const int width = 20;
const int height = 20;
int hx, hy, fx, fy, score;
enum Direction {STOP = 0, LEFT, RIGHT, UP, DOWN};
Direction dir, odir;
int tailX[100];
int tailY[100];
int nTail;

int _kbhit()
{
    static const int STDIN = 0;
    static bool initialized = false; if (! initialized)
    {
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    } int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;

}

void Setup() {
    gameOver = false;
    dir = STOP;
    odir = STOP;
    hx = width / 2;
    hy = height / 2;
    fx = rand() % width;
    fy = rand() % height;
}

void Draw() {
    system("clear");
    for (int i = -1; i <= height; i++) {
        for (int j = -1; j <= width; j++) {
            if (i == -1 || i == height || j == -1 || j == width) {
                cout << "*";
            } else if (i == hy && j == hx) {
                cout << "O";
            } else if (i == fy && j == fx) {
                cout << "F";
            } else {
                bool isTail = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        isTail = true;
                    }
                }
                if (isTail) {
                    cout << "o";
                } else {
                    cout << " ";
                }
            }
        }
        cout << endl;
    }
    cout << "SCORE: " << score << endl;
}

bool Input() {
    if(_kbhit()){
        char c = getchar();
        switch(c) {
            case 'w':
                dir = UP;
                break;
            case 'a':
                dir = LEFT;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'x':
                gameOver = true;
                break;
            default:
                dir = STOP;
                break;
        }
        return true;
    }
    return false;
}

void Logic() {
    if ((odir == LEFT && dir == RIGHT) || (odir == UP && dir == DOWN) || (odir == RIGHT && dir == LEFT) || (odir == DOWN && dir == UP) || dir == STOP) {
        dir = odir;
    } else {
        odir = dir;
    }
    int ohx = hx;
    int ohy = hy;
    switch(dir) {
        case UP:
            hy--;
            break;
        case LEFT:
            hx--;
            break;
        case DOWN:
            hy++;
            break;
        case RIGHT:
            hx++;
            break;
        default:
            break;
    }
    if(hx == fx && hy == fy) {
        score += 10;
        fx = rand() % width;
        fy = rand() % height;
        nTail++;
    }
    if(hx >= width || hy >= height || hx < 0 || hx < 0) {
        gameOver = true;
        hx = ohx;
        hy = ohy;
    } else {
        for (int i = nTail-1; i > 0; i--) {
            tailX[i] = tailX[i-1];
            tailY[i] = tailY[i-1];
        }
        tailX[0] = ohx;
        tailY[0] = ohy;
    }
    for(int i = 0; i < nTail; i++) {
        if (tailX[i] == hx && tailY[i] == hy) {
            gameOver = true;
            return;
        }
    }
}




int main() {
    Setup();
    int j = 1;
    while(!gameOver){
        int k = j % 200; 
        if (Input() || k == 1) {
            Logic();
            Draw();
        }
        j++;
        usleep(1000);
    }
    cout << "Game Over!" << endl;
    return 0;
}
