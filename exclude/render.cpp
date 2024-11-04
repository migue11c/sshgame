#include <fstream>
#include <ncurses/ncurses.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <vector>

struct vertex{
    double y,x;
};

struct map{
    vertex points[30];
};
struct shell{
    vertex points[70];
};
struct wall{
    std::vector<vertex> points;
};

inline vertex cpos={0,0};
inline int offs=0;
inline double scale=1;

void refreshTimer(bool done) {
    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        refresh();
        offs++;
    }
}

void wait() {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

void textAnimation(std::string text, int y, int x){
    WINDOW* anim = newwin(1, text.length()+4, y, x);
  //curs_set(2);
    std::string buffer;
    for (int i = 0;i<text.length();i++){
        wmove(anim,0, ((text.length()+4)/2) - 1 - i/2);
        buffer += text[i];
        if (x>=0 && y>=0 && x<=getmaxx(stdscr) && y<=getmaxy(stdscr)){
            wprintw(anim,"[%s]",buffer.c_str());
        }
        wrefresh(anim);
        wait();
    }
  //curs_set(0);
  //wclear(anim); //make sure to always wrefresh after you want to show something
    delwin(anim);
  //refresh();
}

void drawVector(int sty, int stx, int finy, int finx, int div, int offs){ // needs to NOT wprintw when invalid position
    int dy = abs(finy - sty);
    int sy = sty < finy ? 1 : -1;
    int dx = abs(finx - stx);
    int sx = stx < finx ? 1 : -1;
    int err = (dx > dy ? dx : -dy)/2, e2;
    int i = 0;
    while(1){
       	wmove(stdscr, sty, stx);
        if (sty>=0 && stx>=0 && sty<=getmaxy(stdscr) && stx<=getmaxx(stdscr)){
            if ((i+offs)%div == 0 ){
                wprintw(stdscr, "#");
            }
        }

        if (stx==finx && sty==finy){
            break;
        }

        e2=err;

        if (e2 > -dx){
            err -= dy;
            stx += sx;
        }

        if (e2 < dy){
            err += dx;
            sty += sy;
        }
        end:
        i++;
    }
}

// work on this later
void animVector(){
    float i=0;
    bool done = false;
    std::thread rf(refreshTimer,done);
    rf.detach();
    while (1){
        clear(); //this clears entire window, need to localize it to vectors
        drawVector(7, 5, 7, 20, 3, offs/3);
        drawVector(7, 20, 17+cos(i/4)*5, 22+sin(i/4)*5, 3, offs/3);
        drawVector(17+cos(i/4)*5, 22+sin(i/4)*5, 14, 8, 3, offs/3);
        mvprintw(0, 0, "y: %f, x: %f",(17+cos(i/4)*5),(22+cos(i/4)*5));
        drawVector(14, 8, 7, 5, 3, offs/3);
        wait();
        i++;
    }
    done = true;
}

//this is temporary and needs to be replaced
shell getShell(){
    shell dist;
    std::ifstream in("walls");
    for (int i=0;i<70;i++){
        in >> dist.points[i].x >> dist.points[i].y;
    }
    return dist;
}

void drawShell(shell dist){
    int maxy,maxx;
    getmaxyx(stdscr, maxy, maxx);
    for (int i = 0; i<69; i++){ //draws each segment to scale and with camera
        // warning: this needs to be centered
        drawVector(scale*(dist.points[i].y+0-cpos.y), scale*(dist.points[i].x+0-cpos.x),
            scale*(dist.points[(i+1)].y+0-cpos.y), scale*(dist.points[(i+1)].x+0-cpos.x), 1, 0);
    }
    for (int i = 0; i<70;i++){
        mvprintw(scale*(dist.points[i].y+0-cpos.y), scale*(dist.points[i].x+0-cpos.x), "%d",i+1);
    }
}

map getMap(){
    map city;
    std::ifstream in("map");
    for (int i=0;i<30;i++){
        in >> city.points[i].x >> city.points[i].y;
    }
    return city;
}

void drawCity(map city) {
    int maxy,maxx;
    getmaxyx(stdscr, maxy, maxx);
    for (int i = 0; i<30; i++){ //draws each segment to scale and with camera
        // warning: this needs to be centered
        drawVector(scale*(city.points[i].y+0-cpos.y), scale*(city.points[i].x+0-cpos.x),
            scale*(city.points[(i+1)%30].y+0-cpos.y), scale*(city.points[(i+1)%30].x+0-cpos.x), 1, 0);
    }
    for (int i = 0; i<30;i++){
        mvprintw(scale*(city.points[i].y+0-cpos.y), scale*(city.points[i].x+0-cpos.x), "%d",i+1);
    }
}

void cityrender(){
    shell dist = getShell();
    map city = getMap();
    initscr();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    scale = 0.1;
    cpos.x = 10;
    cpos.y = 12;
  //for (int i=0;i<29;i++){
  //    wmove(stdscr, i, 0);
  //    wprintw(stdscr, "x:%f, y: %f", city.points[i].x, city.points[i].y);
  //}
  //getch();
    int a;
    while(1){ // the animations here are linear. find a way to add a bezier curve to them
        clear();
        drawCity(city);
        if (scale >= 0.25){
            drawShell(dist);
        }
        refresh();
        textAnimation("The City", scale*(94.044-cpos.y), scale*(230.273-cpos.x)-4); // this does not work
        ch:
        a = getch(); // this manually moves the camera by pixels (this will be useful for dungeons but not for this)
        switch (a) {
            case KEY_UP:{
                for (int i = 0; i<5; i++){
                    cpos.y = (cpos.y*scale - 2)/scale;
                    drawCity(city);
                    if (scale >= 0.25){
                        drawShell(dist);
                    }
                    refresh();
                    wait();
                }
                break;
            }
            case KEY_DOWN:{
                for (int i = 0; i<5; i++){
                    cpos.y = (cpos.y*scale + 2)/scale;
                    drawCity(city);
                    if (scale >= 0.25){
                        drawShell(dist);
                    }
                    refresh();
                    wait();
                }
                break;
            }
            case KEY_LEFT:{
                for (int i = 0; i<5; i++){
                    cpos.x = (cpos.x*scale - 2)/scale;
                    drawCity(city);
                    if (scale >= 0.25){
                        drawShell(dist);
                    }
                    refresh();
                    wait();
                }
                break;
            }
            case KEY_RIGHT:{
                for (int i = 0; i<5; i++){
                    cpos.x = (cpos.x*scale + 2)/scale;
                    drawCity(city);
                    if (scale >= 0.25){
                        drawShell(dist);
                    }
                    refresh();
                    wait();
                }
                break;
            }
            case 'w':{
                for (int i = 0; i<5; i++){
                    scale += 0.01;
                    drawCity(city);
                    if (scale >= 0.25){
                        drawShell(dist);
                    }
                    refresh();
                    wait();
                }
                break;
            }
            case 'd':{
                if (scale - 0.05 > 0.05){
                    for (int i = 0; i<5; i++){
                        scale -= 0.01;
                        drawCity(city);
                        if (scale >= 0.25){
                            drawShell(dist);
                        }
                        refresh();
                        wait();
                    }
                }
                break;
            }
            default:{
                goto ch;
            }
        }
    }
    //drawVector(city.points[0].y+0, city.points[0].x+0, city.points[1].y+0, city.points[1].x+0, 1, 0);
    //animVector();
}
int main(){
    cityrender();
    return 0;
}
