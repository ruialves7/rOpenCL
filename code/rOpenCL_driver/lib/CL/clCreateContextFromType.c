#include "pocl_cl.h"
CL_API_ENTRY cl_context CL_API_CALL
POname(clCreateContextFromType)(const cl_context_properties *properties,cl_device_type device_type,void (*pfn_notify)(const char *, const void *, size_t, void *),void *user_data,cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clCreateContextFromType primitive\n ---");    
#endif 
    
        char id = 0x26;
        struct sockaddr_in addr;
        void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL,*ptr = NULL;
        int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0, num_properties = 0,i=0;

        cl_context result_;
        cl_int errcode_ret_;
        cl_context_properties pp[3];
        cl_opencl_object * obj = NULL;
        
        if (properties != NULL) 
        {
            while (properties[num_properties += 2] != 0);
            num_properties++;
            
            size_buffer_data_request += (sizeof (cl_context_properties) * num_properties);
            
        }else
        {
            if(errcode_ret!=NULL)
                *errcode_ret=CL_INVALID_PLATFORM;
            return NULL;
        }
        
        if(num_properties>3)
        {
             if(errcode_ret!=NULL)
                *errcode_ret=CL_INVALID_PLATFORM;
            return NULL;  
        }
       
        for(i=0; i<num_properties-1; i+=2)
        {
                
              pp[i] = properties[i];
              ptr = lookup_object((void*)properties[i+1]);
              if(ptr==NULL)
              {
                    if(errcode_ret!=NULL)
                        *errcode_ret=CL_INVALID_PLATFORM;
                    return NULL; 
              }
              else
              {
                obj = *(cl_opencl_object**)ptr;
                addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
                addr.sin_family = obj->daemon->addr.sin_family;
                addr.sin_port = obj->daemon->addr.sin_port;
              }
              
              pp[i+1] = (cl_context_properties)obj->object_remote; 
        }
            pp[i] = 0;
        
        int size_user_data=0;
        char * user = (char*)user_data;
        
        if(user_data!=NULL)
            size_user_data = strlen(user)+1;
            
        size_buffer_data_request += (sizeof (int)*2 + sizeof (cl_device_type));
        if(size_user_data>0)
            size_buffer_data_request +=size_user_data;
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

        _ccl_memcpy(buffer_data_request, &num_properties, sizeof (int), &offset_buffer);
        buffer_data_request += sizeof (int);
        
        _ccl_memcpy(buffer_data_request, pp, sizeof (cl_context_properties) * num_properties, &offset_buffer);
        buffer_data_request += sizeof (cl_context_properties) * num_properties;
        
        _ccl_memcpy(buffer_data_request, &device_type, sizeof (cl_device_type), &offset_buffer);
        buffer_data_request += sizeof (cl_device_type);
        
        _ccl_memcpy(buffer_data_request, &size_user_data, sizeof (int), &offset_buffer);
        buffer_data_request += sizeof (int);
            
        if(size_user_data>0)
        {
           _ccl_memcpy(buffer_data_request, user, size_user_data, &offset_buffer);
            buffer_data_request += size_user_data; 
        }

        buffer_data_request -= offset_buffer;
        size_buffer_data_reply = sizeof (cl_context) + sizeof (cl_int);
        buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0   
        
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local,&addr,  "clCreateContextFromType");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clCreateContextFromType");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateContextFromType") == 1) {
            if (errcode_ret != NULL)
                *errcode_ret = CL_ERROR_NETWORK;
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return NULL;
        }
        closeSocketUdp(&fd);
        
#else
        int fd_connect = init_tcp(&fd, &addr, "clCreateContextFromType");
        //header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clCreateContextFromType");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clCreateContextFromType");
        closeSocketTCP(&fd,&fd_connect);
#endif
        free(header);
        free(buffer_data_request);

        offset_buffer = 0;
        _ccl_memcpy(&result_, buffer_data_reply, sizeof (cl_context), &offset_buffer);
        buffer_data_reply += sizeof (cl_context);

        _ccl_memcpy(&errcode_ret_, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (errcode_ret_ == CL_SUCCESS)
        {
            ptr  = register_object(result_, getDispachPointer(2), obj->daemon);
            obj = *(cl_opencl_object**)ptr;
            result_ = obj->object_local;
            
        }else
            result_=NULL;
        
        if(errcode_ret!=NULL)
            *(errcode_ret) = errcode_ret_;
#if DEBUG == 0
        puts("--- End execute clCreateContextFromType primitive\n ---");    
#endif         
        return (result_); 
}
POsym(clCreateContextFromType)
