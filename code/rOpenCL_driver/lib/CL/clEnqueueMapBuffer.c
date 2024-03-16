#include "pocl_cl.h"
CL_API_ENTRY void *CL_API_CALL
POname(clEnqueueMapBuffer)(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_map, cl_map_flags map_flags, size_t offset, size_t size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
    // struct timeval t0, t1,t0_,t1_;
    // gettimeofday(&t0, NULL);

#if DEBUG == 0
    puts("--- Start execute clEnqueueMapBuffer primitive\n ---");
#endif
    if (blocking_map == CL_FALSE)
    {
        puts("clEnqueueMapBuffer CL_FALSE not supported\n");
        blocking_map = CL_TRUE;
    }

    char id = 0x21;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *ptr_ = NULL, *result_ = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_opencl_object *obj = NULL;

    struct
    {
        cl_event event;
        cl_int errcode_ret;
        uintptr_t ptr_remote;
    } ccl_reply;

    // result_ = malloc(size);

    ptr_ = lookup_object(command_queue);

    if (ptr_ != NULL)
    {
        obj = *(cl_opencl_object **)ptr_;
        addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
        addr.sin_family = obj->daemon->addr.sin_family;
        addr.sin_port = obj->daemon->addr.sin_port;
    }
    else
    {
        if (errcode_ret != NULL)
            *errcode_ret = CL_INVALID_VALUE;
        return NULL;
    }

    struct
    {
        cl_command_queue command_queue;
        cl_mem buffer;
        cl_bool blocking_map;
        cl_map_flags map_flags;
        size_t offset;
        size_t size;
        cl_uint num_events_in_wait_list;
        char event_is_null;
    } ccl_request = {.command_queue = obj->object_remote, .blocking_map = blocking_map, .map_flags = map_flags, .offset = offset, .size = size, .num_events_in_wait_list = num_events_in_wait_list, .event_is_null = (event == NULL)};

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

    ptr_ = lookup_object(buffer);
    if (ptr_ != NULL)
    {
        obj = *(cl_opencl_object **)ptr_;
    }
    else
    {
        if (errcode_ret != NULL)
            *errcode_ret = CL_INVALID_VALUE;
        return NULL;
    }

    ccl_request.buffer = obj->object_remote;
    _ccl_memcpy(buffer_data_request, &ccl_request, sizeof(ccl_request), &offset_buffer);
    buffer_data_request += sizeof(ccl_request);

    if (num_events_in_wait_list > 0)
    {
        cl_event *ss = malloc(sizeof(cl_event) * num_events_in_wait_list);

        for (int ii = 0; ii < num_events_in_wait_list; ii++)
        {
            ptr_ = lookup_object((void *)event_wait_list[ii]);
            obj = *(cl_opencl_object **)ptr_;
            ss[ii] = obj->object_remote;
        }
        _ccl_memcpy(buffer_data_request, ss, sizeof(cl_event) * num_events_in_wait_list, &offset_buffer);
        buffer_data_request += (sizeof(cl_event) * num_events_in_wait_list);

        free(ss);
    }

    buffer_data_request -= offset_buffer;

    size_buffer_data_reply = sizeof(ccl_reply) + size;

    buffer_data_reply = malloc(size_buffer_data_reply);

    ptr_ = lookup_object(buffer);
    obj = *(cl_opencl_object **)ptr_;

#if PROTOCOL == 0

    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clEnqueueMapBuffer");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    void *header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clEnqueueMapBuffer");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueMapBuffer") == 1)
    {
        if (errcode_ret != NULL)
            *errcode_ret = CL_ERROR_NETWORK;
        closeSocketUdp(&fd);
        return NULL;
    }
    closeSocketUdp(&fd);
#else
    int fd_connect = init_tcp(&fd, &addr, "clEnqueueMapBuffer");
    // gettimeofday(&t0_, NULL);
    send_data_tcp(&fd, buffer_data_request, NULL, size_buffer_data_request, "clEnqueueMapBuffer");
    // gettimeofday(&t1_, NULL);
    // unsigned long long tu0_ = (unsigned long long)t0_.tv_sec*1000000L+(unsigned long long)t0_.tv_usec;
    // unsigned long long tu1_ = (unsigned long long)t1_.tv_sec*1000000L+(unsigned long long)t1_.tv_usec;
    //         printf("send=%.3lfs\n", (tu1_ - tu0_) / 1000000.0);
    // gettimeofday(&t0_, NULL);
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueMapBuffer");

    // gettimeofday(&t1_, NULL);
    // tu0_ = (unsigned long long)t0_.tv_sec*1000000L+(unsigned long long)t0_.tv_usec;
    // tu1_ = (unsigned long long)t1_.tv_sec*1000000L+(unsigned long long)t1_.tv_usec;
    //         printf("recv=%.3lfs\n", (tu1_ - tu0_) / 1000000.0);

    closeSocketTCP(&fd, &fd_connect);
#endif

    free(buffer_data_request);

    memcpy(&ccl_reply, buffer_data_reply, sizeof(ccl_reply));
    // memcpy(result_, buffer_data_reply+sizeof(ccl_reply), size);

    result_ = buffer_data_reply + sizeof(ccl_reply);

    // free(buffer_data_reply);

    if (ccl_reply.errcode_ret == CL_SUCCESS)
    {
        obj->ptr_remote = malloc(sizeof(uintptr_t));
        memcpy(obj->ptr_remote, &ccl_reply.ptr_remote, sizeof(uintptr_t));

        if (event != NULL)
        {
            ptr_ = register_object(ccl_reply.event, getDispachPointer(7), obj->daemon);
            obj = *(cl_opencl_object **)ptr_;
            ccl_reply.event = obj->object_local;
            memcpy(event, &ccl_reply.event, sizeof(cl_event));
        }
    }

    if (errcode_ret != NULL)
        *errcode_ret = ccl_reply.errcode_ret;

#if DEBUG == 0
    puts("--- End execute clEnqueueMapBuffer primitive\n ---");
#endif

    // gettimeofday(&t1, NULL);

    // unsigned long long tu0 = (unsigned long long)t0.tv_sec*1000000L+(unsigned long long)t0.tv_usec;
    // unsigned long long	tu1 = (unsigned long long)t1.tv_sec*1000000L+(unsigned long long)t1.tv_usec;
    //	printf("clMap time=%.3lfs\n", (tu1 - tu0) / 1000000.0);
    return (result_);
}
POsym(clEnqueueMapBuffer)
