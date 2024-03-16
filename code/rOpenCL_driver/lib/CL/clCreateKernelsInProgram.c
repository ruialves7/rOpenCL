#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clCreateKernelsInProgram)(cl_program program, cl_uint num_kernels, cl_kernel *kernels, cl_uint *num_kernels_ret) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
    puts("--- Start execute clCreateKernelsInProgram primitive\n ---");
#endif
    char id = 0x5B;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_int result;
    cl_uint num_kernels_ret_;
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
        return CL_INVALID_VALUE;
    }
    size_buffer_data_request = sizeof(cl_program) + sizeof(cl_uint);
    buffer_data_request = malloc(size_buffer_data_request);

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_program), &offset_buffer);
    buffer_data_request += sizeof(cl_program);

    _ccl_memcpy(buffer_data_request, &num_kernels, sizeof(cl_uint), &offset_buffer);
    buffer_data_request += sizeof(cl_uint);

    buffer_data_request -= offset_buffer;

    size_buffer_data_reply = sizeof(cl_uint) + num_kernels * sizeof(cl_kernel);
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
    init_udp(&fd, &addr_local, &addr, "clCreateKernelsInProgram");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateKernelsInProgram");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateKernelsInProgram") == 1)
    {
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return CL_ERROR_NETWORK;
    }
    closeSocketUdp(&fd);

#else
    int fd_connect = init_tcp(&fd, &addr, "clCreateKernelsInProgram");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateKernelsInProgram");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateKernelsInProgram");
    closeSocketTCP(&fd, &fd_connect);
#endif
    offset_buffer = 0;
    free(header);
    free(buffer_data_request);

    _ccl_memcpy(&result, buffer_data_reply, sizeof(cl_int), &offset_buffer);
    buffer_data_reply += sizeof(cl_int);

    if (result == CL_SUCCESS)
    {
        _ccl_memcpy(&num_kernels_ret_, buffer_data_reply, sizeof(cl_uint), &offset_buffer);
        buffer_data_reply += sizeof(cl_uint);

        if (num_kernels_ret_ > 0)
        {
            cl_kernel *kernelsss = malloc(sizeof(cl_kernel) * num_kernels_ret_);
            _ccl_memcpy(kernelsss, buffer_data_reply, sizeof(cl_kernel) * num_kernels_ret_, &offset_buffer);
            buffer_data_reply += sizeof(cl_kernel) * num_kernels_ret_;

            for (int i = 0; i < num_kernels; i++)
            {
                ptr = register_object(kernelsss[i], getDispachPointer(6), obj->daemon);
                obj = *(cl_opencl_object **)ptr;
                kernels[i] = obj->object_local;
            }
            free(kernelsss);
        }
    }
    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);

    if (num_kernels_ret != NULL)
        *num_kernels_ret = num_kernels_ret_;
#if DEBUG == 0
    puts("--- End execute clCreateKernelsInProgram primitive\n ---");
#endif
    return (result);
}
POsym(clCreateKernelsInProgram)
