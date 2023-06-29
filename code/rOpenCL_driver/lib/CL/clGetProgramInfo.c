#include "pocl_cl.h"

CL_API_ENTRY cl_int CL_API_CALL
POname(clGetProgramInfo)(cl_program program,cl_program_info param_name,size_t param_value_size,void *param_value,size_t *param_value_size_ret) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clGetProgramInfo primitive\n ---");    
#endif
    char id = 0x59;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL,*ptr = NULL;
    int fd = 0;size_t size_buffer_data_request = 0;int size_buffer_data_reply = 0, offset_buffer = 0;
    int nr_position=0;

    cl_int result;
    size_t _param_value_size_ret;
    size_t * size_s = NULL;
    char * _param_value = NULL;
    char **pp = NULL;
    cl_opencl_object * obj = NULL;
    ptr = lookup_object(program);
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
    
   
    size_buffer_data_request = sizeof (cl_program) + sizeof (cl_program_info) + sizeof (size_t);
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

    _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_program), &offset_buffer);
    buffer_data_request += sizeof (cl_program);

    _ccl_memcpy(buffer_data_request, &param_name, sizeof (cl_program_info), &offset_buffer);
    buffer_data_request += sizeof (cl_program_info);
    
     _ccl_memcpy(buffer_data_request, &param_value_size, sizeof (size_t), &offset_buffer);
    buffer_data_request += sizeof (size_t);

    buffer_data_request -= offset_buffer;
     
    if(param_name == CL_PROGRAM_BINARIES)
    {
        // Foi pedido pelo programado os binaries dos program, mas  como isto é remoto,  não me basta o apontador para o array de  strings, entao precisamos de ir buscar o tamanho.
        nr_position = param_value_size / sizeof(char*);
        size_s =  malloc(sizeof(size_t)*nr_position);
        cl_int res = POname(clGetProgramInfo)(program,CL_PROGRAM_BINARY_SIZES,sizeof(size_t)*nr_position,size_s,NULL);   
    }
    
    size_buffer_data_reply = sizeof (cl_int) + sizeof (size_t);
    if(param_name!=CL_PROGRAM_BINARIES)
    {
        if(param_value_size>0)
            size_buffer_data_reply+=param_value_size; 
    }else
    {
        for(int i = 0; i<nr_position;i++)
            size_buffer_data_reply+=size_s[i];
    }
    buffer_data_reply = malloc(size_buffer_data_reply);

#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local,&addr, "clGetProgramInfo");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetProgramInfo");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetProgramInfo") == 1) {
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;
        }
        closeSocketUdp(&fd);
#else
        int fd_connect = init_tcp(&fd, &addr, "clGetProgramInfo");
        //header = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clGetProgramInfo");
        recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetProgramInfo");
        closeSocketTCP(&fd,&fd_connect);
#endif
        offset_buffer = 0;
        free(header);
        free(buffer_data_request);
   
        _ccl_memcpy(&result, buffer_data_reply, sizeof (cl_int), &offset_buffer);
        buffer_data_reply += sizeof (cl_int);

        _ccl_memcpy(&_param_value_size_ret, buffer_data_reply, sizeof (size_t), &offset_buffer);
        buffer_data_reply += sizeof (size_t);
        
        if(param_name != CL_PROGRAM_BINARIES)
        {   
            if(param_value_size>0)
            {
                _param_value = malloc(_param_value_size_ret);
                _ccl_memcpy(_param_value, buffer_data_reply, _param_value_size_ret, &offset_buffer);
                buffer_data_reply += _param_value_size_ret;
            }
        }else
        {
             pp =  malloc(sizeof(char*)*nr_position);
             for(int i=0;i<nr_position;i++)
             {
                    pp[i]= malloc(size_s[i]);
                    _ccl_memcpy(pp[i], buffer_data_reply, size_s[i], &offset_buffer);
                    buffer_data_reply += size_s[i]; 
             }
            
        }
        
        buffer_data_reply -= offset_buffer;
        free(buffer_data_reply);

        if (result == CL_SUCCESS) 
        {
            if (param_value_size_ret != NULL)
                *param_value_size_ret = _param_value_size_ret;

            if (param_value != NULL) 
            {
                if(param_name==CL_PROGRAM_DEVICES)
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
                        memcpy(vetor_re,_param_value , val);
                        cl_device_id *vetor =  malloc(sizeof(cl_device_id)*num);
                    
                        for(int i =0; i<num;i++)
                        {
                            ptr  = register_object(vetor_re[i],getDispachPointer(1), obj->daemon);
                            obj = *(cl_opencl_object**)ptr;
                            vetor[i] = obj->object_local;    
                        }
                        
                        memcpy(param_value, vetor, val);
                        free(vetor);
                        free(_param_value);
                }else if(param_name == CL_PROGRAM_BINARIES)
                {
                    memcpy(param_value, pp, _param_value_size_ret);
                }
                else
                {
                    memcpy(param_value, _param_value, _param_value_size_ret);
                    free(_param_value);
                }
            }
        }
#if DEBUG == 0
        puts("--- End execute clGetProgramInfo primitive\n ---");    
#endif
        return (result);
}
POsym(clGetProgramInfo)
