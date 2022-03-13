#include "pocl_cl.h"
CL_API_ENTRY cl_program CL_API_CALL POname (clCreateProgramWithBinary) (cl_context context, cl_uint num_devices, const cl_device_id *device_list,const size_t *lengths, const unsigned char **binaries,cl_int *binary_status, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clCreateProgramWithBinary primitive\n ---");    
#endif 
    char id = 0x4E;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL,*ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0,binary_status_=0;
    cl_program result_;
    cl_int errcode_ret_;
    size_t *lengths_ = NULL;
    ptr  = lookup_object(context);
    cl_opencl_object * obj = NULL;
    
    if (ptr != NULL) {
        
        obj = *(cl_opencl_object**)ptr;
        addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
        addr.sin_family = obj->daemon->addr.sin_family;
        addr.sin_port = obj->daemon->addr.sin_port;
        
    } else 
    {
        *errcode_ret = CL_INVALID_VALUE;
        return NULL;
    }
    
    
    size_buffer_data_request = sizeof (cl_context) + sizeof (cl_uint);

    if (lengths != NULL) 
    {
            for (int iii = 0; iii < num_devices; iii++)
                size_buffer_data_request += *(lengths + iii);
    } else 
    {
        lengths_ = malloc(sizeof (size_t) * num_devices);
        for (int iii = 0; iii < num_devices; iii++) 
        {
                char *s = (char*) binaries[iii];
                *(lengths_ + iii) = strlen(s)+1;
                size_buffer_data_request += *(lengths_ + iii);
        }
    }
    if (num_devices > 0) 
    {
        size_buffer_data_request += (sizeof (size_t) * num_devices);
        size_buffer_data_request += (sizeof (cl_device_id) * num_devices);
    }
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

        _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_context), &offset_buffer);
        buffer_data_request += sizeof (cl_context);


        _ccl_memcpy(buffer_data_request, &num_devices, sizeof (cl_uint), &offset_buffer);
        buffer_data_request += sizeof (cl_uint);

        if (num_devices > 0) 
        {
            cl_device_id *ss = malloc(sizeof(cl_device_id)*num_devices);
            for(int ii =0;ii<num_devices;ii++)
            {
                ptr = lookup_object((void*)device_list[ii]);
                obj = *(cl_opencl_object**)ptr;
                ss[ii]=obj->object_remote;
            }
            
            _ccl_memcpy(buffer_data_request, ss, sizeof (cl_device_id) * num_devices, &offset_buffer);
            buffer_data_request += sizeof (cl_device_id) * num_devices;
        }

        if (lengths != NULL) {
            size_t * sss = (size_t*) lengths;
            _ccl_memcpy(buffer_data_request, sss, sizeof (size_t) * num_devices, &offset_buffer);
        } else
        {  _ccl_memcpy(buffer_data_request, lengths_, sizeof (size_t) * num_devices, &offset_buffer);}

        buffer_data_request += sizeof (size_t) * num_devices;

        for (int iii = 0; iii < num_devices; iii++) 
        {
            int d = lengths == NULL ? lengths_[iii] : lengths[iii];
            char *ssss = (char*) binaries[iii];
            _ccl_memcpy(buffer_data_request, ssss, d, &offset_buffer);
            buffer_data_request += d;
        }
        buffer_data_request -= offset_buffer;

        if (lengths == NULL)
            free(lengths_);
        size_buffer_data_reply = sizeof (cl_program) + sizeof (cl_int) + sizeof (int);
        buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local,&addr,  "clCreateProgramWithBinary");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateProgramWithBinary");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateProgramWithBinary") == 1) {
            free(buffer_data_reply);
            if (errcode_ret != NULL)
                *errcode_ret = CL_ERROR_NETWORK;
            closeSocketUdp(&fd);
            return NULL;
        }
        closeSocketUdp(&fd);
#else
        int fd_connect = init_tcp(&fd, &addr, "clCreateProgramWithBinary");
        //header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateProgramWithBinary");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateProgramWithBinary");
        closeSocketTCP(&fd,&fd_connect);
#endif
        
        offset_buffer = 0;
        free(header);
        free(buffer_data_request);

        _ccl_memcpy(&result_, buffer_data_reply, sizeof (cl_program), &offset_buffer);
        buffer_data_reply += sizeof (cl_program);

        _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);
        
        _ccl_memcpy(&binary_status_, buffer_data_reply, sizeof (int), &offset_buffer);
        buffer_data_reply += sizeof (int);
        
        
        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (errcode_ret_ == CL_SUCCESS)
        {
            ptr  = register_object(result_, getDispachPointer(5), obj->daemon);
            obj = *(cl_opencl_object**)ptr;
            result_ = obj->object_local;
        }
        
        if(binary_status!=NULL)
            *binary_status=binary_status_;

    if (errcode_ret != NULL)
        *errcode_ret = errcode_ret_;
#if DEBUG == 0
        puts("--- End execute clCreateProgramWithBinary primitive\n ---");    
#endif 
    return (result_);
}
POsym(clCreateProgramWithBinary)
