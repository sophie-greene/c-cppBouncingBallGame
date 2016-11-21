#define  IDLE -15
#define  VLOW  -2
#define  LOW -1
#define  NORMAL 0
#define  HIGH 1
#define  VHIGH 2
#define  TOP 15
#define BUFSIZE 512

struct PIPE {
       int Buffer[256];  //Buffer for pipe
       int InP;         // In pointer
       int OutP;         // Out Pointer
       HANDLE PipMutex;   // Mutex to protect from clashes with pointers
       };
       
struct TaskArray {
       HANDLE ID;
       DWORD PRI;
       };
       
HANDLE CreateAMutex(char* MutexName);       
HANDLE CreateATask(DWORD vID);
DWORD StartThisTask(HANDLE Task);
DWORD WINAPI InstanceThread(LPVOID); 
TCHAR* ReadPipe(HANDLE lpvParam);
int WriteToPipe (char* PipeName,char* Input);
long WaitOnMutex(HANDLE hMutex, DWORD tOut);
void SetTaskPriority(HANDLE Task, signed long Priority);
void EndTask(HANDLE hTaskID);
HANDLE CreateAnEvent(char* EventName);
long PriorityOnMutex(HANDLE hHandle, HANDLE hMutex, DWORD tOut);
void PriorityOnRelease(HANDLE hHandle, HANDLE hMutex);
void Thread_Join(HANDLE hTaskIDx);
HANDLE RunThisThread(DWORD vID);
int IntPipe_Write( PIPE *pip_str,int data);
int IntPipe_Read(PIPE *pip_str, int *home);
DWORD WaitForEvent(HANDLE hEvent,int Timedelay);


