#include <stdio.h>
#include <windows.h>
#include "vTask.h"



static TaskArray TaskIDBuffer[64];

DWORD WaitForEvent(HANDLE hEvent,int Timedelay)
{
      int reply;
      reply = WaitForSingleObject(hEvent,Timedelay); 
      return reply;
      }
      
HANDLE CreateAnEvent(char* EventName)
{CreateEvent( 
        NULL,               // default security attributes
        TRUE,               // manual-reset event
        FALSE,              // initial state is nonsignaled
        EventName  // object name
        ); 
}

int IntPipe_Write( PIPE *pip_str,int data)
{
     WaitOnMutex(pip_str->PipMutex,1000); //Mutex to avoid clashes
     if (pip_str->OutP!= (pip_str->InP+1&0x0ff))// if pointers indicate buffer not full
     {
          // printf("OutP = %d  InP+1 = %d\n",pip_str->OutP,pip_str->InP+1&0x0ff);
          
     pip_str->Buffer[(pip_str->InP+1&0x0ff)]= data; // insert data one place ahead
     pip_str->InP++;   // increment pointer
     pip_str->InP=pip_str->InP&0xff;  //mask to 256 only
     ReleaseMutex(pip_str->PipMutex); // release Mutex
     return 1;  // Success
     }
     else
     { 
            ReleaseMutex(pip_str->PipMutex);
            return 0; // Cannot write this item     
       }
}
int IntPipe_Read(PIPE *pip_str, int *home)
{
    WaitOnMutex(pip_str->PipMutex,1000);
    if(pip_str->InP != pip_str->OutP) //Is Pipe not full?
    {
       //  printf("OutP = %d  InP = %d\n",pip_str->OutP,pip_str->InP); 
            
        pip_str->OutP = (pip_str->OutP+1)&0x0ff;  //Increment OutP pointer
          *home = pip_str->Buffer[pip_str->OutP]; // Get data  
        ReleaseMutex(pip_str->PipMutex);
        return 1;
       }
       else
      { 
           
            ReleaseMutex(pip_str->PipMutex);
            return 0;
        }
}


HANDLE RunThisThread(DWORD vID)
 {
     HANDLE ThreadID;
    ThreadID = CreateThread(0, 1024, (LPTHREAD_START_ROUTINE)vID, 0, 0, 0); 
     //Threads created and started
   return ThreadID;
}

void Thread_Join(HANDLE hTaskIDx)
{
     WaitForSingleObject(hTaskIDx,INFINITE);
     }

void EndTask(HANDLE hTaskID)
{
     DWORD dwExitWord;
     
     GetExitCodeThread(hTaskID,&dwExitWord);
     TerminateThread(hTaskID,dwExitWord);
     CloseHandle(hTaskID);
     
     return;
     }

long PriorityOnMutex(HANDLE hHandle, HANDLE hMutex, DWORD tOut)
{
     long status;
     status = WaitForSingleObject(hMutex, tOut);
  //  printf("flag = %d\n",status);
     if (!status)
     {
      
      SetThreadPriority(hHandle,TOP);
      }
    return status;
}
void PriorityOnRelease(HANDLE hHandle, HANDLE hMutex)
{     
     int i;
     DWORD OldP,CurrentID;
     
     for(i=0;i<64;i++)
     {
         if (TaskIDBuffer[i].ID == hHandle)
         {
       
         OldP=TaskIDBuffer[i].PRI;      
         SetTaskPriority(hHandle,OldP);
         ReleaseMutex(hMutex);
      
         return;
         }
     
     }
return;
}

void SetTaskPriority(HANDLE TaskHandle, signed long Priority)
{

     int i;
    
     SetThreadPriority(TaskHandle,Priority);
     
     for(i=0;i<64;i++)
     {
      
         if (TaskIDBuffer[i].ID== TaskHandle||TaskIDBuffer[i].ID==0)
         {
            
            TaskIDBuffer[i].PRI = Priority;
            TaskIDBuffer[i].ID = TaskHandle;
            return;
            }
         }    
return;
}

long WaitOnMutex(HANDLE hMutex, DWORD tOut)
{
    
 return WaitForSingleObject(hMutex, tOut);
 
    
    }

 long WaitOnEvent(HANDLE hEvent, DWORD tOut)
{
    
 return WaitForSingleObject(hEvent, tOut);
 
    
    }   
int WriteToPipe (char* PipeName,char* Input)
{
  
   BOOL fConnected; 
   DWORD dwThreadId; 
   HANDLE hPipe, hThread; 
   char Pipeaddress[80] = "\\\\.\\pipe\\";
   strcat(Pipeaddress,PipeName);
   LPTSTR lpszPipename = TEXT(Pipeaddress); 
   
   TCHAR chRequest[BUFSIZE]; 
   TCHAR chReply[BUFSIZE]; 
   DWORD cbBytesRead, cbReplyBytes, cbWritten; 
   BOOL fSuccess; 
   strcpy(chReply,Input);


// The main loop creates an instance of the named pipe and 
// then waits for a client to connect to it. When the client 
// connects, a thread is created to handle communications 
// with that client, and the loop is repeated. 
 
   for (;;) 
   { 
      hPipe = CreateNamedPipe( 
          lpszPipename,             // pipe name 
          PIPE_ACCESS_DUPLEX,       // read/write access 
          PIPE_TYPE_MESSAGE |       // message type pipe 
          PIPE_READMODE_MESSAGE |   // message-read mode 
          PIPE_WAIT,                // blocking mode 
          PIPE_UNLIMITED_INSTANCES, // max. instances  
          BUFSIZE,                  // output buffer size 
          BUFSIZE,                  // input buffer size 
          0,                        // client time-out 
          NULL);                    // default security attribute 

      if (hPipe == INVALID_HANDLE_VALUE) 
      {
          printf("CreatePipe failed"); 
          return 0;
      }
 
      // Wait for the client to connect; if it succeeds, 
      // the function returns a nonzero value. If the function
      // returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 
 
      fConnected = ConnectNamedPipe(hPipe, NULL) ? 
         TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 
     
      if (fConnected) 
      { //printf("fConnected = $%d\n",fConnected);
     

      fSuccess = ReadFile( 
         hPipe,        // handle to pipe 
         chRequest,    // buffer to receive data 
         BUFSIZE*sizeof(TCHAR), // size of buffer 
         &cbBytesRead, // number of bytes read 
         NULL);        // not overlapped I/O 



   //  if (! fSuccess || cbBytesRead == 0) 
  //       return 0; 
     
     cbReplyBytes = strlen(Input);

   // Write the reply to the pipe. 
     fSuccess = WriteFile( 
         hPipe,        // handle to pipe 
         chReply,      // buffer to write from 
         cbReplyBytes, // number of bytes to write 
         &cbWritten,   // number of bytes written 
         NULL);        // not overlapped I/O 
         
   /*      printf("Write fSuccess = %d\n",fSuccess);
         printf("Reply = %s\n",chReply);
         printf("bytes written = %d\n",cbWritten);
     */    
    //  if (! fSuccess || cbReplyBytes != cbWritten) break; 
 
 
// Flush the pipe to allow the client to read the pipe's contents 
// before disconnecting. Then disconnect the pipe, and close the 
// handle to this pipe instance. 
 
   FlushFileBuffers(hPipe); 
   DisconnectNamedPipe(hPipe); 
   CloseHandle(hPipe); 
 
  // printf("%s\n",chRequest);
   return 1;
}}}

TCHAR* ReadFromPipe(char* PipeName)
{
   TCHAR* ReturnValue;
   LPTSTR lpszWrite;            
   TCHAR chReadBuf[BUFSIZE]; 
   BOOL fSuccess; 
   DWORD cbRead;  
   char Pipeaddress[80] = "\\\\.\\pipe\\";
   strcat(Pipeaddress,PipeName);
   LPTSTR lpszPipename = TEXT(Pipeaddress); 
   
  
 
 // while(!WaitNamedPipe(lpszPipename,20000)); // wait until pipe connected
   
   fSuccess = CallNamedPipe( 
      lpszPipename,        // pipe name 
      lpszWrite,           // message to server 
      (lstrlen(lpszWrite)+1)*sizeof(TCHAR), // message length 
      chReadBuf,              // buffer to receive reply 
      BUFSIZE*sizeof(TCHAR),  // size of read buffer 
      &cbRead,                // number of bytes read 
      20000);                 // wait time 


// printf("CallNamedPipe fSuccess = %d\n",fSuccess);
  // printf("readbuf = %s\n",chReadBuf);  
  Sleep(300);
  if(!fSuccess) return 0;
  ReturnValue=chReadBuf;
  
   return  ReturnValue; 
}



HANDLE CreateATask(DWORD vID)
 {
     HANDLE ThreadID;
    ThreadID = CreateThread(0, 1024, (LPTHREAD_START_ROUTINE)vID, 0, CREATE_SUSPENDED, 0); 
      //    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)vID, 0, 0, 0);             
// TASKS CREATED HERE - SUSPENDED
   return ThreadID;
}
DWORD StartThisTask(HANDLE Task)
{
 (DWORD) ResumeThread(Task);
          
}
HANDLE  CreateAMutex(char* MutexName)
{
        HANDLE hMutex;
      hMutex = CreateMutex(NULL, FALSE, (char *)MutexName);
     
        return hMutex;
        }
