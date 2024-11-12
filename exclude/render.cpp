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

// TODO: scanline floodfill algorithm https://lodev.org/cgtutor/floodfill.html
//          This is so that you (likely) don't have to implement drawing each polygon.
//          Good idea is to use mvwinchstr()
// drawVector refactor (done)
// Anchored camera view (done)
// Unified system of loading vertices and returning a vector of vectors (i fucking hate and love this)
// Point of interest additional information
// draw and move between different sets of POIs based on scale
// add hidden POIs that can't be interacted with unless a condition is satisfied
// change camera anchor based on sector and scale

struct vertex{
    double y,x;
};

struct sector{ // this will essentially be player information
    int ar,dis,plc; //area, district and place
};

struct poi{ // needs additional info
    vertex coord;
    int cond; //condition for display and interaction
    char name[24];
    int len;
    float scl; //scales the poi accordingly (what is this again?)
    // make sure to make an array of poi to make things easier later
};

struct map{ // these two are practically the same, there is no reason for them to exist
    vertex points[30];
};
struct shell{
    vertex points[70];
};
struct wall{ // redundant
    std::vector<vertex> points;
};

inline vertex cpos={0,0};
inline int offs=0;
static vertex anchor{96,235};

void refreshTimer(bool done) {
    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        refresh();
        offs++;
    }
}

void wait() {
    std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

void textAnimation(WINDOW* win, std::string text, int len, int y, int x, int offs){
    WINDOW* anim = newwin(1, len+4, y+offs, x+offs);
  //curs_set(2);
    std::string buffer;
    for (int i = 0;i<=len;i++){
        wmove(anim,0, ((len+4)/2) - 1 - i/2);
        buffer += text[i];
        if (x>=0 && y>=0 && x<getmaxx(win) && y<getmaxy(win)){
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

void drawVector(WINDOW* win, int sty, int stx, int finy, int finx, int div, int offs){
    int dy = abs(finy - sty);
    int sy = sty < finy ? 1 : -1;
    int dx = abs(finx - stx);
    int sx = stx < finx ? 1 : -1;
    int err = (dx > dy ? dx : -dy)/2, e2;
    int i = 0;
    while(1){ // i still need to understand this
       	wmove(win, sty, stx);
        if (sty>=0 && stx>=0 && sty<getmaxy(win) && stx<getmaxx(win) && (i+offs)%div == 0){
            wprintw(win, "#");
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
void animVector(WINDOW* win){
    float i=0;
    bool done = false;
    std::thread rf(refreshTimer,done);
    rf.detach();
    while (1){
        wclear(win); //this clears entire window, need to localize it to vectors
        // ...which means you also need a special window for vectors and to call it in args...
        //
        // also please refactor drawVector() to use vertex instead of x,y, and make a char arg to control what is being printed
        drawVector(win,7, 5, 7, 20, 3, offs/3);
        drawVector(win,7, 20, 17+cos(i/4)*5, 22+sin(i/4)*5, 3, offs/3);
        drawVector(win,17+cos(i/4)*5, 22+sin(i/4)*5, 14, 8, 3, offs/3);
        mvprintw(0, 0, "y: %f, x: %f",(17+cos(i/4)*5),(22+cos(i/4)*5));
        drawVector(win,14, 8, 7, 5, 3, offs/3);
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

// this is essentially drawCity()
void drawShell(WINDOW* win, shell dist, double scale){
    float maxy,maxx;
    getmaxyx(win, maxy, maxx);
    for (int i = 0; i<69; i++){ //draws each segment to scale and with camera
        if (maxy/2+scale*(dist.points[i].y-cpos.y) >= 0 || maxx/2+scale*(dist.points[i].x-cpos.x) >= 0 ||
                maxy/2+scale*(dist.points[(i+1)].y-cpos.y) < maxy || maxx/2+scale*(dist.points[(i+1)].x-cpos.x) < maxx){
        }
        drawVector(win,maxy/2+scale*(dist.points[i].y-cpos.y), maxx/2+scale*(dist.points[i].x-cpos.x),
            maxy/2+scale*(dist.points[(i+1)].y-cpos.y), maxx/2+scale*(dist.points[(i+1)].x-cpos.x), 1, 0);
    }
  //for (int i = 0; i<70;i++){ // this needs to be removed on release
  //    mvprintw(maxy/2+scale*(dist.points[i].y-cpos.y), maxx/2+scale*(dist.points[i].x-cpos.x), "%d",i+1);
  //}
}

map getMap(){
    map city;
    std::ifstream in("map");
    for (int i=0;i<30;i++){
        in >> city.points[i].x >> city.points[i].y;
    }
    return city;
}

void drawCity(WINDOW* win, map city, double scale) {
    float maxy,maxx;
    getmaxyx(win, maxy, maxx);

    for (int i = 0; i<30; i++){ //draws each segment to scale and with camera
        if (maxy/2+scale*(city.points[i].y-cpos.y) >=0 || maxx/2+scale*(city.points[i].x-cpos.x >= 0) ||
            maxy/2+scale*(city.points[(i+1)%30].y-cpos.y) < maxy || maxx/2+scale*(city.points[(i+1)%30].x-cpos.x) < maxx){
                drawVector(win, maxy/2+scale*(city.points[i].y-cpos.y), maxx/2+scale*(city.points[i].x-cpos.x),
                    maxy/2+scale*(city.points[(i+1)%30].y-cpos.y), maxx/2+scale*(city.points[(i+1)%30].x-cpos.x), 1, 0);
            }
    }
  //for (int i = 0; i<30;i++){ // remove on release
  //    if (maxy/2+scale*(city.points[i].y-cpos.y) >= 0 && maxx/2+scale*(city.points[i].x-cpos.x >= 0) &&
  //        maxy/2+scale*(city.points[i].y-cpos.y) < maxy && maxx/2+scale*(city.points[i].x-cpos.x < maxx)){
  //            mvwprintw(win, maxy/2+scale*(city.points[i].y-cpos.y), maxx/2+scale*(city.points[i].x-cpos.x), "%d",i+1);
  //        }
  //}
}

void drawPos(vertex cam, double scale){
    mvwprintw(stdscr,0,0, "x = %f; y = %f; scale = %f",cam.x,cam.y,scale);
}

double distance(vertex st, vertex en){
    return std::sqrt((en.x-st.x)*(en.x-st.x)+(en.y-st.y)*(en.y-st.y));
}

std::vector<poi> getPoi(){
    std::vector<poi> Poi;
    std::ifstream file("map.dat");
    int i = 0;
    while (i<25){
        poi buffer;
        file >> buffer.coord.y >> buffer.coord.x >> buffer.cond;
        buffer.len=0;
        while (buffer.len<24){
            file.get(buffer.name[buffer.len]);
            if (buffer.name[buffer.len] == '\n'){
                buffer.name[buffer.len] = 0;
                break;
            }
            buffer.len++;
        }
        Poi.push_back(buffer);
        i++;
    }
    file.close();
    return Poi;
}

std::vector<poi> tempgetPoi(){
    std::vector<poi> poi;
    poi.push_back({{96,235},0,"District 1: A",12});
    poi.push_back({{80,223},0,"District 2: B",12});
    poi.push_back({{81,267},0,"District 3: C",12});
    poi.push_back({{106,266},0,"District 4: D",12});
    poi.push_back({{113,209},0,"District 5: E",12});
    poi.push_back({{86,181},0,"District 6: F",12});
    poi.push_back({{61,211},0,"District 7: G",12});
    poi.push_back({{63,283},0,"District 8: H",12});
    poi.push_back({{90,315},0,"District 9: I",12});
    poi.push_back({{124,291},0,"District 10: J",13});
    poi.push_back({{132,221},0,"District 11: K",13});
    poi.push_back({{120,167},0,"District 12: L",13});
    poi.push_back({{87,139},0,"District 13: M",13});
    poi.push_back({{53,153},0,"District 14: N",13});
    poi.push_back({{41,209},0,"District 15: O",13});
    poi.push_back({{40,263},0,"District 16: P",13});
    poi.push_back({{55,338},0,"District 17: Q",13});
    poi.push_back({{105,367},0,"District 18: R",13});
    poi.push_back({{140,341},0,"District 19: S",13});
    poi.push_back({{149,273},0,"District 20: T",13});
    poi.push_back({{153,197},0,"District 21: U",13});
    poi.push_back({{125,125},0,"District 22: V",13});
    poi.push_back({{85,86},0,"District 23: W",13});
    poi.push_back({{49,97},0,"District 24: X",13});
    poi.push_back({{23,176},0,"District 25: Y",13});
    return poi;
}
void drawPoi(WINDOW* win, std::vector<poi> poi,double scale,int offs){
    float maxx,maxy;
    int mark;
    getmaxyx(win, maxy, maxx);
    for (int i=0; i<poi.size(); i++){
        if (cpos.x != poi[i].coord.x && cpos.y != poi[i].coord.y &&
            maxy/2+scale*(poi[i].coord.y-cpos.y) >= 0 && maxy/2+scale*(poi[i].coord.y-cpos.y)<maxy &&
            maxx/2+scale*(poi[i].coord.x-cpos.x) >= 1 && maxx/2+scale*(poi[i].coord.x-cpos.x)<maxx-1){
            mvwprintw(win, maxy/2+scale*(poi[i].coord.y-cpos.y), maxx/2+scale*(poi[i].coord.x-cpos.x)-1, "[x]");
        }
        else if (cpos.x == poi[i].coord.x && cpos.y == poi[i].coord.y){
            mark = i;
        }
    }
    wrefresh(win);
    if (scale>=0.25) {
        textAnimation(win, poi[mark].name,poi[mark].len, maxy/2, (maxx/2)-2-poi[mark].len/2,offs); // the +2 needs to be fixed
    }
}

void cityrender(){
    WINDOW* border = newwin(getmaxy(stdscr)-2, getmaxx(stdscr)-2, 1, 1);
    WINDOW* worldmap = newwin(getmaxy(stdscr)-4, getmaxx(stdscr)-4, 2, 2);
    shell dist = getShell();

    int offs = 2; // this actually needs to be implemented because some things are not displayed relative to the center but to the corner

    std::vector<poi> poi = tempgetPoi();
    map city = getMap();
    keypad(worldmap, true);
    int sc=0;
    double scale = 0.2;
    cpos = poi[0].coord;
    float maxx,maxy;
    getmaxyx(worldmap, maxy, maxx);
    box(border, 0, 0);
    mvwprintw(border, 0, 3, "[World Map]");
    wrefresh(border);
    while(1){ // the animations here are linear. find a way to add a bezier curve to them
        wclear(worldmap);
        drawCity(worldmap,city,scale);
        if (scale > 0.25){
            drawShell(worldmap, dist, scale);
        }
        //drawPos(cpos, scale);
        drawVector(worldmap, maxy, maxx/2-maxy, 0, maxx/2+maxy, 4, 0); // this is direction testing
        drawVector(worldmap, 0, maxx/2-maxy, maxy, maxx/2+maxy, 4, 0);
        wrefresh(worldmap);
        vertex cit = {86,210};

        if (scale <= 0.25){ // make this a new thread
            textAnimation(worldmap, "The City",7, maxy/2+scale*(cit.y-cpos.y), maxx/2+scale*(cit.x-cpos.x)-4, offs); // THIS IS NOT RELATIVE TO A WINDOW
        }
        else {
            drawPoi(worldmap,poi, scale,offs);
        }
        // this manually moves the camera by pixels (this will be useful for dungeons but not for this)
        ch:
        switch (wgetch(worldmap)) { // please work on the dist method. it doesn't work properly
            case KEY_UP:{
                std::vector<double> dist;
                for (int i=0; i<poi.size();i++){
                    if (2*(poi[i].coord.y-cpos.y) <= -std::abs(poi[i].coord.x-cpos.x) && // vertical
                        poi[i].coord.y-cpos.y != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i].coord));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it].coord;
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
                    if (2*(poi[i].coord.y-cpos.y) >= std::abs(poi[i].coord.x-cpos.x) && // vertical
                        poi[i].coord.y-cpos.y != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i].coord));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it].coord;
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
                    if (poi[i].coord.x-cpos.x <= -2*std::abs(poi[i].coord.y-cpos.y) && // vertical
                        poi[i].coord.x-cpos.x != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i].coord));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it].coord;
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
                    if (poi[i].coord.x-cpos.x >= 2*std::abs(poi[i].coord.y-cpos.y) && // vertical
                        poi[i].coord.x-cpos.x != 0){ // it goes up
                            dist.push_back(distance(cpos, poi[i].coord));
                    }
                    else{
                        dist.push_back(99999);
                    }
                  //mvwprintw(stdscr, i, 0, "%d, %f",i,dist[i]);
                }
                int it = std::distance(dist.begin(),std::min_element(dist.begin(),dist.end()));
                if (dist[it] != 99999){
                    cpos = poi[it].coord;
                  //cpos = {poi[it].coord.y - anchor.y,poi[it].coord.x - anchor.x}; //test of anchor
                }
                else{
                    goto ch;
                }
              //getch();
                break;
            }

            case 'z':{
                switch (sc){
                    case 0:{
                        for (int i = 0; i<5; i++){
                            scale += 0.05;
                            drawCity(worldmap, city,scale);
                            if (scale >= 0.25){
                                drawShell(worldmap,dist,scale);
                            }
                            wrefresh(worldmap);
                            wait();
                        }
                        sc++;
                        break;
                    }
                    case 1:{
                        for (int i = 0; i<5; i++){
                            scale += 0.11;
                            drawCity(worldmap, city,scale);
                            if (scale >= 0.25){
                                drawShell(worldmap,dist,scale);
                            }
                            wrefresh(worldmap);
                            wait();
                        }
                        sc++;
                        break;
                    }
                    default:{
                        goto ch;
                    }
                }
                break;
            }
            case 'x':{
                switch (sc){
                    case 1:{
                        for (int i = 0; i<5; i++){
                            scale -= 0.05;
                            drawCity(worldmap, city,scale);
                            if (scale >= 0.25){
                                drawShell(worldmap,dist,scale);
                            }
                            wrefresh(worldmap);
                            wait();
                        }
                        sc--;
                        break;
                    }
                    case 2:{
                        for (int i = 0; i<5; i++){
                            scale -= 0.11;
                            drawCity(worldmap, city,scale);
                            if (scale >= 0.25){
                                drawShell(worldmap,dist,scale);
                            }
                            wrefresh(worldmap);
                            wait();
                        }
                        sc--;
                        break;
                    }
                    default:{
                        goto ch;
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
    initscr();
    noecho();
    curs_set(0);
    cityrender();
    return 0;
}
