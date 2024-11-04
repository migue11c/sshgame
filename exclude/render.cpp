#include <algorithm>
#include <fstream>
#include <iterator>
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

struct poi{
    vertex coord;
    std::string name;
};

struct map{ //these two are practically the same, there is no reason for them to exist
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
        if (x>=0 && y>=0 && x<getmaxx(stdscr) && y<getmaxy(stdscr)){
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
        if (sty>=0 && stx>=0 && sty<getmaxy(stdscr) && stx<getmaxx(stdscr) && (i+offs)%div == 0){
            wprintw(stdscr, "#");
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

void drawShell(shell dist, double scale){
    float maxy,maxx;
    getmaxyx(stdscr, maxy, maxx);
    for (int i = 0; i<69; i++){ //draws each segment to scale and with camera
        if (maxy/2+scale*(dist.points[i].y-cpos.y) >= 0 || maxx/2+scale*(dist.points[i].x-cpos.x) >= 0 ||
                maxy/2+scale*(dist.points[(i+1)].y-cpos.y) < maxy || maxx/2+scale*(dist.points[(i+1)].x-cpos.x) < maxx){
        }
        drawVector(maxy/2+scale*(dist.points[i].y-cpos.y), maxx/2+scale*(dist.points[i].x-cpos.x),
            maxy/2+scale*(dist.points[(i+1)].y-cpos.y), maxx/2+scale*(dist.points[(i+1)].x-cpos.x), 1, 0);
    }
    for (int i = 0; i<70;i++){ // this needs to be removed on release
        mvprintw(maxy/2+scale*(dist.points[i].y-cpos.y), maxx/2+scale*(dist.points[i].x-cpos.x), "%d",i+1);
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

void drawCity(map city, double scale) {
    float maxy,maxx;
    getmaxyx(stdscr, maxy, maxx);
    for (int i = 0; i<30; i++){ //draws each segment to scale and with camera
        if (maxy/2+scale*(city.points[i].y-cpos.y) >=0 || maxx/2+scale*(city.points[i].x-cpos.x >= 0) ||
            maxy/2+scale*(city.points[(i+1)%30].y-cpos.y) < maxy || maxx/2+scale*(city.points[(i+1)%30].x-cpos.x) < maxx){
                drawVector(maxy/2+scale*(city.points[i].y-cpos.y), maxx/2+scale*(city.points[i].x-cpos.x),
                    maxy/2+scale*(city.points[(i+1)%30].y-cpos.y), maxx/2+scale*(city.points[(i+1)%30].x-cpos.x), 1, 0);
            }
    }
    for (int i = 0; i<30;i++){ // remove on release
        if (maxy/2+scale*(city.points[i].y-cpos.y) >= 0 && maxx/2+scale*(city.points[i].x-cpos.x >= 0) &&
            maxy/2+scale*(city.points[i].y-cpos.y) < maxy && maxx/2+scale*(city.points[i].x-cpos.x < maxx)){
                mvprintw(maxy/2+scale*(city.points[i].y-cpos.y), maxx/2+scale*(city.points[i].x-cpos.x), "%d",i+1);
            }
    }
}

void drawPos(vertex cam, double scale){
    mvwprintw(stdscr,0,0, "x = %f; y = %f; scale = %f",cam.x,cam.y,scale);
}

double distance(vertex st, vertex en){
    return std::sqrt((en.x-st.x)*(en.x-st.x)+(en.y-st.y)*(en.y-st.y));
}

std::vector<vertex> getPoi(){
    std::vector<vertex> poi;
    poi.push_back({94,240});
    poi.push_back({80,223});
    poi.push_back({81,267});
    poi.push_back({106,266});
    poi.push_back({113,209});
    poi.push_back({86,181});
    poi.push_back({61,211});
    poi.push_back({63,283});
    poi.push_back({90,315});
    poi.push_back({124,291});
    poi.push_back({132,221});
    poi.push_back({120,167});
    poi.push_back({87,339});
    poi.push_back({53,153});
    poi.push_back({41,209});
    poi.push_back({40,263});
    poi.push_back({55,338});
    poi.push_back({105,367});
    poi.push_back({140,341});
    poi.push_back({149,273});
    poi.push_back({153,197});
    poi.push_back({125,125});
    poi.push_back({85,86});
    poi.push_back({49,97});
    poi.push_back({23,176});
    return poi;
}
void drawPoi(std::vector<vertex> poi,double scale){
    float maxx,maxy;
    std::string text;
    text = "Current Location";
    getmaxyx(stdscr, maxy, maxx);
    for (int i=0; i<poi.size(); i++){
        if (cpos.x != poi[i].x && cpos.y != poi[i].y &&
            maxy/2+scale*(poi[i].y-cpos.y) >= 0 && maxy/2+scale*(poi[i].y-cpos.y)<maxy &&
            maxx/2+scale*(poi[i].x-cpos.x) >= 1 && maxx/2+scale*(poi[i].x-cpos.x)<maxx-1){
            mvwprintw(stdscr, maxy/2+scale*(poi[i].y-cpos.y), maxx/2+scale*(poi[i].x-cpos.x)-1, "[x]");
        }
    }
    wrefresh(stdscr);
    if (scale>=0.25) {
        textAnimation(text, maxy/2, (maxx/2)-2-text.length()/2);
    }
}

void cityrender(){
    shell dist = getShell();
    std::vector<vertex> poi = getPoi();
    map city = getMap();
    initscr();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    int sc=0;
    double scale = 0.2;
    cpos = poi[0];
    float maxx,maxy;
    getmaxyx(stdscr, maxy, maxx);
    while(1){ // the animations here are linear. find a way to add a bezier curve to them
        clear();
        drawCity(city,scale);
        if (scale > 0.25){
            drawShell(dist, scale);
        }
        drawPos(cpos, scale);
        drawVector(maxy, maxx/2-maxy, 0, maxx/2+maxy, 4, 0);
        drawVector(0, maxx/2-maxy, maxy, maxx/2+maxy, 4, 0);
        refresh();
        vertex cit = {94.044,230.273};
        if (scale <= 0.25){
            textAnimation("The City", maxy/2+scale*(cit.y-cpos.y), maxx/2+scale*(cit.x-cpos.x)-4); // this does not work
        }
        else {
            drawPoi(poi, scale);
        }
        // this manually moves the camera by pixels (this will be useful for dungeons but not for this)
        ch:
        switch (getch()) { // please work on the dist method. it doesn't work properly
            case KEY_UP:{
                std::vector<double> dist;
                for (int i=0; i<poi.size();i++){
                    if (2*(poi[i].y-cpos.y) <= -std::abs(poi[i].x-cpos.x) && // vertical
                        poi[i].y-cpos.y != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i]));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it];
                }
                else{
                    goto ch;
                }
              //getch();
                break;
            }
            case KEY_DOWN:{
                std::vector<double> dist;
                for (int i=0; i<poi.size();i++){
                    if (2*(poi[i].y-cpos.y) >= std::abs(poi[i].x-cpos.x) && // vertical
                        poi[i].y-cpos.y != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i]));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it];
                }
                else{
                    goto ch;
                }
              //getch();
                break;
            }
            case KEY_LEFT:{
                std::vector<double> dist;
                for (int i=0; i<poi.size();i++){
                    if (poi[i].x-cpos.x <= -2*std::abs(poi[i].y-cpos.y) && // vertical
                        poi[i].x-cpos.x != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i]));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it];
                }
                else{
                    goto ch;
                }
              //getch();
                break;
            }
            case KEY_RIGHT:{
                std::vector<double> dist;
                for (int i=0; i<poi.size();i++){
                    if (poi[i].x-cpos.x >= 2*std::abs(poi[i].y-cpos.y) && // vertical
                        poi[i].x-cpos.x != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i]));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it];
                }
                else{
                    goto ch;
                }
              //getch();
                break;
            }

            //SCALE NEEDS TO BE DONE IN 3 STEPS, STEP 3 NEEDS TO CHECK FOR NODE
            case 'z':{
                switch (sc){
                    case 0:{
                        for (int i = 0; i<5; i++){
                            scale += 0.05;
                            drawCity(city,scale);
                            if (scale >= 0.25){
                                drawShell(dist,scale);
                            }
                            refresh();
                            wait();
                        }
                        sc++;
                        break;
                    }
                    case 1:{
                        for (int i = 0; i<5; i++){
                            scale += 0.11;
                            drawCity(city,scale);
                            if (scale >= 0.25){
                                drawShell(dist,scale);
                            }
                            refresh();
                            wait();
                        }
                        sc++;
                        break;
                    }
                    default:{
                        break;
                    }
                }
                break;
            }
            case 'x':{
                switch (sc){
                    case 1:{
                        for (int i = 0; i<5; i++){
                            scale -= 0.05;
                            drawCity(city,scale);
                            if (scale >= 0.25){
                                drawShell(dist,scale);
                            }
                            refresh();
                            wait();
                        }
                        sc--;
                        break;
                    }
                    case 2:{
                        for (int i = 0; i<5; i++){
                            scale -= 0.11;
                            drawCity(city,scale);
                            if (scale >= 0.25){
                                drawShell(dist,scale);
                            }
                            refresh();
                            wait();
                        }
                        sc--;
                        break;
                    }
                    default:{
                        break;
                    }
                }
                break;
            }
            default:{
                goto ch; //this skips the text rendering
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
