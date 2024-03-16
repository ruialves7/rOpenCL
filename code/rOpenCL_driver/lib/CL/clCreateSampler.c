#include "pocl_cl.h"
extern CL_API_ENTRY cl_sampler CL_API_CALL
POname(clCreateSampler)(cl_context context, cl_bool normalized_coords, cl_addressing_mode addressing_mode, cl_filter_mode filter_mode, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
    puts("--- Start execute clCreateSampler primitive\n ---");
#endif
    char id = 0x49;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_sampler result_;
    cl_int errcode_ret_;
    ptr = lookup_object(context);
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
        if (errcode_ret != NULL)
            *errcode_ret = CL_INVALID_VALUE;
        return NULL;
    }
    size_buffer_data_request = sizeof(cl_context) + sizeof(cl_bool) + sizeof(cl_addressing_mode) + sizeof(cl_filter_mode);
    buffer_data_request = malloc(size_buffer_data_request);

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_context), &offset_buffer);
    buffer_data_request += sizeof(cl_context);

    _ccl_memcpy(buffer_data_request, &normalized_coords, sizeof(cl_bool), &offset_buffer);
    buffer_data_request += sizeof(cl_bool);

    _ccl_memcpy(buffer_data_request, &addressing_mode, sizeof(cl_addressing_mode), &offset_buffer);
    buffer_data_request += sizeof(cl_addressing_mode);

    _ccl_memcpy(buffer_data_request, &filter_mode, sizeof(cl_filter_mode), &offset_buffer);
    buffer_data_request += sizeof(cl_filter_mode);
    buffer_data_request -= offset_buffer;

    size_buffer_data_reply = sizeof(cl_sampler) + sizeof(cl_int);
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

#if PROTOCOL == 0
    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clCreateSampler");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateSampler");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateSampler") == 1)
    {
        free(buffer_data_reply);
        if (errcode_ret != NULL)
            *errcode_ret = CL_ERROR_NETWORK;
        closeSocketUdp(&fd);
        return NULL;
    }
    closeSocketUdp(&fd);
#else
    int fd_connect = init_tcp(&fd, &addr, "clCreateSampler");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateSampler");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateSampler");
    closeSocketTCP(&fd, &fd_connect);
#endif
    buffer_data_request-=offset_buffer;
    free(header);
    free(buffer_data_request);
    offset_buffer=0;
    _ccl_memcpy(&result_, buffer_data_reply, sizeof(cl_sampler), &offset_buffer);
    buffer_data_reply += sizeof(cl_sampler);

    _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof(cl_int), &offset_buffer);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);

    if (errcode_ret_ == CL_SUCCESS)
    {
        ptr = register_object(result_, getDispachPointer(8), obj->daemon);
        obj = *(cl_opencl_object **)ptr;
        result_ = obj->object_local;
    }

    if (errcode_ret != NULL)
        *(errcode_ret) = errcode_ret_;
#if DEBUG == 0
    puts("--- End execute clCreateSampler primitive\n ---");
#endif

    return (result_);
}
POsym(clCreateSampler)
