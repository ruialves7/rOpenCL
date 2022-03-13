#ifndef COMMON_H
#define COMMON_H

#define CL_TARGET_OPENCL_VERSION 120
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
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <CL/cl_egl.h>
#include<CL/cl_gl_ext.h>
#include <GL/gl.h>
#include <pthread.h>
#include <math.h>
#include"network.h"
#include"storage.h"
#include <netinet/tcp.h>
#define MAX_UDP_PACKET_SIZE 1468
#define MAX_NUM_THREADS  2048
#define CL_PLATFORM_HOSTNAME 0x000
#define CL_ERROR_NETWORK 0x001
#define LOCAL_INTERFACE "192.168.45.50"
#define PORT 1050
#define SIZE_TRANSACTION_ID 64
#define FILL_COLOR 64
#define SIZE_HEADER_UDP 96
#define SIZE_HEADER_TCP 5
#define SIZE_HOST_PROGRAM 1000
#define BUFFER_DATA 2147483647
#define STR_BUFFER 32
#define DEBUG 0
#define SINGLE_THREAD 1
#define _ccl_memcpy(dst, src, size_data, total_offset) \
memcpy(dst, src, size_data); \
*total_offset += size_data;

extern int lock_primitives[256];
//extern void * tree_transaction_pendents;
extern void * tree_transaction_done;
extern void * tree_command_queues;
extern pthread_rwlock_t lock_mutex;
extern cl_device_id * devices;
extern pthread_rwlock_t *mutexs;
extern cl_uint totaldevices;

typedef struct _cl_transaction_ 
{
    char id_transaction [SIZE_TRANSACTION_ID];
    char id;
    void * data;
    int fd;
    int packets_received;

} available_primitive_udp;

typedef struct _cl_command_queue_object_ 
{
    int index_mutex;
    cl_command_queue queue;
} cl_command_queue_object;

typedef struct _primitive_ 
{
    void * data;
    int fd;
    char id;
} ropencl_primitive_request;

extern int fd, primitive_available_udp, primitive_available_tcp, fd_tcp, counter;
extern struct sockaddr_in addr, addr_tcp, cli_tcp;
extern pthread_mutex_t mutex_ts;
extern pthread_mutex_t mutex_udp;
extern pthread_rwlock_t tree_done;
extern pthread_cond_t cond_udp;
extern pthread_t dispatcher_thread[MAX_NUM_THREADS];

void init_daemon();
void _ccl_perror_and_exit(char*str);
void _ccl_check_size_data(int real, int theorical, char *primitives, int operation);
int _ccl_equals_addr(struct sockaddr_in * s1, struct sockaddr_in * s2);
//void _ccl_memcpy(void * dst, void *src, int size_data, int * total_offset);
int compare_network(const void *pa, const void *pb);
int compare(const void *pa, const void *pb);
int findDevice(cl_device_id device);
int compare_commandqueue(const void *pa, const void *pb);

#endif 




