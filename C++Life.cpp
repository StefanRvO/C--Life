//Conways game of life i cpp using allegro for graphic
//gonna se performance difference compered to python
#include<iostream>
#include<allegro.h>
#include<cstdlib>
#include<time.h>
#include "C++Life.h"
#include <math.h>
#include <thread>

#define threading 
    using namespace std;

const int SCREENSIZE_x=1300;
const int SCREENSIZE_y=700;

const int UNITSIZE=5;
int GOTHROUGH=0;
volatile int ticks = 0;
BITMAP *buffer;


void ticker()
{
    ticks++;
}
END_OF_FUNCTION(ticker)
 int updates_per_second = 100000;


class Tick
{
public:
    long int generation;
    int CellArray[SCREENSIZE_x/UNITSIZE][SCREENSIZE_y/UNITSIZE];
    void nexttick();
    Tick();
    void ChangeCell(int cords[2]);
    void Generate(int seed, int sizex, int sizey);
    int boardsizex;
    int boardsizey;
private:
    int bufferArray[SCREENSIZE_x/UNITSIZE][SCREENSIZE_y/UNITSIZE];


};

Tick::Tick()   //Cunstructor
{
    generation=0;

}

void Tick::Generate(int seed, int sizex, int sizey)
{
    srand(seed);
    boardsizex=sizex;
    boardsizey=sizey;
    generation=0;
    for (int i=0; i<boardsizex; i++)
    {
        for (int j=0; j<boardsizey; j++)
        {

            if( rand() %10==0)
                CellArray[i][j]=1;
            else
                CellArray[i][j]=0;
        }
    }
}
void Tick::nexttick()
{
    generation+=1;
    //zero out bufferarray zero
    for (int i=0; i<boardsizex; i++)
    {
        for (int j=0; j<boardsizey; j++)
        {
            bufferArray[i][j]=0;
        }
    }

    for (int x=0; x<boardsizex; x++)
    {
        for (int y=0; y<boardsizey; y++)
        {
            if (!GOTHROUGH)
            {
                //We should add 1 to all neighbours if cell is live
                if (CellArray[x][y]==1)
                {
                    if (x!=(boardsizex-1))
                    {
                        bufferArray[x+1][y]+=1;
                        if (y!=(boardsizey-1))
                        {
                            bufferArray[x+1][y+1]+=1;
                        }
                        if (y!=0)
                        {
                            bufferArray[x+1][y-1]+=1;
                        }
                    }
                    if ( x!=0)
                    {
                        bufferArray[x-1][y]+=1;

                        if (y!=(boardsizey-1))
                        {
                            bufferArray[x-1][y+1]+=1;
                        }
                        if (y!=0)
                        {
                            bufferArray[x-1][y-1]+=1;
                        }
                    }
                    if (y!=(boardsizey-1))
                    {
                        bufferArray[x][y+1]+=1;
                    }
                    if (y!=0)
                    {
                        bufferArray[x][y-1]+=1;
                    }
                }
            }
//            else {}
        }
    }
    for (int x=0; x<boardsizex; x++)
    {
        for (int y=0; y<boardsizey; y++)
        {
            if (bufferArray[x][y]==3)
                CellArray[x][y]=1;
            else if (bufferArray[x][y]==2)
                CellArray[x][y]=CellArray[x][y];
            else if (bufferArray[x][y]<=1)
                CellArray[x][y]=0;
            else if (bufferArray[x][y]>=4)
                CellArray[x][y]=0;

        }
    }
}


Tick game;













class unit
{
public:
    int x;
    int y;
    int gridsize[2];
    unit(int boardsizex, int boardsizey=0);

};

unit::unit(int pixelsizex, int pixelsizey=0)   //constructor
{
    x=pixelsizex;
    if (pixelsizey==0)
    {
        y=x;
    }
    else
    {
        y=pixelsizey;
    }
    gridsize[0]=SCREENSIZE_x/x;
    gridsize[1]=SCREENSIZE_y/y;
}


void DrawScreen( unit U)
{
//Clear Screen
    #ifdef threading
    while (1){
    #endif
    clear_to_color( buffer, makecol( 0, 0, 0));
    //Draw the shit
    for (int x=0; x<U.gridsize[0]; x++)
    {
        for (int y=0; y<U.gridsize[1]; y++)
        {
            if(game.CellArray[x][y]==1)
            {
                //cout <<"1" << endl;
                rectfill(buffer,x*U.x,y*U.y,(x+1)*U.x,(y+1)*U.y,makecol(255,255,255));
            }
            //else
            //cout << 0<<endl;
        }
    }
    acquire_screen();
    blit(buffer, screen, 0, 0, 0, 0, SCREENSIZE_x, SCREENSIZE_y);
    release_screen();
    //cout << "Drawing" << /*game.generation <<*/ endl;
    #ifdef threading
    }
    #endif
}
void Init()
{
    allegro_init();
    install_keyboard();
    install_timer();
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));
    set_gfx_mode( GFX_AUTODETECT_WINDOWED, SCREENSIZE_x, SCREENSIZE_y, 0, 0);
    request_refresh_rate(1000);
}


int main()
{

    using namespace std;
 //   cout << "1" << endl;
    unit U(UNITSIZE);
 //   cout << "1" << endl;
   // Tick  game;
 //   cout << "1" << endl;
    int gameseed=rand()% time(NULL);
    game.Generate(gameseed,U.gridsize[0],U.gridsize[1]);
 //   cout << "1" << endl;
    Init();
 //   cout << "1" << endl;
    int w=0;
    buffer=create_bitmap(SCREENSIZE_x, SCREENSIZE_y);
    #ifdef threading
    thread t1(DrawScreen,U);
    cout << "Threading" << endl;
    #endif
    while(1)
    {
        while (ticks==0)
        {
            rest(1); //rest for tick to pass
        }
        while(ticks>0)
        {
            int old_ticks=ticks;
            //thread t1(DrawScreen,game,U);
            game.nexttick();
            #ifndef threading
            DrawScreen(U);
            #endif
            //cout << "ticking" << game.generation << endl;


            ticks--;
            if(old_ticks<=ticks){ //abort logic if too slow
            break;
            }
        }
        /*if (rand()% static_cast<int>(pow(updates_per_second,1.1))==0)
        {
            updates_per_second++;
            install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));
        }*/
        //if(game.generation>100000)
        //return 0;
        //cout  << game.generation << "-- -- "<< updates_per_second << endl;

    }
    #ifdef threading
    t1.join();
    #endif
    return 0;
}
END_OF_MAIN()

