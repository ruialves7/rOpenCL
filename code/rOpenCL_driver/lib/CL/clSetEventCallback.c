#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clSetEventCallback) (cl_event event ,cl_int  command_exec_callback_type ,void (CL_CALLBACK *  pfn_notify )(cl_event, cl_int, void *),
void * user_data ) CL_API_SUFFIX__VERSION_1_1
{
#if DEBUG == 0
        puts("--- Start execute clSetEventCallback primitive\n ---");    
#endif
    char id = 0x6A;
    struct sockaddr_in addr;
    void *ptr = NULL, * buffer_data_request = NULL, *header = NULL;
    int fd = 0, size_buffer_data_request = 0, offset_buffer = 0;

    ptr = lookup_object(event);
   
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

	
    struct
    {
        cl_event event;
        char isnull;
        uintptr_t callback;
 	cl_int command_exec_callback_type;

    }ccl_request = {.command_exec_callback_type=command_exec_callback_type,.event = obj->object_remote,.isnull = pfn_notify==NULL};

    if(!ccl_request.isnull) 
       	ccl_request.callback = (uintptr_t)pfn_notify;

 
    struct
    {
        cl_int result;
    }ccl_reply;

    size_buffer_data_request = sizeof (ccl_request)+sizeof(struct sockaddr_in);


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


    int socketfd, connsfd, len;
    struct sockaddr_in servaddr, cli;
  
    // socket create and verification  
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) 
    {
        puts("socket creation failed...\n");
        exit(0);
    }
  
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip_server);
    servaddr.sin_port = 0;
    
    // Binding newly created socket to given IP and verification
    if ((bind(socketfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) != 0) {
        puts("socket bind failed...\n");
        exit(0);
    }

    int sa_len = sizeof(servaddr);
    if(getsockname(socketfd, (struct sockaddr *)&servaddr, &sa_len) == -1) 
    {
      	perror("getsockname() failed");
      	exit(0);
    }

    // Now server is ready to listen and verification
    if ((listen(socketfd, 1)) != 0) {
        puts("Listen failed...\n");
        exit(0);
    }

    _ccl_memcpy(buffer_data_request, &servaddr, sizeof(struct sockaddr_in), &offset_buffer);
    buffer_data_request += sizeof(struct sockaddr_in);

    pthread_t thread;
    pthread_create(&thread, NULL, clCallbackclSetEventCallck, (int *) (long) socketfd);    
    buffer_data_request -= offset_buffer;
#if PROTOCOL == 0
        
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local, &addr, "clSetEventCallback");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clSetEventCallback");
        free(id_transaction);

        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "clSetEventCallback") == 1) {
            free(buffer_data_reply);
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;
        }
        closeSocketUdp(&fd);
        
#else
        int fd_connect = init_tcp(&fd, &addr, "clSetEventCallback");
        send_data_tcp(&fd, buffer_data_request, NULL, size_buffer_data_request, "clSetEventCallback");
        recv_data_tcp(&fd, &ccl_reply, sizeof(ccl_reply), "clSetEventCallback");
        closeSocketTCP(&fd,&fd_connect);
#endif
        
       
        free(buffer_data_request);
#if DEBUG == 0
        puts("--- End execute clSetEventCallback primitive\n ---");    
#endif
    return (ccl_reply.result);
 
    
}

POsym(clSetEventCallback)
