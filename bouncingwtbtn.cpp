#include <stdio.h>
#include <graphics.h>
#include <math.h>
#include <windows.h> // WinApi header 

using namespace std;
void button(int x,int y);
int main(int argc, char *argv[])

{
    //xdir and ydir are added to the balls position to move it, by changing them to negative
    //the ball can change direction
    // i = start x position of ball, j = starting y pos
    int i=49,j=239,delay,xdir=1,ydir=1;
    signed int x2,y2;
    //oldi is needed, can you see why?
    //What does tag do?
    int distance,oldi,k,tag = -1;
    initwindow(450,450);
    //Define button object positions
   // int btns[] = {50,240,225,240,400,240};
    //No of buttons
    //int num_btns = 3;
    
    while(1)
    {
        //Draw buttons
    //button(btns[0],btns[1]);
    //button(btns[2],btns[3]);
    //button(btns[4],btns[5]);

    setcolor(YELLOW);
  //Edge detections, these are the walls of the box  
     oldi=i;
     if (i==440) 
     {xdir = -1;}
     
     if (i==10)
     {xdir = 1;}
     i=i+xdir;
     
     if (j==440)
      {ydir = -1;}
      
      if (j==10)
      {ydir = 1;}
      j=j+ydir;
   //Collision Detect   
  
  //for(k=0;k<6;k=k+2)
  //{ 
        // Find the absolute x and y values for the distance to the object
  // x2=btns[k]-i;    
  // y2=btns[k+1]-j; 
   
 //Use Pythagorus to find distance to each object's centre
  // distance=(int)sqrt((float)(x2*x2)+(y2*y2));
   
    if(/*(distance<20) &&*/ (tag == -1))
    {
                     
   // Beep(100,10);//create a beep sound for 10 milliseconds of 100Hrz frequency when a button is hit by the ball
     tag = 1;
     if (ydir==1 && xdir==1) 
        {
                  xdir = -1;
                  }
      else
      if (ydir==1 && xdir==-1) 
        {
                  xdir = 1;
                  }
      else
      if (ydir==-1 && xdir==1) 
        {
                  ydir = 1;
                  }
      else
      if (ydir==-1 && xdir==-1) 
        {
                  ydir = 1;
                  }
       
       }
    // if (/*(distance >80)&&*/(tag ==1))
//        { tag = -1;}
         
     //}
    /* 
     Observe this program running, it has been amended to add two more buttons. ALL button
     addresses are now held in an array and therefore a loop can be run to see if the ball 
     is in proximity of any of them. the Vapour trail has been removed by making the
     ball position move take place as two operations, up then across. The collision detector
     is now using a change to i or j according to the direction of the ball at the time.   
      Note the possibility of a use of  getch() instead of a delay, to track actions. Now introduce
      a different response mechanism for the ball when it hits a button, adding some
      randomness to the next direction. ie use rnd().
     */ 
      
      fillellipse(oldi,j,10,10);
     
      
      setcolor(BLACK);
      fillellipse(oldi,j,10,10);
      
      fillellipse(i,j,10,10);
     for(delay=0;delay<500000;delay++);
//getch();
      setcolor(BLACK);
      fillellipse(i,j,10,10);
      
      }
    
   
    closegraph();
    return 0;
}
void button(int x,int y)
    {
    int i;
    i=y;
    setcolor(YELLOW);
    for(i;y>i-10;y--)
         {
          fillellipse(x,y,10,10);
          setcolor(RED);
          fillellipse(x,y,10,10);
          }
    setcolor(BLUE);
    fillellipse(x,y,5,5);
   
}
