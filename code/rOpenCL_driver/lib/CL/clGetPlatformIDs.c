#include "pocl_cl.h"

#ifdef BUILD_ICD

static struct _cl_platform_id _platforms[1]  = {{&pocl_dispatch}};

#else

static struct _cl_platform_id _platforms[1] = {{ 1 }};

#endif

CL_API_ENTRY cl_int CL_API_CALL
POname(clGetPlatformIDs)(cl_uint num_entries, cl_platform_id * platforms,cl_uint *num_platforms) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clGetPlatformIDs primitive\n ---");    
#endif
   
    if(isReady==0)
    {
        pthread_mutex_lock(&protected_daemons);
            init_hostprogram();
        pthread_mutex_unlock(&protected_daemons);
        
    }
    
        
        if(daemon_count!=0)
        {
            struct sockaddr_in addr_local,addr;
            char id = 0x01,ip[16], *id_transaction = NULL;
            cl_int result = 0;
            cl_uint fill_num_entries = 0, num_platforms_returned = 0;
            void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL, *ptr = NULL;
            cl_opencl_object * obj = NULL;
            int fd = 0, i = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;


            size_buffer_data_request = sizeof (cl_uint);
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
            _ccl_memcpy(buffer_data_request, &fill_num_entries, sizeof (cl_uint), &offset_buffer);
            buffer_data_request += sizeof (cl_uint);
            buffer_data_request -= offset_buffer;

            size_buffer_data_reply = sizeof (cl_int) + sizeof (cl_uint) + NUM_MAX_PLATFORMS * sizeof (cl_platform_id);
            buffer_data_reply = malloc(size_buffer_data_reply);

            for (i = 0; i < daemon_count; i++) 
            {
#if PROTOCOL == 0   
                    memcpy(&addr, &daemons[i].addr,sizeof(struct sockaddr_in));
                    init_udp(&fd, &addr_local, &addr, "clGetPlatformIDs");
                    
                    inet_ntop(AF_INET, &((daemons[i].addr.sin_addr)), ip, 16);
                    id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
                    header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
                    send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetPlatformIDs");
                    free(id_transaction);
                    free(header);
            
                    if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetPlatformIDs") == 1)
                    {       closeSocketUdp(&fd);
                            return CL_ERROR_NETWORK;
                    }
                    
#else          
                    int fd_connect = init_tcp(&fd, &daemons[i].addr, "clGetPlatformIDs");
                    //void *header_tcp = build_header_tcp(id, &size_buffer_data_request);
                    send_data_tcp(&fd, buffer_data_request, NULL, size_buffer_data_request, "clGetPlatformIDs");
                    recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetPlatformIDs");
                    free(header);
                    closeSocketTCP(&fd,&fd_connect);
#endif
                
                offset_buffer = 0;
                _ccl_memcpy(&result, buffer_data_reply, sizeof (cl_int), &offset_buffer);
                buffer_data_reply += sizeof (cl_int);
           
                if (result == CL_SUCCESS) 
                {
                    _ccl_memcpy(&num_platforms_returned, buffer_data_reply, sizeof (cl_uint), &offset_buffer);
                    buffer_data_reply += sizeof (cl_uint);
                    if ((platforms != NULL)) 
                    {
                        for (int p = 0; p < num_platforms_returned; p++) 
                        {
                            remote_daemon * daemon =(remote_daemon*)&daemons[i].addr;
                            memcpy(&platforms[(fill_num_entries) + p], buffer_data_reply, sizeof (cl_platform_id));
                            buffer_data_reply += sizeof (cl_platform_id);
                            offset_buffer += sizeof (cl_platform_id);
                            
                             ptr  = register_object(platforms[(fill_num_entries) + p],getDispachPointer(0),daemon);
                             obj = *(cl_opencl_object**)ptr;
                             platforms[(fill_num_entries) + p] = obj->object_local;    
                        }
                    }
                    fill_num_entries += num_platforms_returned;
                    
                } else 
                {
                    buffer_data_reply -= offset_buffer;
                    break;
                }
                buffer_data_reply -= offset_buffer;
        }
        
#if PROTOCOL == 0
            closeSocketUdp(&fd);
#endif

        if (num_platforms != NULL)
            *num_platforms = fill_num_entries;
        
        free(buffer_data_request);
        free(buffer_data_reply);
#if DEBUG == 0
        puts("--- End execute clGetPlatformIDs primitive\n ---");    
#endif
        return CL_SUCCESS;
        
        
    }else
    {
        if (platforms != NULL) 
            platforms[0] = &_platforms[0];
  
        if (num_platforms != NULL)
            *num_platforms = 1;
                
        return CL_SUCCESS;
    }
  
}
POsym(clGetPlatformIDs)
