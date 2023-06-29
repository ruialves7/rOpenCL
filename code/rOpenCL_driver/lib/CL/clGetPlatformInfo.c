#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clGetPlatformInfo)(cl_platform_id platform,cl_platform_info param_name, size_t param_value_size, void * param_value,size_t * param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{ 
#if DEBUG == 0
        puts("--- Start execute clGetPlatformInfo primitive\n ---");    
#endif
    
     pthread_mutex_lock(&protected_daemons);
    if(isReady==0)
    { 
        init_hostprogram();
    }
    pthread_mutex_unlock(&protected_daemons);
    
    
    if (daemon_count!= 0)
    {   
        char id = 0x03;
        struct sockaddr_in addr;
        void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL,*ptr = NULL;
        int fd = 0; size_t size_buffer_data_request = 0; int size_buffer_data_reply = 0, offset_buffer = 0;
        cl_int result;
        size_t _param_value_size_ret;
        void * _param_value = NULL;
        ptr = lookup_object(platform);
        cl_opencl_object * obj = NULL;

        if (ptr != NULL) 
        {
            obj = *(cl_opencl_object**)ptr;
            addr.sin_addr.s_addr = obj->daemon->addr.sin_addr.s_addr;
            addr.sin_family = obj->daemon->addr.sin_family;
            addr.sin_port = obj->daemon->addr.sin_port;
            
        } else 
        {
            return CL_INVALID_VALUE;
        }

        size_buffer_data_request = sizeof (cl_platform_id) + sizeof (cl_platform_info) + sizeof (size_t);
        
#if PROTOCOL == 1
    size_buffer_data_request+=SIZE_HEADER_TCP;
#endif

    buffer_data_request = malloc(size_buffer_data_request);

#if PROTOCOL == 1

    _ccl_memcpy(buffer_data_request, &id, sizeof (char), &offset_buffer);
    buffer_data_request += sizeof (char);

    _ccl_memcpy(buffer_data_request, &size_buffer_data_request, sizeof (size_t), &offset_buffer);
    buffer_data_request += sizeof (size_t);

#endif

        _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_platform_id), &offset_buffer);
        buffer_data_request += sizeof (cl_platform_id);

        _ccl_memcpy(buffer_data_request, &param_name, sizeof (cl_platform_info), &offset_buffer);
        buffer_data_request += sizeof (cl_platform_info);

        _ccl_memcpy(buffer_data_request, &param_value_size, sizeof (size_t), &offset_buffer);
        buffer_data_request += sizeof (size_t);
        
        buffer_data_request -= offset_buffer;

        size_buffer_data_reply = sizeof (cl_int) + sizeof (size_t);
        
        if(param_value_size>0)
            size_buffer_data_reply+=param_value_size;
        
        buffer_data_reply = malloc(size_buffer_data_reply);
       
#if PROTOCOL == 0
        
            char *id_transaction = NULL;
            char ip[16];
            struct sockaddr_in addr_local;
            init_udp(&fd, &addr_local, &addr, "clGetPlatformInfo");
            inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
            id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
            header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
            send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetPlatformInfo");
            free(id_transaction);

            if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetPlatformInfo") == 1) 
            {
                    free(buffer_data_reply);
                    closeSocketUdp(&fd);
                return CL_ERROR_NETWORK;
            }
            closeSocketUdp(&fd);
            
#else
            
            int fd_connect = init_tcp(&fd, &addr, "clGetPlatformInfo");
            //header = build_header_tcp(id, &size_buffer_data_request);
            send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clGetPlatformInfo");
            recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetPlatformInfo");
            closeSocketTCP(&fd,&fd_connect);
            
#endif   
        free(buffer_data_request);
        free(header);

        offset_buffer = 0;

        _ccl_memcpy(&result, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        _ccl_memcpy(&_param_value_size_ret, buffer_data_reply, sizeof (size_t), &offset_buffer);
        buffer_data_reply += sizeof (size_t);
        
        if(param_value_size>0)
        {
            _param_value = malloc(_param_value_size_ret);
            _ccl_memcpy(_param_value, buffer_data_reply, _param_value_size_ret, &offset_buffer);
            buffer_data_reply += _param_value_size_ret;
        }

        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);
        if (result == CL_SUCCESS) 
        {
            if (param_value_size_ret != NULL)
                *param_value_size_ret = _param_value_size_ret;

            if (param_value != NULL) 
            {
                if(param_name == CL_PLATFORM_NAME)
                {
                  /* char *s =  (char*)_param_value;
                    int len1 = strlen(s);
                    char *name_plat =  malloc(len1+1);
                    memcpy(name_plat,s,len1+1);
                    name_plat[len1+1]='\0';
                    cl_int res = POname(clGetPlatformInfo)(platform,CL_PLATFORM_HOSTNAME,param_value_size, param_value,param_value_size_ret);
                    s = (char*)param_value;
                    int len2 =  strlen(s);
                    char *host =  malloc(len2+1);
                    memcpy(host,s,len2+1);
                    host[len2+1]='\0';
                    int total = len1+len2+3;
                    
                    char * final_string =  malloc(total);
                    memcpy(final_string,name_plat,len1+1);
                    final_string[strlen(final_string)+1]='\0';
                    free(name_plat);
                    
                    memcpy(final_string+len1," [",3);
                    final_string[strlen(final_string)+1]='\0';
                    
                    memcpy(final_string+len1+2,host,len2+1);
                    final_string[strlen(final_string)+1]='\0';
                    free(host);
                    
                    memcpy(final_string+len1+2+len2,"] ",3);
                    final_string[strlen(final_string)+1]='\0';
                    
                    free(_param_value);
                    memset(param_value,0,param_value_size);
                    memcpy(param_value, final_string, strlen(final_string)+1);*/
   
                }else
                {
                    memcpy(param_value, _param_value, _param_value_size_ret);
                    free(_param_value);
                }
            }
        }
#if DEBUG == 0
        puts("--- End execute clGetPlatformInfo primitive\n ---");    
#endif

        return (result);

    }else
    {
        const char *ret;
        size_t retlen;
       switch (param_name)
        {
            case CL_PLATFORM_PROFILE:
                {
                    POCL_RETURN_GETINFO_STR("Missing HostFile");
                }
            case CL_PLATFORM_VERSION:
                {
                    POCL_RETURN_GETINFO_STR ("Missing HostFile");
                }
            case CL_PLATFORM_NAME:
                {
                    POCL_RETURN_GETINFO_STR("Missing HostFile");
                }
            case CL_PLATFORM_VENDOR:
                {
                    POCL_RETURN_GETINFO_STR("Missing HostFile");
                }
            case CL_PLATFORM_EXTENSIONS:
                {
                    #ifdef BUILD_ICD
      POCL_RETURN_GETINFO_STR("cl_khr_icd");
#else
      POCL_RETURN_GETINFO_STR("");
#endif
    }
    case CL_PLATFORM_ICD_SUFFIX_KHR:
    {
      POCL_RETURN_GETINFO_STR("Missing HostFile");
    }

    default:
    {
      return CL_INVALID_VALUE;
    }
  }
  retlen = strlen(ret) + 1;
  if (param_value != NULL)
  {
    if (param_value_size < retlen)
      return CL_INVALID_VALUE;

    memcpy(param_value, ret, retlen);
  }

  if (param_value_size_ret != NULL)
    *param_value_size_ret = retlen;
  return CL_SUCCESS;
 }
 

}
POsym(clGetPlatformInfo)
