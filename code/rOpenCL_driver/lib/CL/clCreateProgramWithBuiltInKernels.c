
#include "pocl_cl.h"
CL_API_ENTRY cl_program CL_API_CALL
POname(clCreateProgramWithBuiltInKernels)(cl_context context, cl_uint num_devices, const cl_device_id *device_list, const char *kernel_names, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_2
{
#if DEBUG == 0
    puts("--- Start execute clCreateProgramWithBuiltInKernels primitive\n ---");
#endif
    char id = 0x4F;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_program result_;
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

    int size = strlen(kernel_names);
    size_buffer_data_request = sizeof(cl_context) + sizeof(cl_uint);

    if (num_devices > 0)
    {
        size_buffer_data_request += (sizeof(cl_device_id) * num_devices);
    }
    if (size > 0)
        size_buffer_data_request += size;

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

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_context), &offset_buffer);
    buffer_data_request += sizeof(cl_context);

    _ccl_memcpy(buffer_data_request, &num_devices, sizeof(cl_uint), &offset_buffer);
    buffer_data_request += sizeof(cl_uint);

    if (num_devices > 0)
    {
        cl_device_id *ss = malloc(sizeof(cl_device_id) * num_devices);
        for (int ii = 0; ii < num_devices; ii++)
        {
            ptr = lookup_object((void *)device_list[ii]);
            obj = *(cl_opencl_object **)ptr;
            ss[ii] = obj->object_remote;
        }

        _ccl_memcpy(buffer_data_request, ss, sizeof(cl_device_id) * num_devices, &offset_buffer);
        buffer_data_request += sizeof(cl_device_id) * num_devices;
    }

    _ccl_memcpy(buffer_data_request, &size, sizeof(int), &offset_buffer);
    buffer_data_request += sizeof(int);

    if (size > 0)
    {
        char *sss = (char *)kernel_names;
        _ccl_memcpy(buffer_data_request, sss, size, &offset_buffer);
        buffer_data_request += size;
    }

    size_buffer_data_reply = sizeof(cl_program) + sizeof(cl_int) + sizeof(int);
    buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clCreateProgramWithBuiltInKernels");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateProgramWithBuiltInKernels");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateProgramWithBuiltInKernels") == 1)
    {
        free(buffer_data_reply);
        if (errcode_ret != NULL)
            *errcode_ret = CL_ERROR_NETWORK;
        closeSocketUdp(&fd);
        return NULL;
    }
    closeSocketUdp(&fd);
#else

    int fd_connect = init_tcp(&fd, &addr, "clCreateProgramWithBuiltInKernels");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateProgramWithBuiltInKernels");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateProgramWithBuiltInKernels");
    closeSocketTCP(&fd, &fd_connect);
#endif

    offset_buffer = 0;
    free(header);
    free(buffer_data_request);

    _ccl_memcpy(&result_, buffer_data_reply, sizeof(cl_program), &offset_buffer);
    buffer_data_reply += sizeof(cl_program);

    _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof(cl_int), &offset_buffer);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);

    if (errcode_ret_ == CL_SUCCESS)
    {
        ptr = register_object(result_, getDispachPointer(5), obj->daemon);
        obj = *(cl_opencl_object **)ptr;
        result_ = obj->object_local;
    }

    if (errcode_ret != NULL)
        *errcode_ret = errcode_ret_;
#if DEBUG == 0
    puts("--- End execute clCreateProgramWithBuiltInKernels primitive\n ---");
#endif
    return (result_);
}
POsym(clCreateProgramWithBuiltInKernels)
