// 
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <math.h>
#include <windows.h> // WinApi header 

using namespace std;
void button(int x,int y);
int main(int argc, char *argv[])

{
    int i=49,j=389,delay,xdir=1,ydir=1;
    int tag,oldi,oldj,k,l;
   
    initwindow(450,450);
   tag=1;
    while(1)
    {
            setcolor(YELLOW);
            oldi=i;
            oldj=j;
            if (i>=440)
            {
              //xdir = -1;
               srand(time(NULL));
             xdir=-(int(rand()%4 )+1);
             tag=-1;
              //goto ssss;
            }
            if (i<=10)
            {
                //xdir = 1;
                srand(time(NULL));
                xdir=int(rand()%4)+1;
                tag=1;
               // goto ssss;
            }
            i=i+xdir;
            if (j>=440)
            {
               srand(time(NULL));
                ydir=-(int(rand()%4)+1);
            }
            if (j<=10)
            {
               srand(time(NULL));
               ydir=int(rand()%4)+1;
            }
            j=j+ydir;//there should be a loop here somewhere to draw the ball for xdir number of times
            for (k=0;k<=abs(xdir);k++)
            {
               for (l=0;l<abs(ydir);l++)
                {
                  //k=(xdir/abs(xdir))*k;
                  //l=ydir/abs(ydir);  
                    //fillellipse(oldi+k-1,oldj+l,10,10);
//                    setcolor(BLACK);
//                    fillellipse(oldi+k-1,oldj+l,10,10);
//
                   fillellipse(oldi+k,oldj+l,10,10);
     
                    for(delay=0;delay<500000;delay++);  
                    setcolor(BLACK);
cleardevice(); 
                   // fillellipse(oldi+k,oldj+l,10,10);
                  }
        }
      
      }
        closegraph();
    return 0;
}

