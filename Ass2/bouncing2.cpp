/*A program is to be written that places a random pattern of circular objects 
within a rectangle drawn on the screen. 
The ball must be launched onto this rectangle and then bounce off each object,
as well as the edges of the screen.  */
// Program By: Raphael Petta Kamgang
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <math.h> //math library contains rand() and srand()
#include <windows.h> // WinApi header 
// define the values so that they can be changed easly if needed
//since these values are used many times throughout the program
#define winWidth 800
#define winHeight 600
#define ballRaduis 10
#define NumberOfRand 10
#define Xmargin 40
#define Ymargin 40
#define btnRaduis 20
#define btnMax 20
#define btnMin 4
#define PI 3.14159265
using namespace std;

//define point structure
struct point {
  int posX;
  int posY;
} ;
void button(int x,int y);
int main(int argc, char *argv[])
{
    int Bouncecount=0; //integer used to accumulate the number of rebounces 
    int i=49;//will hold the x-axis coordinate for the center of the circule
    int j=389;//will hold the y-axis coordinate for the center of the circule
    int delay;
    int xdir=1;/*will hold the value to be added to the x-axis 
                      coordinate of the circle */
    int ydir=1;
    int tag=-1;//used to store the index of the button which found to be in collision with the ball
    int oldi;
    int oldj;
   // int *btnPosArray[2];//holds the x,y coordinates of the  buttons
    int cnt;
    int xSumBtn; //stores the number of buttons which fit horizantally without overlapping
    int ySumBtn; //stores the number of buttons which fit vertically without overlapping
    point ** posMatrix;//dynamic array to store all possible positions of buttons
    int x,y;
    int randomNoX,randomNoY;
    int distance;//distance of the centre of the ball from the centre of any button
    int rndNBtn;//random number of buttons to be generated at start of game btnMin-btnMax  
    int btnSide; /*an integer variable that the quarter number of 
    the button which the ball is approaching */  
    double collNormalAngle;
      /* int initwindow (int width, int height, const char* title="Windows BGI",
    int left=0, int top=0, bool dbflag=false, bool closeflag=true);*/
    initwindow(winWidth,winHeight,
    "Bouncing Ball                                By: Raphael Petta Kamgang"  ,20,20);
    floodfill(Xmargin, Ymargin, WHITE);
    setcolor(RED);  
    //setfillstyle(SOLID_FILL ,BLUE);
    rectangle (Xmargin, Ymargin, winWidth-Xmargin, winHeight-Ymargin);
    
    //calculate xSumBtn and ySumBtn
    xSumBtn=(winWidth-2*Xmargin)/(4*btnRaduis);
    ySumBtn=(winHeight-2*Ymargin)/(4*btnRaduis);
    
    /*allocate the space for dynamic two dimentional array to 
    hold all the possible positions of the buttons*/
    posMatrix = new point*[xSumBtn];
    for ( cnt = 0; cnt < xSumBtn; ++cnt) 
    posMatrix[cnt] = new point [ySumBtn];
    
    //generate the all possible positions and store in posMatrix
    for (x=0;x<xSumBtn-1;x++)
    {
        for (y=0;y<ySumBtn-1;y++)
        {
            posMatrix[x][y].posX=Xmargin+4*(x+1)*(btnRaduis);
            posMatrix[x][y].posY=Ymargin+4*(y+1)*(btnRaduis);
        }
    }
    srand(time(NULL)); //seed the randomization process using the system clock    
    rndNBtn=(rand()%(btnMax-btnMin))+btnMin+1;
    int(*btnPosArray)[2]=new int[rndNBtn][2];
    for (cnt=0;cnt<rndNBtn;cnt++)
    {
       srand(time(NULL)); //seed the randomization process using the system clock
gen:   randomNoX=rand()% (xSumBtn-1) ; 
       randomNoY=rand()%(ySumBtn-1);
    //insure no position will be selected more than once
        if (posMatrix[randomNoX][randomNoY].posX==-1)goto gen;
        btnPosArray[cnt][0]=posMatrix[randomNoX][randomNoY].posX;
        posMatrix[randomNoX][randomNoY].posX=-1;
        btnPosArray[cnt][1]=posMatrix[randomNoX][randomNoY].posY;
        button(btnPosArray[cnt][0],btnPosArray[cnt][1]);
    //give chance for the randomization seed to change by applying delay
       for(delay=0;delay<99999999;delay++);
    }          
    delete [] posMatrix ; //free memory
    
    while(1)
    {    
         for (cnt=0;cnt<rndNBtn;cnt++)
         {
                button(btnPosArray[cnt][0],btnPosArray[cnt][1]);  
         }

        setcolor(YELLOW);
        oldi=i;
        oldj=j;
        /*detect the edges of the window
        subtract the ball raduis from the width or length
        this is to acheieve bouncing before any part of the ball get 
        out of the window*/
        //detect Right Edge
        if (i>=-xdir+winWidth-Xmargin-ballRaduis)
        {
           srand(time(NULL));
           xdir=-((rand()%NumberOfRand )+1);
           tag=-1;
           Bouncecount++;
        }
        //detect left edge
        if (i<=-xdir+Xmargin+ballRaduis)
        {
            //xdir = 1;
            srand(time(NULL));
            xdir=(rand()%NumberOfRand)+1;
            tag=1;
            Bouncecount++;
        }
        i=i+xdir;

        //detect bottom edge
        if (j>=-ydir+winHeight-Ymargin-ballRaduis)
        {
           srand(time(NULL));
            ydir=-1*((rand()%NumberOfRand)+1);
            Bouncecount++;
        }
        //detect top edge
        if (j<=-ydir+Ymargin+ballRaduis)
        {
           srand(time(NULL));
           ydir=(rand()%NumberOfRand)+1;
           Bouncecount++;
           //printf("%d\n",rand());
        }
        j=j+ydir;
                     
        /*detect possible collision with buttons by calculating the 
        the distance of the ball centre from each of the buttons centres
        using Pythagoras' theorem */
        
        for(cnt=0;cnt<rndNBtn;cnt++)
        {      
               srand(time(NULL)); //seed the randomization process using the system clock          
           //Use Pythagorus to find distance to each object's centre
           distance=(int)sqrt((float)pow((btnPosArray[cnt][0]-i),2)+
           pow(btnPosArray[cnt][1]-j,2));
           
           if((distance<(btnRaduis+ballRaduis+sqrt(pow(xdir,2)+pow(ydir,2)))) && tag==-1)
           {
               collNormalAngle=atan2(j-btnPosArray[cnt][1], i-btnPosArray[cnt][0]);
               xdir=((rand()%NumberOfRand)+1)*(int)(fabs(cos (collNormalAngle))/cos (collNormalAngle)) ;
               ydir=((rand()%NumberOfRand)+1)*(int)(fabs(sin (collNormalAngle))/sin (collNormalAngle));
               bgiout <<180*collNormalAngle/PI;
               outstreamxy (10,10);
           }
           if ((distance >2*(btnRaduis+ballRaduis))&&(tag ==cnt)) tag = -1;
         
         } 
			setbkcolor(WHITE);  
			setcolor(BLACK);
         bgiout << "                        "<< endl;
         outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
         bgiout << "Xdir = "<< xdir << endl;
         outstreamxy((winWidth-200-(2*Xmargin))/2, Ymargin/3);
         bgiout << "                        "<< endl;
         outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);
         bgiout << "Ydir = "<< ydir << endl;
         outstreamxy((winWidth-(2*Xmargin))/2, Ymargin/3);    
         setcolor(RED);  
         setfillstyle(SOLID_FILL ,BLUE);
         rectangle (Xmargin, Ymargin, winWidth-Xmargin, winHeight-Ymargin);
        
         setcolor(WHITE);
         setfillstyle(1,WHITE); 
         fillellipse(oldi,oldj,ballRaduis,ballRaduis);
         setfillstyle(1,RED);
         fillellipse(i,j,ballRaduis,ballRaduis);
         //delay is directly proportional to the x and y displacement
        for(delay=0;delay<(1000*(pow(xdir,2) + pow(ydir,2)));delay++); 
        //getch();     
  }
    closegraph();
    return 0;
}

void button(int x,int y)
{     
      setcolor(RED);
      setfillstyle(1,BLUE); 
     fillellipse(x,y,btnRaduis,btnRaduis);
     setfillstyle(1,RED); 
     fillellipse(x,y,btnRaduis/2,btnRaduis/2);
 
 }
