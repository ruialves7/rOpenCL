#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clGetContextInfo)(cl_context context,cl_context_info param_name,size_t param_value_size,void *param_value,size_t *param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clGetContextInfo primitive\n ---");    
#endif
    char id = 0x29;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *ptr = NULL;
    int fd = 0;size_t size_buffer_data_request = 0;int size_buffer_data_reply = 0, offset_buffer = 0;
   
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

    struct{

        cl_context context;
        cl_context_info param_name;
        size_t param_value_size;
        
    }ccl_request ={.context=obj->object_remote,.param_name=param_name,.param_value_size=param_value_size};

    struct{
            cl_int result;
            size_t param_value_size_ret;
    }ccl_reply;
   
    size_buffer_data_request = sizeof (ccl_request);

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

    _ccl_memcpy(buffer_data_request, &ccl_request, sizeof (ccl_request), &offset_buffer);
    buffer_data_request += sizeof (ccl_request);

    buffer_data_request -= offset_buffer;

    size_buffer_data_reply = sizeof (ccl_reply) + param_value_size;
    buffer_data_reply = malloc(size_buffer_data_reply);
     
#if PROTOCOL == 0
        
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clGetContextInfo");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        void * header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetContextInfo");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetContextInfo") == 1) {
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;
        }
        closeSocketUdp(&fd);
        
#else
        
       int fd_connect =init_tcp(&fd, &addr, "clGetContextInfo");
        send_data_tcp(&fd, buffer_data_request, NULL, size_buffer_data_request, "clGetContextInfo");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetContextInfo");
        closeSocketTCP(&fd,&fd_connect);
        
#endif

        free(buffer_data_request);
        memcpy(&ccl_reply, buffer_data_reply, sizeof (ccl_reply));
        
        if (ccl_reply.result == CL_SUCCESS) 
        {
            if (param_value_size_ret != NULL)
                *param_value_size_ret = ccl_reply.param_value_size_ret;

            if (param_value != NULL) 
            { 
                memcpy(param_value,buffer_data_reply+sizeof(ccl_reply),ccl_reply.param_value_size_ret);
                free(buffer_data_reply);
                
		/*puts("X");
                if(param_name==CL_CONTEXT_DEVICES)
                {
                    int num = -1,val=0;

                    if(param_value_size_ret!=NULL)
                    {
                        num = *param_value_size_ret/sizeof(cl_device_id); 
                        val = *param_value_size_ret;
                    }else
                    {
                        num = param_value_size/sizeof(cl_device_id);
                        val = param_value_size;
                    }
                    
		   cl_device_id * dev = NULL;
                   cl_device_id * dev_array =  malloc(val); 
		   for(int i =0; i<num;i++)
                    {
			dev = (cl_device_id*)(param_value+(sizeof(cl_device_id)*i));
                        ptr  = register_object(dev,getDispachPointer(1), obj->daemon);
                        obj = *(cl_opencl_object**)ptr;
			*(dev_array+i) = obj->object_local;
                             
                    }   
			memcpy(param_value,dev_array,val);    
			free(dev_array);
                }*/
		if(param_name==CL_CONTEXT_DEVICES)
                {
                    int num = -1,val=0;
                    if(param_value_size_ret!=NULL)
                    {
                        num = *param_value_size_ret/sizeof(cl_device_id); 
                        val = *param_value_size_ret;
                    }else
                    {
                        num = param_value_size/sizeof(cl_device_id);
                        val = param_value_size;
                    }
                    
                    cl_device_id *vetor_re =  malloc(sizeof(cl_device_id)*num);  
                    memcpy(vetor_re,param_value , val);    
                    cl_device_id *vetor =  malloc(sizeof(cl_device_id)*num);
                    
                    
                    for(int i =0; i<num;i++)
                    {
                        ptr  = register_object(vetor_re[i],getDispachPointer(1), obj->daemon);
                        obj = *(cl_opencl_object**)ptr;
                        vetor[i] = obj->object_local;     
                    }
                    
                    memcpy(param_value, vetor, val);
                    free(vetor);
                    //free(_param_value);
                    
                }else
                {
                    //memcpy(param_value, _param_value, _param_value_size_ret);
                    //free(_param_value);
                }

            }
        }
#if DEBUG == 0
        puts("--- End execute clGetContextInfo primitive\n ---");    
#endif
    return (ccl_reply.result);  
}
POsym(clGetContextInfo)
