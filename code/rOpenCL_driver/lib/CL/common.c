#include"common.h"

pthread_mutex_t subtag_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _omx_send = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_tree_objects  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tree_done  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t protected_daemons  = PTHREAD_MUTEX_INITIALIZER;
void * tree_objects = NULL;
void * tree_descriptores= NULL;
int daemon_count = 0, transaction_nr = 0;
remote_daemon * daemons = NULL;
void * tree_info_udp = NULL;
int  port = 1050;
char ip_server[16];
int mtu = 1024;
int protocol =-1;
int  isReady=0;
char * PATH_1 = "/.rOpenCL/config.txt";
char * PATH_2 = "/etc/OpenCL/rOpenCL/config.txt";
int _ccl_connect_to_daemons() 
{
    daemons=NULL;
    char *file=NULL;
    file = malloc(strlen(getenv("HOME")) + strlen(PATH_1) + 1);
    strcpy(file, getenv("HOME"));
    strcat(file, PATH_1);

    FILE * fp = NULL;
    fp = fopen(file, "r");
    if (fp == NULL)
    {
        fp = fopen(PATH_2, "r");
        if(fp==NULL)
            return 0;        
    } 

    struct sockaddr_in addr_local, addr_rDaemon;
    char ip[16], id = 0x00, * id_transaction = NULL;
    void * buffer_data_request = NULL, *buffer_data_reply = NULL,* header = NULL;
    int fd = 0;size_t size_buffer_data_request = 0; int size_buffer_data_reply = 0, count = 0,i=0;;

    int return_f = fscanf(fp, "%d", &mtu);
    fseek(fp, return_f, SEEK_CUR);
    
    return_f=fscanf(fp, "%d", &protocol);
    fseek(fp, return_f, SEEK_CUR);
    
    return_f=fscanf(fp, "%s", ip_server);
    fseek(fp, return_f, SEEK_CUR); 
    
    fscanf(fp, "%d", &port);
    fseek(fp, return_f, SEEK_CUR);  
    


 //buffer_data_request = malloc(size_buffer_data_request);
#if PROTOCOL == 1
    size_buffer_data_request+=SIZE_HEADER_TCP;
#endif

    buffer_data_request = malloc(size_buffer_data_request);

#if PROTOCOL == 1

memcpy(buffer_data_request, &id, sizeof (char));

    memcpy(buffer_data_request+sizeof(char), &size_buffer_data_request, sizeof (size_t));
   

 //   memcpy(buffer_data_request+sizeof(int), &id, sizeof (char));
    //buffer_data_request += sizeof (int);

#endif
    size_buffer_data_reply = sizeof (char);
    buffer_data_reply = malloc(size_buffer_data_reply);

    while (fscanf(fp, "%s", ip) > 0) 
    {
        daemons = realloc(daemons, sizeof (remote_daemon) * ++count);
        daemons[count - 1].addr.sin_addr.s_addr = inet_addr(ip);
        daemons[count - 1].addr.sin_family = AF_INET;
        daemons[count - 1].addr.sin_port = htons(port);
        strcpy(daemons[count-1].ip,ip);

#if PROTOCOL == 1
        
        init_tcp(&fd, &daemons[count - 1].addr, "connect_daemons");
        //void *header_tcp = build_header_tcp(id, &size_buffer_data_request);
        send_data_tcp(&fd, buffer_data_request, NULL, size_buffer_data_request, "connect_daemons");
        
        if(recv_data_tcp(&fd, buffer_data_reply, size_buffer_data_reply, "connect_daemons")==1)
                printf("CL_NETWORK_ERROR - Daemon %s not responding\n",ip);
        
        //closeSocketTCP(&fd,&fd);
        
#else
        
        memcpy(&addr_rDaemon,  &daemons[count - 1].addr, sizeof(struct sockaddr_in));
        
        init_udp(&fd, &addr_local, &addr_rDaemon, "connect_daemons");
        id_transaction = generate_uuid_transaction(ip, getpid(), (unsigned int) pthread_self());
        header = build_header_udp(id, 0, 0, size_buffer_data_request, id_transaction, &addr_local);
        send_data_udp(&fd, buffer_data_request, header, size_buffer_data_request, &addr_rDaemon, "connect_daemons");
        if (recv_data_udp(&fd, buffer_data_reply, size_buffer_data_reply, "connect_daemons") == 1)
                printf("CL_NETWORK_ERROR - Daemon %s not responding\n",ip);
        
        free(header);
        free(id_transaction);
        
#endif

    }
    
#if PROTOCOL == 0
        closeSocketUdp(&fd);
#endif

    free(buffer_data_request);
    free(buffer_data_reply);
    free(file);
    pclose(fp);
    
    
    isReady=1;
    
    return (count);
}


int init_hostprogram() 
{   
  daemon_count = _ccl_connect_to_daemons();
    return daemon_count;
}




void _ccl_perror_and_exit(char*str) {
    perror(str);
    exit(1);
}


void _ccl_check_size_data(size_t real, size_t theorical, char *primitives, int operation) {
    
    if (real >= 0) {
        if (real != theorical) {
            printf("An error has occurred\nReal data:%ld\n Theorical data:%ld\n Primitive: %s\n Operation:%d\n", real, theorical, primitives, operation);
            exit(0);
        }
    } else {
        printf("Primitive: %s\n Operation:%d\n", primitives, operation);
        _ccl_perror_and_exit("Error");
    }
}


int _ccl_equals_addr(struct sockaddr_in * s1, struct sockaddr_in * s2) {

    if ((s1->sin_addr.s_addr == s2->sin_addr.s_addr)&&(s1->sin_port == s2->sin_port))
        return 0;
    else
        return 1;
}
void *clCallbackclSetEventCallck(void *arg)
{
  int socketfd = (long) ((int *) arg);
  struct sockaddr_in cli;
 struct
  {
      cl_event event;
      uintptr_t callback;
      cl_int event_command_status;
  }ccl_request; 

  int len;
  
  // Accept the data packet from client and verification
   int connsfd = accept(socketfd, (struct sockaddr *) &cli, &len);
  
   if(connsfd < 0)
   {
     puts("server accept failed \n");
     exit(0);
   }

   int rec = recv(connsfd,&ccl_request,sizeof(ccl_request),0);
   close(connsfd);
   close(socketfd);

   void (*func)(cl_event,cl_int,void*)=(void*)ccl_request.callback;
  
   (*func)(ccl_request.event,ccl_request.event_command_status,NULL);
   pthread_exit(NULL);
}

