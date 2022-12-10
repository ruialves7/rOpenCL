#include "pocl_cl.h"
#include <stdio.h>
#include <sys/time.h>
CL_API_ENTRY cl_int CL_API_CALL
POname(clSetKernelArg)(cl_kernel kernel, cl_uint arg_index, size_t arg_size, const void *arg_value) CL_API_SUFFIX__VERSION_1_0
{

#if DEBUG == 0
struct timeval t0, t1;
    puts("--- Start execute clSetKernelArg primitive\n ---");    
#endif
    char id = 0x0E;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, * ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, offset_buffer = 0;

    ptr = lookup_object(kernel);
    cl_opencl_object * obj = NULL;
    if (ptr != NULL) 
    {
       obj = *(cl_opencl_object**)ptr;
       memcpy(&addr,&obj->daemon->addr,sizeof(struct sockaddr_in));
       
    } else 
    {
        return CL_INVALID_VALUE;
    }

    struct {
        cl_kernel kernel;
        cl_uint arg_index;
        size_t arg_size;
	char arg_value_is_null;
	uintptr_t ptr;
        char is_ptr;
  }ccl_request ={.kernel=obj->object_remote,.arg_index=arg_index,.arg_size=arg_size,.arg_value_is_null=(arg_value==NULL)};

    struct {
        cl_int result;
    }ccl_reply;

        size_buffer_data_request = sizeof (ccl_request);
        if (arg_value!=NULL)
            size_buffer_data_request += arg_size;
        
        
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

        //TODO Problem
        if (arg_value!=NULL)
        {
            cl_mem * mem = (cl_mem*)arg_value;
            void *ss = (void*) arg_value;
            ptr = lookup_object(*mem);
            
            //cl_opencl_object * obj = NULL;
            if (ptr != NULL) 
            { 
               cl_opencl_object * obj1 = *(cl_opencl_object**)ptr;
               ccl_request.is_ptr='1';

                memcpy(&ccl_request.ptr,&obj1->object_remote,sizeof(uintptr_t));
               _ccl_memcpy(buffer_data_request, &ccl_request, sizeof (ccl_request), &offset_buffer);
                buffer_data_request += sizeof (ccl_request);
		
            } else 
            {

                ccl_request.is_ptr='0';
                _ccl_memcpy(buffer_data_request, &ccl_request, sizeof (ccl_request), &offset_buffer);
                buffer_data_request += sizeof (ccl_request);
                   
                _ccl_memcpy(buffer_data_request, ss, arg_size, &offset_buffer);
                buffer_data_request += arg_size;
             }
        }
	
        buffer_data_request -= offset_buffer;

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr,"clSetKernelArg");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        void * header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clSetKernelArg");
        free(id_transaction);
        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clSetKernelArg") == 1) 
        {
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;
        }
       closeSocketUdp(&fd);
#else
        int fd_connect = init_tcp(&fd, &addr, "clSetKernelArg");
        send_data_tcp(&fd, buffer_data_request, NULL, size_buffer_data_request, "clSetKernelArg");
        recv_data_tcp(&fd, &ccl_reply, sizeof(ccl_reply), "clSetKernelArg");
        closeSocketTCP(&fd,&fd_connect);
#endif

        free(buffer_data_request);
        

        
#if DEBUG == 0
        puts("--- End execute clSetKernelArg primitive\n ---");    
#endif
         return (ccl_reply.result);
    
}
POsym(clSetKernelArg)
