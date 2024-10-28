#include <cstring>
#include <fstream>
#include <ncurses/ncurses.h>
#include <string>
#include <thread>

#include "audiostuff.h"
#include "player.h"
#include "logs.h"

using namespace std;

//bool login(int ymax, int xmax);
//bool auth(string username, string password);

bool auth(string username, string password) {
    ifstream list("userlist");
    string buffer;
    LogThis("entered auth");
    while(list >> buffer){
        if (strcmp(username.c_str(), buffer.c_str())==0){
            list >> buffer;
            list.close();
            return (strcmp(password.c_str(), buffer.c_str())==0);
        }
    }
    list.close();
    return false;
}

bool login(int ymax, int xmax){
    WINDOW* loginbox = newwin(7, 30, ymax/2 - 3, xmax/2 - 15);
    int chr;
    int err = 0;

    while(1){
        string un,pw;
        un = "";
        pw = "";
        clear();
        switch (err) {
            case 0:
                mvprintw(ymax/2 - 6,xmax/2 - 7,"Please login..");
                mvprintw(ymax/2 - 5,xmax/2 - 7,"(max 15 chars)");
                box(loginbox, 0, 0);
                break;
            case 1:
                mvprintw(ymax/2 - 5, xmax/2 - 12, "Wrong username/passowrd!");
                box(loginbox,0,0);
                break;
            default:
                break;
        }
        refresh();
        wrefresh(loginbox);
        //raw();
        mvprintw(ymax/2 - 1, xmax/2 - 13, "Username:");
        mvprintw(ymax/2 + 1, xmax/2 - 13, "Password:");
        start:

        move(ymax/2 - 1, xmax/2 - 3);
        curs_set(2);
        //username field
        for (int i=0;i<15;i++){
            uInp:
            chr = getch();
            std::thread press(playKey,chr); //this right here took me so long
            press.detach();

            if (chr == KEY_BACKSPACE || chr == KEY_DC || chr == 127){
                if (i==0)
                    goto uInp;
                i--;
                move(ymax/2 - 1 , xmax/2 - 3 + i);
                printw(" ");
                move(ymax/2 - 1 , xmax/2 - 3 + i);
                un.erase(un.begin()+i);
                goto uInp;
            }
            if (chr == 27){
                goto alt;
            }
            if (chr == '\n'){
                if (i==0) {
                    mvprintw(ymax/2 +5, xmax/2 -14, "Do not leave the field blank");
                    mvprintw(ymax/2 +6, xmax/2 -7, "(Press Enter)");
                    getch();
                    std::thread press(playKey,chr);
                    press.detach();

                    move(ymax/2 + 5, 0);
                    clrtobot();
                    move(ymax/2 - 1, xmax/2 - 3);
                    goto uInp;
                }
                break;
            }
            printw("%c",chr);
            un += (char)chr;
        }

        move(ymax/2 + 1, xmax/2 - 3);
        //password field
        for (int i=0;i<15;i++){
            pInp:
            chr = getch();
            std::thread press(playKey,chr);
            press.detach();

            if (chr == KEY_BACKSPACE || chr == KEY_DC || chr == 127){
                if (i==0)
                    goto pInp;
                i--;
                move(ymax/2 + 1 , xmax/2 - 3 + i);
                printw(" ");
                move(ymax/2 + 1 , xmax/2 - 3 + i);
                pw.erase(pw.begin()+i);
                goto pInp;
            }
            if (chr == 27){
                goto alt;
            }
            if (chr == '\n'){
                if (i==0) {
                    mvprintw(ymax/2 +5, xmax/2 -14, "Do not leave the field blank");
                    mvprintw(ymax/2 +6, xmax/2 -7, "(Press Enter)");
                    getch();
                    std::thread press(playKey,chr);
                    press.detach();

                    move(ymax/2 + 5, 0);
                    clrtobot();
                    move(ymax/2 + 1, xmax/2 - 3);
                    goto pInp;
                }
                break;
            }
            printw("*");
            pw += (char)chr;
        }

        if (fetchData(un,pw) == 0){
            wclear(loginbox);
            clear();
            curs_set(0);
            delwin(loginbox);
            refresh();
            LogThis("user %s logged in",un.c_str());
            //noraw();
            LogThis(player::username);
            //getPlayerData();
            return true;
        }
        else {
            LogThis(un);
            err = 1;
        }
    }

    alt:
    wclear(loginbox);
    clear();
    curs_set(0);
    delwin(loginbox);
    refresh();
    LogThis("exited login");
    //noraw();
    return false;
}
