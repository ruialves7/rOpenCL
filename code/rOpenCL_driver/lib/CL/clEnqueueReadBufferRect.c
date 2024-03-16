#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueReadBufferRect)(cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, const size_t *buffer_origin, const size_t *host_origin, const size_t *region, size_t buffer_row_pitch, size_t buffer_slice_pitch, size_t host_row_pitch, size_t host_slice_pitch, void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event) CL_API_SUFFIX__VERSION_1_1
{

#if DEBUG == 0
    puts("--- Start execute clEnqueueReadBufferRect primitive\n ---");
#endif
    char id = 0x1A;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr_ = NULL;
    int size_ptr = 0, x = 0, y = 0, fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;

    cl_int result_;
    cl_event event_;

    cl_opencl_object *obj = NULL;
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
        return CL_INVALID_VALUE;
    }
    size_buffer_data_request = sizeof(cl_bool) + (1 * sizeof(char)) + sizeof(cl_command_queue) + sizeof(cl_mem) + (13 * sizeof(size_t)) + sizeof(cl_uint);

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

    ptr_ = lookup_object(buffer);
    if (ptr_ != NULL)
    {
        obj = *(cl_opencl_object **)ptr_;
    }
    else
    {
        return CL_INVALID_VALUE;
    }

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_mem), &offset_buffer);
    buffer_data_request += sizeof(cl_mem);

    _ccl_memcpy(buffer_data_request, &blocking_read, sizeof(cl_bool), &offset_buffer);
    buffer_data_request += sizeof(cl_bool);

    size_t *s = (size_t *)buffer_origin;
    _ccl_memcpy(buffer_data_request, s, sizeof(size_t) * 3, &offset_buffer);
    buffer_data_request += sizeof(size_t) * 3;

    size_t *ss = (size_t *)host_origin;
    _ccl_memcpy(buffer_data_request, ss, sizeof(size_t) * 3, &offset_buffer);
    buffer_data_request += sizeof(size_t) * 3;

    size_t *sss = (size_t *)region;
    _ccl_memcpy(buffer_data_request, sss, sizeof(size_t) * 3, &offset_buffer);
    buffer_data_request += sizeof(size_t) * 3;

    _ccl_memcpy(buffer_data_request, &buffer_row_pitch, sizeof(size_t), &offset_buffer);
    buffer_data_request += sizeof(size_t);

    _ccl_memcpy(buffer_data_request, &buffer_slice_pitch, sizeof(size_t), &offset_buffer);
    buffer_data_request += sizeof(size_t);

    _ccl_memcpy(buffer_data_request, &host_row_pitch, sizeof(size_t), &offset_buffer);
    buffer_data_request += sizeof(size_t);

    _ccl_memcpy(buffer_data_request, &host_slice_pitch, sizeof(size_t), &offset_buffer);
    buffer_data_request += sizeof(size_t);

    _ccl_memcpy(buffer_data_request, &num_events_in_wait_list, sizeof(cl_uint), &offset_buffer);
    buffer_data_request += sizeof(cl_uint);

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

    char event_is_null = (event == NULL);
    memcpy(buffer_data_request, &event_is_null, sizeof(char));
    buffer_data_request += sizeof(char);

    buffer_data_request -= offset_buffer;

    if (host_row_pitch != 0)
        x = host_row_pitch;
    else
        x = region[0];

    if (host_slice_pitch != 0)
        y = host_slice_pitch;
    else
        y = region[1];
    size_ptr = x * y;

    size_buffer_data_reply = sizeof(cl_int) + sizeof(cl_event) + size_ptr;
    buffer_data_reply = malloc(size_buffer_data_reply);
#if PROTOCOL == 0
    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clEnqueueReadBufferRect");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clEnqueueReadBufferRect");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueReadBufferRect") == 1)
    {
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return CL_ERROR_NETWORK;
    }
    closeSocketUdp(&fd);
#else
    int fd_connect = init_tcp(&fd, &addr, "clEnqueueReadBufferRect");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clEnqueueReadBufferRect");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clEnqueueReadBufferRect");
    closeSocketTCP(&fd, &fd_connect);
#endif
    offset_buffer = 0;
    free(header);
    free(buffer_data_request);

    _ccl_memcpy(&result_, buffer_data_reply, sizeof(cl_int), &offset_buffer);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(&event_, buffer_data_reply, sizeof(cl_event), &offset_buffer);
    buffer_data_reply += sizeof(cl_event);

    _ccl_memcpy(ptr, buffer_data_reply, size_ptr, &offset_buffer);
    buffer_data_reply += size_ptr;

    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);

    if (result_ == CL_SUCCESS)
        if (event != NULL)
        {
            ptr_ = register_object(event_, getDispachPointer(7), obj->daemon);
            obj = *(cl_opencl_object **)ptr_;
            event_ = obj->object_local;
            memcpy(event, &event_, sizeof(cl_event));
        }
#if DEBUG == 0
    puts("--- End execute clEnqueueReadBufferRect primitive\n ---");
#endif
    return (result_);
}
POsym(clEnqueueReadBufferRect)
