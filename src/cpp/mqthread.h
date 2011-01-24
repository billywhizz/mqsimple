//Headers
#if defined(LINUX)
	#include <pthread.h>
#elif defined(WINDOWS)
	#include <windows.h>
	#include <process.h>
#endif

//Data types
#if defined(LINUX)
	#define MUTEX pthread_mutex_t
#elif defined(WINDOWS)
	#define MUTEX HANDLE
#endif

//Functions
int MUTEX_INIT(MUTEX *mutex);
int MUTEX_LOCK(MUTEX *mutex);
int MUTEX_UNLOCK(MUTEX *mutex);
