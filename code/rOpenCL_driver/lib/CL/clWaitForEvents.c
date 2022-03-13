#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clWaitForEvents)(cl_uint  num_events ,const cl_event * event_list ) CL_API_SUFFIX__VERSION_1_0
{
//struct timeval t0, t1;
//gettimeofday(&t0, NULL); 
#if DEBUG == 0
        puts("--- Start execute clWaitForEvents primitive\n ---");    
#endif
        char id = 0x62;
        struct sockaddr_in addr;
        void * buffer_data_request = NULL, * ptr = NULL;
        int fd = 0, size_buffer_data_request = 0, offset_buffer = 0;

        cl_opencl_object * obj = NULL;
        cl_event * event_wait_list_in = NULL;

        struct
        {
            cl_uint num_events;

        }ccl_request={.num_events=num_events};

        struct
        {
            cl_int result;
        }ccl_reply;

        if(ccl_request.num_events>0)
        {
            event_wait_list_in = malloc(sizeof(cl_event)*ccl_request.num_events);
            ptr = lookup_object(event_list[0]);
            if (ptr != NULL) 
            {
                obj = *(cl_opencl_object**)ptr;
                memcpy(&addr,&obj->daemon->addr, sizeof(struct sockaddr_in));
                //Save in first position
                event_wait_list_in[0]=obj->object_remote;

                for (int i = 1; i < num_events; i++) 
                {
                    ptr = lookup_object(event_list[i]);
                    if (ptr != NULL) 
                    {
                        obj = *(cl_opencl_object**)ptr;
                        event_wait_list_in[i]=obj->object_remote;
                
                    } else 
                    {
                        return CL_INVALID_VALUE;
                    }
                }
            } else {
                return CL_INVALID_VALUE;
            }

        }else
        {
            return CL_INVALID_VALUE;
        }

        size_buffer_data_request = sizeof (cl_event) * ccl_request.num_events + sizeof (ccl_request);

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

    _ccl_memcpy(buffer_data_request, &ccl_request, sizeof (ccl_request), &offset_buffer);
    buffer_data_request += sizeof (ccl_request);

    _ccl_memcpy(buffer_data_request, event_wait_list_in, sizeof (cl_event) * ccl_request.num_events, &offset_buffer);
    buffer_data_request+=sizeof (cl_event) * ccl_request.num_events;

    buffer_data_request -= offset_buffer;
    
#if PROTOCOL == 0
            
            char *id_transaction = NULL;
            char ip[16];
            struct sockaddr_in addr_local;
            init_udp(&fd, &addr_local, &addr, "clWaitForEvents");
            inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
            id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
            void * header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
            send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clWaitForEvents");
            free(id_transaction);

            if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clWaitForEvents") == 1) {
                free(buffer_data_reply);
                closeSocketUdp(&fd);
                return CL_ERROR_NETWORK;
            }
            closeSocketUdp(&fd);
            
#else
            int fd_connect = init_tcp(&fd, &addr, "clWaitForEvents");
            send_data_tcp(&fd, buffer_data_request, NULL, size_buffer_data_request, "clWaitForEvents");
            recv_data_tcp(&fd, &ccl_reply, sizeof(ccl_reply), "clWaitForEvents");
            closeSocketTCP(&fd,&fd_connect);
#endif
            free(buffer_data_request);

#if DEBUG == 0
        puts("--- End execute clWaitForEvents primitive\n ---");    
#endif

//gettimeofday(&t1, NULL);

//unsigned long long tu0 = (unsigned long long)t0.tv_sec*1000000L+(unsigned long long)t0.tv_usec;
//unsigned long long      tu1 = (unsigned long long)t1.tv_sec*1000000L+(unsigned long long)t1.tv_usec;
//        printf("clMap time=%.3lfs\n", (tu1 - tu0) / 1000000.0);


        return (ccl_reply.result);
}
POsym(clWaitForEvents)
