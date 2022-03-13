#ifndef COMMON_H
#define COMMON_H
#define _GNU_SOURCE 
#include <search.h>
#include<CL/cl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <CL/cl_egl.h>
#include<CL/cl_gl_ext.h>
#include <GL/gl.h>
#include <pthread.h>
#include <math.h>
#include <sys/syscall.h>
#define NUM_MAX_PLATFORMS 100
#define TIMEOUT_CONNECT 120  
#define MAX_NUM_THREADS   1
#define CL_PLATFORM_HOSTNAME 0x000
#define CL_ERROR_NETWORK 0x001
#define SIZE_TRANSACTION_ID 64
#define FILL_COLOR 64
#define SIZE_HEADER_UDP 96
#define SIZE_HEADER_TCP 5
#define NUM_CALLS 12
#define NUM_CALLS_DEVICES 3
#define STR_BUFFER 32
#define PROTOCOL 1
#define DEBUG 1
#define _ccl_memcpy(dst, src, size_data, total_offset) \
memcpy(dst, src, size_data); \
*total_offset += size_data;
extern pthread_mutex_t subtag_mutex;
extern pthread_mutex_t _omx_send;
extern pthread_mutex_t mutex_tree_objects;
extern pthread_mutex_t tree_done ;
extern pthread_mutex_t protected_daemons;
extern int daemon_count, transaction_nr;
extern int port;
extern char ip_server[16];
extern int mtu;
extern int protocol;
extern int nr;
extern int  isReady;
extern char  * PATH_1;
extern char  * PATH_2;
typedef struct _remote_daemon_ 
{
    struct sockaddr_in addr;
    char ip [16];
    
} remote_daemon;

/*
    Foi necessário adicionar  o campo ptr_remote e o ptr_local pois as vezes é preciso guardar campos auxiliares, por exemplo o mapp_ptr no createBuffer
 */

typedef struct _cl_descriptor_object_ 
{
    char key[32];
    int descriptor;
    
} cl_descriptor_object;

typedef struct _cl_info_udp_object_ 
{
    char key[32];
    int descriptor;
    struct sockaddr_in host_socket;
    struct sockaddr_in rDaemon_socket;
    
} cl_info_udp_object;

#include"network.h"
#include"storage.h"

extern void * tree_objects;
extern void * tree_descriptores;
extern remote_daemon * daemons;
extern void * tree_info_udp;
typedef struct _cl_opencl_object_ 
{
    remote_daemon * daemon;
    void *object_local;
    void *object_remote;
    void * ptr_remote;
    void * ptr_local;
    
} cl_opencl_object;
void * clCallbackclSetEventCallck(void*arg);
int init_hostprogram();
int _ccl_connect_to_daemons();
void _ccl_perror_and_exit(char*str);
void _ccl_check_size_data(int real, int theorical, char *primitives, int operation);
int _ccl_equals_addr(struct sockaddr_in * s1, struct sockaddr_in * s2);
#endif /* COMMON_H */
