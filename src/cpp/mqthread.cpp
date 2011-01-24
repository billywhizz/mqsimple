#include "mqthread.h"

int MUTEX_INIT(MUTEX *mutex)
{
    #if defined(LINUX)
		return pthread_mutex_init (mutex, NULL);;
    #elif defined(WINDOWS)
        *mutex = CreateMutex(0, FALSE, 0);;
        return (*mutex==0);
    #endif
    return -1;
}

int MUTEX_LOCK(MUTEX *mutex)
{
    #if defined(LINUX)
        return pthread_mutex_lock( mutex );
    #elif defined(WINDOWS)
        return (WaitForSingleObject(*mutex, INFINITE)==WAIT_FAILED?1:0);
    #endif
    return -1;
}

int MUTEX_UNLOCK(MUTEX *mutex)
{
    #if defined(LINUX)
        return pthread_mutex_unlock( mutex );
    #elif defined(WINDOWS)
        return (ReleaseMutex(*mutex)==0);
    #endif
    return -1;
}
