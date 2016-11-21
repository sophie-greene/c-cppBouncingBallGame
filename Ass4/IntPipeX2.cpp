#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "vTask.h"


PIPE pipe1,pipe2;
 
long WINAPI Task1()
{
    
    int i;
    //print this on the screen
    printf( "Task1 running\n"); 
    for(i=1;i<514;i++)
     {  // uses address of pipe and actual data value to be written
    while(!IntPipe_Write(&pipe1,i)); // Wait for true value - indicating successful write
    printf("Task1 sends %d\n",i);
    }
    
    printf("Task1 ends\n");                                             
}

long WINAPI Task2()
{
    int stuff;
    //print this on the screen
     printf( "Task2 running\n"); 
     for(;;)
     {      // uses address of pipe and address of data to be returned
            if(IntPipe_Read(&pipe1,&stuff))  // If read returns true then print the data and write to pipe2
        {
            printf("Pipe 1 = %d\n",stuff);
            while(!IntPipe_Write(&pipe2,stuff));// Wait for true value - indicating successful write
            } 
    }
    //change "for(;;)" loop to conditional?  
    printf( "Task2 ends\n");
                                              
}

long WINAPI Task3()
{
    int morestuff;
    //print this on the screen
     printf( "Task3 running\n"); 
     for(;;)
     {      // uses address of pipe and address of data to be returned
            if(IntPipe_Read(&pipe2,&morestuff))  // If read returns true then print the data
                {printf("Pipe 2 = %d\n",morestuff);}
}
     //change "for(;;)" loop to conditional?  
    printf( "Task3 ends\n");                                          
}
//int main()                                                            
//
//{
//
// HANDLE TaskID1, TaskID2, TaskID3;
//
// TaskID1=RunThisThread((DWORD) Task1);
// TaskID2=RunThisThread((DWORD) Task2);
// TaskID3=RunThisThread((DWORD) Task3);
// 
// 
//    //Main() - Waits for key so the user can see the output displayed by the other threads
//
//
//    getchar();                                                       
//    return 1;
//}

 

