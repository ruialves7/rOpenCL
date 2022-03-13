#include "pocl_cl.h"
extern CL_API_ENTRY cl_int CL_API_CALL
POname(clGetSupportedImageFormats) (cl_context   context,cl_mem_flags flags,cl_mem_object_type  image_type, cl_uint num_entries,cl_image_format * image_formats,cl_uint *  num_image_formats) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clGetSupportedImageFormats primitive\n ---");    
#endif
    char id = 0x30;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_int result;
    cl_uint num_image_formats_;
    cl_opencl_object * obj = NULL;
    ptr = lookup_object(context);
    
    if (ptr != NULL) {
        
       obj = *(cl_opencl_object**)ptr;
       addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;
        
    } else {
        return CL_INVALID_VALUE;
    }

        size_buffer_data_request = sizeof (cl_context) + sizeof (cl_mem_flags) + sizeof (cl_mem_object_type) + sizeof (cl_uint);
#if PROTOCOL == 1
    size_buffer_data_request+=SIZE_HEADER_TCP;
#endif

    buffer_data_request = malloc(size_buffer_data_request);

#if PROTOCOL == 1

    _ccl_memcpy(buffer_data_request, &id, sizeof (char), &offset_buffer);
    buffer_data_request += sizeof (char);

    _ccl_memcpy(buffer_data_request, &size_buffer_data_request, sizeof (int), &offset_buffer);
    buffer_data_request += sizeof (int);

#endif

        _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_context), &offset_buffer);
        buffer_data_request += sizeof (cl_context);

        _ccl_memcpy(buffer_data_request, &flags, sizeof (cl_mem_flags), &offset_buffer);
        buffer_data_request += sizeof (cl_mem_flags);

        _ccl_memcpy(buffer_data_request, &image_type, sizeof (cl_mem_object_type), &offset_buffer);
        buffer_data_request += sizeof (cl_mem_object_type);

        _ccl_memcpy(buffer_data_request, &num_entries, sizeof (cl_uint), &offset_buffer);
        buffer_data_request += sizeof (cl_uint);

        buffer_data_request -= offset_buffer;

        size_buffer_data_reply = sizeof (cl_int) + sizeof (cl_uint) + sizeof (cl_image_format) * num_entries;
        buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clGetSupportedImageFormats");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetSupportedImageFormats");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetSupportedImageFormats") == 1) {
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;
        }
        closeSocketUdp(&fd);
#else
        int fd_connect = init_tcp(&fd, &addr, "clGetSupportedImageFormats");
        //header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clGetSupportedImageFormats");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetSupportedImageFormats");
        closeSocketTCP(&fd,&fd_connect);
#endif
        offset_buffer = 0;
        free(header);
        free(buffer_data_request);

        _ccl_memcpy(&result, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        _ccl_memcpy(&num_image_formats_, buffer_data_reply, sizeof (cl_uint), &offset_buffer);
        buffer_data_reply += sizeof (cl_uint);

        if (image_formats != NULL) {

            _ccl_memcpy(image_formats, buffer_data_reply, sizeof (cl_image_format) * num_image_formats_, &offset_buffer);
            buffer_data_reply += sizeof (cl_image_format) * num_image_formats_;
        }
        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (result == CL_SUCCESS) {
            if (num_image_formats != NULL)
                *num_image_formats = num_image_formats_;
        }
#if DEBUG == 0
        puts("--- End execute clGetSupportedImageFormats primitive\n ---");    
#endif
    return (result);
} 
POsym(clGetSupportedImageFormats)