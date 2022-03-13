#include "pocl_cl.h"

CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueNDRangeKernel)(cl_command_queue command_queue, cl_kernel kernel,cl_uint work_dim,const size_t *global_work_offset,const size_t *global_work_size,const size_t *local_work_size,cl_uint num_events_in_wait_list,const cl_event *event_wait_list,cl_event *event) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueNDRangeKernel primitive\n ---");    
#endif
    char id = 0x0F;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0, *ptr = NULL;

    cl_int result_;
    cl_event event_;
    cl_opencl_object * obj = NULL;
    ptr = lookup_object(command_queue);

    if (ptr != NULL) 
    {
          obj = *(cl_opencl_object**)ptr;
          memcpy(&addr,obj->daemon,sizeof(struct sockaddr_in));

    } else {
        return CL_INVALID_VALUE;
    }
    struct
        {
            cl_event event;
            cl_int result;
        }ccl_reply;
    
    struct
    {
           cl_command_queue command_queue;
           cl_kernel kernel;
           cl_uint work_dim;
           cl_uint num_events_in_wait_list;
           char event_is_null;
           char local_work_size_is_null;
           char global_work_offset_is_null;
    }ccl_request={.global_work_offset_is_null = (global_work_offset == NULL),.local_work_size_is_null = (local_work_size == NULL),.command_queue=obj->object_remote, .work_dim=work_dim, .num_events_in_wait_list=num_events_in_wait_list, .event_is_null=(event== NULL)};
    
    ptr = lookup_object(kernel);
    if (ptr != NULL) 
    {
          obj = *(cl_opencl_object**)ptr;
    } else {
          return CL_INVALID_VALUE;
    }
    
    ccl_request.kernel=obj->object_remote;
    size_buffer_data_request = sizeof(ccl_request)+(sizeof (size_t) * work_dim);

    if (!ccl_request.local_work_size_is_null)
        size_buffer_data_request += sizeof (size_t) * work_dim;

    if (!ccl_request.global_work_offset_is_null)
        size_buffer_data_request += sizeof (size_t) * work_dim;

    if (ccl_request.num_events_in_wait_list > 0)
        size_buffer_data_request += sizeof (cl_event) * num_events_in_wait_list;

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
    
    if (!ccl_request.global_work_offset_is_null) 
    {
            size_t *ss = (size_t*) global_work_offset;
            _ccl_memcpy(buffer_data_request, ss, sizeof (size_t) * work_dim, &offset_buffer);
            buffer_data_request += sizeof (size_t) * work_dim;
    }
    size_t * ss_ = (size_t*) global_work_size;
    _ccl_memcpy(buffer_data_request, ss_, sizeof (size_t) * work_dim, &offset_buffer);
    buffer_data_request += sizeof (size_t) * work_dim;

    if (!ccl_request.local_work_size_is_null) {
        size_t* sss = (size_t*) local_work_size;
        _ccl_memcpy(buffer_data_request, sss, sizeof (size_t) * work_dim, &offset_buffer);
        buffer_data_request += sizeof (size_t) * work_dim;
    }

    if (num_events_in_wait_list > 0) 
    {
        cl_event *ss =  malloc(sizeof(cl_event)*num_events_in_wait_list);
        for(int ii =0 ; ii<num_events_in_wait_list;ii++)
        {
            ptr = lookup_object((void*)event_wait_list[ii]);
            obj = *(cl_opencl_object**)ptr;
            ss[ii]=obj->object_remote;
        }
             
        _ccl_memcpy(buffer_data_request, ss, sizeof (cl_event) * num_events_in_wait_list, &offset_buffer);
        buffer_data_request += sizeof (cl_event) * num_events_in_wait_list;
    }

        buffer_data_request -= offset_buffer;
        size_buffer_data_reply = sizeof (ccl_reply);
     

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, " clEnqueueNDRangeKernel");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clEnqueueNDRangeKernel");
        free(id_transaction);

        if (recv_data_udp(&fd, &ccl_reply, size_buffer_data_reply, " clEnqueueNDRangeKernel") == 1) {
           
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;
        }
        closeSocketUdp(&fd);
#else

      int fd_connect =  init_tcp(&fd, &addr, "clEnqueueNDRangeKernel");
        //header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, " clEnqueueNDRangeKernel");
        recv_data_tcp(&fd, &ccl_reply, size_buffer_data_reply, " clEnqueueNDRangeKernel");
        closeSocketTCP(&fd,&fd_connect);
#endif
        free(header);
        free(buffer_data_request);
        
        if ((ccl_reply.result == CL_SUCCESS) && (event != NULL)) 
        {
             ptr  = register_object(ccl_reply.event, getDispachPointer(7), obj->daemon);
             obj = *(cl_opencl_object**)ptr;
             ccl_reply.event = obj->object_local;
             memcpy(event, &ccl_reply.event, sizeof (cl_event));
        }
#if DEBUG == 0
        puts("--- End execute clEnqueueNDRangeKernel primitive\n ---");    
#endif
        return (ccl_reply.result);   
}
POsym(clEnqueueNDRangeKernel)
