//	The following function returns a pointer to the 
//created datapool. The type of pointer is void
//	which means it can point to anything, this means you 
//will have to cast this to your own type of pointer
//	depending upon the data you have put in the datapool
// bState = NOTSIGNALLED means not signalled
// caller will be suspended if resource if already in use.
//	The following function returns a pointer to the 
//created datapool. The type of pointer is void
//	which means it can point to anything, this means you 
//will have to cast this to your own type of pointer
//	depending upon the data you have put in the datapool
// bState = NOTSIGNALLED means not signalled
// caller will be suspended if resource if already in use.
// bState = NOTSIGNALLED means not signalled
//	The following function returns a pointer to the 
//created datapool. The type of pointer is void
//	which means it can point to anything, this means you 
//will have to cast this to your own type of pointer
//	depending upon the data you have put in the datapool
// bState = NOTSIGNALLED means not signalled
// caller will be suspended if resource if already in use.
//	The following function returns a pointer to the 
//created datapool. The type of pointer is void
//	which means it can point to anything, this means you 
//will have to cast this to your own type of pointer
//	depending upon the data you have put in the datapool
// caller will be suspended if resource if already in use.
// bState = NOTSIGNALLED means not signalled
#ifndef	__RT__
#define __RT__

#include <process.h>	// for spawnl and createthread
#include <windows.h>	// for perror and sleep
#include <stdio.h>		// for printf
#include <limits.h>		// for UINT_MAX
#include <conio.h>		// for _kbhit(), getch() and getche()
#include <iostream>
#include <string>

using namespace std ;


//	the following describes all the global variables used by this thread
//	However, we cannot simply use global variables, because if we created two threads based
//	around this source file, then they would share the same application wide variables, i.e. the
//	two threads would interact adversely by sharing the same variables
//
//	To overcome this MFC provides the concept of thread local storage of TLS (see help) 
//  thus we define 'Thread' as a type specific modifier

#define PerThreadStorage  __declspec(thread)
#define _CRT_SECURE_NO_WARNINGS	


#define SUSPENDED			101000		// for threads
#define ACTIVE				101001		// for thread
#define OWN_WINDOW			101002		// for FORK_PROCESS
#define PARENT_WINDOW		101003		// ditto
#define MAXMESSAGEVALUE		32000		// maximum value of a user generated message

#define MANUAL				101004		// for events
#define AUTORESET			101005		// for events

#define SINGLE_RELEASE		101100		// for events
#define MULTIPLE_RELEASE	101101		// for events

#define SIGNALLED			101006		// for events
#define NOTSIGNALLED		101007		// for events

#define OWNED				101008		// for mutex's
#define NOTOWNED			101009		// for mutex's

#define ECHO_ON()		/* no definition for OS9 compatibility */
#define ECHO_OFF()		/* no definition for OS9 compatibility */


//
//	The followng #ifdef...#endif section is needed if you are doing work on Borland
//	C++. The reason is that some of the #define constant names used in MS.Visual C++
//	differ from those in Borland C++ in that the former usually has an '_' character
//	preceeding the name (there are not many of these cases, so it's not a big problem)
//
//	Now because Borland compilers automatically define the symbol __BORLANDC__ at 
//	compile time and Microsoft ones don't, we can see if the symbol __BORLANDC__ is defined
//	and if so, redefine the conflicting constant names written originally to be
//	microsoft compatible to be Borland compatible. If you find similar discreprences
//	you can add the changes to the section below

#ifdef __BORLANDC__						// if the compiler is Borand C++
	#define _P_NOWAIT		P_NOWAIT	// For Borland Builder Compatibility
	#define	_WAIT_CHILD		WAIT_CHILD
#endif									// end Borland C++ section

// data structure related to datapools

//##ModelId=3DE6123902E9
typedef struct {
	HANDLE DataPoolHandle ;		// handle to datapool
	void *DataPoolPointer ;		// pointer that can be used to access datapool
}DATAPOOLINFO ;



//	Miscellaneous functions
void	SLEEP(UINT	Time);			// suspend current thread for 'Time' mSec
BOOL	TEST_FOR_KEYBOARD();		// tests a keyboard for a key press returns true if key pressed
HANDLE	GET_STDIN();				// get handle to standard input device (keyboard)
HANDLE	GET_STDOUT();				// ditto output device
HANDLE	GET_STDERR();				// ditto erro device
UINT	WAIT_FOR_CONSOLE_INPUT(HANDLE hEvent, DWORD Time = INFINITE);	//wait for console input to happen


void	MOVE_CURSOR(int x, int y) ;	// move console cursor to x,y coord
void	CURSOR_ON() ;				// turn flashing cursor on (the default)
void	CURSOR_OFF() ;				// turn flashing cursor off
void	REVERSE_ON() ;				// turn on inverse video
void	REVERSE_OFF() ;				// turn off inverse video
void	CLEAR_SCREEN() ;			// clears the screen

void PERR(bool bSuccess, string ErrorMessageString) ;


UINT	WAIT_FOR_MULTIPLE_OBJECTS(	UINT nCount,             // number of handles in the handle array
									CONST HANDLE *lpHandles , // pointer to the object-handle array
									DWORD Time = INFINITE
								);


////////////////////////////////////////////////////////////////////////////////////////
//	For those programmers that wish to use a more C++ approach, encapsualtion and methods etc
//	you can use the following Classes
////////////////////////////////////////////////////////////////////////////////////////

//##ModelId=3DE612390389
class CProcess {					// see Process related functions in rt.cpp for more details
private:
	PROCESS_INFORMATION pInfo ;
	const string ProcessName ;		// name of process

public:
	// constructor to create a child process, takes four 
	//arguments, note that the last 3 make use
	//	of default argument, that is, if you do not supply 
	//real arguments in the call, then the default values will
	//	be used for the ones you do NOT supply
	//	this means that when you create a new CProcess object, 
	//the only argument you HAVE to supply is the Name of the 
	//executable
	//	all other values can be supplied by default

	CProcess(const string &Name,							// pathlist/name of executable program (.exe) that will be the child process
			int Priority = NORMAL_PRIORITY_CLASS ,		// Priority of the process
														//	The allowed priorities are:	
														//	HIGH_PRIORITY_CLASS			Use for important time critical tasks
														//	IDLE_PRIORITY_CLASS			Use for processes that run when all other processes are idle
														//	NORMAL_PRIORITY_CLASS		Normal processing shared with other normal processes
														//	REALTIME_PRIORITY_CLASS		highest priority blocks out all other processes including O.System so take care

			BOOL bUseNewWindow = PARENT_WINDOW,			// use OWN_WINDOW to make new process use its own windows
														// use PARENT_WINDOW to make it share its parent window
		
			BOOL bCreateSuspended = ACTIVE,				// use SUSPENDED to make new child processes main thread suspended when process is created
														// use ACTIVE to make new child processes main thread active when process iscreated
			const string &ChildProcessArgString = ""
		) ;

	virtual ~CProcess() {}				// destructor (does nothing by default)
	inline operator string	() const {return ProcessName ;}
	inline string	GetName() const { return ProcessName ; }	
	
	inline HANDLE	GetProcessHandle() const { return pInfo.hProcess ; }		// get the process's handle
	inline HANDLE	GetThreadHandle() const { return pInfo.hThread ; }		// get the process's main thread handle
	inline UINT		GetProcessId() const { return pInfo.dwProcessId ; }		// get the process's ID
	inline UINT		GetThreadId() const { return pInfo.dwThreadId; }		// get the main thread's ID

	inline operator PROCESS_INFORMATION() const { return pInfo; }	// get general info about process

	BOOL Suspend() const ;										// called to suspend the child process i.e. pause it
	BOOL Resume() const ;										// allows child to resume processing
	BOOL WaitForProcess(DWORD Time=INFINITE) const ;			// allows parent to wait for child child process to finish
	BOOL SetPriority(int Priority) const ;						// changes the priority of the child process (see allowed process values in constructor)
	BOOL Post(UINT Message) const ;							// allows a signal/message to be sent to the process

private:
	void Exit(UINT ExitCode) const ;		// called by process to terminate itself
	inline BOOL TerminateProcess(UINT ExitStatus = 0) { return ::TerminateProcess(GetProcessHandle(), ExitStatus) ; }
};

	
// Example use of CProcess
/*

#include <stdio.h>
#include "rt.h"

#define STOP	100		// signal code value for Signal() function

int main()
{
	printf("Creating Process Paul1.....\n") ;

	CProcess p1("c:\\users\\paul\\parent\\debug\\paul1 arg1 arg2 arg3",		// pathlist to '.exe.' file we wish to set running
				NORMAL_PRIORITY_CLASS,							// priority
				OWN_WINDOW,										// process uses own window, not this program window
				ACTIVE											// create process in active
	) ;
	

	if(p1.Suspend() == FALSE)	{
		  perror("Cannot Suspend Process") ;
		  exit(0) ;
	}
	
	if(p1.Resume() == FALSE)	{	
		  perror("Cannot Resume Process") ;
		  exit(0) ;
	}


  	if(p1.SetPriority(NORMAL_PRIORITY_CLASS)  == FALSE)	{
		  perror("Cannot Change Process Priority") ;
		  exit(0) ;
	}

  	if(p1.WaitForProcess() == WAIT_FAILED )	{	// check for error
		  perror("Cannot Wait For child Process") ;
		  exit(0) ;
	}

  	if(p1.Signal(STOP) == FALSE )	{	// check for error
		  perror("Cannot send Signal to child Process") ;
		  exit(0) ;
	}

  return 0 ;
}


/////////////////////////////the child process/////////////////////////////
// this one has been written to receive the args from the parent
// argc states the number of arguments passed to the child process by parent
// this is alway at least 1
// 
// all arguments are passed as strings within argv array
// argv[0] is always the pathlist to the program name
// argv[1] is the 1st argument
// argv[2] is the 2nd etc
// the progra below demonstrates the point
///////////////////////////////////////////////////////////////////////////

  int main(int argc, char *argv[] )
  {
	printf("number of arguments = %d\n", argc) ;

	for(int i = 0; i < argc; i ++)
		printf("argv[%d] = %s\n", i, argv[i]

	// rest of program

	...
	...


*/

//##ModelId=3DE6123A015A
class CThread {									// see Thread related functions in rt.cpp for more details
private:
	//##ModelId=3DE6123A0164
	HANDLE	ThreadHandle ;	// handle to thread
	//##ModelId=3DE6123A016E
	UINT	ThreadID ;		// id of thread

protected:
	//##ModelId=3DE6123A0178
	CThread(BOOL bCreateState=SUSPENDED) ;		// default constructor ONLY suitable when CThread is a base class 
												// for Active classes.

public:
	//##ModelId=3DE6123A0182
	CThread(UINT __stdcall Function( void *),	// name/pointer to function that is to be the new thread
			BOOL bCreateState=ACTIVE ,			// use SUSPENDED thread created in suspended state
												// use ACTIVE to make thread active
			void *ThreadArgs=NULL) ;			// any args to the thread


	//##ModelId=3DE6123A018C
	inline virtual ~CThread() { ::TerminateThread(ThreadHandle, 0); } 


	//##ModelId=3DE6123A018E
	inline HANDLE	GetThreadHandle() const { return ThreadHandle ; }
	//##ModelId=3DE6123A0197
	inline UINT	GetThreadID() const { return ThreadID ; }

	//##ModelId=3DE6123A01A0
	inline operator HANDLE() const { return ThreadHandle ; } 
	//##ModelId=3DE6123A01A2
	inline operator UINT()	const { return ThreadID ; }

	//##ModelId=3DE6123A01AA
	BOOL Suspend() const ;					// can be used to suspend the thread
	//##ModelId=3DE6123A01B4
	BOOL Resume() const ;					// can be used to activate a suspended thread
	//##ModelId=3DE6123A01B6
	UINT WaitForThread(DWORD Time=INFINITE) const ;			// caller waits for the thread to terminate
	//##ModelId=3DE6123A01BF
	BOOL SetPriority(UINT Priority) const ;	// caller sets thread priority, see SET_THREAD_PRORITY() in rt.cpp
	//##ModelId=3DE6123A01CA
	BOOL Post(UINT Message) const ;		// caller sends a signal/message to the thread

private:
	//##ModelId=3DE6123A01D4
	void Exit(UINT ExitCode=0) const ;		// called by thread to terminate itself

	inline BOOL TerminateThread(DWORD ExitStatus = 0) { return ::TerminateThread(ThreadHandle, ExitStatus) ; }
} ;

/*
////////////////////////////////////////////////////////////////////////////////////////////
//	Example use of CThread etc.
////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "rt.h"

 #define STOP	100		// signal code value for Signal() function

UINT __stdcall ThreadFn1(void *args)	// args points to any data intended for the child thread
{										// this is given to the thread by the parent thread during the call
										// to Create_Thread (see below)
										// code for this thread to perform
	for(int i = 0; i < 100; i ++)
		printf("Hello from child thread.....\n") ;

	return 0 ;							// terminate thread
}

int main()
{
	printf("Parent Thread: Creating Child Thread 1 in Active State\n") ;

	CThread	c1(ThreadFn1,				// name of thread function
			   ACTIVE,					// thread is created in ACTIVE state
			   NULL						// no arguments to the thread function
	) ;

	if(c1.Suspend() == FALSE)	{	
		  perror("Cannot Suspend Thread") ;
		  exit(0) ;
	}
		
	if(c1.Resume() == FALSE)	{	
		  perror("Cannot Resume Thread") ;
		  exit(0) ;
	}
	
  	if(c1.SetPriority(THREAD_PRIORITY_NORMAL)  == FALSE)	{
		  perror("Cannot Change Thread Priority") ;
		  exit(0) ;
	}
	
 	if(c1.Post(STOP)  == FALSE)	{
		  perror("Cannot Post thread") ;
		  exit(0) ;
	}
	
	if(c1.WaitForThread() == WAIT_FAILED)	{	// if thread already dead, then proceed, otherwise wait for it to finish
		perror("Cannot Wait for Thread") ;
		exit(0) ;
	}
	
	return 0 ;
}

////////////////////////////////////////////////////////////////////////////////////////////
//	Another Example use of CThread etc.
//
//	let's suppose the child thread needs to be told something
//	like the bit number of a port to read in range 0-7
//	and let's suppose we need 8 threads, 1 for each port
/////////////////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include "rt.h"

int PortNum[8] = {0,1,2,3,4,5,6,7} ;	

//
//	This is the child thread code. We are going to make 8 concurrent copies of this thread
//	by creating 8 CThread objects that use this function.
//	However, we want each thread to something a little different, so when we create the thread
//	we have to pass it info in the form of arguments to give the thread and indication of what
//	it is to that is different.
//
//	All thread declare a void pointer to the data/args we pass to them
//	therefore we must cast the pointer to be a pointer to the actual data, in this case
//	an int, so that we can extract the data and let the thread process it
//

UINT __stdcall ThreadFn1(void *args)	
{	
	int Port = *(int *)(args); 				
//					^----------------------- cast args from a void pointer to an int pointer
//			   ^---------------------------- get the int port number
  
	printf("Hello from Thread number [%d]:. I am monitoring Bit [%d]\n", Port, Port) ;

	return 0 ;
}

int main()
{
	CThread *Threads[8] ;	// Create pointers to the new thread
	int i ;

//
//	Now here is the clever bit with threads. Let's create 8 instances of the above thread
//	and let each thread know which port to monitor
//

	for(i = 0; i < 8; i ++) {
		printf("Parent Thread: Creating Child Thread 1 in Active State\n") ;
		if((Threads[i] = new CThread(ThreadFn1, ACTIVE, &PortNum[i])) == NULL) 	{	// create thread in activemode passing it address of 's'
			perror("Cannot Create Thread") ;
			exit(0) ;
		}
	}
	
  
	SLEEP(2000) ;	  

	for(i = 0; i < 8; i ++) {
		printf("Parent Thread: Waiting for Thread %d\n", i) ;
		if(Threads[i]->WaitForThread()  == WAIT_FAILED)	{	// if thread already dead, then proceed, otherwise wait for it to finish
			perror("Cannot Wait for Thread") ;
			exit(0) ;
		}
	}
	getch() ;		// pause until return pressed
	return 0 ;
}

*/
  
// this is a base class for deriving classes that will eventually give rise to objects with their own thread
//	of parallel processing. Just override the virtual function main() and put the code/statemachine for your active object in here just like a C program
//
//	If necessary, create a constructor for your derived class and call the base class constructor to control the active/suspended state of your thread
//	then control the threads activity via the CThread member functions

//##ModelId=3DE6123A0218
class ActiveClass : public CThread  
{	// see Thread related functions in rt.cpp for more details
private:
	BOOL	TerminateFlag ;		// can be polled by class thread to see if parent wants it to terminate

public:
	void	RequestTerminate() { TerminateFlag = TRUE ;}		// set a flag requesting the active class to terminate
	BOOL	TerminateStatus() const { return TerminateFlag ; }	// can be called by active class to see if it should terminate

	//##ModelId=3DE6123A0223
	ActiveClass() ;						// default constructor creates class in suspended state, i.e. not running.
	// this is important as we want to allow the derived 
	//classes constructor i.e. the one
	// written by the user, to complete its constructor 
	//before the class main() is run
	// otherwise the class might run with unitialised 
	//variables/state
	// This means that the user has to call Resume() for the 
	//class object to allow it to run.
	//##ModelId=3DE6123A022C
	virtual ~ActiveClass() ;

	//##ModelId=3DE6123A022E
	virtual int main() = 0;  			// pure virtual, makes this an abstract class therefore it must be overridden in your derived class
} ;
	
//
//	Template class for threads running within a class
//	by default threads created with this code run in either ACTIVE or SUSPENDED state depending up the argument bState
//	The default is to alow them to run, since the containing class will have been constructed and initialised and therefore it is
//	not unsafe. However you can control this if req'd
//

//##ModelId=3DE6123A027C
template <class ClassName>
class ClassThread : 
public ActiveClass  
{
private:
	//##ModelId=3DE6123A0287
	void *ThreadData ;
	
	//##ModelId=3DE6123A0290
	ClassName *PointerToClassInstanceOwningThread ;							// a pointer to the object which is creating the thread
	//##ModelId=3DE6123A029A
	int (ClassName::*PointerToClassFunctionActingAsTheThread)(void *ThreadData) ;		// a pointer to the function acting as the thread. Must belong to the same class as the object pointer above

public:
	// constructor

	//##ModelId=3DE6123A02A4
	ClassThread(ClassName *_PointerToClassInstanceOwningThread,		// pointer to the threads enacpsulating object, usually 'this'
				int (ClassName::*_PointerToClassFunctionActingAsTheThread)(void *ThreadData),	// pointer to 'int function(void *)' function in class acting as the threafd
				int bState,				// run state
				void *_ThreadData		// pointer to thread data
	
	)
		:	PointerToClassInstanceOwningThread(_PointerToClassInstanceOwningThread), 
			PointerToClassFunctionActingAsTheThread(_PointerToClassFunctionActingAsTheThread), 
			ThreadData(_ThreadData),
			ActiveClass()					// state of class thread will be suspened by deault
	{
		if(bState == ACTIVE)
			Resume() ;			// allow class threads to resume if the programmer asked for it, otherwise they control it within main program rather than constructor
	}	

	//##ModelId=3DE6123A02AF
	virtual int main()		// override from ActiveClass and CThread, thus will run when the Resume() function in constructor above is run or later in users program
	{
		return (PointerToClassInstanceOwningThread->*PointerToClassFunctionActingAsTheThread)(ThreadData) ;	// invoke function in dispatcher as an active thread
	}
} ;

/*
//
//	This is the simplest kind of active class. Just create a class derived from CThread and put an int main() function in the public section
//	When an instance of the class is created, it will immediately run the main() function.
//	Note a constructor for your class is optional, put one in if you need to initialise the class in any way before the thread runs
//

#include "rt.h"

class MyClass : public ActiveClass  {
public:
	// constructor/destructor if required

	MyClass() : x(1000) {}	// constructor to initialise 'x'

private: 
	// class variables if required
	int x ;


	// 2nd thread within the class this ios prviate as we do not want outside users invoking this thread as a function

	int function1(void *Loopcount)
	{
		int finalval = *(int *)(Loopcount) ;	// get at integer data passed to thread via the void pointer

		for(int k = 0; k < finalval; k ++)
			cout << " k = " << k << endl ;

		return 0 ;
	}

	//	Start of threads program, think of this like main() in C where a program begins. Ditto it needs to be private

	int main() 
	{
		// create instance of an active thread, and pass address of 'x' to thread

		ClassThread<MyClass> MyThread(this, function1, ACTIVE, &x) ;

		for(int j = 0; j < 1000; j ++)
			cout << " j = " << j << endl ;

		MyThread.WaitForThread() ;			// wait for thread to terminate
	
		return 0 ;
	}

} ;

*/

//##ModelId=3DE6123A034E
class CMutex {												// see Mutex related functions in rt.cpp for more details
	//##ModelId=3DE6123A0358
	HANDLE	MutexHandle ;		// handle to the mutex
	//##ModelId=3DE6123A0363
	const string MutexName;
	
public:
	
	//##ModelId=3DE6123A036D
	UINT Wait(DWORD Time = INFINITE) const ;		// performs a wait on mutex for ever or until specified time
	//##ModelId=3DE6123A0377
	BOOL Signal() const ;	// performs a signal on mutex
	//##ModelId=3DE6123A0381
	BOOL Read() const ; 	// read mutex state i.e. signalled or not signalled
	//##ModelId=3DE6123A0383
	BOOL Unlink() const ;	// finished with mutex
	
	//##ModelId=3DE6123A038C
	inline HANDLE	GetHandle() const { return MutexHandle ; }			// return value of 'hMutex'
	//##ModelId=3DE6123A0395
	inline operator HANDLE	() const { return MutexHandle; }			// ditto

	inline operator string	() const {return MutexName ;}
	inline string	GetName() const { return MutexName ; }

	// constructs mutex with name and indicates whether 
	//object protected by mutex is owned by creator or not
	// Note use of default argument, i.e. if you do not 
	//specifiy a value for bOwned argument
	// then it will default to NOTOWNED

	//##ModelId=3DE6123A0397
	CMutex(const string &Name, BOOL bOwned = NOTOWNED) ;	
	//##ModelId=3DE6123A03A9
	inline virtual ~CMutex() { Unlink() ; 	}			// destructor unlinks mutex
} ;



/********************************************************************************
Example	use of a CMutex Object

#include <stdio.h>		
#include "rt.h"

int x = 0 ;

// Now a child thread to update x

UINT __stdcall Thread1(void *args)
{
	int i ;

	CMutex m1("MyMutex") ;		// child thread attempts to create and/or line
								// to new/existing mutex

	printf("Child Thread running.....\n") ;

	for(i=0; i < 500000; i ++) {
		m1.Wait();			// gain access to the resource via the critical section
		x = x + 1 ;			// safe to access non-sharable resource
		m1.Signal() ;		// release the resource
	}
	return 0;				// end of thread
}

//	The main thread in the process
	
int main()
{
	int i ;
	CMutex m1("MyMutex") ;		// Parent thread attempts to create and/or line
								// to new/existing mutex

	CThread	Child( Thread1 ) ;	// create another child thread
	printf("Parent Thread running.....\n") ;

	for(i=0; i < 500000; i ++) {
		m1.Wait();			// gain access to the resource via the critical section
		x = x + 1 ;			// safe to access non-sharable resource
		m1.Signal() ;		// release the resource
	}
	
	Child.WaitForThread() ;			// wait for child thread to finish

	printf("x = %d\n", x ) ;		// print out final value of x

	return 0 ;					// end of main thread and hence process
}
*/


//
//	This class allows single or multiple threads (depending upon the value of the argument 'bType' in the constructor)
//	to synchonise themselves with an Event.
//
//	First create the event in the signalled or non-signalled state (the default).
//	Processes/threads can then wait for the event in any order and any number of processes
//
//	A controlling process/thread can signal that the event has occurred, which will allow either ONE process to resume
//	from the list of processes waiting, or all waiting processes/threads to resume processing
//	Once the process(es)/thread(s) have been released, the event is automatically reset preventing others resuming until
//	the event is signalled again etc.
//

class CEvent				// based around Win32 event
{
protected:

	HANDLE	EventHandle ;			// handle to the event
	const	string EventName ;		// Name of the event

public:	
	

	inline HANDLE GetHandle() const { return EventHandle ; }				// get handle to event
	inline operator HANDLE	() const {return EventHandle ;}			// ditto
	inline operator string	() const {return EventName ;}
	inline string	GetName() const {return EventName ; }

	inline ~CEvent() { 	Unlink() ;	}
	
	CEvent(const string &Name, BOOL bType = MULTIPLE_RELEASE, BOOL bState = NOTSIGNALLED) ;	// btype = SINGLE_RELEASE or MULTIPLE_RELEASE to allow one or many thread to resume when event is signalled
																		// bState = SIGNALLED or NOTSIGNALLED to indicate the initial or creation state of the event

	BOOL Unlink() const;								// unlink from event, i.e. we have finished using it
	BOOL Signal() const ;								// signal event
	UINT Wait(DWORD Time = INFINITE) const; 			// perform a wait on an event for ever or until specified time

} ;

/*
//
//	Example Use of an Event
//

CEvent e1("Paul") ;		// a global event which when signalled will release all waiting threads

UINT __stdcall ThreadFn1(void *args)	// args points to any data intended for the child thread
{										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 100; i ++)	{
		e1.Wait() ;
		printf("Thread 1 got Event\n") ;
	}

	return 0 ;							// terminate thread
}

UINT __stdcall ThreadFn2(void *args)	// args points to any data intended for the child thread
{										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 100; i ++)	{
		e1.Wait() ;
		printf("Thread 2 got Event\n") ;
	}

	return 0 ;							// terminate thread
}

UINT __stdcall ThreadFn3(void *args)	// args points to any data intended for the child thread
{										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 100; i ++)	{
		e1.Wait() ;
		printf("Thread 3 got Event\n") ;
	}

	return 0 ;							// terminate thread
}

int main()
{
	CThread t1(ThreadFn1) ;
	CThread t2(ThreadFn2) ;
	CThread t3(ThreadFn3) ;


	printf("Hit Return to Trigger Event\n") ;

	for(int i = 0; i < 100; i ++)	{
		getchar() ;
		e1.Signal() ;
	}

	t1.WaitForThread() ;
	t2.WaitForThread() ;
	t3.WaitForThread() ;
	
	return 0 ;
}

*/

//
//	The Manual Reset Condition (2nd argument in constructor = MANUAL (the Default) 
//	class allows multiple threads to synchonise themselves with a condition.
//
//	First create the condition in the signalled or non-signalled state (the default).
//	Processes/threads can then wait for the condition to become true
//
//	A controlling process/thread can signal that the condition is TRUE, which will allow all processes/threads to resume processing
//
//	If this sounds similar to an event, then it is, with one important exception. Conditions must be reset to the FALSE state by the
//	program, they are not automatically reset as they are for an event. In essence then an event is like a pulse or trigger of infinately
//	short period whereby only those threads that are waiting for it when it occurs are released.
//
//	With a condition, it may remain true for a specified period of time, so that all threads waiting, and those that arrive 
//	latter, can be released, up to the point when it is reset back to false.
//
//
//	The Auto Reset Condition (2nd argument in condtructor is set to AUTORESET) 
//	class allows a single thread to synchonise itself to a condition.
//
//	First create the auto reset condition in the signalled or non-signalled state (the default).
//	Processes/threads can then wait for the condition to become true
//
//	A controlling process/thread can signal that the condition is TRUE, which will allow one processes/threads to resume processing
//	after which it is automatically reset. If non process/thread is waiting for when the condition is signalled then it remains
//	signalled/true until a thread waits, the thread will not be delayed and is allowed to resume immediately after which the auto reset
//	condition is reset back to false or non-signalled.
//
//	If this sounds similar to an single thread release event, then it is, with one important exception. An autorreset condition
//	will WAIT for a thread (if non are waiting) before resetting. This can be useful in avoiding race and timing problems when
//	synchronising
//	

class CCondition 						// create a condition class that releases all threads when signalled
{
protected:

	HANDLE	ConditionHandle ;			// handle to the Condition
	const string ConditionName ;		// Name of the Condition

public:	
	
	inline HANDLE GetHandle() const { return ConditionHandle ; }		// get handle to Condition
	inline operator HANDLE	() const {return ConditionHandle ;}			// ditto
	inline operator string	() const {return ConditionName ;}
	inline string	GetName() const {return ConditionName ; }
	inline ~CCondition() { 	Unlink() ; }

	//
	//	bType default to a Manual reset event, use AUTORESET if you want auto reseting after waiting for one thread
	//

	CCondition(const string &Name, BOOL bType = MANUAL , BOOL bState = NOTSIGNALLED) ;	
	BOOL Unlink() const ;								// unlink from Condition, i.e. we have finished using it

	// Signal() sets the Condition and will release ALL Waiting threads. It can be reset by calling Reset()

	BOOL Signal() const ;	
	UINT Wait(DWORD Time = INFINITE) const 	;		// perform a wait on a Condition for ever or until specified time
	BOOL Reset() const ;							// reset the condition back to false or not signalled
	BOOL Test() const ; 							// see if condition is signalled	
} ;

//
//	The Auto Reset Condition class allows a single thread to synchonise itself to a condition.
//
//	First create the auto reset condition in the signalled or non-signalled state (the default).
//	Processes/threads can then wait for the condition to become true
//
//	A controlling process/thread can signal that the condition is TRUE, which will allow one processes/threads to resume processing
//	after which it is automatically reset. If non process/thread is waiting for when the condition is signalled then it remains
//	signalled/true until a thread waits, the thread will not be delayed and is allowed to resume immediately after which the auto reset
//	condition is reset back to false or non-signalled.
//
//	If this sounds similar to an single thread release event, then it is, with one important exception. An autorreset condition
//	will WAIT for a thread (if non are waiting) before resetting. This can be useful in avoiding race and timing problems when
//	synchronising
//	

///////////////////////////////////////////////////////////////////////////////////////////
//	Example use of Conditions
///////////////////////////////////////////////////////////////////////////////////////////
/*
#include "C:\Users\PAUL\DTP - Word and PageMaker Files\Real Time Software Course Material\Win 32 C++ Files\rt.h"

CCondition e1("Paul") ;		// a global condition

UINT __stdcall ThreadFn1(void *args)	// args points to any data intended for the child thread
{										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 100; i ++)	{
		e1.Wait() ;
		printf("Thread 1 got Event\n") ;
	}

	return 0 ;							// terminate thread
}

UINT __stdcall ThreadFn2(void *args)	// args points to any data intended for the child thread
{										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 100; i ++)	{
		e1.Wait() ;
		printf("Thread 2 got Event\n") ;
	}

	return 0 ;							// terminate thread
}

UINT __stdcall ThreadFn3(void *args)	// args points to any data intended for the child thread
{										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 100; i ++)	{
		e1.Wait() ;
		printf("Thread 3 got Event\n") ;
	}

	return 0 ;							// terminate thread
}

int main()
{
	CThread t1(ThreadFn1) ;
	CThread t2(ThreadFn2) ;
	CThread t3(ThreadFn3) ;


	for(int i = 0; i < 100; i ++)	{
		getchar() ;
		e1.Signal() ;			// signal the condition (No reset here to demonstrate that they do not autoreset)
	}

	t1.WaitForThread() ;
	t2.WaitForThread() ;
	t3.WaitForThread() ;
	
	return 0 ;
}
*/

//
//	A critical section is an alternative method of implementing mutual exclusion
//	but ONLY between THREADS within the same PROCESS !!!.
//	Why bother ?, because they execute much faster than Mutex object although you could
//	still use a mutex if you want. Note the Criticl section object must be Global to be shared
//	between threads
//

//##ModelId=3DE6123B015B
class CriticalSection
{
private:
	//##ModelId=3DE6123B0165
	CRITICAL_SECTION cs ;

public:
	//##ModelId=3DE6123B016F
	CriticalSection() { InitializeCriticalSection( &cs) ; }		// constructor initialises the critical section
	//##ModelId=3DE6123B0170
	virtual ~CriticalSection() { DeleteCriticalSection( &cs ) ; }

	//##ModelId=3DE6123B017A
	void Enter() { EnterCriticalSection(&cs) ; }				// call this when a thread needs to enter a protected resource
	// caller will be suspended if resource if already in use.
	//##ModelId=3DE6123B017B
	void Leave() { LeaveCriticalSection(&cs) ; }				// call this when a thread leaves a protected reasource

//
//	This function is not supported under windows 95/98 but requires
//	NT 4 or later. It attempts to enter a critical section. If it is successful
//	and doesn't get blocked, it returns TRUE. If the critical section is in use
//	it returns FALSE and doesn't get blocked. It can try again later
//

//	BOOL TryEnter() { return TryEnterCriticalSection( &cs) ; } 
} ;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Example use of a critical section 
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
//
//	A GLOBAL critical section object MUST can be created that can be accessed by all threads in the process
//


#include <stdio.h>		
#include "rt.h"

CriticalSection cs ;	// global object created at start of process and destroyed at the end of the process

int x = 0 ;		// A simple non-sharable resource which we wish to protect with a 
				// critical section

// Now a child thread to update x

UINT __stdcall Thread1(void *args)
{
	int i ;

	printf("Child thread running.....\n") ;

	for(i=0; i < 5000000; i ++) {
		cs.Enter();			// gain access to the resource via the critical section
		x = x + 1 ;			// safe to access non-sharable resource
		cs.Leave() ;		// release the resource
	}
	return 0;				// end of thread
}

//	The main thread in the process
	
int main()
{
	int i ;
	CThread	Child( Thread1 ) ;		// create another child thread

	for(i=0; i < 5000000; i ++) {
		cs.Enter() ;		// gain access to the resource via the critical section
		x = x + 1 ;			// safe to access non-sharable resource
		cs.Leave() ;		// release the resource
	}
	
	Child.WaitForThread() ;			// wait for child thread to finish

	printf("x = %d\n", x ) ;		// print out final value of x

	return 0 ;					// end of main thread and hence process
}
*/

//##ModelId=3DE6123B0260
class CSemaphore {				// see Semaphore related functions in rt.cpp for more details
	//##ModelId=3DE6123B0261
	HANDLE	SemaphoreHandle ;		// handle to the semaphore
	//##ModelId=3DE6123B026B
	const string SemaphoreName ;
	
public:
	
	//##ModelId=3DE6123B0277
	UINT Wait(DWORD Time = INFINITE) const ;		// wait on the semaphore
	//##ModelId=3DE6123B027F
	BOOL Signal(int Increment=1) const ;	// signal the semaphore
	//##ModelId=3DE6123B0289
	UINT Read() const ;		// read semaphore value
	//##ModelId=3DE6123B0293
	BOOL Unlink() const ;
	//##ModelId=3DE6123B029C
	HANDLE	GetHandle() const { return SemaphoreHandle ; }					// get sempahore handle
	//##ModelId=3DE6123B029E
	operator HANDLE	() const { return SemaphoreHandle; } 

	inline operator string	() const {return SemaphoreName ;}
	inline string	GetName() const { return SemaphoreName ; }


	// construct semaphore by name with initial and max 
	//permissible values

	//##ModelId=3DE6123B02A6
	CSemaphore(const string &Name, int InitialVal, int MaxVal=1) ;
	//##ModelId=3DE6123B02B1
	virtual ~CSemaphore() { Unlink() ; }
};

/********************************************************************************
Example	use of a Semaphore


#include <stdio.h>		
#include "rt.h"

int main()
{
	CSemaphore s1("sem1", 1, 10 ) ;			// create a semaphore with value 1 and max value of 10

	if(s1.Signal() == FALSE)	{			// signal semaphore
		perror("Cannot Signal semaphore\n") ;
		exit(0) ;					// exit thread on error
	}
	
	if(s1.Wait() == WAIT_FAILED)	{			// wait for semaphore and check for error
		perror("Cannot Wait for semaphore\n") ;
		exit(0) ;					// exit thread on error
	}
    
 	return 0 ;
}

*/


//##ModelId=3DE6123B0350
class CMailbox								// see Message related functions in rt.cpp for more details
{
public:
	//##ModelId=3DE6123B035A
	CMailbox() ;
	//##ModelId=3DE6123B035B
	void WaitForMessage() const ;
	//##ModelId=3DE6123B0365
	BOOL TestForMessage(UINT MinMessageValue=0, UINT MaxMessageValue = MAXMESSAGEVALUE) const  ;
	//##ModelId=3DE6123B0370
	UINT GetMessage(UINT MinMessageValue=0, UINT MaxMessageValue = MAXMESSAGEVALUE) const ;

	//##ModelId=3DE6123B037B
	virtual ~CMailbox() {} 
} ;


/***************************************************************************************
**	An example program to demonstrate use of threads and message queues
****************************************************************************************/
/*
#include <stdio.h>
#include "rt.h"

UINT __stdcall ThreadFn1(void   *args)		// A child thread
{	 	
	UINT		Message ;			// A variable to hold a message read for message queue

	CMailbox 	MyMailBox ;		// Create a message queue object for this particular child thread
							// Note each thread can create its own unique message queue

	do {
		printf("Child Thread 1: Polling Message Queue\n") ;
		Sleep(500) ;	// sleep for 1/2 second

		if(MyMailBox.TestForMessage() == TRUE) {		// see if a message exists in the message queue
			Message = MyMailBox.GetMessage() ;		// if so get the message
			if(Message == 1000)	{			// decide what message means
				printf("Child Thread 1 received Message 1000.....\n") ;
			}
			else if(Message == 1001)	{		// if not 1000, is it 1001
				printf("Child Thread 1 received END Message.....\n") ;
				break ;							// break from loop
			}
		}
	}while(1) ;			
	return 0 ;					// terminate child thread
}

UINT __stdcall ThreadFn2(void   *args)		// A 2nd child thread
{	 	
	UINT		Message ;			// A variable to hold a message read for message queue

	CMailbox 	MyMailBox ;		// Create a message queue object for this particular child thread
	do {
		printf("Child Thread 2: Polling Message Queue\n") ;
		Sleep(500) ;						// sleep for 1/2 second

		if(MyMailBox.TestForMessage() == TRUE) {		// see if a message exists in the message queue
			Message = MyMailBox.GetMessage() ;		// if so get the message
			if(Message == 2000)	{			// decide what message means
				printf("Child Thread 2 received Message 2000.....\n") ;
			}
			else if(Message == 2001)	{		// if not 2000, is it 2001
				printf("Child Thread 2 received END Message.....\n") ;
				break ;							// break from loop
			}
		}
	}while(1) ;		
	return 0 ;					// terminate child thread
}

int main()
{
	UINT Message ;
	CMailbox 	MyMailBox ;		// create a message queue for the main process thread

	CThread	Child1(ThreadFn1, ACTIVE) ;		// Create 2 child threads in Active state
	CThread	Child2(ThreadFn2, ACTIVE) ;

	do {
		printf("Main Thread: Polling Message Queue\n") ;
		Sleep(500) ;						// sleep for 1/2 second

		if(MyMailBox.TestForMessage() == TRUE) {		// is there a message in main message queue
			Message = MyMailBox.GetMessage() ;		// if so, get the message
			if(Message == 1000)	 			// if message intended for thread1
				Child1.Post(Message) ;		// send the message to it
			if(Message == 2000)				// if message intended for thread2
				Child2.Post(Message) ;			// send the message to it
			if(Message == 3000)	{			// if message from another process 
				printf("Main Thread GOT Message 3000, Killing Child Threads.....\n") ;
				Child1.Post(1001) ;			// kill child by sending agreed END message
				Child2.Post(2001) ;			// kill child by sending agreed END message
				break ;					// end loop
			}
		}
		// if no message, see if keyboard character pressed
		if(TEST_FOR_KEYBOARD() != 0)	{
			if(getch() == 'x')	{	// if keyboard character is 'x'
				printf("Main Thread GOT X Key, Killing Child Threads.....\n") ;
				Child1.Post(1001) ;		// kill child by sending agreed END message
				Child2.Post(2001) ;			// kill child by sending agreed END message
				break ;						// end loop
			}
		}
	}while(1) ;	
		
	// wait for the child thread to die(if they haven't already)

	Child1.WaitForThread() ;
	Child2.WaitForThread() ;
	return 0 ;
}
*/


//
//	A Timer class. You should only create ONE of these within each unique thread, 
//	as they will all send the same timer messages to your message queue and thus there will be no 
//	way to distinguish between them if two or more are set running.
//
//	If you want to set up a timer to go off every say 20 mS and every 50 mS, then 
//	set up a single timer to go off at 10 mS (the lowest common denominator) and when the 
//	timer message is read appears in your message queue (every 10mS) you will have to
//	wait until 2 and 5 message respectively occur before carrying out the action
//	associated with that time interval
// 
//	NOTE: To use the timer a thread must have created a CMessageQueue object to receive
//	the messages sent by the timer.
//

//##ModelId=3DE6123C00A8
class	CTimer	{			// see Timer related functions in rt.cpp for more details
private:
	//##ModelId=3DE6123C00B2
	UINT TimerID ;			// internal ID for the timer

public:
	//##ModelId=3DE6123C00BC
	CTimer() ;				// Create the Timer but don't set it running, use Set latter to start timer

	// create a running timer
	//	If callbackfn is not supplied or is NULL, then timer 
	//posts messages to the threads message queue
	//	if callbackfn is supplied, then the callback function 
	//is invoked rather than a message being sent to the 
	//threads message queue
	//##ModelId=3DE6123C00BD
	CTimer(UINT Timer, TIMERPROC callbackfn=NULL) ;	

	//##ModelId=3DE6123C00C7
	virtual ~CTimer();				// destructor, kill timer if it is running

	//##ModelId=3DE6123C00D0
	BOOL StopTimer() ;					// stops a timer from posting messages to threads message queue
	
	//	Setup timer to go off at the specified rate in mSec
	//	If callbackfn is not supplied or is NULL, then timer 
	//posts messages to the threads message queue
	//	if callbackfn is supplied, then the callback function 
	//is invoked rather than a message being sent to the 
	//threads message queue

	//##ModelId=3DE6123C00D1
	UINT SetTimer(UINT Timer, TIMERPROC callbackfn=NULL) ;
	//##ModelId=3DE6123C00DB
	BOOL TestForTimer() ; 				// returns TRUE if timer gone off
	//##ModelId=3DE6123C00E4
	void WaitForTimer() ;				// waits for the timer to go off
} ;

/*

class CWaitableTimer {
private:
	HANDLE hTimer ;	

public:
	CWaitableTimer(const char *TimerName, BOOL type, int Interval) {
		hTimer = CreateWaitableTimer(NULL, TRUE, TimerName);
		PERR(htimer, "Error Cannot create Waitable Timer.....\n") ;
	}

//	void CancelTimer( CancelWaitableTimer(hTimer)) ;
//	virtual ~CWaitableTimer( CancelWaitableTimer(hTimer)) ;
};

*/
/*

///////////////////////////////////////////////////////////////////////////////////////////
//	Example use of Creating threads, using Messages  and Timers
////////////////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>		
#include "rt.h"				// change pathlist to rt.h to suit location on disk

#define END		1000
#define	JOB1	1001
#define JOB2	1002


UINT __stdcall ThreadFn1(void *args)	// The child thread
{		
	UINT	Message ;
	CTimer	t1(500) ;					// set up repetitive timer to go off at 1/2 second intervals

	CMailbox MyMailBox ;					// Create a message queue for this child thread

	do {
		if(MyMailBox.TestForMessage() == TRUE) {	// see if any messages arrived for this thread
			Message = MyMailBox.GetMessage() ;		// if so get the message
			if(Message == JOB1)	{			// look at message and carry our appropriate task
				// do job 1 code here
			}
		}
		if(t1.TestForTimer() == TRUE)		// if the threads timer has gone off, print message
			printf("ThreadFn1 Got Timer.....\n") ;

	}while(Message != END) ;		// terminate loop and thus thread when END message received
	
	t1.KillTimer() ;				// stop the child threads timer
		
	return 0 ;						// terminate child thread
}


UINT __stdcall ThreadFn2(void *args)	// The child thread
{		
	UINT	Message ;

	CMailbox MyMailBox ;					// Create a message queue for this child thread
	CTimer	t1(500) ;					// set up repetitive timer to go off at 1/2 second intervals

	do {
		if(mq1.TestForMessage() == TRUE) {	// see if any messages arrived for this thread
			Message = MyMailBox.GetMessage() ;	// if so get the message
			if(Message == JOB2)	{			// look at message and carry our appropriate task
				// do job 2 code here
			}
			// else-if etc for other message values sent to this thread
		}
		if(t1.TestForTimer() == TRUE)		// if this threads timer has gone off, print message
			printf("ThreadFn2 Got Timer.....\n") ;

	}while(Message != END) ;		// terminate loop and thus thread when END message received
	
	t1.KillTimer() ;						// stop the child threads timer
		
	return 0 ;						// terminate child thread
}

UINT __stdcall ThreadFn3(void *args)	// The child thread
{		
	CMailbox MyMailBox ;					// Create a message queue for this child thread
	CTimer	t1(500) ;					// set up repetitive timer to go off at 1/2 second intervals

	for(int i = 0; i < 5; i ++)	{
		t1.WaitForTimer() ;				// wait for the timer 5 times
		printf("ThreadFn3 Got Timer.....\n") ;
	}
	t1.KillTimer() ;					// stop the child threads timer
		
	return 0 ;							// terminate child thread
}

int main()
{
	UINT Message ;

	CMailbox MyMailBox ;					// create a message queue for the main process thread
	CTimer	t1(3000) ;						// create a repetitive 3 sec timer

	// Create a child thread

	printf("Parent Thread: Creating Child Thread 1 and 2 in Active State\n") ;

	CThread	Child1(ThreadFn1, ACTIVE) ;
	CThread	Child2(ThreadFn2, ACTIVE) ;
	CThread	Child3(ThreadFn3, ACTIVE) ;

	do {
		if(mainq.TestForMessage() == TRUE) {				// see if we have got any messages from other processes
			Message = MyMailBox.GetMessage() ;					// if so, read the message
			if(Message == JOB1 )	{					// if the message if for the child thread1
				Child1.Post(Message) ;				// send the message to it
			}
			if(Message == JOB2 )	{					// if the message if for the child thread2
				Child2.Post(Message) ;				// send the message to it
			}
			// etc for any other messages
		}
		if(t1.TestForTimer() == TRUE)	{				// if this main threads timer has gone off
			printf("main() Got Timer.....\n") ;
			Child1.Post(END) ;						// tell child thread to stop by sending END message
			Child2.Post(END) ;						// tell child thread to stop by sending END message
			break ;										// break from loop
		}
		// etc for any other messages
	}while(1) ;											// do forever
		
	t1.StopTimer() ;											// stop the main threads timer
	
	// wait for the child thread to die

	printf("Parent Thread: Waiting for Thread 1\n") ;

	if(Child1.WaitForThread()  == WAIT_FAILED)	{	// if thread already dead, then proceed, otherwise wait for it to finish
		perror("Cannot Wait for Thread1") ;
		exit(0) ;
	}

	printf("Parent Thread: Waiting for Thread 2\n") ;

	if(Child2.WaitForThread()  == WAIT_FAILED)	{	// if thread already dead, then proceed, otherwise wait for it to finish
		perror("Cannot Wait for Thread2") ;
		exit(0) ;
	}

	printf("Parent Thread: Waiting for Thread 3\n") ;

	if(Child3.WaitForThread()  == WAIT_FAILED)	{	// if thread already dead, then proceed, otherwise wait for it to finish
		perror("Cannot Wait for Thread3") ;
		exit(0) ;
	}

	return 0 ;
}
*/



//##ModelId=3DE6123C01AD
class CDataPool	{							// see Datapool related functions in rt.cpp for more details
	//##ModelId=3DE6123C01B8
	DATAPOOLINFO	DPInfo ;
	//##ModelId=3DE6123C01C2
	const string DataPoolName ;

public:
	//
	//	Constructor creates a named datapool object with a 
	//specified size
	//##ModelId=3DE6123C01CB
	CDataPool(const string &Name, UINT size) ;
	
	//	The following function returns a pointer to the 
	//created datapool. The type of pointer is void
	//	which means it can point to anything, this means you 
	//will have to cast this to your own type of pointer
	//	depending upon the data you have put in the datapool

	//##ModelId=3DE6123C01D7
	inline void *LinkDataPool() const { return DPInfo.DataPoolPointer ;}
	//##ModelId=3DE6123C01E0
	BOOL Unlink() const ;

	inline operator string	() const {return DataPoolName ;}
	inline string	GetName() const { return DataPoolName ; }


	//##ModelId=3DE6123C01E9
	inline virtual ~CDataPool() {Unlink(); }
} ;

/*
////////////////////////////////////////////////////////////////////////////////////////////////////
//	This example makes use of CDataPool objects and puts value into it
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Note that the best way to use Datapool in an object oriented environment is to
//	derive you own new classes from CDatapool and add your own mutator and accessor functions
//	to access and change the data inside
//

/*
#include <stdio.h>
#include "rt.h"

struct DATA {			// data for the datapool
		int floor;
		int direction ;
		int floors[10] ;
} ;

int main()
{
	CDataPool dp("Car1", sizeof(struct DATA)) ;		// make a named and sized datapool
	
  // now get a pointer to the datapool and cast it to a 'struct test' pointer
	
	struct DATA *MyDataPool = (struct DATA *)(dp.LinkDataPool()) ;


	// put data into the datapool

	MyDataPool->floor = 55 ;
	MyDataPool->direction = 11 ;

	// print out data in the datapool

	printf("Floor = %d\n", MyDataPool->floor);
	printf("Direction = %d\n", MyDataPool->direction) ; 

	// put more data in the data pool

	for(int i = 0; i < 10; i ++)
		MyDataPool->floors[i] = i ;

 // print out the data in the datapool

	for(i=0; i < 10; i ++)
		printf("%d ", MyDataPool->floors[i]) ;

	// Datapool object destroyed at this pointer then the O.System will remove it automatically.

	return 0 ;
}
*/


/**************************************************************************************************
* Alternative use of Datapool, which makes use of inheritance and creates true datapool objects
***************************************************************************************************

#include <stdio.h>
#include "rt.h"

// create a new CLiftData datapool class based on the CDataPool Class


class CLiftData : public CDataPool {
	private:
//
//	Description of the structure template representing the data to be store in the datapool
//
		struct DATA {			// data for the datapool
			int floor;			// the floor the lift is on
			int direction ;		// the direction of the lift
			int floors[10] ;	// an array of floor requests
		} ;						// declare a structure for the data in the data in the pool
	
//
//	pointer to the datapool
//

	struct DATA *MyDataPoolptr ;		

  public:			
	CLiftData(const string &Name) :						// the constructor for the new class
		CDataPool(Name, sizeof(struct DATA)) 
	{
		MyDataPoolptr = (struct DATA *)(LinkDataPool()) ;		// link to the datapool when it is created
	}

//
//	Now add some mutator and accessor functions to access the data in a controlled manner
//	via function calls

	void SetFloor(int floor) { MyDataPoolptr->floor = floor ; }
	void SetDirection(int direction) { MyDataPoolptr->direction = direction ; }
	void SetFloorNumber(int floornumber) { MyDataPoolptr->floors[floornumber] = 1 ; }
	void ClearFloorNumber(int floornumber) { MyDataPoolptr->floors[floornumber] = 0 ; }

	int GetFloor() const { return MyDataPoolptr->floor ; }
	int GetDirection()  const { return MyDataPoolptr->direction  ; }
	int GetFloorStatus(int floornumber)  const { return MyDataPoolptr->floors[floornumber] ; }

} ;


//
//	This is how we would use it
//

int main()
{
	int i ;

	CLiftData Lift("Car1") ;		// make a named CLiftData Object
	
	// put data into the datapool

	Lift.SetFloor(5) ;				// current floor number
	Lift.SetDirection(0) ;			// lift is going up
	for( i = 0; i < 1000; i ++)
		Lift.SetFloorNumber(i) ;		// set current floor request

	// print out data in the datapool

	printf("Floor = %d\n", Lift.GetFloor());
	printf("Direction = %d\n", Lift.GetDirection()) ;

	// CLiftData object destroyed at this pointer then the O.System will remove it automatically.

	return 0 ;
}
*/


class CRendezvous
{
	CDataPool	*RendezvousDataPool ;
	CEvent		*RendezvousEvent ;
	CMutex		*RendezvousMutex ;
	string		RendezvousName ;

	struct RendezvousData {
		int		NumberWaiting ;		// number of threads waiting to rendezvous
		int		NumThreads ;		// number of threads involved in rendezvous, set in the constructor
		char	Initialised[12] ;
	} *ptr ;

public:

	CRendezvous(const string &TheRendezvousName, int NumberThreads);
	~CRendezvous() ;
	void Wait() ;
	inline operator string	() const {return RendezvousName ;}
	inline string	GetName() const { return RendezvousName ; }
} ;

//
//	This version gives preference to readers rather than writers
//

class CReadersWritersMutex	
{
	string		Name ;

	CMutex		*ReadersWritersMutex ;
	CSemaphore	*ReadersWritersSemaphore ;
	CDataPool	*ReadersWritersDataPool ;

	struct Data { 
		int NumberOfReaders ;	
		char Initialised[12] ;
	} *ptr ;

public:

	CReadersWritersMutex(const string &MyName);
	~CReadersWritersMutex();

	void WaitToRead();	// called by a reader when they wish to access to the resource
	void DoneReading();	// called by a reader when they have finished with the resource
	void WaitToWrite(); 	// called by a writer when they wish to access to the resource
		
	void DoneWriting();	// called by a writer when they have finished with the resource

	inline operator string	() const {return Name ;}
	inline string	GetName() const { return Name ; }
};


/*

/////////////////////////////////////////////////////////////////////////////////////////
//	Example of readers/writers mutex problem
/////////////////////////////////////////////////////////////////////////////////////////

UINT __stdcall Reader(void *args)	// args points to any data intended for the child thread
{
	int x = *(int*)(args) ;			// get my thread number given to me by my parent (main() )

	CReadersWritersMutex r1("Paul") ;
										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 1000; i ++)	{
		r1.WaitToRead();
		printf("%d", x)  ;
		Sleep(1) ;
		r1.DoneReading() ;
	}

	return 0 ;							// terminate thread
}


UINT __stdcall Writer(void *args)	// args points to any data intended for the child thread
{
	int x = *(int*)(args) ;			// get my thread number given to me by my parent (main() )

	CReadersWritersMutex r1("Paul") ;
										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 1000; i ++)	{
		r1.WaitToWrite();
		printf("%d", x)  ;
		r1.DoneWriting() ;
	}

	return 0 ;							// terminate thread
}


/////////////////////////////////////////////////////////////////////////////////////////
//	Example of readers/writers mutex problem
/////////////////////////////////////////////////////////////////////////////////////////
/*

int main()
{
	int a[] = {0,1,2,3,4, 5, 6, 7, 8, 9} ;

	CThread t1(Reader, ACTIVE, &a[0]) ;
	CThread t2(Reader, ACTIVE, &a[1]) ;
	CThread t3(Reader, ACTIVE, &a[2]) ;
	CThread t4(Reader, ACTIVE, &a[3]) ;
	CThread t5(Reader, ACTIVE, &a[4]) ;

	CThread t6(Writer, ACTIVE, &a[5]) ;
	CThread t7(Writer, ACTIVE, &a[6]) ;
	CThread t8(Writer, ACTIVE, &a[7]) ;
	CThread t9(Writer, ACTIVE, &a[8]) ;
	CThread t10(Writer, ACTIVE, &a[9]) ;

	t1.WaitForThread() ;
	t2.WaitForThread() ;
	t3.WaitForThread() ;
	t4.WaitForThread() ;
	t5.WaitForThread() ;
	t6.WaitForThread() ;
	t7.WaitForThread() ;
	t8.WaitForThread() ;
	t9.WaitForThread() ;
	t10.WaitForThread() ;
	
	return 0 ;
}
*/

//
//	This version of the readers writers problems gives preference to writers rather than readers
//
class CWritersReadersMutex
{
	string		Name ;

	CMutex			*WritersReadersMutex ;
	CSemaphore		*WritersReadersSemaphore ;
	CDataPool		*WritersReadersDataPool ;
	CCondition		*WritersReadersCondition ;


	struct Data { 
		int NumberOfReaders, NumberOfWriters;
		char Initialised[12] ;
	} *ptr ;

public:

	CWritersReadersMutex(const string &MyName) ;
	~CWritersReadersMutex();
	void WaitToRead() ;
	void DoneReading() ;
	void WaitToWrite() ;
	void DoneWriting() ;
	
	inline operator string	() const {return Name ;}
	inline string	GetName() const { return Name ; }
};

/*

/////////////////////////////////////////////////////////////////////////////////////////
//	Example of writers readers mutex problem
/////////////////////////////////////////////////////////////////////////////////////////


UINT __stdcall Reader(void *args)		// args points to any data intended for the child thread
{
	int x = *(int*)(args) ;				// get my thread number given to me by my parent (main() )

	CWritersReadersMutex r1("Paul") ;
										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 1000; i ++)	{
		r1.WaitToRead();
		printf("%d", x)  ;
		Sleep(1) ;
		r1.DoneReading() ;
	}

	return 0 ;							// terminate thread
}


UINT __stdcall Writer(void *args)		// args points to any data intended for the child thread
{
	int x = *(int*)(args) ;				// get my thread number given to me by my parent (main() )
	CWritersReadersMutex r1("Paul") ;
										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 1000; i ++)	{
		r1.WaitToWrite();
		printf("%d", x)  ;
		r1.DoneWriting() ;
	}

	return 0 ;							// terminate thread
}



int main()
{
	CWritersReadersMutex r1("Paul") ;

	int a[] = {0,1,2,3,4, 5, 6, 7, 8, 9} ;

	CThread t1(Reader, ACTIVE, &a[0]) ;
	CThread t2(Reader, ACTIVE, &a[1]) ;
	CThread t3(Reader, ACTIVE, &a[2]) ;
	CThread t4(Reader, ACTIVE, &a[3]) ;
	CThread t5(Reader, ACTIVE, &a[4]) ;

	CThread t6(Writer, ACTIVE, &a[5]) ;
	CThread t7(Writer, ACTIVE, &a[6]) ;
	CThread t8(Writer, ACTIVE, &a[7]) ;
	CThread t9(Writer, ACTIVE, &a[8]) ;
	CThread t10(Writer, ACTIVE, &a[9]) ;

	t1.WaitForThread() ;
	t2.WaitForThread() ;
	t3.WaitForThread() ;
	t4.WaitForThread() ;
	t5.WaitForThread() ;
	t6.WaitForThread() ;
	t7.WaitForThread() ;
	t8.WaitForThread() ;
	t9.WaitForThread() ;
	t10.WaitForThread() ;
	
	return 0 ;
}
*/



class CSleepingBarbers	
{
	string Name ;

	CSemaphore	*Customers;		// number of customers waiting for hair cut (initially set to 0)
	CSemaphore	*Barbers ;		// number of barbers cutting hair (initially set to 0)
	CMutex		*Mutex ;		// a simple mutex
	CDataPool	*DataPool ;		// a pool to hold data shared by processes/threads using this resource

	struct Data { 
		int NumberOfWaitingCustomers ;	// number of waiting customers
		int NumberOfChairs ;			// number of chairs in the barber shop
		char Initialised[12] ;			// a string to indicate if this object has been initialised
	} *ptr ;

public:

	CSleepingBarbers(const string &_Name, int _NumberOfChairs) : Name(_Name) 
	{
		Mutex = new CMutex(string("__SleepingBarbersMutex__") + Name) ;
		Customers = new CSemaphore(string("__CustomersSemaphore__")+ Name, 0, _NumberOfChairs ) ;
		Barbers = new CSemaphore(string("__BarbersSemaphore__") + Name, 0, _NumberOfChairs ) ;
		DataPool = new CDataPool(string("__SleepingBarbersDataPool__") + Name, sizeof(struct Data)) ;
		ptr = (struct Data *)(DataPool->LinkDataPool()) ;

		if(strcmp(ptr->Initialised, "Initialised") != 0)	{
			strcpy_s(ptr->Initialised, "Initialised") ;
			ptr->NumberOfWaitingCustomers = 0 ;
			ptr->NumberOfChairs = _NumberOfChairs ;
		}
	}

	~CSleepingBarbers() 
	{
		delete Mutex ;
		delete Customers ;
		delete Barbers ;

		strcpy_s(ptr->Initialised, "") ;
		delete DataPool ;
	}

public:

	//
	//	Called by the Barber when he wants his next customer
	//

	void BarberWaitsForCustomer()
	{
		Customers->Wait() ;						// initially this is set to zero so a barber will wait unless there are customers sat in chairs waiting
		Mutex->Wait() ;							// gain exclusive access to data
		--(ptr->NumberOfWaitingCustomers) ;		// decrement the number of waiting customer
		Barbers->Signal() ;						// release access to data
		Mutex->Signal() ;							
	}

	//
	//	Called by customer when they want a hair cur
	//	returns TRUE is customer is sat in the queue
	//	or FALSE if they left in disgust at the length of the queue
	//
	BOOL CustomerWaitsForBarber()
	{
		Mutex->Wait() ;
		if((ptr->NumberOfWaitingCustomers) < (ptr->NumberOfChairs)) 
		{
			++(ptr->NumberOfWaitingCustomers) ;
			Customers->Signal() ;
			Mutex->Signal() ;
			Barbers->Wait() ;
			return TRUE ;		// will get haircut
		}
		else	{
			Mutex->Signal() ;
			return FALSE ;		// left in disgust
		}
	}

	inline operator string	() const {return Name ;}
	inline string	GetName() const { return Name ; }

} ;

/*
///////////////////////////////////////////////////////////////////////////////
//	Example of the Sleeping Barbers Problem
///////////////////////////////////////////////////////////////////////////////

#include "U:\rt.h"

CSleepingBarbers b1("JimsBarberShop", 2) ;		// two chairs in barber shop waiting room, minimum is 1 otherwise everyone leaves in disgust

//
// This thread represent a customer
//

UINT __stdcall Customer(void *args)	
{
	for(int i = 0; i < 10; i ++)			// get 10 haircuts over a suitable long time period
	{
		printf("Customer Arriving.....\n") ;

		if(b1.CustomerWaitsForBarber() == FALSE)	{		// if FALSE returned then I left in disgust
			TEXT_COLOUR(12, 15) ;
			printf("Not Enough Chairs....Left in Disgust\n") ;
			TEXT_COLOUR(15, 0) ;
		}

		SLEEP(30000) ;						// allow hair to grow again (30 Seconds) !!!!
	}

	return 0 ;								// terminate thread
}

//
// This thread represent a Barber
//

UINT __stdcall Barber(void *args)	
{
	while(1)	{
		b1.BarberWaitsForCustomer() ;		// wait for my next customer
		printf(".....Serving Customer.....\n") ;
		SLEEP(1500) ;						// simulate time for cutting hair = 1.5mSec
	}

	return 0 ;							// terminate thread
}

int main()
{
	CThread t10(Barber) ;		// create 1 barber (create more if necessary)
//	CThread t11(Barber) ;		// create another barber (create more if necessary)

	//	create 9 customers with suitable delays in between to simulate the random arrival of customers
	// 1st three customer arrive as soon as the barbers shop opens

	CThread t1(Customer) ;
	CThread t2(Customer) ;
	CThread t3(Customer) ;

	// now create two new customer arriving 1 second later
	SLEEP(1000) ;
	CThread t4(Customer) ;
	CThread t5(Customer) ;

	// then a rush of 4 customer after work closes
	SLEEP(1000) ;
	CThread t6(Customer) ;
	CThread t7(Customer) ;
	CThread t8(Customer) ;
	CThread t9(Customer) ;



	t1.WaitForThread() ;
	t2.WaitForThread() ;
	t3.WaitForThread() ;
	t4.WaitForThread() ;
	t5.WaitForThread() ;
	t6.WaitForThread() ;
	t7.WaitForThread() ;
	t8.WaitForThread() ;
	t9.WaitForThread() ;
	t10.WaitForThread() ;
	
	return 0 ;
}
*/

class CDinningPhilosophers	{
private:

	const		int	NUMBER_OF_PHILOSOPHERS ;
	string		Name ;
	CMutex		*Mutex ;
	CDataPool	*DataPool ;
	CEvent		*Event[5] ;				// array of event pointers, 1 per philosopher


	struct Data {
		int State[5] ;					// one state per philosopher
		char Initialised[12] ;			// to hold an initialised string
	} *ptr ;

	enum PhilosopherStates {THINKING, HUNGRY, EATING } ;	// set of integer constants representing states of philosophers, 
															// each must be in one of these states at any point in time

public:

	CDinningPhilosophers(const string _Name) : Name(_Name), NUMBER_OF_PHILOSOPHERS(5) 
	{
		Mutex = new CMutex(string("__DinningPhilosophersMutex__") + _Name) ;
		DataPool = new CDataPool(string("__DinningPhilosophersDataPool__") + _Name, sizeof(struct Data)) ;
		ptr = (struct Data *)(DataPool->LinkDataPool()) ;

		char buff[10] ;
		for(int i = 0; i < NUMBER_OF_PHILOSOPHERS; i ++)
		{
			sprintf_s(buff, "%d", i) ;				// write 'i' as a string for next statement
			Event[i] = new CEvent(string("__DinningPhilosophersSemaphore__") + _Name + string(buff)) ;
		}

		if(strcmp(ptr->Initialised, "Initialised") != 0)
		{
			strcpy_s(ptr->Initialised, "Initialised") ;
			for(int j = 0; j < NUMBER_OF_PHILOSOPHERS; j ++)
				ptr->State[j] = THINKING ;		// all philosophers atrt off in the thinking state
		}
	}

	~CDinningPhilosophers()
	{
		strcpy_s(ptr->Initialised, "") ;
		delete DataPool;
		delete Mutex ;
		for(int i = 0; i < NUMBER_OF_PHILOSOPHERS; i ++)
			delete Event[i] ;
	}
	

	void Eat(int i)
	{
		Mutex->Wait() ;
		ptr->State[i] = HUNGRY ;	//set my state to hungry
		Test(i) ;
		Mutex->Signal() ;
		Event[i]->Signal() ;
	}

	void DoneEating(int i)
	{
		Mutex->Wait() ;
		ptr->State[i] = THINKING ;
		Test(LeftPhilosopher(i)) ;		// can the left philosopher use my newly put down chopstick
		Test(RightPhilosopher(i)) ;		// can the right philosopher use my newly put down chopstick
		Mutex->Signal() ;
	}


private:
	void Test(int i )
	{
		// If I am hungry and neither of my neighbours is eating, then I can eat
		if(ptr->State[i] == HUNGRY && ptr->State[LeftPhilosopher(i)] != EATING && ptr->State[RightPhilosopher(i)] != EATING)
		{
			ptr->State[i] = EATING ;
			Event[i]->Signal() ;		
		}
	}

	int LeftPhilosopher(int i)
	{
		return ((i + NUMBER_OF_PHILOSOPHERS -1) % NUMBER_OF_PHILOSOPHERS) ;
	}

	int RightPhilosopher(int i)
	{
		return ((i + 1) % NUMBER_OF_PHILOSOPHERS) ;
	}

	inline operator string	() const {return Name ;}
	inline string	GetName() const { return Name ; }

} ;

/*
////////////////////////////////////////////////////////////////////////////////////////////////
//	Example Use of the Dinning Philosophers problem
////////////////////////////////////////////////////////////////////////////////////////////////

// Some threads


UINT __stdcall Philosopher(void *args)	// args points to any data intended for the child thread
{
	int MyPhilosopherNumber = *(int *)(args) ;	// get my number given to me by parent (main())

	CDinningPhilosophers dp1("Paul") ;
										// this is given to the thread by the parent thread during the call
	for(int i = 0; i < 20; i ++)	{
		printf("p%d Thinking\n", MyPhilosopherNumber) ;
		SLEEP(100 * (1 + MyPhilosopherNumber)) ;
		dp1.Eat(MyPhilosopherNumber) ;
		printf("p%d Eating\n", MyPhilosopherNumber) ;
		SLEEP(250 * (1 + MyPhilosopherNumber)) ;
		dp1.DoneEating(MyPhilosopherNumber) ;
	}

	return 0 ;							// terminate thread
}

int main()
{
	int array[] = {0,1,2,3,4} ;

	// create five philosophers
	CThread t1(Philosopher, ACTIVE, &array[0]) ;
	CThread t2(Philosopher, ACTIVE, &array[1]) ;
	CThread t3(Philosopher, ACTIVE, &array[2]) ;
	CThread t4(Philosopher, ACTIVE, &array[3]) ;
	CThread t5(Philosopher, ACTIVE, &array[4]) ;


	t1.WaitForThread() ;
	t2.WaitForThread() ;
	t3.WaitForThread() ;
	t4.WaitForThread() ;
	t5.WaitForThread() ;
	
	return 0 ;
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	The following is a C++ class to represent a non-typed pipeline object
//	that is, it will take any data and write it to a pipe. Reader and writer have to take care to make sure
//	objects or data read and written are compatible at each end or else there will be MAJOR problems, e.g one program
//	could write an 'int' while the other could read a 'char' for example since there is no type checking.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//##ModelId=3DE6123C0351
class CPipe {						// see Pipe related function in rt.cpp for more details
private:

// a structure and typedef for the pipelines

	typedef struct PipeContents {
		UINT	NumBytes ;				// number of bytes in the pipeline waiting to be read
		UINT	SizeOfPipe ;
		UINT	ReadingIndex ;		// index in the data array that marks the index of the next char to be read
		UINT	WritingIndex ;		// index into data array that marks the index of the next char to be written
		BOOL	Initialised ;		// indicates whether data structure has been initialised or not.
	} PIPECONTROL ;

	//##ModelId=3DE6123C0352
	HANDLE			hPipe ;				// pipeline simulated via datapools, this is the handle to the datapool
	HANDLE			hData ;				// ditto for the actual data

	//##ModelId=3DE6123C035C
	PIPECONTROL		*PipePointer ;		// pointer to start address of the pipeline structure (see above)
	BYTE			*DataPointer ;		// pointer to start address of the pipeline data (see above)

	//##ModelId=3DE6123C0370
	CMutex		*pMutex ;					// handle for the mutual exclusion semaphore in the pipeline
	//##ModelId=3DE6123C037A
	CSemaphore	*pProdSemaphore ;			// handle for the producer semaphore in the pipeline
	//##ModelId=3DE6123C038E
	CSemaphore	*pConSemaphore ;			// handle for the consumer semaphore in the pipeline

	//##ModelId=3DE6123C03A2
	const string PipeName ;

public:
	//##ModelId=3DE6123C03AB
	CPipe(const string &Name, UINT SizeOfPipe = 1024);			// default constructor, creates a named pipe of specified size, default is 1024 bytes
	
	//##ModelId=3DE6123C03B5
	virtual ~CPipe();	
	
	//##ModelId=3DE6123C03B7
	BOOL	Read(void *Data, UINT Size) ;		// reads 'Size' bytes of data from a pipe and stores at location pointed to by 'Data'
	//##ModelId=3DE6123C03CA
	BOOL	Write(void *Data, UINT Size) ;		// writes 'Size' bytes of data to a pipe from the location pointed to by 'Data'
	//##ModelId=3DE6123C03D5
	UINT	TestForData() const;				// indicates how many bytes are in a pipe available to read

	inline operator string	() const {return PipeName ;}
	inline string	GetName() const { return PipeName ; }
} ;



//
//	Example use of Pipe
/*

#include "rt.h"

CPipe p1("jo") ;			// a global pipeline (could be local within processes or threads if required

//
//	A thread to do the reading
//

UINT __stdcall ThreadFn1(void *args)	// args points to any data intended for the child thread
{										// this is given to the thread by the parent thread during the call
	int k ;

	for(int i = 0; i < 1000; i ++)	{
		p1.Read(&k, sizeof(k)) ;
		printf("Thread Read %d\n", k) ;
	}

	return 0 ;
}

//
//	A thread to do the writing
//

int main()
{
	CThread t1(ThreadFn1) ;

	for(int i = 0; i < 1000; i ++)	{
		getchar() ;
		p1.Write(&i, sizeof(i)) ;
	}

	t1.WaitForThread() ;

	return 0 ;
}	

*/

//
//	The following is a C++ class to represent a Typesafe pipeline object where only specified objects
//	types can be written/read to/from the pipe. To create a typed pipe, for example, one that allows the
//	reading and writing or say integers, create a pipe like this
//
//	CTypedPipe<int>		Pipe1("MyPipeName", 1000) ;
//
//	For other types of pipes e.g. chars, floats, C++ objects or C structures for example, than simple change
//	the 'int' in the declaration 'CTypedPipe<int>' above to whatever type you want. e.g.
//	
//	CTypedPipe<struct liftdata>		Pipe1("MyPipeName") ;
//
//	Better still you can 'typedef' it into a new more meaningful type like this
//
//	typedef CTypedPipe<int>		IntPipe ;
//
//	The you can create integer pipes like this
//
//	IntPipe	p1("MyPipeName") ;
//
//	To read and write, using p1 above use statements such as p1.Read(&i) or p1.Write(&i), where 'i' is an integer
//	in your program, 'i' will then be passed by reference and written or read from the pipe. 
//

//##ModelId=3DE6123D00E6
template <class T>
class CTypedPipe : 
public CPipe  
{

public:
	//##ModelId=3DE6123D0104
	CTypedPipe(const string &Name, UINT NumElements = 1024);			// default constructor = space for 1024 elements of size T
	//##ModelId=3DE6123D010F
	virtual ~CTypedPipe();	
	
	//##ModelId=3DE6123D0111
	BOOL	Read(T *Data);				// reads a 'T' object from a pipe into 'Data'
	//##ModelId=3DE6123D0119
	BOOL	Write(const T *Data);		// writes a 'T' object from data int a pipe
	//##ModelId=3DE6123D0122
	UINT	TestForData() const;		// indicates how many T's are in a pipe to read

} ;


//
//	This is the constructor to create a Pipeline object. All you need is the name of the pipeline
//	The size of the pipeline is controlled by the '__PIPESIZE__' constant in the "rt.h" header file
//
//	Note that this function will attempt to create the pipeline and then open it. If the pipeline already exists
//	then it does not create it, it simply opens iT
//	
//	In this implementation, the pipeline will be destroyed when both reading and writing process
//	terminate
//


//##ModelId=3DE6123D0104
template <class T>
CTypedPipe<T>::CTypedPipe(const string &Name, UINT NumElements) 
	:CPipe(Name, NumElements * sizeof(T))
{}

//	Destructor for a typed pipeline

//##ModelId=3DE6123D010F
template <class T>
CTypedPipe<T>::~CTypedPipe()
{}

//
//	This functions handles writing data to a TYPED pipeline. All you need is a reference to programs
//	data that is to be transferred to the pipline. The write function takes
//	care of the rest. Note that a process/thread writing to a full pipeline will be suspended until
//	the process at the other end of the pipeline reads some out
//

//##ModelId=3DE6123D0119
template <class T>
BOOL CTypedPipe<T>::Write(const T *Data)	
{
	return CPipe::Write((void *)(Data), sizeof(T)) ;
}


//
//	This functions handles reading data from a TYPED pipeline. All you need is a reference to the programs
//	data. The read function takes care of the rest. 
//

//##ModelId=3DE6123D0111
template <class T>
BOOL CTypedPipe<T>::Read(T *Data) 
{
	return CPipe::Read((void *)(Data), sizeof(T)) ;
}


//
//	This function returns the number of T objects in a typed pipeline
//

//##ModelId=3DE6123D0122
template <class T>
UINT CTypedPipe<T>::TestForData() const 
{
	return CPipe::TestForData()/sizeof(T);			// return number of T objects in pipeline
}


/*
//	Example usage of a typed pipe

#include <stdio.h>
#include "rt.h"

CTypedPipe<int> Pipe1("pipe") ;			// create an 'int' pipe

int main()
{
	int i=5, j=0 ;

	for(int x=0; x < 10; x++)	{
		Pipe1.Write(&x) ;
		printf("Wrote %d into Pipe. Number of ints in pipe = %d\n", x, Pipe1.TestForData()) ;
	}
	
	for(x=0; x < 10; x++)	{
		Pipe1.Read(&j) ;
		printf("Read %d from Pipe. Number of ints in pipe = %d\n", j, Pipe1.TestForData()) ;
	}
	
	getch() ;
	return 0 ;
}
*/

/*Contains a function to change the text colour.

  To Use:

  This function takes 2 arguments, foreground and background.
  foreground and background select one of 15 colours numbered as follows:

  0 - Black
  1 - Dark Blue
  2 - Dark Green
  3 - Dark Cyan
  4 - Dark Red
  5 - Dark Magenta
  6 - Dark Yellow
  7 - Grey
  8 - Black (again)
  9 - Blue
  10 - Green
  11 - Cyan
  12 - Red
  13 - Magenta
  14 - Yellow
  15 - White

  Note... background and foreground cannot be the same value, and an
  error (-1) will be returned.

  If colours are valid, and operation was successful, 0 will be returned.

  Use TEXT_COLOUR with no arguements to return values to default.

  A foreground colour only can be specified, eg TEXT_COLOUR(4) will make text dark red .*/  

int TEXT_COLOUR(unsigned char foreground=7, unsigned char background=0);





#endif
