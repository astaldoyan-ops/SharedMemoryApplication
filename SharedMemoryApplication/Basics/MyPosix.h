#pragma once

#include <sys/stat.h>
#include <fcntl.h>

class pthread_mutex_t {};
int pthread_mutex_lock( pthread_mutex_t* mutex );
int pthread_mutex_unlock( pthread_mutex_t* mutex );

class mode_t { public: mode_t( int ) {} };
int shm_open( const char* name, int oflag, mode_t mode );
void* mmap(
    void* addr, size_t len, int prot, int flags,
    int fildes, off_t off = 0
);
int PROT_READ = 0, MAP_PRIVATE = 1;
void* MAP_FAILED = nullptr;

int munmap( void* addr, size_t length );