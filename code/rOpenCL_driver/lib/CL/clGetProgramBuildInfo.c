#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clGetProgramBuildInfo)(cl_program program,cl_device_id device,cl_program_build_info param_name,size_t param_value_size, void * param_value,size_t * param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clGetProgramBuildInfo primitive\n ---");    
#endif
    char id = 0x5A;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *param_value_ = NULL, *ptr = NULL;
    int fd = 0;size_t size_buffer_data_request = 0;int size_buffer_data_reply = 0, offset_buffer = 0;

    cl_int result_;
    size_t param_value_size_ret_;
    cl_opencl_object * obj = NULL;
    ptr = lookup_object(program);
    
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

        size_buffer_data_request = sizeof (cl_program) + sizeof (cl_device_id) + sizeof (cl_program_build_info) + sizeof (size_t);
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

        _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_program), &offset_buffer);
        buffer_data_request += sizeof (cl_program);
        
        ptr = lookup_object(device);
        if (ptr != NULL) {
            obj = *(cl_opencl_object**)ptr;
        } else {
            return CL_INVALID_VALUE;
        }
    
        _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_device_id), &offset_buffer);
        buffer_data_request += sizeof (cl_device_id);

        _ccl_memcpy(buffer_data_request, &param_name, sizeof (cl_program_build_info), &offset_buffer);
        buffer_data_request += sizeof (cl_program_build_info);

        _ccl_memcpy(buffer_data_request, &param_value_size, sizeof (size_t), &offset_buffer);
        buffer_data_request += sizeof (size_t);

        buffer_data_request -= offset_buffer;

        size_buffer_data_reply = sizeof (cl_int) + sizeof (size_t);
        if(param_value_size>0)
            size_buffer_data_reply+=param_value_size;
        
        buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clGetProgramBuildInfo");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetProgramBuildInfo");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetProgramBuildInfo") == 1) 
        {
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;
        }
        closeSocketUdp(&fd);
#else
        int fd_connect = init_tcp(&fd, &addr, "clGetProgramBuildInfo");
        //header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clGetProgramBuildInfo");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetProgramBuildInfo");
        closeSocketTCP(&fd,&fd_connect);
#endif
        
        offset_buffer = 0;
        free(header);
        free(buffer_data_request);
        
        _ccl_memcpy(&result_, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        _ccl_memcpy(&param_value_size_ret_, buffer_data_reply, sizeof (size_t), &offset_buffer);
        buffer_data_reply += sizeof (size_t);
        
        if(param_value_size>0)
        {
            param_value_ = malloc(param_value_size);
            _ccl_memcpy(param_value_, buffer_data_reply, param_value_size_ret_,&offset_buffer);
            buffer_data_reply += (param_value_size_ret_);
        }

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (param_value != NULL && result_ == CL_SUCCESS) 
        {
            memcpy(param_value, param_value_, param_value_size_ret_);
            free(param_value_);
        }
        
        if (param_value_size_ret != NULL)
        *param_value_size_ret = param_value_size_ret_;
#if DEBUG == 0
        puts("--- End execute clGetProgramBuildInfo primitive\n ---");    
#endif
        return (result_);
}
POsym(clGetProgramBuildInfo)
