#include "pocl_cl.h"

CL_API_ENTRY cl_int CL_API_CALL
POname(clGetEventProfilingInfo)(cl_event event,cl_profiling_info param_name,size_t param_value_size,void *param_value,
size_t *param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
    
#if DEBUG == 0
        puts("--- Start execute clGetEventProfilingInfo primitive\n ---");    
#endif
    char id = 0x10;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL,*ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    ptr = lookup_object(event);
    cl_int result;
    cl_opencl_object * obj = NULL;
    size_t param_value_size_ret_;
    char * param_value_ = NULL;
    
    if (ptr  != NULL) 
    {
        obj = *(cl_opencl_object**)ptr;
        addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;
      //memcpy(&addr,&obj->daemon->addr, sizeof(struct sockaddr_in));
        
    } else 
    {
        return CL_INVALID_VALUE;
    }

        size_buffer_data_request = sizeof (cl_event) + sizeof (cl_profiling_info) + sizeof (size_t);
#if PROTOCOL == 1
    size_buffer_data_request+=SIZE_HEADER_TCP;
#endif

    buffer_data_request = malloc(size_buffer_data_request);

#if PROTOCOL == 1

    _ccl_memcpy(buffer_data_request, &id, sizeof (char), &offset_buffer);
    buffer_data_request += sizeof (char);

    _ccl_memcpy(buffer_data_request, &size_buffer_data_request, sizeof (size_t), &offset_buffer);
    buffer_data_request += sizeof (size_t);

#endif

        _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_event), &offset_buffer);
        buffer_data_request += sizeof (cl_event);


        _ccl_memcpy(buffer_data_request, &param_name, sizeof (cl_profiling_info), &offset_buffer);
        buffer_data_request += sizeof (cl_profiling_info);


        _ccl_memcpy(buffer_data_request, &param_value_size, sizeof (size_t), &offset_buffer);
        buffer_data_request += sizeof (size_t);

        buffer_data_request -= offset_buffer;
        size_buffer_data_reply = sizeof (size_t) + sizeof (cl_int) + sizeof (char)*param_value_size;
        buffer_data_reply = malloc(size_buffer_data_reply);
        
#if PROTOCOL == 0
        
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clGetEventProfilingInfo");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetEventProfilingInfo");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetEventProfilingInfo") == 1) {
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;
        }
        closeSocketUdp(&fd);
#else

        int fd_connect = init_tcp(&fd, &addr, "clGetEventProfilingInfo");
        //header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clGetEventProfilingInfo");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetEventProfilingInfo");
        closeSocketTCP(&fd,&fd_connect);
#endif
        
        offset_buffer = 0;
        free(header);
        free(buffer_data_request);

        _ccl_memcpy(&result, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        _ccl_memcpy(&param_value_size_ret_, buffer_data_reply, sizeof (size_t), &offset_buffer);
        buffer_data_reply += sizeof (size_t);

        param_value_ = malloc(param_value_size_ret_);

        _ccl_memcpy(param_value_, buffer_data_reply, param_value_size_ret_, &offset_buffer);
        buffer_data_reply += param_value_size_ret_;

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (result == CL_SUCCESS) 
        {
            if (param_value_size_ret != NULL)
                *param_value_size_ret = param_value_size_ret_;
            if (param_value != NULL) 
            {
                memcpy(param_value, param_value_, param_value_size_ret_);
                free(param_value_);
            }
        }
#if DEBUG == 0
        puts("--- End execute clGetEventProfilingInfo primitive\n ---");    
#endif
    return (result);
}
POsym(clGetEventProfilingInfo)
