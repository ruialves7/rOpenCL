#include "pocl_cl.h"
CL_API_ENTRY cl_kernel CL_API_CALL
POname(clCreateKernel)(cl_program program, const char *kernel_name, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
    puts("--- Start execute clCreateKernel primitive\n ---");
#endif

    char id = 0x0D;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *header = NULL, *ptr = NULL;
    int size_kernel = 0, fd = 0;
    size_t size_buffer_data_request = 0, offset_buffer = 0;
    struct
    {
        cl_kernel result;
        cl_int errcode_ret;
    } ccl_reply;

    ptr = lookup_object(program);
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

    if (kernel_name != NULL)
    {
        size_kernel = strlen(kernel_name) + 1;
        size_buffer_data_request += size_kernel;
    }

    size_buffer_data_request += (sizeof(int) + sizeof(cl_program));
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

    _ccl_memcpy(buffer_data_request, &size_kernel, sizeof(int), &offset_buffer);
    buffer_data_request += sizeof(int);

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_program), &offset_buffer);
    buffer_data_request += sizeof(cl_program);

    if (size_kernel > 0)
    {

        char *ss = (char *)kernel_name;
        _ccl_memcpy(buffer_data_request, ss, size_kernel, &offset_buffer);
        buffer_data_request += size_kernel;
    }

    buffer_data_request -= offset_buffer;

#if PROTOCOL == 0
    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clCreateKernel");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateKernel");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateKernel") == 1)
    {
        if (errcode_ret != NULL)
            *errcode_ret = CL_ERROR_NETWORK;
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return NULL;
    }
    closeSocketUdp(&fd);
#else

    int fd_connect = init_tcp(&fd, &addr, "clCreateKernel");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateKernel");
    recv_data_tcp(&fd, &ccl_reply, sizeof(ccl_reply), "clCreateKernel");
    closeSocketTCP(&fd, &fd_connect);
#endif
    free(buffer_data_request);
    offset_buffer = 0;

    if (ccl_reply.errcode_ret == CL_SUCCESS)
    {
        ptr = register_object(ccl_reply.result, getDispachPointer(6), obj->daemon);
        obj = *(cl_opencl_object **)ptr;
        ccl_reply.result = obj->object_local;
    }

    if (errcode_ret != NULL)
        *errcode_ret = ccl_reply.errcode_ret;
#if DEBUG == 0
    puts("--- End execute clCreateKernel primitive\n ---");
#endif
    return (ccl_reply.result);
}
POsym(clCreateKernel)
