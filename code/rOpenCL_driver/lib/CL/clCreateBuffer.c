#include "pocl_cl.h"
CL_API_ENTRY cl_mem CL_API_CALL
POname(clCreateBuffer)(cl_context  context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
puts("--- Start execute clCreateBuffer primitive\n ---");    
#endif
        char id = 0x08;
        struct sockaddr_in addr;
        void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
        int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
 
        cl_mem result_;
        cl_int errcode_ret_;
        ptr = lookup_object(context);
        cl_opencl_object * obj = NULL;
         
        if (ptr != NULL) 
        {
            obj = *(cl_opencl_object**)ptr;
            memcpy(&addr,obj->daemon,sizeof(struct sockaddr_in));
        } else {
            
            *errcode_ret = CL_INVALID_VALUE;
            return NULL;
        }
        
        struct
        {
            cl_context context;
            cl_mem_flags flags;
            size_t size;
            char host_ptr_is_null;
        }ccl_request={.context=obj->object_remote,.flags=flags,.size=size, .host_ptr_is_null=(host_ptr == NULL)};
        
        struct
        {
            cl_mem result;
            cl_int errcode_ret;
            uintptr_t ptr_remote;
        }ccl_reply;
 
        size_buffer_data_request = sizeof (ccl_request);
        if(host_ptr!=NULL)
        {    
            size_buffer_data_request+=size;
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
        
        _ccl_memcpy(buffer_data_request, &ccl_request, sizeof (ccl_request), &offset_buffer);
        buffer_data_request += sizeof (ccl_request);

        if(!ccl_request.host_ptr_is_null)
        {
            _ccl_memcpy(buffer_data_request, host_ptr, size, &offset_buffer);
            buffer_data_request += size;
        }
    
        buffer_data_request -= offset_buffer;

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local,&addr,  "clCreateBuffer");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateBuffer");
        free(id_transaction);

        if (recv_data_udp(&fd, &ccl_reply, sizeof(ccl_reply), "clCreateBuffer") == 1) {
            if (errcode_ret != NULL)
                *errcode_ret = CL_ERROR_NETWORK;
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return NULL;
        }
         closeSocketUdp(&fd);
#else
        int fd_connect = init_tcp(&fd, &addr, "clCreateBuffer");
        //header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateBuffer");
        recv_data_tcp(&fd, &ccl_reply, sizeof(ccl_reply), "clCreateBuffer");
        closeSocketTCP(&fd,&fd_connect);
#endif
        free(header);
        free(buffer_data_request);
 
        if (ccl_reply.errcode_ret == CL_SUCCESS)
        {
            ptr  = register_object(ccl_reply.result, getDispachPointer(4), obj->daemon);
            obj = *(cl_opencl_object**)ptr;
            ccl_reply.result = obj->object_local;
            obj->ptr_local=host_ptr;
            
            if(!ccl_request.host_ptr_is_null)
            {
                obj->ptr_remote= malloc(sizeof(uintptr_t));
                memcpy(obj->ptr_remote, &ccl_reply.ptr_remote,sizeof(uintptr_t));
            }
            
        }
  
        if (errcode_ret != NULL)
            *errcode_ret = ccl_reply.errcode_ret;
        
#if DEBUG == 0
puts("--- End execute clCreateBuffer primitive\n ---");    
#endif
        return (ccl_reply.result);
}
POsym(clCreateBuffer)
