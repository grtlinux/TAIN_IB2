/*
 * PROGRAM ID   : Sig.h
 * PROGRAM NAME : Signal 처리를 위해 사용하는 모듈 헤더
 * AUTHOR       : (주)ICA
 * COMMENT      : Signal 처리를 위해 사용하는 모듈 헤더
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 *  // Fake signal functions.
 *  #define SIG_ERR ((__sighandler_t) -1)           // Error return.
 *  #define SIG_DFL ((__sighandler_t) 0)            // Default action.
 *  #define SIG_IGN ((__sighandler_t) 1)            // Ignore signal.
 *
 *  #ifdef __USE_UNIX98
 *  # define SIG_HOLD       ((__sighandler_t) 2)    // Add signal to hold mask.
 *  #endif
 *
 *  // Signals.
 *  #define SIGHUP          1       // Hangup (POSIX).
 *  #define SIGINT          2       // Interrupt (ANSI).
 *  #define SIGQUIT         3       // Quit (POSIX).
 *  #define SIGILL          4       // Illegal instruction (ANSI).
 *  #define SIGTRAP         5       // Trace trap (POSIX).
 *  #define SIGABRT         6       // Abort (ANSI).
 *  #define SIGIOT          6       // IOT trap (4.2 BSD).
 *  #define SIGBUS          7       // BUS error (4.2 BSD).
 *  #define SIGFPE          8       // Floating-point exception (ANSI).
 *  #define SIGKILL         9       // Kill, unblockable (POSIX).
 *  #define SIGUSR1         10      // User-defined signal 1 (POSIX).
 *  #define SIGSEGV         11      // Segmentation violation (ANSI).
 *  #define SIGUSR2         12      // User-defined signal 2 (POSIX).
 *  #define SIGPIPE         13      // Broken pipe (POSIX).
 *  #define SIGALRM         14      // Alarm clock (POSIX).
 *  #define SIGTERM         15      // Termination (ANSI).
 *  #define SIGSTKFLT       16      // Stack fault.
 *  #define SIGCLD          SIGCHLD // Same as SIGCHLD (System V).
 *  #define SIGCHLD         17      // Child status has changed (POSIX).
 *  #define SIGCONT         18      // Continue (POSIX).
 *  #define SIGSTOP         19      // Stop, unblockable (POSIX).
 *  #define SIGTSTP         20      // Keyboard stop (POSIX).
 *  #define SIGTTIN         21      // Background read from tty (POSIX).
 *  #define SIGTTOU         22      // Background write to tty (POSIX).
 *  #define SIGURG          23      // Urgent condition on socket (4.2 BSD).
 *  #define SIGXCPU         24      // CPU limit exceeded (4.2 BSD).
 *  #define SIGXFSZ         25      // File size limit exceeded (4.2 BSD).
 *  #define SIGVTALRM       26      // Virtual alarm clock (4.2 BSD).
 *  #define SIGPROF         27      // Profiling alarm clock (4.2 BSD).
 *  #define SIGWINCH        28      // Window size change (4.3 BSD, Sun).
 *  #define SIGPOLL         SIGIO   // Pollable event occurred (System V).
 *  #define SIGIO           29      // I/O now possible (4.2 BSD).
 *  #define SIGPWR          30      // Power failure restart (System V).
 *  #define SIGSYS          31      // Bad system call.
 *  #define SIGUNUSED       31
 *
 *
 */

#ifndef _SIG_H_
#define _SIG_H_

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/*
 * decleration
 */
/*----------------------------------------------------------------------------*/
void   SigAct_SIGUSR1         ( int signum );
void   SigSet_SIGUSR1         ( );
void   SigAct_SIGALRM         ( int signum );
void   SigAct_SIGALRM_1       ( int signum );
void   SigAct_SIGALRM_2       ( int signum );
void   SigSet_SIGALRM         ( );
void   SigSet_SIGALRM_Param   ( void (*sigfunc) (int) );



#endif




