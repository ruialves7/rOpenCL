#include "pocl_cl.h"

CL_API_ENTRY void * CL_API_CALL
POname(clEnqueueMapImage)(cl_command_queue command_queue,cl_mem image,cl_bool blocking_map, cl_map_flags  map_flags,  const size_t * origin,const size_t *region,size_t * image_row_pitch,size_t * image_slice_pitch,cl_uint            num_events_in_wait_list,const cl_event * event_wait_list, cl_event *  event,cl_int *  errcode_ret ) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueMapImage primitive\n ---");    
#endif
    char id = 0x1F;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;

    cl_int errcode_ret_;
    cl_event event_;
    void *result_ = malloc(sizeof (void*));
    cl_opencl_object * obj = NULL;

    ptr = lookup_object(command_queue);
    if (ptr != NULL) {
        
       obj = *(cl_opencl_object**)ptr;
       addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;

    } else 
    {
        if (errcode_ret != NULL)
            *errcode_ret = CL_INVALID_VALUE;
        return NULL;
    }
        size_buffer_data_request = sizeof (cl_bool) + (1 * sizeof (char)) + sizeof (cl_command_queue) + sizeof (cl_mem) + sizeof (cl_map_flags) + (8 * sizeof (size_t)) + sizeof (cl_uint);

        if (num_events_in_wait_list > 0)
            size_buffer_data_request += (sizeof (cl_event) * num_events_in_wait_list);

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

_ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_command_queue),&offset_buffer);
        buffer_data_request += sizeof (cl_command_queue);
        ptr = lookup_object(image);
        if (ptr != NULL) {
        
        obj = *(cl_opencl_object**)ptr;

    } else 
    {
        if (errcode_ret != NULL)
            *errcode_ret = CL_INVALID_VALUE;
        return NULL;
    }

        _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_mem), &offset_buffer);
        buffer_data_request += sizeof (cl_mem);

        _ccl_memcpy(buffer_data_request, &blocking_map, sizeof (cl_bool), &offset_buffer);
        buffer_data_request += sizeof (cl_bool);

        _ccl_memcpy(buffer_data_request, &map_flags, sizeof (cl_mem_flags), &offset_buffer);
        buffer_data_request += sizeof (cl_mem_flags);

        size_t * ss = (size_t*) origin;
        _ccl_memcpy(buffer_data_request, ss, sizeof (size_t)*3, &offset_buffer);
        buffer_data_request += sizeof (size_t)*3;

        size_t * sss = (size_t*) region;
        _ccl_memcpy(buffer_data_request, sss, sizeof (size_t)*3, &offset_buffer);
        buffer_data_request += sizeof (size_t)*3;

        _ccl_memcpy(buffer_data_request, image_row_pitch, sizeof (size_t), &offset_buffer);
        buffer_data_request += sizeof (size_t);

        _ccl_memcpy(buffer_data_request, image_slice_pitch, sizeof (size_t), &offset_buffer);
        buffer_data_request += sizeof (size_t);

        _ccl_memcpy(buffer_data_request, &num_events_in_wait_list, sizeof (cl_uint), &offset_buffer);
        buffer_data_request += sizeof (cl_uint);

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
        char event_is_null = (event == NULL);
        _ccl_memcpy(buffer_data_request, &event_is_null, sizeof (char), &offset_buffer);
        buffer_data_request += sizeof (char);

        buffer_data_request -= offset_buffer;

        size_buffer_data_reply = sizeof (cl_int) + sizeof (cl_event) + sizeof (void*);
        buffer_data_reply = malloc(size_buffer_data_reply);
#if PROTOCOL ==0 
        
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local,&addr,  "clEnqueueFillImage");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clEnqueueFillImage");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueMapImage") == 1) {
            free(buffer_data_reply);
            if (errcode_ret != NULL)
                *errcode_ret = CL_ERROR_NETWORK;
            closeSocketUdp(&fd);
            return NULL;
        }
        closeSocketUdp(&fd);
        
#else
        
       int fd_connect= init_tcp(&fd, &addr, "clEnqueueMapImage");
       //header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clEnqueueMapImage");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueMapImage");
        closeSocketTCP(&fd,&fd_connect);
        
#endif
        offset_buffer = 0;
        free(header);
        free(buffer_data_request);

        _ccl_memcpy(result_, buffer_data_reply, sizeof (void*), &offset_buffer);
        buffer_data_reply += sizeof (void*);


        _ccl_memcpy(&event_, buffer_data_reply, sizeof (cl_event), &offset_buffer);
        buffer_data_reply += sizeof (cl_event);

        _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (errcode_ret_ == CL_SUCCESS) 
        {
            if (event != NULL)
            {
               ptr  = register_object(result_, getDispachPointer(7), obj->daemon);
               obj = *(cl_opencl_object**)ptr;
               event_ = obj->object_local;
               memcpy(event, &event_, sizeof (cl_event));
            }
        }
    if (errcode_ret != NULL)
        *errcode_ret = errcode_ret_;
#if DEBUG == 0
        puts("--- End execute clEnqueueMapImage primitive\n ---");    
#endif
    return (result_);
}
POsym(clEnqueueMapImage)
