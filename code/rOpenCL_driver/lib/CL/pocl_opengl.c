#include "pocl_cl.h"

CL_API_ENTRY cl_mem CL_API_CALL
POname(clCreateFromGLTexture)(cl_context context, cl_mem_flags flags, cl_GLenum texture_target,cl_GLint  miplevel,cl_GLuint texture,cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_2
{
    char id = 0x71;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, * ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_mem result_;
    cl_int errcode_ret_;
    
    ptr = lookup_object(context);
    cl_opencl_object * obj = NULL;
    if (ptr != NULL) 
    {
       obj = *(cl_opencl_object**)ptr;
       addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;
       
    } else 
    {
        if (errcode_ret != NULL)
            *errcode_ret = CL_INVALID_VALUE;
        return NULL;
    }

        size_buffer_data_request = sizeof (cl_context) + sizeof (cl_mem_flags) + sizeof (GLenum) + sizeof (GLint) + sizeof (GLuint);
        buffer_data_request = malloc(size_buffer_data_request);

        _ccl_memcpy(buffer_data_request, obj->object_remote, sizeof (cl_context), &offset_buffer);
        buffer_data_request += sizeof (cl_context);

        _ccl_memcpy(buffer_data_request, &flags, sizeof (cl_mem_flags), &offset_buffer);
        buffer_data_request += sizeof (cl_mem_flags);

        _ccl_memcpy(buffer_data_request, &texture_target, sizeof (GLenum), &offset_buffer);
        buffer_data_request += sizeof (GLenum);

        _ccl_memcpy(buffer_data_request, &miplevel, sizeof (GLint), &offset_buffer);
        buffer_data_request += sizeof (GLint);

        _ccl_memcpy(buffer_data_request, &texture, sizeof (GLuint), &offset_buffer);
        buffer_data_request += sizeof (GLuint);


        buffer_data_request -= offset_buffer;
        size_buffer_data_reply = sizeof (cl_mem) + sizeof (cl_int);
        buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0 
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clCreateFromGLTexture");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateFromGLTexture");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateFromGLTexture") == 1) {
            if (errcode_ret != NULL)
                *errcode_ret = CL_ERROR_NETWORK;
            free(buffer_data_reply);
            return NULL;
        }
        close(fd);
#else
        init_tcp(&fd, &addr, "clCreateFromGLTexture");
        header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateFromGLTexture");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateFromGLTexture");
        close(fd);
#endif
        free(header);
        free(buffer_data_request);
        offset_buffer = 0;

        _ccl_memcpy(&result_, buffer_data_reply, sizeof (cl_mem), &offset_buffer);
        buffer_data_reply += sizeof (cl_mem);

        _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (errcode_ret_ == CL_SUCCESS)
        {
            ptr  = register_object(result_, getDispachPointer(4), obj->daemon);
            obj = *(cl_opencl_object**)ptr;
            result_= obj->object_local;
        }
        
        if (errcode_ret != NULL)
                *errcode_ret = errcode_ret_;
        return (result_);   
}
POsym(clCreateFromGLTexture)



CL_API_ENTRY cl_mem CL_API_CALL
POname(clCreateFromGLBuffer)(cl_context context,cl_mem_flags flags, cl_GLuint  bufobj, cl_int * errcode_ret)
CL_API_SUFFIX__VERSION_1_0
{
    char id = 0x70;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, * ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_mem result_;
    cl_int errcode_ret_;
    ptr = lookup_object(context);
    
    cl_opencl_object * obj = NULL;
    if (ptr != NULL) 
    {
       obj = *(cl_opencl_object**)ptr;
       addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;
       
    } else {
        if (errcode_ret != NULL)
            *errcode_ret = CL_INVALID_VALUE;
        return NULL;
    }
        size_buffer_data_request = sizeof (cl_context) + sizeof (cl_mem_flags) + sizeof (GLuint);
        buffer_data_request = malloc(size_buffer_data_request);

        _ccl_memcpy(buffer_data_request, obj->object_remote, sizeof (cl_context), &offset_buffer);
        buffer_data_request += sizeof (cl_context);

        _ccl_memcpy(buffer_data_request, &flags, sizeof (cl_mem_flags), &offset_buffer);
        buffer_data_request += sizeof (cl_mem_flags);

        _ccl_memcpy(buffer_data_request, &bufobj, sizeof (GLuint), &offset_buffer);
        buffer_data_request += sizeof (GLuint);

        buffer_data_request -= offset_buffer;
        size_buffer_data_reply = sizeof (cl_mem) + sizeof (cl_int);
        buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
        
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clCreateFromGLBuffer");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateFromGLBuffer");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateFromGLBuffer") == 1) {
            if (errcode_ret != NULL)
                *errcode_ret = CL_ERROR_NETWORK;
            free(buffer_data_reply);
            return NULL;
        }
        close(fd);
#else
        init_tcp(&fd, &addr, "clCreateFromGLBuffer");
        header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateFromGLBuffer");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateFromGLBuffer");
        close(fd);
#endif
        
        free(header);
        free(buffer_data_request);
        offset_buffer = 0;

        _ccl_memcpy(&result_, buffer_data_reply, sizeof (cl_mem), &offset_buffer);
        buffer_data_reply += sizeof (cl_mem);

        _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (errcode_ret_ == CL_SUCCESS)
        {
            ptr  = register_object(result_, getDispachPointer(4), obj->daemon);
            obj = *(cl_opencl_object**)ptr;
            result_= obj->object_local;
        }
        
    if (errcode_ret != NULL)
        *errcode_ret = errcode_ret_;
    
    return (result_);
        
}
POsym(clCreateFromGLBuffer)



CL_API_ENTRY cl_mem CL_API_CALL
POname(clCreateFromGLRenderbuffer)(cl_context context,cl_mem_flags flags, cl_GLuint  renderbuffer,cl_int * errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
    char id = 0x72;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, * ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_mem result_;
    cl_int errcode_ret_;
    ptr = lookup_object(context);
    cl_opencl_object * obj = NULL;
    if (ptr != NULL) 
    {
       obj = *(cl_opencl_object**)ptr;
       addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;
       
    } else {
        
        if (errcode_ret != NULL)
            *errcode_ret = CL_INVALID_VALUE;
        return NULL;
    }

        size_buffer_data_request = sizeof (cl_context) + sizeof (cl_mem_flags) + sizeof (GLuint);
        buffer_data_request = malloc(size_buffer_data_request);

        _ccl_memcpy(buffer_data_request, obj->object_remote, sizeof (cl_context), &offset_buffer);
        buffer_data_request += sizeof (cl_context);

        _ccl_memcpy(buffer_data_request, &flags, sizeof (cl_mem_flags), &offset_buffer);
        buffer_data_request += sizeof (cl_mem_flags);

        _ccl_memcpy(buffer_data_request, &renderbuffer, sizeof (GLuint), &offset_buffer);
        buffer_data_request += sizeof (GLuint);

        buffer_data_request -= offset_buffer;
        size_buffer_data_reply = sizeof (cl_mem) + sizeof (cl_int);
        buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr,"clCreateFromGLRenderbuffer");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateFromGLRenderbuffer");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateFromGLRenderbuffer") == 1) {
            if (errcode_ret != NULL)
                *errcode_ret = CL_ERROR_NETWORK;
            free(buffer_data_reply);
            return NULL;
        }
        close(fd);
#else
        init_tcp(&fd, &addr, "clCreateFromGLRenderbuffer");
        header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateFromGLRenderbuffer");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateFromGLRenderbuffer");
        close(fd);
#endif
        free(header);
        free(buffer_data_request);
        offset_buffer = 0;

        _ccl_memcpy(&result_, buffer_data_reply, sizeof (cl_mem), &offset_buffer);
        buffer_data_reply += sizeof (cl_mem);

        _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (errcode_ret_ == CL_SUCCESS)
        {
            ptr  = register_object(result_, getDispachPointer(4), obj->daemon);
            obj = *(cl_opencl_object**)ptr;
            result_= obj->object_local;
        }

    if (errcode_ret != NULL)
        *errcode_ret = errcode_ret_;
    return (result_);
   
}
POsym(clCreateFromGLRenderbuffer)



CL_API_ENTRY cl_int CL_API_CALL
POname(clGetGLObjectInfo)(cl_mem  memobj,cl_gl_object_type *gl_object_type,cl_GLuint *gl_object_name)CL_API_SUFFIX__VERSION_1_0
{
    char id = 0x73;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_int result_;
    GLuint gl_result;
    ptr = lookup_object(memobj);
    cl_opencl_object * obj = NULL;
    if (ptr != NULL) 
    {
       obj = *(cl_opencl_object**)ptr;
       addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;
       
    } else {
        return CL_INVALID_VALUE;
    }

        size_buffer_data_request = sizeof (cl_mem);
        buffer_data_request = malloc(size_buffer_data_request);

        _ccl_memcpy(buffer_data_request, obj->object_remote, sizeof (cl_mem), &offset_buffer);
        buffer_data_request += sizeof (cl_mem);

        buffer_data_request -= offset_buffer;
        size_buffer_data_reply = sizeof (cl_int) + sizeof (cl_gl_object_type) + sizeof (GLuint);
        buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clGetGLObjectInfo");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetGLObjectInfo");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetGLObjectInfo") == 1) {
            free(buffer_data_reply);
            return CL_ERROR_NETWORK;
        }
        close(fd);
#else
        init_tcp(&fd, &addr, "clGetGLObjectInfo");
        header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clGetGLObjectInfo");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetGLObjectInfo");
        close(fd);
#endif
        free(header);
        free(buffer_data_request);
        offset_buffer = 0;

        _ccl_memcpy(&result_, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        _ccl_memcpy(gl_object_type, buffer_data_reply, sizeof (cl_gl_object_type), &offset_buffer);
        buffer_data_reply += sizeof (cl_gl_object_type);

        _ccl_memcpy(&gl_result, buffer_data_reply, sizeof (GLuint), &offset_buffer);
        buffer_data_reply += sizeof (GLuint);

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (gl_object_name != NULL)
            memcpy(gl_object_name, &gl_result, sizeof (GLuint));
    return (result_);
}
POsym(clGetGLObjectInfo)

CL_API_ENTRY cl_int CL_API_CALL
POname(clGetGLTextureInfo) (cl_mem memobj,cl_gl_texture_info param_name,size_t  param_value_size, void *param_value,size_t  *param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
    char id = 0x74;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
    cl_int result;
    size_t _param_value_size_ret;
    char * _param_value = NULL;
    ptr = lookup_object(memobj);
    cl_opencl_object * obj = NULL;
    if (ptr != NULL) 
    {
       obj = *(cl_opencl_object**)ptr;
       addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
       addr.sin_family = obj->daemon->addr.sin_family;
       addr.sin_port = obj->daemon->addr.sin_port;
       
    } else {
        return CL_INVALID_VALUE;
    }

        size_buffer_data_request = sizeof (cl_mem) + sizeof (cl_gl_texture_info) + sizeof (size_t);
        buffer_data_request = malloc(size_buffer_data_request);

        _ccl_memcpy(buffer_data_request, obj->object_remote, sizeof (cl_mem), &offset_buffer);
        buffer_data_request += sizeof (cl_mem);

        _ccl_memcpy(buffer_data_request, &param_name, sizeof (cl_gl_texture_info), &offset_buffer);
        buffer_data_request += sizeof (cl_gl_texture_info);

        _ccl_memcpy(buffer_data_request, &param_value_size, sizeof (size_t), &offset_buffer);
        buffer_data_request += sizeof (size_t);

        buffer_data_request -= offset_buffer;

        size_buffer_data_reply = sizeof (cl_int) + sizeof (size_t) + sizeof (char)*param_value_size;
        buffer_data_reply = malloc(size_buffer_data_reply);
#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clGetGLTextureInfo");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetGLTextureInfo");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetGLTextureInfo") == 1) {
            free(buffer_data_reply);
            return CL_ERROR_NETWORK;
        }
        close(fd);
#else
        init_tcp(&fd, &addr, "clGetGLTextureInfo");
        header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clGetGLTextureInfo");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetGLTextureInfo");
        close(fd);
#endif
        offset_buffer = 0;
        free(header);
        free(buffer_data_request);

        _ccl_memcpy(&result, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        _ccl_memcpy(&_param_value_size_ret, buffer_data_reply, sizeof (size_t), &offset_buffer);
        buffer_data_reply += sizeof (size_t);

        _param_value = malloc(_param_value_size_ret);

        _ccl_memcpy(_param_value, buffer_data_reply, _param_value_size_ret, &offset_buffer);
        buffer_data_reply += _param_value_size_ret;


        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (result == CL_SUCCESS) {
            if (param_value_size_ret != NULL)
                *param_value_size_ret = _param_value_size_ret;

            if (param_value != NULL) {

                memcpy(param_value, _param_value, _param_value_size_ret);
                free(_param_value);
            }
        }
    return (result); 
}
POsym(clGetGLTextureInfo)

CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueAcquireGLObjects) ( cl_command_queue command_queue,cl_uint num_objects,const cl_mem *mem_objects, cl_uint num_events_in_wait_list,const cl_event *event_wait_list,cl_event *event)
CL_API_SUFFIX__VERSION_1_0
{
  return CL_OUT_OF_RESOURCES;
}
POsym(clEnqueueAcquireGLObjects)



CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueReleaseGLObjects) ( cl_command_queue command_queue, cl_uint num_objects,const cl_mem *mem_objects, cl_uint num_events_in_wait_list,const cl_event *event_wait_list,cl_event *event)
CL_API_SUFFIX__VERSION_1_0
{
  return CL_OUT_OF_RESOURCES;
}
POsym(clEnqueueReleaseGLObjects)



CL_API_ENTRY cl_int CL_API_CALL
POname(clGetGLContextInfoKHR) ( const cl_context_properties  *properties ,cl_gl_context_info  param_name ,size_t  param_value_size ,void  *param_value , size_t  *param_value_size_ret )
CL_API_SUFFIX__VERSION_1_0
{
  return CL_OUT_OF_RESOURCES;
}
POsym(clGetGLContextInfoKHR)
