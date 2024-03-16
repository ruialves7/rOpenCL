#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clBuildProgram)(cl_program program, cl_uint num_devices, const cl_device_id *device_list, const char *options, void(CL_CALLBACK *pfn_notify)(cl_program program, void *user_data), void *user_data) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
    puts("--- Start execute clBuildProgram primitive\n ---");
#endif
    char id = 0x0C;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *header = NULL, *ptr = NULL;
    int size_options = 0, fd = 0;
    size_t size_buffer_data_request = 0, offset_buffer = 0;

    cl_opencl_object *obj = NULL;
    ptr = lookup_object(program);

    if (ptr != NULL)
    {
        obj = *(cl_opencl_object **)ptr;
        memcpy(&addr, obj->daemon, sizeof(struct sockaddr_in));
    }
    else
    {
        return CL_INVALID_VALUE;
    }

    struct
    {
        cl_program program;
        cl_uint num_devices;

    } ccl_request = {.program = obj->object_remote, .num_devices = num_devices};

    struct
    {
        cl_int result;
    } ccl_reply;

    size_buffer_data_request = sizeof(int) + sizeof(ccl_request);
    if (options != NULL)
        size_options = sizeof(char) * strlen(options) + 1;
    size_buffer_data_request += size_options;

    if (num_devices > 0)
        size_buffer_data_request += sizeof(cl_device_id) * num_devices;

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

    _ccl_memcpy(buffer_data_request, &size_options, sizeof(int), &offset_buffer);
    buffer_data_request += sizeof(int);

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

    if (size_options > 0)
    {
        char *op = (char *)options;
        _ccl_memcpy(buffer_data_request, op, size_options, &offset_buffer);
        buffer_data_request += size_options;
    }

    buffer_data_request -= offset_buffer;

#if PROTOCOL == 0

    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clBuildProgram");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clBuildProgram");
    free(id_transaction);

    if (recv_data_udp(&fd, &ccl_reply, sizeof(ccl_reply), "clBuildProgram") == 1)
    {
        closeSocketUdp(&fd);
        return CL_ERROR_NETWORK;
    }
    closeSocketUdp(&fd);
#else
    int fd_connect = init_tcp(&fd, &addr, "clBuildProgram");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clBuildProgram");
    recv_data_tcp(&fd, &ccl_reply, sizeof(ccl_reply), "clBuildProgram");
    closeSocketTCP(&fd, &fd_connect);
#endif
    free(header);
    free(buffer_data_request);

#if DEBUG == 0
    puts("--- End execute clBuildProgram primitive\n ---");
#endif

    return (ccl_reply.result);
}
POsym(clBuildProgram)
