#include "pocl_cl.h"

CL_API_ENTRY cl_int CL_API_CALL
POname(clGetDeviceIDs)(cl_platform_id platform,cl_device_type device_type,cl_uint num_entries,cl_device_id * devices, cl_uint * num_devices) CL_API_SUFFIX__VERSION_1_0
{   
#if DEBUG == 0
        puts("--- Start execute clGetDeviceIDs primitive\n ---");    
#endif
   
    if(isReady==0)
    {  pthread_mutex_lock(&protected_daemons);
        init_hostprogram();
        pthread_mutex_unlock(&protected_daemons);
    }
    

    if (daemon_count!= 0)
    {
       
        char id = 0x04, ip[16], *id_transaction = NULL;
        struct sockaddr_in addr,addr_local;
        void * buffer_data_request = NULL, *buffer_data_reply = NULL, *header = NULL,*ptr = NULL;
        int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0,  i = 0;
        cl_int result;
        cl_uint fill_num_devices = 0, num_devices_returned = 0;
        cl_opencl_object * obj = NULL;
        
        if (platform != NULL) 
        {
            ptr = lookup_object(platform);

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

            size_buffer_data_reply = sizeof (cl_int) + sizeof (cl_uint) + (num_entries) * sizeof (cl_device_id);
            buffer_data_reply = malloc(size_buffer_data_reply);
            
            size_buffer_data_request = sizeof (cl_platform_id) + sizeof (cl_device_type) + sizeof (cl_uint);
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

            _ccl_memcpy(buffer_data_request, &obj->object_remote, sizeof (cl_platform_id), &offset_buffer);
            buffer_data_request += sizeof (cl_platform_id);

            _ccl_memcpy(buffer_data_request, &device_type, sizeof (cl_device_type), &offset_buffer);
            buffer_data_request += sizeof (cl_device_type);

            _ccl_memcpy(buffer_data_request, &num_entries, sizeof (cl_uint), &offset_buffer);
            buffer_data_request += sizeof (cl_uint);

            buffer_data_request -= offset_buffer;


#if PROTOCOL == 0
                init_udp(&fd, &addr_local,&addr,  "clGetDeviceIDs");
                inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
                id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
                header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
                send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clGetDeviceIDs");
                free(id_transaction);

                if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetDeviceIDs") == 1) 
                {
                    free(buffer_data_reply);
                    closeSocketUdp(&fd);
                    return CL_ERROR_NETWORK;
                }
                closeSocketUdp(&fd);

#else          
                int fd_connect = init_tcp(&fd, &addr, "clGetDeviceIDs");
                //header = build_header_tcp(id, &size_buffer_data_request);
                send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clGetDeviceIDs");
                recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetDeviceIDs");
                closeSocketTCP(&fd,&fd_connect);
#endif
            
            free(header);
            free(buffer_data_request);
            offset_buffer = 0;
            
            _ccl_memcpy(&result, buffer_data_reply, sizeof (cl_int), &offset_buffer);
            buffer_data_reply += sizeof (cl_int);

            if (result == CL_SUCCESS) 
            {
                _ccl_memcpy(&num_devices_returned, buffer_data_reply, sizeof (cl_uint), &offset_buffer);
                buffer_data_reply += sizeof (cl_uint);
                if (devices != NULL) 
                {
                _ccl_memcpy(devices, buffer_data_reply, sizeof (cl_device_id) * num_devices_returned, &offset_buffer);
                    buffer_data_reply += sizeof (cl_device_id) * num_devices_returned;
                    for (int i = 0; i < num_devices_returned; i++)
                     {
                          ptr  = register_object(devices[i],getDispachPointer(1), obj->daemon);
                          obj = *(cl_opencl_object**)ptr;
                          devices[i] = obj->object_local;
                     }
                }
                fill_num_devices =num_devices_returned;
            }
            
            buffer_data_reply -= offset_buffer;
            free(buffer_data_reply);

        }else 
        {
            size_buffer_data_reply = sizeof (cl_int) + sizeof (cl_uint) + (num_entries) * sizeof (cl_device_id);
            buffer_data_reply = malloc(size_buffer_data_reply);

            size_buffer_data_request = sizeof (cl_platform_id) + sizeof (cl_device_type) + sizeof (cl_uint);
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

            _ccl_memcpy(buffer_data_request, &platform, sizeof (cl_platform_id), &offset_buffer);
            buffer_data_request += sizeof (cl_platform_id);

            _ccl_memcpy(buffer_data_request, &device_type, sizeof (cl_device_type), &offset_buffer);
            buffer_data_request += sizeof (cl_device_type);

            _ccl_memcpy(buffer_data_request, &num_entries, sizeof (cl_uint), &offset_buffer);
            buffer_data_request += sizeof (size_t);

            buffer_data_request -= offset_buffer;

#if PROTOCOL ==0  
            init_udp(&fd, &addr_local,&addr,  "clGetDeviceIDs");
#endif
            
            for (i = 0; i < daemon_count; i++) 
            {
#if PROTOCOL == 0            
            inet_ntop(AF_INET, &((daemons[i].addr.sin_addr)), ip, 16);
            id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
            header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
            send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &daemons[i].addr, "clGetDeviceIDs");
            free(id_transaction);
            free(header);
            if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetDeviceIDs") == 1)
                    return CL_ERROR_NETWORK;
#else            
            init_tcp(&fd, &daemons[i].addr, "clGetDeviceIDs");
            //void *header_tcp = build_header_tcp(id, &size_buffer_data_request);
            send_data_tcp(&fd, buffer_data_request,NULL, size_buffer_data_request, "clGetDeviceIDs");
            recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "clGetDeviceIDs");
            free(header);
            close(fd);
#endif
                
                offset_buffer = 0;
                _ccl_memcpy(&result, buffer_data_reply, sizeof (cl_int), &offset_buffer);
                buffer_data_reply += sizeof (cl_int);

            if (result == CL_SUCCESS) 
            {

                _ccl_memcpy(&num_devices_returned, buffer_data_reply, sizeof (cl_uint), &offset_buffer);
                buffer_data_reply += sizeof (cl_uint);

                if (devices != NULL) 
                {

                    for (int p = 0; p < num_devices_returned; p++) 
                    {
                        memcpy(&devices[(fill_num_devices) + p], buffer_data_reply, sizeof (cl_device_id));
                        buffer_data_reply += sizeof (cl_device_id);
                        offset_buffer += sizeof (cl_device_id);
                        remote_daemon  daemon;
                            memcpy(&daemon.addr,&daemons[i].addr,sizeof(struct sockaddr_in));
                        ptr  = register_object(devices[(fill_num_devices) + p],getDispachPointer(1), &daemon);
                        obj = *(cl_opencl_object**)ptr;
                        devices[(fill_num_devices) + p] = obj->object_local;
                    }
                    fill_num_devices += num_devices_returned;
                }
            } else {

                buffer_data_reply -= offset_buffer;
                break;
            }
            buffer_data_reply -= offset_buffer;
        }
    }



    if (num_devices != NULL)
        *num_devices = fill_num_devices;
#if DEBUG == 0
        puts("--- End execute clGetDeviceIDs primitive\n ---");    
#endif
    return (result);
    
    }else
    {
        if (num_devices != NULL)
            *num_devices = 0;
        return CL_SUCCESS;
    }
}

POsym(clGetDeviceIDs)

