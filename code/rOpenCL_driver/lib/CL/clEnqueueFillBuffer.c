#include "pocl_cl.h"
extern CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueFillBuffer)(cl_command_queue command_queue, cl_mem buffer, const void *pattern, size_t pattern_size, size_t offset, size_t size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) CL_API_SUFFIX__VERSION_1_2
{
#if DEBUG == 0
    puts("--- Start execute clEnqueueFillBuffer primitive\n ---");
#endif
    char id = 0x1C;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_int result_;
    cl_event event_;

    ptr = lookup_object(command_queue);
    cl_opencl_object *obj = NULL;

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

    size_buffer_data_request = (1 * sizeof(char)) + sizeof(cl_command_queue) + sizeof(cl_mem) + (3 * sizeof(size_t)) + sizeof(cl_uint);

    if (pattern_size > 0)
        size_buffer_data_request += pattern_size;

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

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_command_queue), &offset_buffer);
    buffer_data_request += sizeof(cl_command_queue);

    ptr = lookup_object(buffer);
    if (ptr != NULL)
        obj = *(cl_opencl_object **)ptr;
    else
        return CL_INVALID_VALUE;

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_mem), &offset_buffer);
    buffer_data_request += sizeof(cl_mem);

    _ccl_memcpy(buffer_data_request, &pattern_size, sizeof(size_t), &offset_buffer);
    buffer_data_request += sizeof(size_t);

    _ccl_memcpy(buffer_data_request, &offset, sizeof(size_t), &offset_buffer);
    buffer_data_request += sizeof(size_t);

    _ccl_memcpy(buffer_data_request, &size, sizeof(size_t), &offset_buffer);
    buffer_data_request += sizeof(size_t);

    _ccl_memcpy(buffer_data_request, &num_events_in_wait_list, sizeof(cl_uint), &offset_buffer);
    buffer_data_request += sizeof(cl_uint);

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
        buffer_data_request += sizeof(cl_event) * num_events_in_wait_list;
    }

    char event_is_null = (event == NULL);
    _ccl_memcpy(buffer_data_request, &event_is_null, sizeof(char), &offset_buffer);
    buffer_data_request += sizeof(char);

    if (pattern_size > 0)
    {
        void *p = (void *)pattern;
        _ccl_memcpy(buffer_data_request, p, pattern_size, &offset_buffer);
        buffer_data_request += pattern_size;
    }

    buffer_data_request -= offset_buffer;

    size_buffer_data_reply = sizeof(cl_int) + sizeof(cl_event);
    buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clEnqueueFillBuffer");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clEnqueueFillBuffer");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueFillBuffer") == 1)
    {
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return CL_ERROR_NETWORK;
    }
    closeSocketUdp(&fd);
#else

    int fd_connect = init_tcp(&fd, &addr, "clEnqueueFillBuffer");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clEnqueueFillBuffer");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueFillBuffer");
    closeSocketTCP(&fd, &fd_connect);
#endif
    offset_buffer = 0;
    free(header);
    free(buffer_data_request);

    _ccl_memcpy(&result_, buffer_data_reply, sizeof(cl_int), &offset_buffer);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(&event_, buffer_data_reply, sizeof(cl_event), &offset_buffer);
    buffer_data_reply += sizeof(cl_event);

    buffer_data_reply -= offset_buffer;

    free(buffer_data_reply);

    if ((result_ == CL_SUCCESS) && (event != NULL))
    {

        ptr = register_object(event_, getDispachPointer(7), obj->daemon);
        obj = *(cl_opencl_object **)ptr;
        event_ = obj->object_local;

        memcpy(event, &event_, sizeof(cl_event));
    }

#if DEBUG == 0
    puts("--- End execute clEnqueueFillBuffer primitive\n ---");
#endif

    return (result_);
}
POsym(clEnqueueFillBuffer)
