#include "pocl_cl.h"
CL_API_ENTRY cl_context CL_API_CALL
POname(clCreateContext)(const cl_context_properties *properties, cl_uint num_devices, const cl_device_id *devices, void(CL_CALLBACK *pfn_notify)(const char *, const void *, size_t, void *), void *user_data, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
{

#if DEBUG == 0
    puts("--- Start execute clCreateContext primitive\n ---");
#endif
    cl_device_id *dd = malloc(sizeof(cl_device_id) * num_devices);
    char id = 0x06;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int p = 0, fd = 0, num_properties = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;

    cl_context result_;
    cl_int errcode_ret_;
    cl_opencl_object *obj = NULL;

    for (int i = 0; i < num_devices; i++)
    {
        ptr = lookup_object(devices[i]);
        if (ptr != NULL)
        {
            obj = *(cl_opencl_object **)ptr;
            addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
            addr.sin_family = obj->daemon->addr.sin_family;
            addr.sin_port = obj->daemon->addr.sin_port;

            dd[i] = obj->object_remote;
        }
    }

    cl_context_properties pp[3];

    if (properties != NULL)
    {
        while (properties[num_properties += 2] != 0);
        num_properties++;

        size_buffer_data_request += (sizeof(cl_context_properties) * num_properties);
    }

    if (num_devices > 0)
    {
        size_buffer_data_request += (sizeof(cl_device_id) * num_devices);
    }

    if (num_properties > 0)
    {
        for (p = 0; p < num_properties - 1; p += 2)
        {
            pp[p] = properties[p];
            ptr = lookup_object((void *)properties[p + 1]);

            if (ptr == NULL)
            {
                if (errcode_ret != NULL)
                    *errcode_ret = CL_INVALID_PLATFORM;

                return NULL;
            }
            obj = *(cl_opencl_object **)ptr;
            pp[p + 1] = (cl_context_properties)obj->object_remote;
        }

        pp[p] = 0;
    }

    size_buffer_data_request += (sizeof(int) + sizeof(cl_uint));
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

    _ccl_memcpy(buffer_data_request, &num_properties, sizeof(int), &offset_buffer);
    buffer_data_request += sizeof(int);
    _ccl_memcpy(buffer_data_request, &num_devices, sizeof(cl_uint), &offset_buffer);
    buffer_data_request += sizeof(cl_uint);

    if (num_properties > 0)
    {
        _ccl_memcpy(buffer_data_request, pp, sizeof(cl_context_properties) * num_properties, &offset_buffer);
        buffer_data_request += sizeof(cl_context_properties) * num_properties;
    }
    if (num_devices > 0)
    {
        _ccl_memcpy(buffer_data_request, dd, sizeof(cl_device_id) * num_devices, &offset_buffer);
        buffer_data_request += sizeof(cl_device_id) * num_devices;
    }

    buffer_data_request -= offset_buffer;

    size_buffer_data_reply = sizeof(cl_context) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0

    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clCreateContext");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateContext");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateContext") == 1)
    {
        if (errcode_ret != NULL)
            *errcode_ret = CL_ERROR_NETWORK;

        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return NULL;
    }
    closeSocketUdp(&fd);
#else

    int fd_connect = init_tcp(&fd, &addr, "clCreateContext");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateContext");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateContext");
    closeSocketTCP(&fd, &fd_connect);

#endif

    //        free(header);
    free(buffer_data_request);
    free(dd);

    offset_buffer = 0;
    _ccl_memcpy(&result_, buffer_data_reply, sizeof(cl_context), &offset_buffer);
    buffer_data_reply += sizeof(cl_context);

    _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof(cl_int), &offset_buffer);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);

    if (errcode_ret_ == CL_SUCCESS)
    {
        ptr = register_object(result_, getDispachPointer(2), obj->daemon);
        obj = *(cl_opencl_object **)ptr;
        result_ = obj->object_local;
    }
    else
        result_ = NULL;

    if (errcode_ret != NULL)
        *(errcode_ret) = errcode_ret_;
#if DEBUG == 0
    puts("--- End execute clCreateContext primitive\n ---");
#endif
    return (result_);
}
POsym(clCreateContext)
