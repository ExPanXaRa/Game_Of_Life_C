#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGTH 25

void init_ncurses();
int speed_logic(char key, int speed);
void read_file(int field[HEIGTH][WIDTH]);
void draw_game(int field[HEIGTH][WIDTH]);
int logic(int field[HEIGTH][WIDTH]);
int check_logic(int field[HEIGTH][WIDTH], int i, int j);
int finish_game(char key);
int check_end(int field[HEIGTH][WIDTH]);

int main() {
    init_ncurses();
    int field[HEIGTH][WIDTH];
    read_file(field);
    if (freopen("/dev/tty", "r", stdin)) {
        int speed = 100000;
        int flag = 1;
        char ch = ' ';
        while (finish_game(ch) && flag) {
            ch = getch();
            speed = speed_logic(ch, speed);
            draw_game(field);
            flag = logic(field);
            usleep(speed);
        }
    }
    endwin();
    return 0;
}

void init_ncurses() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, 1);
    nodelay(stdscr, 1);
}

int speed_logic(char key, int speed) {
    int result_speed = speed;
    if (key == '+' && result_speed > 10000) {
        result_speed -= 10000;
    } else if (key == '-') {
        result_speed += 10000;
    }
    return result_speed;
}

int logic(int field[HEIGTH][WIDTH]) {
    for (int i = 0; i < HEIGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 0 && check_logic(field, i, j) == 3) {
                field[i][j] = 2;
            } else if (field[i][j] == 1 && (check_logic(field, i, j) < 2 || check_logic(field, i, j) > 3)) {
                field[i][j] = 3;
            }
        }
    }

    int flag = check_end(field);
    for (int i = 0; i < HEIGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 3) {
                field[i][j] = 0;
            } else if (field[i][j] == 2) {
                field[i][j] = 1;
            }
        }
    }
    return flag;
}

int check_logic(int field[HEIGTH][WIDTH], int i, int j) {
    int check = 0;
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            if (x == 0 && y == 0) continue;
            int ibuff = (i + x + HEIGTH) % HEIGTH;
            int jbuff = (j + y + WIDTH) % WIDTH;
            if (field[ibuff][jbuff] == 1 || field[ibuff][jbuff] == 3) {
                check++;
            }
        }
    }
    return check;
}

void read_file(int field[HEIGTH][WIDTH]) {
    char c;
    for (int i = 0; i < HEIGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            scanf("%c", &c);
            if (c == '*') {
                field[i][j] = 1;
            } else {
                field[i][j] = 0;
            }
        }
        while (c != '\n') {
            scanf("%c", &c);
        }
    }
}

void draw_game(int field[HEIGTH][WIDTH]) {
    for (int i = 0; i < HEIGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 1) {
                mvaddch(i, j, '*');
            } else if (field[i][j] == 0) {
                mvaddch(i, j, ' ');
            }
        }
    }
}

int finish_game(char key) {
    int flag = 1;
    if (key == 'q' || key == 'Q') {
        flag = 0;
    }
    return flag;
}

int check_end(int field[HEIGTH][WIDTH]) {
    int count = 0;
    for (int i = 0; i < HEIGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 2 || field[i][j] == 3) {
                count++;
            }
        }
    }
    return count;
}