#include "pocl_cl.h"

CL_API_ENTRY cl_int CL_API_CALL
POname(clReleaseContext)(cl_context context) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
    puts("--- Start execute clReleaseContext primitive\n ---");
#endif
    char id = 0x28;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *ptr = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, offset_buffer = 0;

    cl_opencl_object *obj = NULL;
    ptr = lookup_object(context);

    if (ptr != NULL)
    {

        obj = *(cl_opencl_object **)ptr;
        memcpy(&addr, &obj->daemon->addr, sizeof(struct sockaddr_in));
    }
    else
    {
        return CL_INVALID_VALUE;
    }

    struct
    {
        cl_context context;
    } ccl_request = {.context = obj->object_remote};

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    size_buffer_data_request = sizeof(ccl_request);

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
    buffer_data_request -= offset_buffer;

#if PROTOCOL == 0

    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clReleaseContext");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    void *header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clReleaseContext");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clReleaseContext") == 1)
    {
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return CL_ERROR_NETWORK;
    }
    closeSocketUdp(&fd);

#else
    int fd_connect = init_tcp(&fd, &addr, "clReleaseContext");
    send_data_tcp(&fd, buffer_data_request, NULL, size_buffer_data_request, "clReleaseContext");
    recv_data_tcp(&fd, &ccl_reply, sizeof(ccl_reply), "clReleaseContext");
    closeSocketTCP(&fd, &fd_connect);
#endif

    free(buffer_data_request);
#if DEBUG == 0
    puts("--- End execute clReleaseContext primitive\n ---");
#endif
    return (ccl_reply.errcode_ret);
}
POsym(clReleaseContext)
