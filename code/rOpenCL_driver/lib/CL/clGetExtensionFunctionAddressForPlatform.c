#include "pocl_cl.h"
CL_API_ENTRY void *CL_API_CALL
POname(clGetExtensionFunctionAddressForPlatform)(cl_platform_id platform, const char *func_name) CL_EXT_SUFFIX__VERSION_1_2
{
#if DEBUG == 0
    puts("--- Start execute clGetExtensionFunctionAddressForPlatform primitive\n ---");
#endif
    char id = 0x2B;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    void *result = malloc(sizeof(void *));

    ptr = lookup_object(platform);
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

        return NULL;
    }
    size_buffer_data_request = sizeof(cl_platform_id) + sizeof(int);
    int size = strlen(func_name);

    if (size > 0)
        size_buffer_data_request += (strlen(func_name));

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

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_platform_id), &offset_buffer);
    buffer_data_request += sizeof(cl_platform_id);

    _ccl_memcpy(buffer_data_request, &size, sizeof(int), &offset_buffer);
    buffer_data_request += sizeof(int);

    if (size > 0)
    {
        char *ss = (char *)func_name;
        _ccl_memcpy(buffer_data_request, ss, strlen(func_name), &offset_buffer);
        buffer_data_request += strlen(func_name);
    }

    buffer_data_request -= offset_buffer;
    size_buffer_data_reply = sizeof(void *);
    buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0

    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clGetExtensionFunctionAddressForPlatform");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetExtensionFunctionAddressForPlatform");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetExtensionFunctionAddressForPlatform") == 1)
    {
        free(buffer_data_reply);
        closeSocketUdp(&fd);
        return NULL;
    }
    closeSocketUdp(&fd);
#else
    int fd_connect = init_tcp(&fd, &addr, "clGetExtensionFunctionAddressForPlatform");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clGetExtensionFunctionAddressForPlatform");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetExtensionFunctionAddressForPlatform");
    closeSocketTCP(&fd, &fd_connect);
#endif

    offset_buffer = 0;
    free(header);
    free(buffer_data_request);

    _ccl_memcpy(result, buffer_data_reply, sizeof(void *), &offset_buffer);
    buffer_data_reply += sizeof(void *);

    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);
#if DEBUG == 0
    puts("--- End execute clGetExtensionFunctionAddressForPlatform primitive\n ---");
#endif
    return (result);
}
POsymAlways(clGetExtensionFunctionAddressForPlatform)
