#include "pocl_cl.h"
extern CL_API_ENTRY cl_mem CL_API_CALL
POname(clCreateImage)(cl_context context, cl_mem_flags flags, const cl_image_format *image_format, const cl_image_desc *image_desc, void *host_ptr, cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_2
{
#if DEBUG == 0
    puts("--- Start execute clCreateImage primitive\n ---");
#endif
    char id = 0x11;
    struct sockaddr_in addr;
    void *buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0;
    size_t size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;

    cl_opencl_object *obj = NULL;
    ptr = lookup_object(context);
    cl_mem result;
    cl_int errcode_ret_;

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

    size_t size_host_ptr = 0;
    if (host_ptr != NULL)
    {
        size_host_ptr = image_desc->image_width * image_desc->image_height * sizeof(size_t);
        /*  switch (image_desc->image_type)
          {
              case (CL_MEM_OBJECT_IMAGE1D):
              {
                  size_host_ptr = image_desc->image_row_pitch;
                  break;
              }
              case (CL_MEM_OBJECT_IMAGE1D_BUFFER):
              {
                  size_host_ptr = image_desc->image_row_pitch;
                  break;
              }
              case (CL_MEM_OBJECT_IMAGE2D):
              {
                  size_host_ptr = image_desc->image_row_pitch * image_desc->image_height;
                  break;
              }
              case (CL_MEM_OBJECT_IMAGE3D):
              {
                  size_host_ptr = image_desc->image_slice_pitch * image_desc->image_depth;
                  break;
              }
              case CL_MEM_OBJECT_IMAGE1D_ARRAY:
              {
                  size_host_ptr = image_desc->image_slice_pitch * image_desc->image_array_size;
                  break;
              }
              case CL_MEM_OBJECT_IMAGE2D_ARRAY:
              {
                  size_host_ptr = image_desc->image_slice_pitch * image_desc->image_array_size;
                  break;
              }
              default:
              {
                  *errcode_ret = CL_INVALID_HOST_PTR;
                  return NULL;
              }
          }*/
    }
    size_buffer_data_request = sizeof(size_t) + size_host_ptr + sizeof(cl_context) + sizeof(cl_mem_flags) + sizeof(cl_image_format) + sizeof(cl_image_desc);
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

    _ccl_memcpy(buffer_data_request, &size_host_ptr, sizeof(size_t), &offset_buffer);
    buffer_data_request += sizeof(size_t);

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof(cl_context), &offset_buffer);
    buffer_data_request += sizeof(cl_context);

    _ccl_memcpy(buffer_data_request, &flags, sizeof(cl_mem_flags), &offset_buffer);
    buffer_data_request += sizeof(cl_mem_flags);

    cl_image_format *ss = (cl_image_format *)image_format;
    _ccl_memcpy(buffer_data_request, ss, sizeof(cl_image_format), &offset_buffer);
    buffer_data_request += sizeof(cl_image_format);

    cl_image_desc *sss = (cl_image_desc *)image_desc;
    _ccl_memcpy(buffer_data_request, sss, sizeof(cl_image_desc), &offset_buffer);
    buffer_data_request += sizeof(cl_image_desc);

    if (host_ptr != NULL)
    {
        _ccl_memcpy(buffer_data_request, host_ptr, size_host_ptr, &offset_buffer);
        buffer_data_request += size_host_ptr;
    }
    buffer_data_request -= offset_buffer;
    size_buffer_data_reply = sizeof(cl_mem) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
    char *id_transaction = NULL;
    char ip[16];
    struct sockaddr_in addr_local;
    init_udp(&fd, &addr_local, &addr, "clCreateImage");
    inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int)pthread_self());
    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateImage");
    free(id_transaction);

    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateImage") == 1)
    {
        free(buffer_data_reply);
        if (errcode_ret != NULL)
            *errcode_ret = CL_ERROR_NETWORK;
        closeSocketUdp(&fd);
        return NULL;
    }
    closeSocketUdp(&fd);
#else
    int fd_connect = init_tcp(&fd, &addr, "clCreateImage");
    // header = build_header_tcp(id, &size_buffer_data_request);
    send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateImage");
    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateImage");
    closeSocketTCP(&fd, &fd_connect);
#endif
    offset_buffer = 0;
    free(header);
    free(buffer_data_request);

    _ccl_memcpy(&result, buffer_data_reply, sizeof(cl_mem), &offset_buffer);
    buffer_data_reply += sizeof(cl_mem);

    _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof(cl_int), &offset_buffer);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_buffer;
    free(buffer_data_reply);

    if (errcode_ret_ == CL_SUCCESS)
    {
        ptr = register_object(result, getDispachPointer(4), obj->daemon);
        obj = *(cl_opencl_object **)ptr;
        result = obj->object_local;
    }

    if (errcode_ret != NULL)
        *errcode_ret = errcode_ret_;
#if DEBUG == 0
    puts("--- End execute clCreateImage primitive\n ---");
#endif
    return (result);
}
POsym(clCreateImage)
