#include "pocl_cl.h"
CL_API_ENTRY cl_program CL_API_CALL
POname (clLinkProgram) (cl_context context,cl_uint num_devices,const cl_device_id *device_list,const char *options,cl_uint num_input_programs,
const cl_program *input_programs,void (CL_CALLBACK *pfn_notify) (cl_program program, void *user_data),void *user_data,cl_int *errcode_ret)CL_API_SUFFIX__VERSION_1_2
{
#if DEBUG == 0
        puts("--- Start execute clLinkProgram primitive\n ---");    
#endif
    char id = 0x57;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_program result_;
    cl_int errcode_ret_;
    cl_opencl_object * obj = NULL;
    ptr = lookup_object(context);

    if (ptr != NULL) {
        
       obj = *(cl_opencl_object**)ptr;
       addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;
        
    } else {
        if (errcode_ret != NULL)
            *errcode_ret = CL_INVALID_VALUE;
        return NULL;
    }

        size_buffer_data_request = sizeof (cl_context) + 3 * sizeof (cl_uint);

        if (num_devices > 0)
            size_buffer_data_request += sizeof (cl_device_id) * num_devices;

        cl_uint size_options = 0;

        if (options != NULL)
            size_options = strlen(options);

        if (size_options > 0)
            size_buffer_data_request += size_options;

        if (num_input_programs > 0) {

            size_buffer_data_request += sizeof (cl_program) * num_input_programs;
            size_buffer_data_request += sizeof (size_t) * num_input_programs;
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

        _ccl_memcpy(buffer_data_request, &num_input_programs, sizeof (cl_uint), &offset_buffer);
        buffer_data_request += sizeof (cl_uint);

        _ccl_memcpy(buffer_data_request, &size_options, sizeof (cl_uint), &offset_buffer);
        buffer_data_request += sizeof (cl_uint);

        if (num_devices > 0) 
        {
            cl_device_id *ss = malloc(sizeof(cl_device_id)*num_devices);
            for(int ii =0;ii<num_devices;ii++)
            {
                ptr = lookup_object(device_list[ii]);
                obj = *(cl_opencl_object**)ptr;
                ss[ii]=obj->object_remote;
            }
               
            
            _ccl_memcpy(buffer_data_request, ss, sizeof (cl_device_id) * num_devices, &offset_buffer);
            buffer_data_request += sizeof (cl_device_id) * num_devices;
        }

        if (size_options > 0) {
            char *ss = (char*) options;
            _ccl_memcpy(buffer_data_request, ss, size_options, &offset_buffer);
            buffer_data_request += size_options;
        }

        if (num_input_programs > 0) {

            cl_program *p = (cl_program*) input_programs;
            _ccl_memcpy(buffer_data_request, p, sizeof (cl_program) * num_input_programs, &offset_buffer);
            buffer_data_request += sizeof (cl_program) * num_input_programs;
        }
        buffer_data_request -= offset_buffer;

        size_buffer_data_reply = sizeof (cl_program) + sizeof (cl_int);
        buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr,"clLinkProgram");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clLinkProgram");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clLinkProgram") == 1) {
            free(buffer_data_reply);
            if (errcode_ret != NULL)
                *errcode_ret = CL_ERROR_NETWORK;
            closeSocketUdp(&fd);
            return NULL;
        }
        closeSocketUdp(&fd);
#else
        int fd_connect = init_tcp(&fd, &addr, "clLinkProgram");
       // header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clLinkProgram");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clLinkProgram");
        closeSocketTCP(&fd,&fd_connect);
#endif
        
        offset_buffer = 0;
        free(header);
        free(buffer_data_request);

        _ccl_memcpy(&result_, buffer_data_reply, sizeof (cl_program), &offset_buffer);
        buffer_data_reply += sizeof (cl_program);

        _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

       if(errcode_ret_ == CL_SUCCESS)
        {
             ptr  = register_object(result_,getDispachPointer(5), obj->daemon);
             obj = *(cl_opencl_object**)ptr;
             result_ = obj->object_local;    
        }

        
    if (errcode_ret != NULL)
        *errcode_ret = errcode_ret_;
#if DEBUG == 0
        puts("--- End execute clLinkProgram primitive\n ---");    
#endif
    return (result_);
    
}
POsym (clLinkProgram)
