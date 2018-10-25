///////////////////////////////////////////////////////////////////////////////////////////////
//	Notes for using this code on Microsofts Visual C++ V6.0
//
//	If you have trouble linking the program, with errors relating to
//	_beginthreadex() undefined or similar, then see the project settings options in your handout
//	on creating concurrent software using Visual C++
///////////////////////////////////////////////////////////////////////////////////////////////


//#include <StdAfx.h>		// delete this line if you are NOT using Microsoft MFC in your project
						// for example, delete this line if you are creating a Console project
						// but if you are creating a GUI or window type application, leave it in

// NOTE You will have to change the pathlist to the 'rt.h' header file below to indicate
// to the compiler where it can find this file. This will of course depend upon where it resides
// on your computer i.e. where you copied it to.

#include "rt.h"

// constructor to create a child process, takes four 
//arguments, note that the last 3 make use
//	of default argument, that is, if you do not supply 
//real arguments in the call, then the default values will
//	be used for the ones you do NOT supply
//	this means that wehn you create a new CProcess object, 
//the only argument you HAVE to supply is the Name of the 
//executable
//	all other values can be supplied by default

//##ModelId=3DE6123A0037
CProcess::CProcess(	
		const string &Name,		// path/name of executable program (.exe)
		int Priority,			// Priority of the process
		BOOL bUseNewWindow,		// use OWN_WINDOW to make new process run in its own window
								// use PARENT_WINDOW to make it share its parent window
		BOOL bCreateSuspended,	// use SUSPENDED to make new child process's main thread suspended when process is created
								// use ACTIVE to make new child processes main thread active when process iscreated
		
		const string &ChildProcessArgString)

		: ProcessName(Name)
{
	// Check priority level is valid

	

	PERR((	(Priority == HIGH_PRIORITY_CLASS) ||
			(Priority == IDLE_PRIORITY_CLASS) ||
			(Priority == NORMAL_PRIORITY_CLASS) ||
			(Priority == REALTIME_PRIORITY_CLASS)) ,
			string("Illegal 2nd Argument (Process Priority) for Process: ") + Name) ;

	
	PERR ((bUseNewWindow == OWN_WINDOW || bUseNewWindow == PARENT_WINDOW), 
		string("Use OWN_WINDOW or PARENT_WINDOW as 3rd argument for Process: ") + Name ) ;
	PERR ((bCreateSuspended == SUSPENDED || bCreateSuspended == ACTIVE), 
		string("Use SUSPENDED or ACTIVE as 4th Argument for Process: ") + Name) ;


	STARTUPINFO	StartupInfo = {
		sizeof(STARTUPINFO) ,
		NULL ,			// reserved
		NULL ,			// ignored in console applications
		(char *)(Name.c_str()) ,	// displayed in title bar for console applications
		0,0,			// dwx, dwy, offset of top left of new window relative to top left of screen in pixel
						// flags below must specify STARTF_USEPOSITION. Ignored for console apps'
		0,0,			// dwxsize, dwysize: Width and height of the window if new window specified
						// must use flags STARTF_USESIZE. Ignored for console apps'
		0,0,			// size of console in characters, only if STARTF_USECOUNTCHARS flag specified, 
						// Ignored for console apps
		0,				// Colour control, for background and text. Ignored for console apps
		0,				// Flags. Ignored for console applications
		0,				// ignored unless showwindow flag set
		0 ,
		NULL,
		0,0,0			// stdin, stdout and stderr handles (inherited from parent)
	} ;

	UINT flags = Priority ; // Priority,

	if(bUseNewWindow == OWN_WINDOW)		// if caller has specified that child should have its won window
		flags |= CREATE_NEW_CONSOLE ;
	
	if(bCreateSuspended == SUSPENDED)	// if caller has specified that child process should be immediately suspended
		flags |= CREATE_SUSPENDED ;

	BOOL Success = CreateProcess(	NULL,	// application name
					(char *)(Name.c_str()),			// Command line to the process if you want to pass one to main() in the process
					NULL,			// process attributes
					NULL,			// thread attributes
					TRUE,			// inherits handles of parent
					flags,			// Priority and Window control flags,
					NULL,			// use environent of parent
					NULL,			// use same drive and directory as parent
					&StartupInfo ,	// controls appearance of process (see above)
					&pInfo			// Stored process handle and ID into this object
	); 
	PERR( Success == TRUE, string("CProcess Call Unable to Create New Process: ") + Name) ;	// check for error and print message if appropriate
}


////////////////////////////////////////////////////////////////////////////////////////////
//	The following function can be used to change the 'base' priority of a process once it has been
//	created (irrespective of whether it is active or suspended. Processes with higher priority
//	will block/stop processes with lower priority from executing so be careful
//
//	All you need is the handle for the process obtained from FORK_PROCESS() (see above)
//	Windows NT has a limited number of thread priorities and the following symbolic
//	constants can be used as a valid value for the parameter 'Priority' in the function
//	Normally a thread will inherit a 'base' priority from its parent thread, or, if it is
//	the primary thread, from its parent process (See FORK_PROCESS() above
//	The constant below simply allow you to make small adjustments to these value
/////////////////////////////////////////////////////////////////////////////////////////////
//
//	HIGH_PRIORITY_CLASS		Specify this class for a process that performs time-critical tasks that must be executed immediately. The threads of the process preempt the threads of normal or idle priority class processes. An example is Windows Task List, which must respond quickly when called by the user, regardless of the load on the operating system. Use extreme care when using the high-priority class, because a high-priority class application can use nearly all available CPU time. 
//	IDLE_PRIORITY_CLASS		Specify this class for a process whose threads run only when the system is idle. The threads of the process are preempted by the threads of any process running in a higher priority class. An example is a screen saver. The idle-priority class is inherited by child processes. 
//	NORMAL_PRIORITY_CLASS	Specify this class for a process with no special scheduling needs. 
//	REALTIME_PRIORITY_CLASS Specify this class for a process that has the highest possible priority. The threads of the process preempt the threads of all other processes, including operating system processes performing important tasks. For example, a real-time process that executes for more than a very brief interval can cause disk caches not to flush or cause the mouse to be unresponsive. 
//
//	Returns TRUE on Success/False on failure
//

//##ModelId=3DE6123A009C
BOOL CProcess::SetPriority(int Priority) const
{
	BOOL Success;

	// Check priority level 

	PERR(((Priority == HIGH_PRIORITY_CLASS) || (Priority == IDLE_PRIORITY_CLASS) ||
			(Priority == NORMAL_PRIORITY_CLASS) || (Priority == REALTIME_PRIORITY_CLASS)) ,
			string("Illegal Priority value in call to SetPiority()")) ;

	Success = SetPriorityClass(GetProcessHandle(), Priority) ;
	PERR( Success == TRUE, string("Unable to Set Thread Priority of Process: ") + ProcessName) ;	// check for error and print error message as appropriate
	
	return Success ;
}

//
//	These two functions can be called to suspend and resume a processes activity. 
//	Actually, it suspend/resumes the process primary thread and not necessarily any
//	other child threads it may have. Once suspended	the thread will no longer 
//	run and consume CPU time. Note that if you suspend a process more than once, 
//	then it will require the appropriate number	of resumes to allow it to continue
//
//
//	Returns TRUE on Success, FALSE on Failure
//

//##ModelId=3DE6123A007F
BOOL CProcess::Suspend() const
{
	UINT	Result = SuspendThread(GetThreadHandle()) ;
	PERR( Result != 0xffffffff, string("Cannot Suspend Process: ") + ProcessName) ;

	if(Result != 0xffffffff)	// if no error
		return TRUE ;
	else
		return FALSE ;
}

//##ModelId=3DE6123A0088
BOOL CProcess::Resume() const
{
	UINT	Result = ResumeThread(GetThreadHandle()) ;
	PERR( Result != 0xffffffff, string("Cannot Resume Process: ") + ProcessName) ;

	if(Result != 0xffffffff)	// if no error
		return TRUE ;
	else
		return FALSE ;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	This function waits for a child process to terminate provide it has been
//	created as a CProcess Object. Or returns if the specified time elapses before the process has terminates
//
//	Note that if the child process has already terminated, when the call to wait for
//	it is made, then this is deemed a success, NOT a failure and the function returns immediately without waiting
//
//	This function returns WAIT_FAILED if there was an error, WAIT_TIMEOUT if the wait operation timed out
//	of WAIT_OBJECT_0 if the operation did in fact wait and then returned
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//##ModelId=3DE6123A0092
BOOL	CProcess::WaitForProcess(DWORD Time) const
{
	UINT Result = WaitForSingleObject(GetProcessHandle(), Time) ;	
	PERR( Result != WAIT_FAILED, string("Cannot Wait for Child Process: ") + ProcessName + string(" to Terminate.\n It might already be dead")) ;	// check for error and print message if appropriate== WAIT_OBJECT_0 ) 

	return Result ;
}


//	The following function sends a message to a process' main thread
//	If the thread is currently suspended, waiting for a message, then posting it a message
//	will wake it up and allow it to continue
//
//	The Message must be in the range 0 - 32000
//	In order to receive messages, the thread receiving the message must have created a message queue.
//	(See CMessageQueue later)
//

//##ModelId=3DE6123A00AF
BOOL CProcess::Post(UINT Message) const	// message value and ID of thread
{
	BOOL Result ;

	if(Message > 32000)	{
		PERR( Message <= 32000, string("Could not Signal Process:") + ProcessName + string("\nReason: Message Value > 32000")) ;	// check for error and print error message as appropriate
		return FALSE ;
	}
	else {
		Result = PostThreadMessage(GetThreadId(), WM_USER + Message, 0, 0L) ;
		PERR( Result != 0, string("Could not Signal Process:") + ProcessName + string("\nProcess May be Dead")) ;	// check for error and print error message as appropriate
		return Result ;
	}
}

//
//	This calls standard exit() function to terminate the process
//

//##ModelId=3DE6123A00BA
void CProcess::Exit(UINT ExitCode) const
{
	exit(ExitCode) ;
}

//	This function create a parallel thread within a process (do not confuse this
//	with creating a new process. Each and every process (i.e. program/application)
//	can have many threads. At startup, a process will have just 1 thread which commences
//	with the execution of main() in your 'C' program. Once started, this 'primary' thread
//	can create secondary threads which run in parallel with each other AND the main thread
//	
//	A thread is simply a 'C' style function (see examples below) defined in a special way.
//	A thread inherits the priority of its parent thread, but can be changed later
//
//	A Thread function MUST be declared in a prescribed style (see example below). 
//	A process can create/maintain many such threads and they can share global variables 
//	thus implementing thread-to-thread communication in a simple and effective way

//	and even call the same functions.


//##ModelId=3DE6123A0182
CThread::CThread(UINT __stdcall Function( void *), // name/pointer to function that is to be the new thread
				 BOOL bCreateState,		// A flag indicating if the thread should commence SUSPENDED (TRUE) or ACTIVE (FALSE)
				 void *ThreadArgs		// a generic pointer (can point to anything) to any data the calling thread
										// wishes to pass to the child thread
					 )
{
	UINT		ThreadControlFlags = 0 ;

	if(bCreateState == SUSPENDED)		// if caller wants thread initially suspended
		ThreadControlFlags = CREATE_SUSPENDED ;

	ThreadHandle = (HANDLE)(_beginthreadex(NULL, 0, Function, ThreadArgs, ThreadControlFlags, &ThreadID)) ;
	PERR( ThreadHandle != 0, string("Unable to Create Thread")) ;	// check for error and print message if appropriate
}


/*
**	The following two functions work together to assist in the design of CThread derived classes with a main() to simulate the main of the thread
**	All active class objects have their root thread within this function
*/

UINT __stdcall __GlobalThreadMain__(void *theThreadPtr) 	// receives a pointer to the thread object 
{
	ExitThread(((ActiveClass *)(theThreadPtr))->main()) ;	// run the activeclass virtual main function it should be overridden in derived class
	return 0 ;
}


//##ModelId=3DE6123A0178
CThread::CThread(BOOL bCreateState)			// A flag indicating if the thread should commence SUSPENDED (TRUE) or ACTIVE (FALSE)
{
	UINT		ThreadControlFlags = 0 ;

	if(bCreateState == SUSPENDED)		// if caller wants thread initially suspended
		ThreadControlFlags = CREATE_SUSPENDED ;

	ThreadHandle = (HANDLE)(_beginthreadex(NULL, 
							0, 
							__GlobalThreadMain__,	
							this,				//pointer to cthread derived object passed to __GlobalThreadMain__() above
							ThreadControlFlags,	// thread is created in suspended state 
							&ThreadID)) ;

	PERR( ThreadHandle != 0, string("Unable to Create Thread")) ;	// check for error and print message if appropriate
}


//##ModelId=3DE6123A0223
ActiveClass::ActiveClass()
: TerminateFlag(FALSE) ,
CThread()		// Call base class constructor to create the SUSPENDED thread, i.e. main in derived class does not yet run
				// All objects derived from active class are thus initially created in the suspended state and have to
				// be RESUMED before they become active. This is the safest way, as it allows control over the
				// order in which objects run and avoids potential race hazards.
{}				// Note that active class objects like all derived classes are constructed from the base upwards 
				// and thus we have to make sure that base/derived class constructors are run to completion
				// before the thread for the class (which is created in the base class) is allowed to resume
				// otherwise the thread could run BEFORE the active class object had completed its construction
				// this was the main reason for creating all ActiveClass objects in the Suspended state so don't change it
				// I learn't the hard way and it was painful to debug !!!


// this is important as we want to allow the derived 
//classes constructor i.e. the one
// written by the user, to complete its constructor 
//before the class main() is run
// otherwise the class might run with unitialised 
//variables/state
// This means that the user has to call Resume() for the 
//class object to allow it to run.
//##ModelId=3DE6123A022C
ActiveClass::~ActiveClass()
{}



//
//	The following can be used to terminate a thread at any point in the thread execution
//	If the thread reaches the end of its 'thread function' then executing return 0 does the same trick.
//

//##ModelId=3DE6123A01D4
void CThread::Exit(UINT	ExitCode) const
{
	ExitThread(ExitCode) ;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////
//	These two functions can be called to suspend and resume a threads activity. Once suspended
//	the thread will no longer run and consume CPU time
//
//	Note that if you suspend a thread more than once, then it will require that many number
//	of resumes to allow it to continue
//
//	Both functions require the thread HANDLE, which can be obtained when the thread is created
///////////////////////////////////////////////////////////////////////////////////////////////

//##ModelId=3DE6123A01AA
BOOL CThread::Suspend() const
{
	UINT	Result = SuspendThread(ThreadHandle) ;
	PERR( Result != 0xffffffff, string("Cannot Suspend Thread\n")) ;	// check for error and print message if appropriate

	if(Result != 0xffffffff)
		return TRUE ;
	else
		return FALSE ;
}

//##ModelId=3DE6123A01B4
BOOL CThread::Resume() const
{
	UINT	Result = ResumeThread(ThreadHandle) ;
	PERR( Result != 0xffffffff, string("Cannot Resume Thread\n")) ;	// check for error and print message if appropriate

	if(Result != 0xffffffff)
		return TRUE ;
	else
		return FALSE ;
}

////////////////////////////////////////////////////////////////////////////////////////////
//	The following function can be used to change the priority of a thread once it has been
//	created (irrespective of whether it is active or suspended). Threads with higher priority
//	will block/stop threads with lower priority from executing so be careful.
//
//	All you need is the handle for the thread obtained from CREATE_THREAD() (see above)
//	Windows NT has a limited number of thread priorities and the following symbolic
//	constants can be used as a valid value for the parameter 'Priority' in the function
//	Normally a thread will inherit a 'base' priority from its parent thread, or, if it is
//	the primary thread, from its parent process (See FORK_PROCESS() above
//	The constant below simply allow you to make small adjustments to the thread priority
/////////////////////////////////////////////////////////////////////////////////////////////
//
//	THREAD_PRIORITY_ABOVE_NORMAL  = Indicates 1 point above normal priority for the priority class. 
//	THREAD_PRIORITY_BELOW_NORMAL  = Indicates 1 point below normal priority for the priority class. 
//	THREAD_PRIORITY_HIGHEST = Indicates 2 points above normal priority for the priority class. 
//	THREAD_PRIORITY_IDLE = Indicates a base priority level of 1 for IDLE_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes, and a base priority level of 16 for REALTIME_PRIORITY_CLASS processes. 
//	THREAD_PRIORITY_LOWEST = Indicates 2 points below normal priority for the priority class. 
//	THREAD_PRIORITY_NORMAL = Indicates normal priority for the priority class. 
//	THREAD_PRIORITY_TIME_CRITICAL = Indicates a base priority level of 15 for IDLE_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes, and a base priority level of 31 for REALTIME_PRIORITY_CLASS processes. 
/////////////////////////////////////////////////////////////////////////////////////////////

//##ModelId=3DE6123A01BF
BOOL CThread::SetPriority(UINT Priority) const
{	
	BOOL Success;

	// check for error in priority value and print message if appropriate

	PERR(((Priority == THREAD_PRIORITY_ABOVE_NORMAL) ||
			(Priority == THREAD_PRIORITY_BELOW_NORMAL) ||
			(Priority == THREAD_PRIORITY_HIGHEST) ||
			(Priority == THREAD_PRIORITY_IDLE) ||
			(Priority == THREAD_PRIORITY_LOWEST) ||
			(Priority == THREAD_PRIORITY_NORMAL) ||
			(Priority == THREAD_PRIORITY_TIME_CRITICAL)) ,
			string("Illegal Priority value specified for Thread in call to CThread::SetPriority()")) ;

	Success = SetThreadPriority(ThreadHandle, Priority) ;	// set priority

	PERR( Success == TRUE, string("Cannot Set Thread Priority\n")) ;	// check for error and print message if appropriate
	
	return Success ;
}


//
//	This function will wait for the child thread specified by ThreadHandle to terminate
//	main or parent threads can use this to wait for their child thread to terminate
//
//	A time period can be specified indicating the maximum time to attempt a wait
//
//	This function returns WAIT_FAILED if there was an error, WAIT_TIMEOUT if the wait operation timed out
//	of WAIT_OBJECT_0 if the operation did in fact wait and then returned
//

//##ModelId=3DE6123A01B6
UINT CThread::WaitForThread(DWORD Time) const		
{
	UINT Result = WaitForSingleObject(ThreadHandle, Time) ;	// return WAIT_FAILED on error
	PERR( Result != WAIT_FAILED, string("Cannot Wait For Thread")) ;	// check for error and print error message as appropriate

	return Result ;
}

//	The following function sends a message to a thread
//	If the thread is currently suspended, waiting for a message, then posting it a message
//	will wake it up and allow it to continue
//
//	Message must bve in the range 0 - 32000
//	In order to use messages, the thread receiving the message must have created a message queue.
//	(See CMessageQueue later )
//

//##ModelId=3DE6123A01CA
BOOL CThread::Post(UINT Message) const	// message value and ID of thread
{
	BOOL Result ;

	if(Message > 32000)	{
		PERR( Message <= 32000, string("Could not Post User Message: Message > 32000")) ;	// check for error and print error message as appropriate
		return FALSE ;
	}
	else {
		Result = PostThreadMessage(ThreadID, WM_USER + Message, 0, 0L) ;
		PERR( Result != 0, string("Could not Post User Message. The Thread might have died")) ;	// check for error and print error message as appropriate
		return Result ;
	}
}


// constructs mutex with name and indicates whether 
// object protected by mutex is owned by creator or not
// Note use of default argument, i.e. if you do not 
// specifiy a value for bOwned argument
// then it will default to NOTOWNED

//##ModelId=3DE6123A0397
CMutex::CMutex(const string &Name, BOOL bOwned)		// needs a name for the mutex (i.e. a string) and a flag 
													// indicating if the mutex is owned by the process that created it (Use OWNED or NOTOWNED for this value)
	:MutexName(Name)
{
	if(bOwned == OWNED)	bOwned = TRUE ;
	else				bOwned = FALSE ;

	MutexHandle  = CreateMutex(NULL, bOwned, (char *)(Name.c_str())) ;
	PERR( MutexHandle != NULL, string("Cannot Create Mutex: ") + Name) ;	// check for error and print message if appropriate
}


//////////////////////////////////////////////////////////////////////
//	unlinks a thread from a Mutex, provided you have its handle
//	Returns TRUE/FALSE on SUCCESS/FAIL
//	when all threads have unlinked from a mutex it will be destroyed automatically
//////////////////////////////////////////////////////////////////////

//##ModelId=3DE6123A0383
BOOL	CMutex::Unlink() const
{
	BOOL Success = CloseHandle(MutexHandle) ;
	PERR( Success == TRUE, string("Cannot Unlink from Mutex:") + MutexName) ;	// check for error and print message if appropriate
	return Success ;
}		


//
//	This function will attempt to perform a WAIT on a Mutex. If the Mutex has been signalled
//	(new mutexs are created in the 'signalled' or free state), then the process will be allowed to
//	continue and the mutex will be set to the non-signalled state. This would mean
//	that any other process coming along and performing a wait would be blocked until the mutex
//	is signalled once again.
//	
//	This function returns WAIT_FAILED if there was an error, WAIT_TIMEOUT if the wait operation timed out
//	of WAIT_OBJECT_0 if the operation did in fact wait and then returned
//

//##ModelId=3DE6123A036D
UINT CMutex::Wait(DWORD Time) const				// return an unsigned int or UINT
{
	UINT	Result = WaitForSingleObject(MutexHandle, Time) ;				// returns WAIT_FAILED on error
	PERR( Result != WAIT_FAILED, string("Cannot Perfom WAIT operation on Mutex: ") + MutexName) ;	// check for error and print message if appropriate
	return Result ;
}


//
//	This function will attempt to 'signal' a Mutex. This will allow one blocked
//	process to proceed (setting the mutex back to the non-signalled state), otherwise
//	if no processes are waiting/blocked, the mutex remains in the signalled state
//	


//##ModelId=3DE6123A0377
BOOL CMutex::Signal() const
{
	BOOL Success = ReleaseMutex( MutexHandle) ;		// FALSE on failure, TRUE on success
	PERR( Success == TRUE, string("Cannot Perfom SIGNAL operation on Mutex: ") + MutexName) ;	// check for error and print message if appropriate
	return Success ;
}

//
//	This function reads the value of a Mutex, without signalling or waiting on it
//	it can be used to poll the value of the Mutex to perhaps determine if it is safe to
//	perform a wait without getting suspended/blocked
//	returns WAIT_FAILED if there was an error.
//
//	If the Mutex has been signalled, then this function returns TRUE
//	if the Mutex has NOT been signalled, then this function returns FALSE
//

//##ModelId=3DE6123A0381
BOOL CMutex::Read() const	// Handle of the Mutex needed
{							// returns true/false state of Mutex
	BOOL Signalled ;

	// first wait for the object. Timeout is zero, so function
	// should immediately wait and decrement the Mutex value if it is signalled (i.e. >1)
	
	Signalled = WaitForSingleObject(MutexHandle, 0) ;	// see of Mutex is signalled or not
	PERR( Signalled != WAIT_FAILED, string("Cannot Perfom READ operation on Mutex: ") + MutexName) ;	// check for error and print message if appropriate	
	
	// Now if we did a wait and managed to decrement the Mutex, then
	// the above function call should have returned WAIT_OBJECT_0, thus the process
	// of attempting to read the Mutex has unfortunately resulted in its value
	// being decremented, so we have to put that write by signalling it again. This should be
	// safe, since we cannot exceed the limits of the Mutex max value as we are
	// only putting it back to what it was before we attempted this

	if(Signalled == WAIT_FAILED) 
		return WAIT_FAILED ;

	if(Signalled == WAIT_OBJECT_0)	{
		BOOL Success = ReleaseMutex(MutexHandle) ;			// signal mutex if we decremented it
		PERR( Success == TRUE, string("Cannot Perfom READ operation on Mutex: ") + MutexName) ;		// check for error and print message if appropriate
		return (UINT)TRUE ;
	}
	else 
		return (UINT)FALSE ;			// non-signalled

}

////////////////////////////////////////////////////////////
//	ReaderWriters Mutex Problem
////////////////////////////////////////////////////////////

//
//	This version gives preference to readers rather than writers
//

CReadersWritersMutex::CReadersWritersMutex(const string &MyName) : Name(MyName)
{
	ReadersWritersMutex = new CMutex(string("__CReadersWritersMutex__") + MyName) ;
	ReadersWritersSemaphore = new CSemaphore(string("__CReadersWritersSemaphore__") + MyName, 1, 1) ;
	ReadersWritersDataPool = new CDataPool(string("__CReadersWritersDataPool__") + MyName, sizeof(struct Data)) ;
	ptr = (struct Data *)(ReadersWritersDataPool->LinkDataPool()) ;
		
	if(strcmp(ptr->Initialised, "Initialised") != 0)	{	// if pool not initialised
		ptr->NumberOfReaders = 0 ;
		strcpy_s(ptr->Initialised, "Initialised");			// show me as initialised
	}
}

CReadersWritersMutex::~CReadersWritersMutex()
{
	delete ReadersWritersMutex;
	delete ReadersWritersSemaphore;
	strcpy_s(ptr->Initialised, "") ;
	delete ReadersWritersDataPool;
}

//
// called by a reader when they wish to access to the resource
//
void CReadersWritersMutex::WaitToRead()
{
	ReadersWritersMutex->Wait();			// wait for exclusive access

	// increment the number of waiting readers

	if(++(ptr->NumberOfReaders) == 1)		// if this is the 1st reader
		ReadersWritersSemaphore->Wait();	// wait on the semaphore as a writer may be using the resource

	ReadersWritersMutex->Signal() ;		// release access to resource
}


// 
// called by a reader when they have finished with the resource
//

void CReadersWritersMutex::DoneReading() 
{
	ReadersWritersMutex->Wait();			// wait for exclusive access

	// decrement the number of waiting readers
	if(--(ptr->NumberOfReaders) == 0)			// if no readers
		ReadersWritersSemaphore->Signal() ;		// signal that writers may now enter

	ReadersWritersMutex->Signal() ;			// release access to resource
}

//
// called by a writer when they wish to access to the resource
//

void CReadersWritersMutex::WaitToWrite()
{
	ReadersWritersSemaphore->Wait() ;		// used to exclude all readers
}

// 
// called by a writer when they have finished with the resource
//

void CReadersWritersMutex::DoneWriting()
{		
	PERR((ptr->NumberOfReaders < 2) , "HELP");
	ReadersWritersSemaphore->Signal() ;		// allow readers in
}

////////////////////////////////////////////////////////////
//	WritersReader Mutex Problem
////////////////////////////////////////////////////////////

//
//	This version gives preference to writers rather than readers
//


CWritersReadersMutex::CWritersReadersMutex(const string &MyName) : Name(MyName)
{
	WritersReadersMutex = new CMutex(string("__CWritersReadersMutex__") + MyName) ;
	WritersReadersSemaphore = new CSemaphore(string("__CWritersReadersSemaphore__") + MyName, 1, 1) ;
	WritersReadersCondition = new CCondition(string("__CWritersReadersCondition__") + MyName, MANUAL, SIGNALLED) ;

	WritersReadersDataPool = new CDataPool(string("__CWritersReadersDataPool__") + MyName, sizeof(struct Data)) ;
	ptr = (struct Data *)(WritersReadersDataPool->LinkDataPool()) ;
		
	if(strcmp(ptr->Initialised, "Initialised") != 0)	{	// if pool not initialised
		ptr->NumberOfReaders = 0 ;
		ptr->NumberOfWriters = 0 ;
		strcpy_s(ptr->Initialised, "Initialised");			// show me as initialised
	}
}

CWritersReadersMutex::~CWritersReadersMutex()
{
	delete WritersReadersMutex;
	delete WritersReadersSemaphore;
	delete WritersReadersCondition ;
	strcpy_s(ptr->Initialised, "") ;
	delete WritersReadersDataPool;
}

//
// called by a reader when they wish to access to the resource
//
void CWritersReadersMutex::WaitToRead()
{
	WritersReadersCondition->Wait() ;		// wait until no writers
	WritersReadersMutex->Wait();			// wait for exclusive access

	// increment the number of waiting readers

	if(++(ptr->NumberOfReaders) == 1)		// if this is the 1st reader
		WritersReadersSemaphore->Wait();	// wait on the semaphore as a writer may be waiting to use the resource

	WritersReadersMutex->Signal() ;		// release access to resource
}


// 
// called by a reader when they have finished with the resource
//
void CWritersReadersMutex::DoneReading() 
{
	WritersReadersMutex->Wait();				// wait for exclusive access

	if(--(ptr->NumberOfReaders) == 0)			// if no readers
		WritersReadersSemaphore->Signal() ;		// signal last reader has left and that any waiting writers may now enter

	WritersReadersMutex->Signal() ;				// release access to resource
}

//
// called by a writer when they wish to access to the resource
//
void CWritersReadersMutex::WaitToWrite()
{
	WritersReadersMutex->Wait();				// wait for exclusive access to data

	if(++(ptr->NumberOfWriters) == 1 )			// if I am the only writer waiting
		WritersReadersCondition->Reset() ;		// block future readers until this writer has gained access and has released it

	WritersReadersMutex->Signal() ;				// release access to data
	
	WritersReadersSemaphore->Wait() ;			// wait until all other process have left resource (readers or writers)
}

// 
// called by a writer when they have finished with the resource
//
void CWritersReadersMutex::DoneWriting()
{		
	WritersReadersSemaphore->Signal() ;				// allow next writer or reader in depending on who is first in line

	WritersReadersMutex->Wait();					// wait for exclusive access to data

	if(--(ptr->NumberOfWriters) == 0 )				// if there are no more waiting writers
		WritersReadersCondition->Signal() ;			// remove the condition blocking entry by readers
		
	WritersReadersMutex->Signal() ;					// release access to data
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CRendezvous Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////

CRendezvous::CRendezvous(const string &TheRendezvousName, int NumberThreads) : RendezvousName(TheRendezvousName)
{
	RendezvousMutex = new CMutex(string("__RendezvousMutex__") + RendezvousName) ;
	RendezvousEvent = new CEvent(string("__RendezvousEvent__") + RendezvousName) ;
		
	RendezvousDataPool = new CDataPool(string("__RendezvousDataPool__") + RendezvousName, sizeof(struct RendezvousData)) ;
	ptr = (struct RendezvousData *)(RendezvousDataPool->LinkDataPool()) ;

	RendezvousMutex->Wait() ;
	if(strcmp(ptr->Initialised, "Initialised") != 0)	// if not initialised
	{
		strcpy_s(ptr->Initialised, "Initialised") ;							// initialise it
		ptr->NumberWaiting = NumberThreads ;					// set number of threads waiting to rendezvous
		ptr->NumThreads = NumberThreads ;
	}
	else								// check the number is the same as previous initialisation of this rendezvous
		PERR(NumberThreads == ptr->NumThreads, string("Rendezvous '") + RendezvousName + string("' Already Created with a Different Number of Clients")) ;

	RendezvousMutex->Signal() ;
}

void CRendezvous::Wait() {
	RendezvousMutex->Wait() ;
	if(--(ptr->NumberWaiting) == 0)
	{
		ptr->NumberWaiting = ptr->NumThreads ;			// reset the count
		RendezvousEvent->Signal() ;			// release all waiting threads
		RendezvousMutex->Signal() ;
	}
	else	
	{
		RendezvousMutex->Signal() ;
		RendezvousEvent->Wait() ;
	}
}

CRendezvous::~CRendezvous()
{
	strcpy_s(ptr->Initialised, "") ;
	delete RendezvousDataPool ;
	delete RendezvousMutex ;
	delete RendezvousEvent ;
}


////////////////////////////////////////////////////////////
//	Event Functions
////////////////////////////////////////////////////////////


CEvent::CEvent(const string &Name, BOOL bType, BOOL bState)			// btype = SINGLE_RELEASE or MULTIPLE_RELEASE to allow one or many thread to resume when event is signalled
{																	// bState = SIGNALLED or NOTSIGNALLED to indicate the initial or creation state of the event
	PERR(bState == SIGNALLED || bState == NOTSIGNALLED, string("Illegal Signalled/NotSignalled Type specified when creating CEvent: ") + EventName) ;
	
	if(bState == SIGNALLED)		
		bState = TRUE ;
	else
		bState = FALSE ;

	PERR(bType == SINGLE_RELEASE || bType == MULTIPLE_RELEASE, string("Illegal Single or Multithread Type specified when creating CEvent: ") + EventName) ;
	
	if(bType == SINGLE_RELEASE)
		bType = FALSE ;			// Win32 Auto-reset event
	else
		bType = TRUE ;			// Win32 manual-reset event
	
	EventHandle = CreateEvent(NULL, bType, bState, (char *)(Name.c_str())) ;		
	PERR( EventHandle != NULL, string("Cannot Create CEvent: ") + Name) ;	// check for error and print message if appropriate
}
	
BOOL CEvent::Unlink() const {								// unlink from event, i.e. we have finished using it
	BOOL Success = CloseHandle(EventHandle) ;
	PERR(Success != 0, string("Cannot Unlink the CEvent: ") + EventName) ;	// check for error and print message if appropriate
	return Success ;
}

// Signal() sets the event and will release ONLY ONE Waiting thread. It is then automatically reset.
// If no thread is waiting at the time it is signalled, then it is STILL RESET immediately

BOOL CEvent::Signal() const 
{ 	
	BOOL Success = PulseEvent(EventHandle) ;
	PERR( Success != 0, string("Cannot SignalAndReset() the CEvent: ") + EventName) ;	// check for error and print message if appropriate
	return Success ; 
}
	
UINT CEvent::Wait(DWORD Time) const 			// perform a wait on an event for ever or until specified time
{
	UINT	Status = WaitForSingleObject(EventHandle, Time) ;
	PERR(Status != WAIT_FAILED, string("Cannot Wait for CEvent: ") + EventName) ;	// check for error and print message if appropriate
	return Status ;
}

////////////////////////////////////////////////////////////
//	Condition Functions
////////////////////////////////////////////////////////////

CCondition::CCondition(const string &Name, BOOL bType, BOOL bState) 
{
	PERR(bState == SIGNALLED || bState == NOTSIGNALLED, string("Illegal Signalled/NotSignalled Type specified when creating CCondition: ") + ConditionName) ;

	if(bState == SIGNALLED)		bState = TRUE ;
	else						bState = FALSE ;

	PERR(bType == MANUAL || bType == AUTORESET, string("Illegal Signalled/NotSignalled Type specified when creating CCondition: ") + ConditionName) ;

	if(bType == MANUAL)		bType = TRUE ;
	else					bType = FALSE ;

	
	ConditionHandle = CreateEvent(NULL, bType, bState, (char *)(Name.c_str())) ;		// based around a Win32 manual event
	PERR( ConditionHandle != NULL, string("Cannot Create CCondition: ") + Name) ;	// check for error and print message if appropriate
}

BOOL CCondition::Unlink() const {								// unlink from Condition, i.e. we have finished using it
	BOOL Success = CloseHandle(ConditionHandle) ;
	PERR(Success != 0, string("Cannot Unlink the CCondition: ") + ConditionName) ;	// check for error and print message if appropriate
	return Success ;
}


// Signal() sets the Condition and will release ALL Waiting threads. It can be reset by calling Reset()
BOOL CCondition::Signal() const { 	
	BOOL Success = SetEvent(ConditionHandle) ;
	PERR( Success != 0, string("Cannot Set() the CCondition: ") + ConditionName) ;	// check for error and print message if appropriate
	return Success ; 
}	
	

UINT CCondition::Wait(DWORD Time) const 			// perform a wait on a Condition for ever or until specified time
{
	UINT	Status = WaitForSingleObject(ConditionHandle, Time) ;
	PERR(Status != WAIT_FAILED, string("Cannot Wait for CCondition: ") + ConditionName) ;	// check for error and print message if appropriate
	return Status ;
}

BOOL CCondition::Reset() const		// reset the condition back to false or not signalled
{
	BOOL Success = ResetEvent(ConditionHandle) ;
	PERR( Success != 0, string("Cannot Reset CCondition: ") + ConditionName) ;	// check for error and print message if appropriate
	return Success ;
}
	
BOOL CCondition::Test() const 									// see if condition is signalled	
{
	UINT	Status = WaitForSingleObject(ConditionHandle, 0) ;
	PERR( Status != WAIT_FAILED, string("Cannot Test Value of CAutoResetCondition: ") + ConditionName) ;	// check for error and print message if appropriate

	if(Status == WAIT_FAILED)
		return WAIT_FAILED ;
	else if(Status == WAIT_OBJECT_0)		// if event signalled
		return TRUE ;						// return TRUE
	else 
		return FALSE ;						// esle return FALSE
}


////////////////////////////////////////////////////////////
//	Semaphore Functions
////////////////////////////////////////////////////////////
//
//	Semaphore's are used primarily by processes that need to make sure that a limited number of
//	processes are allowed to access a resource at any one time.
//
//	In order to use a Semaphore, create one using the function below, giving it a name
//	if the Semaphore is new, i.e. there is not already a Semaphore with that name in existence, 
//	then a new one will be created using the initial value specified and limiting its maximum value to that specified
//	However, if one already exists, then the function below simply links to it 
//	or creates a reference to it. (the max and initial values will in this instance be ignored)
//
//	In essence, all processes that wish to use the Semaphore attempt to create it.
//	Only one process will actually create it, the others simply reference or share the Semaphore
//	with the same name. In either case, the function returns a HANDLE to the object.
//	This handle can be used in subsequent calls relating to the use of Semaphores. In the event
//	that an error occurs, the returned value of the Handle will be NULL. the calling process
//	can check this and take appropriate action
//
//	An alternative is to use the LINK_SEMAPHORE() function below if you only want to
//	use the Semaphore and not create it
//
//	For a Binary semaphore specify Max value as 1


// construct semaphore by name with initial and max 
//permissible values
//##ModelId=3DE6123B02A6
CSemaphore::CSemaphore(const string &Name, int InitialVal, int MaxVal)	// name, starting value and Maximum value needed
	:SemaphoreName(Name)
{
	SemaphoreHandle = CreateSemaphore(0, InitialVal, MaxVal, (char *)(Name.c_str())) ;
	PERR( SemaphoreHandle != NULL, string("Cannot Create Semaphore: ") + Name) ;	// check for error and print message if appropriate
}



//////////////////////////////////////////////////////////////////////
//	unlinks a Semaphore, provided you have its handle
//	Returns TRUE/FALSE on SUCCESS/FAIL
//	
//	A sempahore is removed when all threads have unlinked from it
//////////////////////////////////////////////////////////////////////

//##ModelId=3DE6123B0293
BOOL	CSemaphore::Unlink() const	// Handle of the semaphore needed
{													// return TRUE/FALSE on Success/Failure
	BOOL Success = CloseHandle(SemaphoreHandle) ;
	PERR( Success == TRUE, string("Cannot Unlink from Semaphore: ") + SemaphoreName ) ;	// check for error and print message if appropriate
	return Success ;
}


//
//	This function will attempt to perform a wait on a sempahore. If the sempahore has been signalled
//	(new sempahores are NOT necessarily created in the 'signalled' state), then the process will be allowed to
//	continue and the sempahore will be set to the non-signalled state. This would mean
//	that any other process coming along and performing a wait would be blocked until the sempahore
//	is signalled once again.
//
//	This function returns WAIT_FAILED if there was an error, WAIT_TIMEOUT if the wait operation timed out
//	of WAIT_OBJECT_0 if the operation did in fact wait and then returned
///

//##ModelId=3DE6123B0277
UINT CSemaphore::Wait(DWORD Time) const	// Handle of the semaphore needed
{
	UINT Result = WaitForSingleObject(SemaphoreHandle, Time) ;		// return WAIT_FAILED on error
	PERR( Result != WAIT_FAILED, string("Cannot Wait on Semaphore: ") + SemaphoreName ) ;	// check for error and print message if appropriate
	return Result ;
}


//
//	This function will attempt to 'signal' a semaphore. This will allow one or more blocked
//	process to proceed (setting the mutex back to the non-signalled state), otherwise
//	if no processes are waiting/blocked, the sempahore remains in the signalled state
//	

//##ModelId=3DE6123B027F
BOOL CSemaphore::Signal( int Increment)	const	// value by which sempahore increases (default is 1)
{											// return TRUE/FALSE on Success/Failure
	BOOL Success = ReleaseSemaphore( SemaphoreHandle, Increment, NULL) ; 
	PERR( Success == TRUE, string("Cannot Signal Semaphore: ") + SemaphoreName + string("\nMaxmimum Value may have been exceeded")) ;	// check for error and print message if appropriate
	return Success ;
}



//
//	This function reads the value of a Semaphore, without signalling or waiting on it
//	it can be used to poll the value of the Semaphore to perhaps determine if it is safe to
//	perform a wait without getting suspended/blocked
//	returns WAIT_FAILED if there was an error	
//
//	If the Semaphore has been signalled, then this function returns TRUE
//	if the Semaphore has NOT been signalled, then this function returns FALSE
//


//##ModelId=3DE6123B0289
UINT CSemaphore::Read() const	// Handle of the semaphore needed
{												// returns current value of semaphore
	BOOL Signalled ;

	// first wait for the object. Timeout is zero, so function
	// should immediately wait and decrement the sempahores value if it is signalled (i.e. >1)
	
	Signalled = WaitForSingleObject(SemaphoreHandle, 0) ;	// see of semaphore is signalled or not
	PERR( Signalled != WAIT_FAILED, string("Cannot Read Semaphore: ") + SemaphoreName ) ;	// check for error and print message if appropriate	
	
	// Now if we did a wait and managed to decrement the semaphore, then
	// the above function call should have returned WAIT_OBJECT_0, thus the process
	// of attempting to read the semaphore has unfortunately resulted in its value
	// being decremented, so we have to put that write by signalling it again. This should be
	// safe, since we cannot exceed the limits of the sempahores max value as we are
	// only putting it back to what it was before we attempted this
	//
	//	In the process of putting it back, we also get the value of the semaphore as
	//	a bonus (+1 added to make sure it the value before be decremented it
	//
	//	You might well ask why we cannot simply just use ReleaseSemaphore and specify an
	//	increment of 0 in place of all this code, so that the sempahore doesn't change at all
	//	but we get the previous value back. That is what we are after in the end isn't it ?
	//	The answer is that it work in Windoes 95/98 but not on NT which insists
	//	on having a release value > 0 and thus doesn't have a future in windows 2000

	if(Signalled == WAIT_OBJECT_0)	{
		LONG Prev ;		// to hold the previous value of the semapahore
		BOOL Success = ReleaseSemaphore(SemaphoreHandle, 1, &Prev) ;
		PERR( Success == TRUE, string("Cannot Signal Semaphore") + SemaphoreName ) ;	// check for error and print message if appropriate
		return (UINT)(Prev+1) ;	// returns true if semaphore signalled
	}

	else if(Signalled == WAIT_TIMEOUT)	// the operation timed-out and the sempahore is not-signalled
		return (UINT)0 ;				// non-signalled

	else
		return WAIT_FAILED ;			// error, possibly invalid handle

}


//
//	The following function can be used to prevent DEADLOCK in a multi-threaded
//	application where several thread/processes are competing to acquire resources
//
//	For example consider two processes A and B, both waiting for both resources X and Y.
//	If Process A manages to acquire X (via a wait on it Mutex) and Process B acquires Y (via its mutex)
//	and then both A and B try to acquire the other resource that is now owned by the other
//	process, a deadlock arises, where each process blocks the other.
//
//	One way around this would be for one or other process to release its currently owned resource
//	and try again some short time later and hope that it then successful in acquiring
//	both resources, but you can never be sure and its not a good solution in real-time systems
//
//	Windows therefore provides this function to allow a thread/process to simply wait until
//	all the resources it requires are free, and then grab them all in one indivisible
//	operation, that is, no other thread can grab half of them while this is going on
//
//	In essence, the function simply requires an array of HANDLES. These handles 
//	are the handles to objects such as mutex's, semaphores, events etc that protect a required resource.
//	If the objects have been signalled, then they are free and not owned by a thread.
//	This function will wait until all handles are in their signalled state and then acquire
//	them by setting them back to their non-signalled state (as it you had done a wait on them)
//	thus preventing other threads from acquiring them.
//	Of course you have to remember to release the resource EACH resource when you have finished
//
//	This function returns WAIT_FAILED if there was an error or WAIT_TIMEOUT if the wait operation timed out
//

UINT	WAIT_FOR_MULTIPLE_OBJECTS(	UINT nCount,             // number of handles in the handle array
									CONST HANDLE *lpHandles,  // pointer to the object-handle array
									DWORD Time
								)
{
	UINT Result = WaitForMultipleObjects(nCount, lpHandles, TRUE, Time) ;
	PERR( Result != WAIT_FAILED, string("Cannot Wait for Multiple Objects")) ;	// check for error and print message if appropriate
	return Result;
}


///////////////////////////////////////////////////////////////////////
// Example
//////////////////////////////////////////////////////////////////////
/*

#include <stdio.h>
#include "rt.h"
  
int main()
{
	...
	...
	CSemaphore c1(.....) , c2(.....) ;					// two sempahore objects
	CEvent		e1(.....), e2(.....) ;					// two event objects

	
	HANDLE	hArray[4] = {c1.GetHandle(), c2.GetHandle(), e1.GetHandle(), e2.GetHandle() } ;
	
	WAIT_FOR_MULTIPLE_OBJECTS(4,hArray) ;	// wait for all objects

	...
	...
	return 0 ;
}

*/

//////////////////////////////////////////////////////////////////////////////////////////////////////
//	PIPELINE Functions
//	
//	The following functions are C++ member function for the CPipe class
//	See example usage later, after the code for all the functions has been shown
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	This is the constructor to create a Pipeline object. All you need is the name of the pipeline
//	The size of the pipeline is controlled by the '__PIPESIZE__' constant in the "rt.h" header file
//
//	Note that this function will attempt to create the pipeline and then open it. If the pipeline already exists
//	then it does not create it, it simply opens it
//	
//	In this implementation, the pipeline will be destroyed when both reading and writing process
//	terminate
//


//##ModelId=3DE6123C03AB
CPipe::CPipe(const string &Name, UINT SizeOfPipe) :PipeName(Name)
{
	// check the pipeline meets minimum size requirements of 2 bytes

	if(SizeOfPipe < 1)	{
		printf("Sorry Pipeline size is too small, Minimum is 1 byte.\n") ;	// check for error and print error message as appropriate
		getchar() ;
		exit(0) ;
	}


	const string PipeName = "__PipeLine__" + Name;
	const string PipeDataName = "__PipeLineData__" + Name;
	const string MutexName = "__PipelineMutex__" + Name;
	const string ProdSemaName = "__PipelineProducerSemaphore__" + Name;
	const string ConSemaName = "__PipelineConsumerSemaphore__" + Name;
		
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// now create the pipeline as a small data pool based around the contents of the struct PipeContents 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	hPipe = CreateFileMapping((HANDLE)0xFFFFFFFF, 
						NULL, 
						PAGE_READWRITE,
						0,
						sizeof(PIPECONTROL),
						(char *)(PipeName.c_str())
	) ;
	
	PERR(hPipe != NULL, string("Cannot Make Datapool For Pipeline ") + Name) ;	// check for error and print error message as appropriate
	
	PipePointer = (PIPECONTROL *)MapViewOfFile(
		hPipe,						// file-mapping object to map into 
									// address space
		FILE_MAP_WRITE ,
		0,							// high-order 32 bits of file offset
		0,							// low-order 32 bits of file offset
		0							// number of bytes to map, 0 means all
	) ;


	PERR(PipePointer != NULL, string("Cannot Make Datapool For Pipeline ") + Name) ;	// check for error and print error message as appropriate
	
	if(PipePointer == NULL)		{
		CloseHandle(hPipe) ;	// close datapool handle
		exit(0) ;
	} 


	/////////////////////////////////////////////////////////////////////////////////////////////
	// now a datapool for the data in the pipeline itself
	/////////////////////////////////////////////////////////////////////////////////////////////

	hData = CreateFileMapping((HANDLE)0xFFFFFFFF, 
						NULL, 
						PAGE_READWRITE,
						0,
						sizeof(SizeOfPipe),
						(char *)(PipeDataName.c_str())
	) ;
	
	PERR(hData != NULL, string("Cannot Make Datapool For Pipeline ") + Name) ;	// check for error and print error message as appropriate
	
	DataPointer = (BYTE *)MapViewOfFile(
		hData,						// file-mapping object to map into 
									// address space
		FILE_MAP_WRITE ,
		0,							// high-order 32 bits of file offset
		0,							// low-order 32 bits of file offset
		0							// number of bytes to map, 0 means all
	) ;


	PERR(DataPointer != NULL, string("Cannot Make Datapool For Pipeline ") + Name) ;	// check for error and print error message as appropriate
	
	if(DataPointer == NULL)		{
		CloseHandle(hPipe) ;	// close datapool handle
		CloseHandle(hData) ;
		exit(0) ;
	}


	// create mutex name for this pipeline and create the producer and consumer semaphores

	pMutex = new CMutex(MutexName) ;
	pProdSemaphore = new CSemaphore(ProdSemaName,  0, SizeOfPipe) ;					// create semaphore NOTE value of 0
	pConSemaphore = new CSemaphore(ConSemaName, SizeOfPipe, SizeOfPipe) ;		// create semaphore note value of SizeofPipe

	// now allocate some storage for the datapool and initialise the pointers which are all in the datapool
	// for cross process communication

	pMutex->Wait() ;
	if(PipePointer->Initialised != 0x4afc)	{		// if datapool not initialised
		PipePointer->Initialised = 0x4afc ;			// show as being initialised
		PipePointer->ReadingIndex = 0 ;		
		PipePointer->WritingIndex = 0 ;
		PipePointer->NumBytes = 0 ;
		PipePointer->SizeOfPipe = SizeOfPipe ;
	}
	else	{	// if it is initialised, make sure the size was specified the same in all processes creating it
		PERR( SizeOfPipe == PipePointer->SizeOfPipe, string("Size of Pipeline Name:") + PipeName + string(" Conflicts with size already specified by another process"));	// check for error and print error message as appropriate
		if(SizeOfPipe != PipePointer->SizeOfPipe)	{
			CloseHandle(hPipe) ;	// close datapool handles
			CloseHandle(hData) ;
			exit(0);
		}
	}

	pMutex->Signal() ;
}



//the constant __PIPESIZE__ in rt.h header file, 
//currently set to 1024 bytes

//##ModelId=3DE6123C03B5
CPipe::~CPipe()
{
	pMutex->Wait() ;

	if(PipePointer->NumBytes == 0)	{			// if no data in pipeline
		PipePointer->Initialised = 0 ;			// show pipeline as uninitialised

		BOOL Success = UnmapViewOfFile(PipePointer) ;	// unlink from data pool view
		PERR( Success == TRUE, string("Cannot Destroy Datapool Object for Pipeline: ") + PipeName);	// check for error and print error message as appropriate

		Success = CloseHandle(hPipe) ;	// close handle to pipeline
		PERR( Success == TRUE, string("Cannot Destroy Datapool Object for Pipeline: ") + PipeName);	// check for error and print error message as appropriate

		Success = UnmapViewOfFile(DataPointer) ;	// unlink from data pool view
		PERR( Success == TRUE, string("Cannot Destroy Datapool Object for Pipeline: ") + PipeName);	// check for error and print error message as appropriate

		Success = CloseHandle(hData) ;	// close handle to pipeline
		PERR( Success == TRUE, string("Cannot Destroy Datapool Object for Pipeline: ") + PipeName);	// check for error and print error message as appropriate
	
	}

	pMutex->Signal() ;

	delete pMutex ;
	delete pConSemaphore ;
	delete pProdSemaphore ;
}


//
//	This functions handles writing data to a pipeline. All you need is the address of the programs
//	data that is to be transferred to the pipline, and the size of that data. The write function takes
//	care of the rest. Note that a process/thread writing to a full pipeline will be suspended until
//	the process at the other end of the pipeline reads some out
//

//##ModelId=3DE6123C03CA
BOOL CPipe::Write(void *Data, UINT Size)	// producer process
{
	//  transfer data from application address to pipeline and update the writing pointers

	LPBYTE	Addr = (LPBYTE)(Data) ;		// cast from void to byte pointer
	
	for(UINT i = 0; i < Size; i ++)	{
		pConSemaphore->Wait() ;	// make sure there is space in the pipeline, if not, suspend
		pMutex->Wait() ;		// make sure no other process is using the pipeline, if not grab it

		if(PipePointer->WritingIndex == PipePointer->SizeOfPipe)			// if we have GONE PAST the end of the pipeline buffer
			PipePointer->WritingIndex = 0 ;									// reset it back to the start
		
		DataPointer[(PipePointer->WritingIndex)++] = *(BYTE *)(Addr++) ;	// store next byte
		++ (PipePointer->NumBytes) ;										// Increment count of bytes in pipeline
		
		pMutex->Signal() ;													// release the process/thread blocking mutex
		pProdSemaphore->Signal();											// wake it up after the write operation

	}
	return TRUE ;
}

//
//	This functions handles reading data from a pipeline. All you need is the address of the programs
//	data and the size of that data. The read function takes	care of the rest. 
//	Note that a process/thread reading from an empty pipeline will be suspended until
//	the process at the other end of the pipeline writes some in
//

//##ModelId=3DE6123C03B7
BOOL CPipe::Read(void *Data, UINT Size) 
{

	//  transfer data from pipeline to application and update the reading pointers
	//	if we attempt to overtake the writing process then suspend the thread. 
	//	We will need a Mutex somewhere in here to avoid two processes accessing the pipeline 
	//	at the same instant.

	LPBYTE	Addr = (LPBYTE)(Data) ;								// cast from void to byte pointer

	for(UINT i = 0; i < Size; i ++)	{
		pProdSemaphore->Wait() ;								// make sure there is data to read in the pipeline, if not suspend
		pMutex->Wait() ;										// make sure no other process is using the pipeline, if not grab it

		if(PipePointer->ReadingIndex == PipePointer->SizeOfPipe)			// if we have GONE PAST the end of the pipeline buffer
			PipePointer->ReadingIndex = 0 ;		// reset it back to the start
		
		*(BYTE *)(Addr++) = DataPointer[(PipePointer->ReadingIndex)++] ;		// read and store next byte
		-- (PipePointer->NumBytes) ;							// decrement count of bytes in pipeline
		
		pMutex->Signal() ;									// release the process/thread blocking mutex
		pConSemaphore->Signal() ;								// wake it up after the write operation

	}
	return TRUE ;
}


//
//	This function returns the number of bytes in a pipeline
//

//##ModelId=3DE6123C03D5
UINT CPipe::TestForData() const 
{
	pMutex->Wait() ;						// make sure no other process is using the pipeline, if not grab it
	int NumBytesInPipe =  PipePointer->NumBytes ;
	pMutex->Signal() ;						// make sure no other process is using the pipeline, if not grab it
	return NumBytesInPipe ;
}

//
//	Constructor creates a named datapool object with a 
//specified size

//##ModelId=3DE6123C01CB
CDataPool::CDataPool(const string &Name, UINT size)
	:DataPoolName(Name)
{
	DPInfo.DataPoolHandle = CreateFileMapping((HANDLE)0xFFFFFFFF, 
						NULL, 
						PAGE_READWRITE,
						0,
						size,
						(char *)(Name.c_str())
	) ;
	
	PERR(DPInfo.DataPoolHandle != NULL, string("Cannot Make Datapool: ") + Name) ;	// check for error and print error message as appropriate
	
	DPInfo.DataPoolPointer = MapViewOfFile(
		DPInfo.DataPoolHandle,			// file-mapping object to map into 
									// address space
		FILE_MAP_WRITE ,
		0,							// high-order 32 bits of file offset
		0,							// low-order 32 bits of file offset
		0							// number of bytes to map, 0 means all
	) ;


	PERR(DPInfo.DataPoolPointer != NULL,  string("Cannot Make Datapool: ") + Name) ;	// check for error and print error message as appropriate
	
	if(DPInfo.DataPoolPointer == NULL)	
		CloseHandle(DPInfo.DataPoolHandle) ;	// close datapool handle
}


//
//	This function unlinks from a datapools 'view', when the thread/process has finished using it
//	call this only as many times as you Link to it
//

//##ModelId=3DE6123C01E0
BOOL	CDataPool::Unlink()	const // DataPoolHandle obtained by calling Link_Datapool()
{
	BOOL Success = UnmapViewOfFile(DPInfo.DataPoolPointer) ;	// unlink from data pool view
	PERR( Success == TRUE, string("Cannot UnLink from Datapool: ") + DataPoolName ) ;		// check for error and print error message as appropriate

	Success = CloseHandle(DPInfo.DataPoolHandle) ;
	PERR( Success == TRUE, string("Cannot UnLink from Datapool: ") + DataPoolName ) ;		// check for error and print error message as appropriate

	return Success ;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//	This example makes a datapool and puts value into it
/*

#include <stdio.h>
#include "rt.h"
  
struct test {
		int floor;
		int direction ;
		int floors[1000] ;
} ;

int main()
{
	...
	...
	DATAPOOLINFO dp = MAKE_DATAPOOL("Car1", sizeof(struct test)) ;
	
	// check the validity of the above operation

	if(dp.DataPoolHandle == NULL || dp.DataPoolPointer == NULL)	{
		perror("Cannot make Datapool \n") ;
		EXIT_PROCESS(0) ;
	}

  // now get the datapool pointer from the DATAPOOLINFO object 'dp' and cast it to a 'struct test' pointer
	
	struct test *MyDataPool = (struct test *)(dp.DataPoolPointer) ;


	// put data into the datapool

	MyDataPool->floor = 55 ;
	MyDataPool->direction = 11 ;

	printf("Floor = %d\n", MyDataPool->floor);
	printf("Direction = %d\n", MyDataPool->direction) ; ;

	for(int i = 0; i < 1000; i ++)
		MyDataPool->floors[i] = i ;

	UNLINK_DATAPOOL(dp) ;				// this call indicates the process has finished using the datapool
										// if no other process are using it, then the O.System will remove it
										// automatically.
	return 0 ;
}
////////////////////////////////////////////////////////////////////////////////////////////////*/


////////////////////////Message/Signal Functions///////////////////////////////////////////////
//	Call this function to force windows to create a message queue for the process/thread
//	The queue will only be created the 1st time it is called, so it can be called many times
//	without problem. If you have multiple threads, then each can create its own message queue
//	to deal with messages sent to it
//
//	An example of the use of messages and timers can be seen further down the file after
//	the functions have been discussed
//////////////////////////////////////////////////////////////////////////////////////////////

//##ModelId=3DE6123B035A
CMailbox::CMailbox()
{
	MSG	MessageBuff ;			// tempory buffer for use with PeekMessage Below

	PeekMessage(&MessageBuff, NULL, WM_USER, WM_USER, PM_NOREMOVE) ;
}

//
//	This function will suspend the current thread until the arrival of a message, the thread
//	can then access the message using GET_USER_MESSAGE(). In order to use messages, the thread
//	must have created a message queue. (See Create_User_Message_Queue() above )
//	
//	Once a message has been received, you must read it to remove it 
//

//##ModelId=3DE6123B035B
void CMailbox::WaitForMessage() const
{
	BOOL Success = WaitMessage() ;
	PERR( Success != 0, string("Unable to Wait For Message from Message Queue"))	;
}

//
//	This function looks into the thread message queue to determine if there are any user messages
//	that can be read. If the function returns TRUE, then at least one message exists. In order to use messages, the thread
//	must have created a message queue. (See Create_User_Message_Queue() above )
//
//	Note that the header file 'rt.h' supplies default arguments for the parameters MinMessageValue and MaxMessageValue
//	this means that you do not have to supply any arguments of your own when you call TEST_FOR_MESSAGE(), in which case
//	the function tests to see if ANY message has been sent to the thread regardless of its value.
//
//	However, you can test for tha arrival of messages within a specified range by supplying the min and max values
//	If you supply these arguments, then the function TEST_FOR_MESSAGE() tests for the existence of a message
//	in the message queue that falls within the bounds of Min and MaxMessage Value (inclusive)
//

//##ModelId=3DE6123B0365
BOOL CMailbox::TestForMessage(UINT MinMessageValue, UINT MaxMessageValue) const
{
	MSG	MessageBuff ;
	return PeekMessage(&MessageBuff, NULL, WM_USER+MinMessageValue, WM_USER + MaxMessageValue, PM_NOREMOVE) ;
}

//
//	This function retrives a message from the threads message queue. 
//

//##ModelId=3DE6123B0370
UINT	CMailbox::GetMessage(UINT MinMessageValue, UINT MaxMessageValue) const
{
	MSG	Message ;
	LPMSG	Mess = &Message ;
	BOOL Result = ::GetMessage(Mess, NULL, WM_USER+MinMessageValue, WM_USER+MaxMessageValue) ;
	PERR( Result != -1, string("Could Not Get Message from Message Queue")) ;	// check for error and print error message as appropriate
	return Message.message - WM_USER ;
}


//
//	This function sets up a timer to go off at the specified time interval.
//	The system posts WM_TIMER signals/messages to the thread that sets the timer, thus it's probably
//	a good idea to have a separate thread for each timer that you need, each thread can then
//	carry out one action in repsonse to one timer.
//	
//	In order for a thread to deal with or intercept messages from timers or messages in general
//	it must have created a message queue, see the function CREATE_MESSAGE_QUEUE() or 
//	CMessageQueue class
//
//	Note: A thread can suspend itself waiting for a timer using the WAIT_FOR_TIMER() function, 
//	which removes the timer message from the message queue when it arrives and wakes up the thread
//
//	Note that timers are repetitive and continue to go at the specified time interval
//	until Stopped with the Stop() function
//

//##ModelId=3DE6123C00BC

CTimer::CTimer() 	// Constructor 1: Create the Timer but don't set it running, use SetTimer() later to start timer
	: TimerID(0)	// sets the timer id to 0
{}	


// create a running timer
//	If callbackfn is not supplied or is NULL, then timer 
//posts messages to the threads message queues
//	if callbackfn is supplied, then the callback function 
//is invoked rather than a message being sent to the 
//threads message queue
//##ModelId=3DE6123C00BD
CTimer::CTimer(UINT Timer, TIMERPROC callbackfn) 	// create a running timer that uses call backs rather than messages
	: TimerID(0)
{
	TimerID = SetTimer(Timer, callbackfn) ;
}


//##ModelId=3DE6123C00C7
CTimer::~CTimer() {						// destructor tidies up by killing the timer so that
	if(TimerID != 0)					// no longer posts timer messages to the thread
		StopTimer(); 						// kill timer
}


//	Setup timer to go off at the specified rate in mSec
//	If callbackfn is not supplied or is NULL, then timer 
//posts messages to the threads message queue
//	if callbackfn is supplied, then the callback function 
//is invoked rather than a message being sent to the 
//threads message queue
//##ModelId=3DE6123C00D1
UINT CTimer::SetTimer(UINT Timer, TIMERPROC callbackfn)
{
	if(TimerID != 0)						// if timer already running
		StopTimer() ;						// kill it i.e. stop it
	
	TimerID = ::SetTimer(NULL, 0, Timer, callbackfn) ;
	PERR( TimerID != 0, string("Error Could NOT  Set Timer")) ;	// check for error and print error message as appropriate

	return TimerID ;		// TRUE if timer successfully set, FALSE otherwise
}

//##ModelId=3DE6123C00DB
BOOL CTimer::TestForTimer()  				// returns TRUE if timer gone off and posted message
{
	MSG	MessageBuff ;
	return PeekMessage(&MessageBuff, NULL, WM_TIMER, WM_TIMER, PM_REMOVE) ; 
}

//##ModelId=3DE6123C00D0
BOOL CTimer::StopTimer()		// kills a timer so that it no longer posts timer messages to thread
{ 
	BOOL Success = ::KillTimer(NULL, TimerID) ;
	PERR( Success == TRUE, string("Error Could Not Kill Timer")) ;	// check for error and print error message as appropriate
	TimerID = 0 ;

	return Success ;
}

//##ModelId=3DE6123C00E4
void CTimer::WaitForTimer()
{
	MSG	MessageBuff ;

	do {
		WaitMessage() ;		// for any message
	}while(!GetMessage(&MessageBuff, NULL, WM_TIMER, WM_TIMER)) ; // is it WM_TIMER
}
	

//
//	This function suspends the current thread until the specified time has elapsed
//	This allows other processes with the same priority to run. Co-operative processing
//	can be implemented with the help of this function,as processes voluntarily relinquish
//	control of the CPU. If the delay is 0, the processes simply gives up the remainder of
//	its CPU time. If the value is 'INFINITE', then the process waits forever. 
//	If the delay is somewhere inbetween, the process is suspended for the duration specified
//	sending it a message will not wake it up prematurely.
//

void	SLEEP(UINT	Time) 
{
	Sleep(Time) ;
}

//
//	This function tests the keyboard to see if a key has been pressed. If so, the value TRUE is
//	returned and the thread can read the character without getting suspended using a function
//	such as _getch(), _getche() etc.
//

BOOL	TEST_FOR_KEYBOARD()
{
	return _kbhit() ;
}


//
//	These functions returns handles to a console standard input/output and error device
//	i.e. keyboard, screen and screen respectively. NULL is returned on error
//	For more example of console functions read the Vis C++ Help/Win32 functions by category/console functions
//

HANDLE	GET_STDIN()
{
	return GetStdHandle(STD_INPUT_HANDLE) ;
}

HANDLE	GET_STDOUT()
{
	return GetStdHandle(STD_OUTPUT_HANDLE) ;
}

HANDLE	GET_STDERR()
{
	return GetStdHandle(STD_ERROR_HANDLE) ;
}

//
//	This function pauses the thread/process until user input is entered at the keyboard
//
//	This function returns WAIT_FAILED if there was an error, WAIT_TIMEOUT if the wait operation timed out
//	of WAIT_OBJECT_0 if the operation did in fact wait and then returned
//

UINT WAIT_FOR_CONSOLE_INPUT(HANDLE hEvent, DWORD Time)
{
	UINT	Status = WaitForSingleObject(hEvent, Time) ;
	PERR( Status != WAIT_FAILED, string("Cannot Wait for Console Input")) ;	// check for error and print message if appropriate
	return Status ;
}

//	moves the cursor to the x,y coord on the screen. [0,0] is top left
//	all calls to printf cause output to occur at the current cursor position
//	obviously, the cursor moves with text output operations

void MOVE_CURSOR(int x, int y)
{
	COORD	c = {(short)x, (short)y}  ;
	SetConsoleCursorPosition(GET_STDOUT(), c) ;
}

//
//	These two function turns off/on the cursor so that it is not visible
//	but it still operates and output text still occurs at the current curso
//	position
//

void CURSOR_OFF()
{
	CONSOLE_CURSOR_INFO	cci = {1, FALSE} ;
	SetConsoleCursorInfo(GET_STDOUT(), &cci) ;
}

void CURSOR_ON()
{
	CONSOLE_CURSOR_INFO	cci = {1, TRUE} ;
	SetConsoleCursorInfo(GET_STDOUT(), &cci) ;
}

//
//	These two functions turns on/off reverse video, so that text comes black on white background
//	For more details set the SetConsoleTextAttribute() function in on-line
//	help
//

void REVERSE_ON()
{
	SetConsoleTextAttribute(GET_STDOUT(), 
		BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE) ;
}

void REVERSE_OFF()
{
	SetConsoleTextAttribute(GET_STDOUT(), 
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
}

void CLEAR_SCREEN()
{
	for(int i = 0; i < 50; i ++)
		putchar('\n') ;
}

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

int TEXT_COLOUR(unsigned char foreground, unsigned char background)
{
	if ((foreground>15)||(foreground<0)||(background>15)||(background<0)||(background==foreground))
	{
		return -1;
	}
	int colour=0;
	background=background<<4;
	colour=colour|background|foreground;
	SetConsoleTextAttribute(GET_STDOUT(), colour);
	return 0;
}


void flush(istream &is)		// can be used to flush an input stream, useful for removing operator entered rubbish
{
	is.clear() ;
	streambuf  *ptr = is.rdbuf() ;		// get pointer to stream buffer
	int avail = (int)(ptr->in_avail()) ;			// get number of chars waiting to be read
	is.ignore(avail) ;						// ignore this many characters
	is.clear() ;
}

void PERR(bool bSuccess, string ErrorMessageString)
{		
	UINT LastError = GetLastError() ;

	if(!(bSuccess)) {
		char buff[512] ;
		Beep(500, 100);
		MOVE_CURSOR(0,0) ;
		REVERSE_ON() ;
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, LastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buff, 1024, NULL );
		printf(" Error %d in Process %s: line %d:\n", LastError, __FILE__, __LINE__);
		printf(" Translation: %s Error: %s", buff, ErrorMessageString.c_str()) ;
		REVERSE_OFF() ;
		printf("\n\nPress Return to Continue...") ;
		_getch();
	}
}