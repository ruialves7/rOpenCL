
#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueWriteBuffer)(cl_command_queue command_queue,cl_mem buffer,cl_bool blocking_write,size_t offset,size_t size,const void *ptr,cl_uint num_events_in_wait_list,const cl_event *event_wait_list,cl_event *event)CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG ==0

    struct timeval t0, t1, t0_0, t1_1;
    
    long long  milisecconds = 0, milisecconds1 = 0;// t0.tv_sec*1000LL+t0.tv_usec/1000;
    
    
    gettimeofday(&t0_0, NULL);
#endif
   
    
  //TODO: blocking_write==FALSE
    if(blocking_write==CL_FALSE)
    {
        puts("-- blocking_write is CL_FALSE -> operation not supportaded---");
        blocking_write = CL_TRUE;
    }

#if DEBUG == 0
        puts("--- Start execute clEnqueueWriteBuffer primitive\n ---");    
#endif
    
   
    char id = 0x0A;
    struct sockaddr_in addr;
    void * buffer_data_request = NULL,  *header = NULL,*ptr_ = NULL;
    int fd = 0, size_buffer_data_request = 0, size_buffer_data_reply = 0, offset_buffer = 0;
#if DEBUG ==0
    gettimeofday(&t0, NULL);
#endif
    ptr_ = lookup_object(command_queue);
    
    cl_opencl_object * obj = NULL;
    if (ptr_  != NULL) 
    {
        obj = *(cl_opencl_object**)ptr_;
        memcpy(&addr, obj->daemon, sizeof(struct sockaddr_in));

    } else 
    {
        return CL_INVALID_VALUE;
    }
    
    struct
    {
            cl_event event;
            cl_int result;
    }ccl_reply;
        
     struct
    {
           cl_command_queue command_queue;
           cl_mem buffer;
           cl_bool blocking_write;
           size_t offset;
           size_t size;
           cl_uint num_events_in_wait_list;
           char event_is_null;   
    }ccl_request={.blocking_write=blocking_write,.offset=offset,.size=size,.num_events_in_wait_list=num_events_in_wait_list, .event_is_null=(event== NULL)};
    
    ccl_request.command_queue = obj->object_remote;
    
#if DEBUG ==0
    gettimeofday(&t1, NULL);
    milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
    milisecconds1 = t1.tv_sec*1000LL+t1.tv_usec/1000;

    printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo gasto na procura da command_queue %ld (ms) -- \n", size, milisecconds1-milisecconds);
    

    gettimeofday(&t0, NULL);
#endif    
ptr_ = lookup_object(buffer);
    
    if (ptr_ != NULL) 
    {
        obj = *(cl_opencl_object**)ptr_;

    } else
    {
        return CL_INVALID_VALUE;
    }
    
    ccl_request.buffer = obj->object_remote;
#if DEBUG ==0    
    gettimeofday(&t1, NULL);
    milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
    milisecconds1 = t1.tv_sec*1000LL+t1.tv_usec/1000;
    printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo gasto na procura da buffer %ld (ms) -- \n", size, milisecconds1-milisecconds);
    
    gettimeofday(&t0, NULL);
#endif
    size_buffer_data_request = sizeof (ccl_request);

        if (num_events_in_wait_list > 0) 
        {
            size_buffer_data_request += (sizeof (cl_event) * num_events_in_wait_list);
        }

        if (size > 0) {
            size_buffer_data_request += size;
        }
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
#if DEBUG ==0 
        gettimeofday(&t1, NULL);
        milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
        milisecconds1 = t1.tv_sec*1000LL+t1.tv_usec/1000;
        printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo gasto a realizar o malloc do buffer global e a copiar a struct cl_request para dentro desse buffer %ld (ms) -- \n", size, milisecconds1-milisecconds);
    
    
    
        gettimeofday(&t0, NULL);
#endif
        if (num_events_in_wait_list > 0) 
        {
             cl_event *ss =  malloc(sizeof(cl_event)*num_events_in_wait_list);
             for(int ii =0 ; ii<num_events_in_wait_list;ii++)
             {
                  ptr_ = lookup_object((void*)event_wait_list[ii]);
                  obj = *(cl_opencl_object**)ptr_;
                  ss[ii]=obj->object_remote;
             }
            
            _ccl_memcpy(buffer_data_request, ss, sizeof (cl_event) * num_events_in_wait_list, &offset_buffer);
            buffer_data_request += (sizeof (cl_event) * num_events_in_wait_list);
        }
#if DEBUG ==0        
        gettimeofday(&t1, NULL);
        milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
        milisecconds1 = t1.tv_sec*1000LL+t1.tv_usec/1000;
        printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo gasto na procura da lista de events %ld (ms) -- \n", size, milisecconds1-milisecconds);
        
        
        gettimeofday(&t0, NULL);
#endif      
  if (size > 0) 
        {
           void *ss = (void*) ptr;
           _ccl_memcpy(buffer_data_request, ss, size, &offset_buffer);
            buffer_data_request += size;
        }
#if DEBUG ==0
        gettimeofday(&t1, NULL);
        milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
        milisecconds1 = t1.tv_sec*1000LL+t1.tv_usec/1000;
        printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo gasto para o memcopy do ptr %ld (ms) -- \n", size, milisecconds1-milisecconds);
#endif
        buffer_data_request -= offset_buffer;
        size_buffer_data_reply = sizeof (ccl_reply);
        
        #if DEBUG ==0
        gettimeofday(&t1_1, NULL);
        milisecconds = t0_0.tv_sec*1000LL+t0_0.tv_usec/1000;
        milisecconds1 = t1_1.tv_sec*1000LL+t1_1.tv_usec/1000;
        printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo total gasto na construção da sms %ld (ms) -- \n", size, milisecconds1-milisecconds);
        #endif
        
#if PROTOCOL == 0
        char *id_transaction = NULL;
        char ip[16];
        struct sockaddr_in addr_local;
        init_udp(&fd, &addr_local,&addr,  "clEnqueueWriteBuffer");
        inet_ntop(AF_INET, &((addr.sin_addr)), ip, 16);
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr, "clEnqueueWriteBuffer");
        free(id_transaction);

        if (recv_data_udp(&fd, &ccl_reply, size_buffer_data_reply, "clEnqueueWriteBuffer") == 1) {
            
            closeSocketUdp(&fd);
            return CL_ERROR_NETWORK;s
        }
        closeSocketUdp(&fd);
#else
#if DEBUG ==0
        gettimeofday(&t0, NULL);
#endif
  int fd_connect =init_tcp(&fd, &addr, "clEnqueueWriteBuffer");
        //header = build_header_tcp(id, &size_buffer_data_request);
#if DEBUG ==0       
 gettimeofday(&t1, NULL);
        milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
        milisecconds1 = t1.tv_sec*1000LL+t1.tv_usec/1000;
        printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo gasto no init da ligação tcp e na criação do seu buffer %ld (ms) -- \n", size, milisecconds1-milisecconds);
       
        gettimeofday(&t0, NULL);
#endif       
 send_data_tcp(&fd, buffer_data_request, header, size_buffer_data_request, "clEnqueueWriteBuffer");
#if DEBUG ==0      
  gettimeofday(&t1, NULL);
        milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
        milisecconds1 = t1.tv_sec*1000LL+t1.tv_usec/1000;
        printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo gasto no send_data_tcp %ld (ms) -- \n", size, milisecconds1-milisecconds);
        
        gettimeofday(&t0, NULL);
#endif
        recv_data_tcp(&fd, &ccl_reply, size_buffer_data_reply, "clEnqueueWriteBuffer");
#if DEBUG ==0      
  gettimeofday(&t1, NULL);
        milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
        milisecconds1 = t1.tv_sec*1000LL+t1.tv_usec/1000;
        printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo gasto no recv_data_tcp %ld (ms) -- \n", size, milisecconds1-milisecconds);
#endif        
        closeSocketTCP(&fd,&fd_connect);
#endif
#if DEBUG ==0    
    
    
    
        gettimeofday(&t0, NULL);
        free(header);
#endif
        free(buffer_data_request);
        if ((ccl_reply.result == CL_SUCCESS) && (event != NULL)) 
        {
           ptr_  = register_object(ccl_reply.event, getDispachPointer(7), obj->daemon);
           obj = *(cl_opencl_object**)ptr_;
           ccl_reply.event = obj->object_local;
           memcpy(event, &ccl_reply.event, sizeof (cl_event));
        }
#if DEBUG == 0
        puts("--- End execute clEnqueueWriteBuffer primitive\n ---");    
#endif
#if DEBUG ==0
        gettimeofday(&t1, NULL);
        milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
        milisecconds1 = t1.tv_sec*1000LL+t1.tv_usec/1000;
        printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo gasto no tratamento da resposta do daemon %ld (ms) -- \n", size, milisecconds1-milisecconds);
#endif
    return (ccl_reply.result); 
 
}
POsym(clEnqueueWriteBuffer)
