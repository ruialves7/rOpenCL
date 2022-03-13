#include "pocl_cl.h"

CL_API_ENTRY cl_int CL_API_CALL
POname(clRetainEvent)(cl_event  event ) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clRetainEvent primitive\n ---");    
#endif
    char id = 0x68;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, * ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, offset_buffer = 0;

    ptr = lookup_object(event);
    
    cl_opencl_object * obj = NULL;
    
    if (ptr != NULL) {
        
       obj = *(cl_opencl_object**)ptr;
       addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;
       
    } else {
        return CL_INVALID_VALUE;
    }

    struct
    {
        cl_event event;
    }ccl_request = {.event = obj->object_remote};

    struct {
        cl_int result;
    }ccl_reply;

        size_buffer_data_request = sizeof (ccl_request);
#if PROTOCOL == 1
    size_buffer_data_request+=SIZE_HEADER_TCP;
#endif

    buffer_data_request = malloc(size_buffer_data_request);

#if PROTOCOL == 1

    _ccl_memcpy(buffer_data_request, &id, sizeof (char), &offset_buffer);
    buffer_data_request += sizeof (char);

    _ccl_memcpy(buffer_data_request, &size_buffer_data_request, sizeof (int), &offset_buffer);
    buffer_data_request += sizeof (int);

#endif

        _ccl_memcpy(buffer_data_request, &ccl_request, sizeof (ccl_request), &offset_buffer);
        buffer_data_request += sizeof (ccl_request);
        buffer_data_request -= offset_buffer;

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clRetainEvent");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        void * header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clRetainEvent");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clRetainEvent") == 1) {
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;
        }
        closeSocketUdp(&fd);
#else
        int fd_connect = init_tcp(&fd, &addr, "clRetainEvent");
        send_data_tcp(&fd, buffer_data_request, NULL, size_buffer_data_request, "clRetainEvent");
        recv_data_tcp(&fd, &ccl_reply, sizeof(ccl_reply), "clRetainEvent");
        closeSocketTCP(&fd,&fd_connect);
#endif
        free(buffer_data_request);

    
#if DEBUG == 0
        puts("--- End execute clRetainEvent primitive\n ---");    
#endif
    return (ccl_reply.result);
  
}

POsym(clRetainEvent)