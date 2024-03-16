#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueUnmapMemObject)(cl_command_queue command_queue, cl_mem memobj, void *mapped_ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) CL_API_SUFFIX__VERSION_1_0
{
    // struct timeval t0, t1;
    // gettimeofday(&t0, NULL);

#if DEBUG == 0
    puts("--- Start execute clEnqueueUnmapMemObject primitive\n ---");
#endif

    char id = 0x47;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *ptr = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, offset_buffer = 0;

    cl_opencl_object *obj = NULL;

    ptr = lookup_object(command_queue);

    if (ptr != NULL)
    {
        obj = *(cl_opencl_object **)ptr;
        addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
        addr.sin_family = obj->daemon->addr.sin_family;
        addr.sin_port = obj->daemon->addr.sin_port;
    }
    else
    {
        return CL_INVALID_VALUE;
    }

    struct
    {
        cl_command_queue command_queue;
        cl_mem memobj;
        cl_uint num_events_in_wait_list;
        char event_is_null;
        uintptr_t str_pointer;
    } ccl_request = {.command_queue = obj->object_remote, .num_events_in_wait_list = num_events_in_wait_list, .event_is_null = (event == NULL)};

    // memcpy(&ccl_request.str_pointer,obj->ptr_remote,sizeof(uintptr_t));

    struct
    {
        cl_int result;
        cl_event event;
    } ccl_reply;

    size_buffer_data_request = sizeof(ccl_request);

    if (num_events_in_wait_list > 0)
        size_buffer_data_request += (sizeof(cl_event) * num_events_in_wait_list);

#if PROTOCOL == 1
    size_buffer_data_request += SIZE_HEADER_TCP;
#endif

    buffer_data_request = malloc(size_buffer_data_request);

#if PROTOCOL == 1

    _ccl_memcpy(buffer_data_request, &id, sizeof(char), &offset_buffer);
    buffer_data_request += sizeof(char);

    _ccl_memcpy(buffer_data_request, &size_buffer_data_request, sizeof(size_t), &offset_buffer);
    buffer_data_request += sizeof(size_t);

#endif

    ptr = lookup_object(memobj);
    if (ptr != NULL)
    {
        obj = *(cl_opencl_object **)ptr;
    }
    else
    {
        return CL_INVALID_VALUE;
    }

    memcpy(&ccl_request.str_pointer, obj->ptr_remote, sizeof(uintptr_t));

    ccl_request.memobj = obj->object_remote;

    _ccl_memcpy(buffer_data_request, &ccl_request, sizeof(ccl_request), &offset_buffer);
    buffer_data_request += sizeof(ccl_request);

    free(obj->ptr_remote);
    obj->ptr_remote = NULL;
    // free(mapped_ptr-24);

    if (num_events_in_wait_list > 0)
    {
        cl_event *ss = malloc(sizeof(cl_event) * num_events_in_wait_list);
        for (int ii = 0; ii < num_events_in_wait_list; ii++)
        {
            ptr = lookup_object((void *)event_wait_list[ii]);
            obj = *(cl_opencl_object **)ptr;
            ss[ii] = obj->object_remote;
        }
        _ccl_memcpy(buffer_data_request, ss, sizeof(cl_event) * num_events_in_wait_list, &offset_buffer);
        buffer_data_request += (sizeof(cl_event) * num_events_in_wait_list);
    }

    buffer_data_request -= offset_buffer;

#if PROTOCOL == 0
    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clEnqueueUnmapMemObject");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    void *header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clEnqueueUnmapMemObject");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueUnmapMemObject") == 1)
    {
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return CL_ERROR_NETWORK;
    }
    closeSocketUdp(&fd);
#else
    int fd_connect = init_tcp(&fd, &addr, "clEnqueueUnmapMemObject");
    send_data_tcp(&fd, buffer_data_request, NULL, size_buffer_data_request, "clEnqueueUnmapMemObject");
    recv_data_tcp(&fd, &ccl_reply, sizeof(ccl_reply), "clEnqueueUnmapMemObject");
    closeSocketTCP(&fd, &fd_connect);
#endif

    free(buffer_data_request);

    if (ccl_reply.result == CL_SUCCESS)
    {
        if (event != NULL)
        {
            ptr = register_object(ccl_reply.event, getDispachPointer(7), obj->daemon);
            obj = *(cl_opencl_object **)ptr;
            ccl_reply.event = obj->object_local;
            memcpy(event, &ccl_reply.event, sizeof(cl_event));
        }
    }

#if DEBUG == 0
    puts("--- End execute clEnqueueUnmapMemObject primitive\n ---");
#endif

    // gettimeofday(&t1, NULL);

    // unsigned long long tu0 = (unsigned long long)t0.tv_sec*1000000L+(unsigned long long)t0.tv_usec;
    // unsigned long long      tu1 = (unsigned long long)t1.tv_sec*1000000L+(unsigned long long)t1.tv_usec;
    //         printf("clMap time=%.3lfs\n", (tu1 - tu0) / 1000000.0);

    // free(mapped_ptr-24);
    return (ccl_reply.result);
}
POsym(clEnqueueUnmapMemObject)
