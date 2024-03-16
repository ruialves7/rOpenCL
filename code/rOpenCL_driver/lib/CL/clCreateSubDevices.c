#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clCreateSubDevices)(cl_device_id in_device, const cl_device_partition_property *properties, cl_uint num_devices,
                           cl_device_id *out_devices, cl_uint *num_devices_ret) CL_API_SUFFIX__VERSION_1_2
{
#if DEBUG == 0
    puts("--- Start execute clCreateSubDevices primitive\n ---");
#endif
    char id = 0x22;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int num_properties = 0, fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;

    cl_int result_;
    cl_uint num_devices_ret_;
    cl_opencl_object *obj = NULL;
    ptr = lookup_object(in_device);

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

    if (properties != NULL)
    {
        while (properties[num_properties += 2] != 0)
            ;
        num_properties++;
    }

    size_buffer_data_request = sizeof(int) + sizeof(cl_device_id) + sizeof(cl_uint);
    if (num_properties > 0)
        size_buffer_data_request += (sizeof(cl_device_partition_property) * num_properties);

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
        cl_device_partition_property *ss = (cl_device_partition_property *)properties;
        _ccl_memcpy(buffer_data_request, ss, sizeof(cl_device_partition_property) * num_properties, &offset_buffer);
        buffer_data_request += sizeof(cl_device_partition_property) * num_properties;
    }

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_device_id), &offset_buffer);
    buffer_data_request += sizeof(cl_device_id);

    buffer_data_request -= offset_buffer;

    size_buffer_data_reply = sizeof(cl_device_id) * num_devices + sizeof(cl_int) + sizeof(cl_uint);
    buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clCreateSubDevices");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateSubDevices");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateSubDevices") == 1)
    {
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return CL_ERROR_NETWORK;
    }
    closeSocketUdp(&fd);
#else
    int fd_connect = init_tcp(&fd, &addr, "clCreateSubDevices");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateSubDevices");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateSubDevices");
    closeSocketTCP(&fd, &fd_connect);
#endif

    offset_buffer = 0;
    free(header);
    free(buffer_data_request);

    _ccl_memcpy(&result_, buffer_data_reply, sizeof(cl_int), &offset_buffer);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(&num_devices_ret_, buffer_data_reply, sizeof(cl_uint), &offset_buffer);
    buffer_data_reply += sizeof(cl_uint);

    _ccl_memcpy(out_devices, buffer_data_reply, sizeof(cl_device_id) * num_devices_ret_, &offset_buffer);
    buffer_data_reply += sizeof(cl_device_id) * num_devices_ret_;

    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);

    for (int i = 0; i < num_devices_ret_; i++)
    {
        ptr = register_object(out_devices[i], getDispachPointer(1), obj->daemon);
        obj = *(cl_opencl_object **)ptr;
        out_devices[i] = obj->object_local;
    }

    if (num_devices_ret != NULL)
        *num_devices_ret = num_devices_ret_;
#if DEBUG == 0
    puts("--- End execute clCreateSubDevices primitive\n ---");
#endif

    return (result_);
}
POsym(clCreateSubDevices)
