#include <iostream>
#include <windows.h>
#include <vector>
#include <time.h>
using namespace std;
int global=0;
void contar ();
vector <HANDLE>threads;
HANDLE thread;
HANDLE mutex1= CreateMutex(NULL, FALSE, NULL);
int main()
{
    int t=4;
    clock_t tempo=clock();
    for(int i=0;i<t;i++)
    {
        thread=CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)contar,NULL,CREATE_SUSPENDED,NULL);
        threads.push_back(thread);
        SetThreadPriority(threads[i],THREAD_PRIORITY_TIME_CRITICAL);
        //THREAD_PRIORITY_TIME_CRITICAL
        //THREAD_PRIORITY_NORMAL
        ResumeThread(threads[i]);
    }
    for(int i=0;i<t;i++)
    WaitForSingleObject(threads[i],INFINITE);
    cout<<clock()-tempo<<endl;
    cout<<global;
    return 0;
}
void contar ()
{
    int j=0;
    for(int i=0;i<100000000;i++)
    {    
        j+=1; 
         
    }
      WaitForSingleObject(mutex1,INFINITE); 
      global+=j;
      ReleaseMutex(mutex1);
    
}
