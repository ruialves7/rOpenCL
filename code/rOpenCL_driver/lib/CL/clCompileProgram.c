#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clCompileProgram)(cl_program program, cl_uint num_devices, const cl_device_id *device_list, const char *options, cl_uint num_input_headers, const cl_program *input_headers, const char **header_include_names, void(CL_CALLBACK *pfn_notify)(cl_program program, void *user_data), void *user_data) CL_API_SUFFIX__VERSION_1_2
{
#if DEBUG == 0
    puts("--- Start execute clCompileProgram primitive\n ---");
#endif
    char id = 0x56;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_int result_;

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

    size_buffer_data_request = sizeof(cl_program) + 3 * sizeof(cl_uint);

    if (num_devices > 0)
        size_buffer_data_request += sizeof(cl_device_id) * num_devices;

    cl_uint size_options = 0;
    size_t *size_array = NULL;

    if (options != NULL)
        size_options = strlen(options);

    if (size_options > 0)
        size_buffer_data_request += size_options;

    if (num_input_headers > 0)
    {

        size_buffer_data_request += sizeof(cl_program) * num_input_headers;
        size_buffer_data_request += sizeof(size_t) * num_input_headers;
        size_array = malloc(sizeof(size_t) * num_input_headers);

        for (int i = 0; i < num_input_headers; i++)
        {
            size_array[i] = strlen(header_include_names[i]);
            size_buffer_data_request += size_array[i];
        }
    }
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

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_program), &offset_buffer);
    buffer_data_request += sizeof(cl_program);

    _ccl_memcpy(buffer_data_request, &num_devices, sizeof(cl_uint), &offset_buffer);
    buffer_data_request += sizeof(cl_uint);

    _ccl_memcpy(buffer_data_request, &num_input_headers, sizeof(cl_uint), &offset_buffer);
    buffer_data_request += sizeof(cl_uint);

    _ccl_memcpy(buffer_data_request, &size_options, sizeof(cl_uint), &offset_buffer);
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

    if (size_options > 0)
    {
        char *ss = (char *)options;
        _ccl_memcpy(buffer_data_request, ss, size_options, &offset_buffer);
        buffer_data_request += size_options;
    }

    if (num_input_headers > 0)
    {
        cl_program *pp = (cl_program *)input_headers;
        _ccl_memcpy(buffer_data_request, pp, sizeof(cl_program) * num_input_headers, &offset_buffer);
        buffer_data_request += sizeof(cl_program) * num_input_headers;

        _ccl_memcpy(buffer_data_request, size_array, sizeof(size_t) * num_input_headers, &offset_buffer);
        buffer_data_request += sizeof(size_t) * num_input_headers;

        for (int i = 0; i < num_input_headers; i++)
        {
            char *ss = (char *)header_include_names[i];
            _ccl_memcpy(buffer_data_request, ss, size_array[i], &offset_buffer);
            buffer_data_request += size_array[i];
        }
        free(size_array);
    }
    buffer_data_request -= offset_buffer;

    size_buffer_data_reply = sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0

    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clCompileProgram");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCompileProgram");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCompileProgram") == 1)
    {
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return CL_ERROR_NETWORK;
    }
    closeSocketUdp(&fd);

#else

    int fd_connect = init_tcp(&fd, &addr, "clCompileProgram");
    //        header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCompileProgram");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCompileProgram");
    closeSocketTCP(&fd, &fd_connect);
#endif

    offset_buffer = 0;
    free(header);
    free(buffer_data_request);

    _ccl_memcpy(&result_, buffer_data_reply, sizeof(cl_int), &offset_buffer);
    buffer_data_reply += sizeof(cl_program);

    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);
#if DEBUG == 0
    puts("--- End execute clCompileProgram primitive\n ---");
#endif

    return (result_);
}
POsym(clCompileProgram)
