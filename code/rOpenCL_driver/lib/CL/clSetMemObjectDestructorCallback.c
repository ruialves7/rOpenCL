#include "pocl_cl.h"

CL_API_ENTRY cl_int CL_API_CALL
POname(clSetMemObjectDestructorCallback)(cl_mem  memobj,void (CL_CALLBACK * pfn_notify)( cl_mem , void*), void * user_data)           CL_API_SUFFIX__VERSION_1_1
{
#if DEBUG == 0
        puts("--- Start execute clSetMemObjectDestructorCallback primitive\n ---");    
#endif
    char id = 0x46;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, * ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    ptr = lookup_object(memobj);
    cl_int result;
    cl_opencl_object * obj = NULL;
    if (ptr != NULL) 
    {
       obj = *(cl_opencl_object**)ptr;
       addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;
       
    } else 
    {
        return CL_INVALID_VALUE;
    }
        int size_user = 0;
        if (user_data != NULL)
            size_user = strlen((char*) user_data);

        size_buffer_data_request = sizeof (cl_mem);
        if (size_user > 0)
            size_buffer_data_request += size_user;
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

        _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_mem), &offset_buffer);
        buffer_data_request += sizeof (cl_mem);


        _ccl_memcpy(buffer_data_request, &size_user, sizeof (int), &offset_buffer);
        buffer_data_request += sizeof (int);

        if (size_user > 0) {
            _ccl_memcpy(buffer_data_request, user_data, size_user, &offset_buffer);
            buffer_data_request += size_user;
        }
        buffer_data_request -= offset_buffer;

        size_buffer_data_reply = sizeof (cl_int);
        buffer_data_reply = malloc(size_buffer_data_reply);
        
#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clSetMemObjectDestructorCallback");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clSetMemObjectDestructorCallback");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clSetMemObjectDestructorCallback") == 1) {
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;
        }
        closeSocketUdp(&fd);
#else

        int fd_connect = init_tcp(&fd, &addr, "clSetMemObjectDestructorCallback");
        //header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clSetMemObjectDestructorCallback");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clSetMemObjectDestructorCallback");
        closeSocketTCP(&fd,&fd_connect);
#endif
        offset_buffer = 0;
        free(header);
        free(buffer_data_request);

        _ccl_memcpy(&result, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);
#if DEBUG == 0
        puts("--- End execute clSetMemObjectDestructorCallback primitive\n ---");    
#endif
    return (result);
}
POsym(clSetMemObjectDestructorCallback)
