#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clReleaseSampler)(cl_sampler sampler) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
    puts("--- Start execute clReleaseSampler primitive\n ---");
#endif
    char id = 0x4B;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;

    ptr = lookup_object(sampler);
    cl_int result;
    cl_opencl_object *obj = NULL;

    if (ptr != NULL)
    {

        obj = *(cl_opencl_object **)ptr;
        /* addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
         addr.sin_family = obj->daemon->addr.sin_family;
         addr.sin_port = obj->daemon->addr.sin_port;*/
        memcpy(&addr, &obj->daemon->addr, sizeof(struct sockaddr_in));
    }
    else
    {

        return CL_INVALID_VALUE;
    }

    size_buffer_data_request = sizeof(cl_sampler);
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

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_sampler), &offset_buffer);
    buffer_data_request += sizeof(cl_sampler);

    buffer_data_request -= offset_buffer;

    size_buffer_data_reply = sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0

    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clReleaseSampler");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clReleaseSampler");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clReleaseSampler") == 1)
    {
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return CL_ERROR_NETWORK;
    }
    closeSocketUdp(&fd);
#else
    int fd_connect = init_tcp(&fd, &addr, "clReleaseSampler");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clReleaseSampler");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clReleaseSampler");
    closeSocketTCP(&fd, &fd_connect);
#endif

    offset_buffer = 0;
    free(header);
    free(buffer_data_request);

    _ccl_memcpy(&result, buffer_data_reply, sizeof(cl_int), &offset_buffer);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);

    // delete_object(obj);
#if DEBUG == 0
    puts("--- End execute clReleaseSampler primitive\n ---");
#endif

    return (result);
}
POsym(clReleaseSampler)
