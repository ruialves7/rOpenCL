
#ifndef STORAGE_H
#define STORAGE_H
#include"common.h"

int compare(const void *pa, const void *pb);
void * register_object(void *object_remote, void *object_local, remote_daemon * daemon);
void * lookup_object(void * object_local);
void * delete_object(void * object_local);

#endif 
