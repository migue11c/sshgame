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
// drawVector refactor
// Anchored camera view
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
    std::string name;
    float scl; //scales the poi accordingly
    int cond; //condition for display and interaction
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

void drawVector(int sty, int stx, int finy, int finx, int div, int offs){
    int dy = abs(finy - sty);
    int sy = sty < finy ? 1 : -1;
    int dx = abs(finx - stx);
    int sx = stx < finx ? 1 : -1;
    int err = (dx > dy ? dx : -dy)/2, e2;
    int i = 0;
    while(1){ // i still need to understand this
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
        wclear(stdscr); //this clears entire window, need to localize it to vectors
        // ...which means you also need a special window for vectors and to call it in args...
        //
        // also please refactor drawVector() to use vertex instead of x,y, and make a char arg to control what is being printed
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

// this is essentially drawCity()
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
  //for (int i = 0; i<30;i++){ // remove on release
  //    if (maxy/2+scale*(city.points[i].y-cpos.y) >= 0 && maxx/2+scale*(city.points[i].x-cpos.x >= 0) &&
  //        maxy/2+scale*(city.points[i].y-cpos.y) < maxy && maxx/2+scale*(city.points[i].x-cpos.x < maxx)){
  //            mvprintw(maxy/2+scale*(city.points[i].y-cpos.y), maxx/2+scale*(city.points[i].x-cpos.x), "%d",i+1);
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
    std::vector<poi> poi;
    poi.push_back({{96,235},"District 1: A",1});
    poi.push_back({{80,223},"District 2: B",2});
    poi.push_back({{81,267},"District 3: C",3});
    poi.push_back({{106,266},"District 4: D",4});
    poi.push_back({{113,209},"District 5: E",5});
    poi.push_back({{86,181},"District 6: F",6});
    poi.push_back({{61,211},"District 7: G",7});
    poi.push_back({{63,283},"District 8: H",8});
    poi.push_back({{90,315},"District 9: I",9});
    poi.push_back({{124,291},"District 10: J",10});
    poi.push_back({{132,221},"District 11: K",11});
    poi.push_back({{120,167},"District 12: L",12});
    poi.push_back({{87,139},"District 13: M",13});
    poi.push_back({{53,153},"District 14: N",14});
    poi.push_back({{41,209},"District 15: O",15});
    poi.push_back({{40,263},"District 16: P",16});
    poi.push_back({{55,338},"District 17: Q",17});
    poi.push_back({{105,367},"District 18: R",18});
    poi.push_back({{140,341},"District 19: S",19});
    poi.push_back({{149,273},"District 20: T",20});
    poi.push_back({{153,197},"District 21: U",21});
    poi.push_back({{125,125},"District 22: V",22});
    poi.push_back({{85,86},"District 23: W",23});
    poi.push_back({{49,97},"District 24: X",24});
    poi.push_back({{23,176},"District 25: Y",25});
    return poi;
}
void drawPoi(std::vector<poi> poi,double scale){
    float maxx,maxy;
    int mark;
    getmaxyx(stdscr, maxy, maxx);
    for (int i=0; i<poi.size(); i++){
        if (cpos.x != poi[i].coord.x && cpos.y != poi[i].coord.y &&
            maxy/2+scale*(poi[i].coord.y-cpos.y) >= 0 && maxy/2+scale*(poi[i].coord.y-cpos.y)<maxy &&
            maxx/2+scale*(poi[i].coord.x-cpos.x) >= 1 && maxx/2+scale*(poi[i].coord.x-cpos.x)<maxx-1){
            mvwprintw(stdscr, maxy/2+scale*(poi[i].coord.y-cpos.y), maxx/2+scale*(poi[i].coord.x-cpos.x)-1, "[x]");
        }
        else if (cpos.x == poi[i].coord.x && cpos.y == poi[i].coord.y){
            mark = i;
        }
    }
    wrefresh(stdscr);
    if (scale>=0.25) {
        textAnimation(poi[mark].name, maxy/2, (maxx/2)-2-poi[mark].name.length()/2);
    }
}


//DRAWPOI IS NOT IMPLEMENTED FOR NAMES, IT ALSO HAS WREFRESH FUNCTION, PLEASE KEEP THAT IN MIND
void cityrender(){
    shell dist = getShell();
    std::vector<poi> poi = getPoi();
    map city = getMap();
    initscr();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    int sc=0;
    double scale = 0.2;
    cpos = poi[0].coord;
    float maxx,maxy;
    getmaxyx(stdscr, maxy, maxx);
    while(1){ // the animations here are linear. find a way to add a bezier curve to them
        clear();
        drawCity(city,scale);
        if (scale > 0.25){
            drawShell(dist, scale);
        }
        //drawPos(cpos, scale);
        drawVector(maxy, maxx/2-maxy, 0, maxx/2+maxy, 4, 0); // this is direction testing
        drawVector(0, maxx/2-maxy, maxy, maxx/2+maxy, 4, 0);
        refresh();
        vertex cit = {86,210};
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
    cityrender();
    return 0;
}
