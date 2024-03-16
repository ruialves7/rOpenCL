#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueReadBuffer)(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t size, void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) CL_API_SUFFIX__VERSION_1_0
{

    if (blocking_read == CL_FALSE)
    {
        puts("clEnqueueReadBuffer CL_FALSE not supported\n");
        blocking_read = CL_TRUE;
    }

#if DEBUG == 0
    puts("--- Start execute clEnqueueReadBuffer primitive\n ---");
#endif
    char id = 0x09;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr_ = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;

    cl_int result_;
    cl_event event_;
    cl_opencl_object *obj = NULL;

    ptr_ = lookup_object(command_queue);
    if (ptr_ != NULL)
    {
        obj = *(cl_opencl_object **)ptr_;
        memcpy(&addr, obj->daemon, sizeof(struct sockaddr_in));
    }
    else
    {
        return CL_INVALID_VALUE;
    }

    struct
    {
        cl_event event;
        cl_int result;
    } ccl_reply;
    struct
    {
        cl_command_queue command_queue;
        cl_mem buffer;
        cl_bool blocking_read;
        size_t offset;
        size_t size;
        cl_uint num_events_in_wait_list;
        char event_is_null;

    } ccl_request = {.blocking_read = blocking_read, .offset = offset, .size = size, .num_events_in_wait_list = num_events_in_wait_list, .event_is_null = (event == NULL)};
    ccl_request.command_queue = obj->object_remote;
    ptr_ = lookup_object(buffer);
    if (ptr_ != NULL)
    {
        obj = *(cl_opencl_object **)ptr_;
    }
    else
    {
        return CL_INVALID_VALUE;
    }
    ccl_request.buffer = obj->object_remote;

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
        buffer_data_request += sizeof(cl_event) * num_events_in_wait_list;
    }

    buffer_data_request -= offset_buffer;

    size_buffer_data_reply = sizeof(ccl_reply);
    if (size > 0)
    {
        size_buffer_data_reply += size;
    }

    buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clEnqueueReadBuffer");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clEnqueueReadBuffer");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueReadBuffer") == 1)
    {
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return CL_ERROR_NETWORK;
    }
    closeSocketUdp(&fd);
#else

    int fd_connect = init_tcp(&fd, &addr, "clEnqueueReadBuffer");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clEnqueueReadBuffer");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueReadBuffer");
    closeSocketTCP(&fd, &fd_connect);
#endif

    free(header);
    free(buffer_data_request);
    offset_buffer = 0;

    _ccl_memcpy(&ccl_reply, buffer_data_reply, sizeof(ccl_reply), &offset_buffer);
    buffer_data_reply += sizeof(ccl_reply);

    if (size > 0)
    {
        _ccl_memcpy(ptr, buffer_data_reply, size, &offset_buffer);
        buffer_data_reply += size;
    }

    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);

    if (ccl_reply.result == CL_SUCCESS)
        if (event != NULL)
        {
            ptr_ = register_object(ccl_reply.event, getDispachPointer(7), obj->daemon);
            obj = *(cl_opencl_object **)ptr_;
            ccl_reply.event = obj->object_local;
            memcpy(event, &ccl_reply.event, sizeof(cl_event));
        }
#if DEBUG == 0
    puts("--- End execute clEnqueueReadBuffer primitive\n ---");
#endif
    return (ccl_reply.result);
}
POsym(clEnqueueReadBuffer)
