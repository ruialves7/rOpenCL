/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "network.h"
#include <netinet/tcp.h>
/**
 * Deve ser guardado na arvore binaria o descritor alocado, a estrutura de socket do host e a estutura do socket do daemon.
 **/


void init_udp(int * fd_descriptor, struct sockaddr_in *addr_host, struct sockaddr_in *addr_rDaemon, char * primitive) 
{   
    //vai a procura da chave na key que guardar os sockets 
    char * key = malloc(sizeof(char)*32);
    memset( key, '\0', sizeof(char)*32);
    char tid_str[10];
    
    int ch = '.';
    char *ptr = NULL;
    
    //Get the tid of the current thread
    pid_t tid = syscall(SYS_gettid);
    snprintf(tid_str, 10, "%d", tid);

    //Get the ip address of the remote node to connect.
    char *ip = (char*)inet_ntoa(addr_rDaemon->sin_addr);
    strcat(key, ip);
    strcat(key, tid_str);
    
    // neste ponto o processo identifica qual é o pid/tid em questao e vai procurar duas coisas,  o socket no host que é usado para enviar dados  e o socket no rDaemon que vai ser usado para receber os dados
    void *obj_rbt = lookup_info_udp(key);
   
    


  //se o obj_rbt for diferente de NULL isto significa que  já há sockets criados então deve ser usado os sockets criados
  if(obj_rbt!=NULL)
  {
      cl_info_udp_object *p = *(cl_info_udp_object**)obj_rbt;
      
      //vai buscar os descriptor de socket que foi criado
      *fd_descriptor=p->descriptor;
     
      
      //guardar o struct  sockaddr_in host
      memcpy(addr_host, &p->host_socket, sizeof(struct sockaddr_in));
    
      //guardar o struct  sockaddr_in rDaemon
      memcpy(addr_rDaemon, &p->rDaemon_socket, sizeof(struct sockaddr_in));
      
      
  }else
  {
        //Neste ponto é criado o socket do lado do host que servirá para enviar as mensagens para o referido daemon e receber as respostas.
        *fd_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
        if (*fd_descriptor <0)
            printf("Failed to create socket in %s \n", primitive);

        addr_host->sin_family = AF_INET;
        addr_host->sin_port = htons(0);
        addr_host->sin_addr.s_addr = inet_addr(ip_server);

        int addrlen = sizeof (addr_host);
        if (bind(*fd_descriptor, (struct sockaddr *) addr_host, sizeof (*addr_host)) <0)
            printf("Failed to bind socket in %s \n", primitive);
        
        getsockname(*fd_descriptor, (struct sockaddr*) (addr_host), (socklen_t *) & addrlen);
        struct timeval tv;
        tv.tv_sec = TIMEOUT_CONNECT;
        tv.tv_usec = 0;
    
        if (setsockopt(*fd_descriptor, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv))<0) 
            printf("Failed to setsockopt socket in %s \n", primitive);
        
        //enviar de criação de um socket do lado do rDaemon em especifico para envio da mensagens
        
        //enviei a o pedido para a criação de um worker do lado  da thread receptora
        int data =1;
        int return_sendto = sendto(*fd_descriptor, &data, sizeof(int), 0, (struct sockaddr *) (addr_rDaemon), sizeof ((*addr_rDaemon)));
        
        //recebi a estrutura do socket pelo qual devo enviar as mensagens após ter sido criado um worker 
        return_sendto = recvfrom(*fd_descriptor, addr_rDaemon, sizeof(struct sockaddr_in), 0, NULL, NULL);
      
        //registar na arvore binaria a informação do udp
        register_info_udp(*fd_descriptor, addr_host, addr_rDaemon, key);
        
 }
    
}

int init_tcp(int * fd_descriptor, struct sockaddr_in * addr, char * primitive)
{

    char * key = malloc(sizeof(char)*32);
    memset( key, '\0', sizeof(char)*32);
    char tid_str[6];
    
    int ch = '.';
    char *ptr = NULL;
    
    //Get the tid of the current thread
    pid_t tid = syscall(SYS_gettid);
    snprintf(tid_str, 6, "%d", tid);

    //Get the ip address of the remote node to connect.
    char *ip = (char*)inet_ntoa(addr->sin_addr);
    strcat(key, ip);
    strcat(key, tid_str);
    

  void *xx = lookup_descriptor(key);

  
  if(xx!=NULL)
  {
      cl_descriptor_object *p = *(cl_descriptor_object**)xx;
      *fd_descriptor=p->descriptor;
      
      return 0;
      
  }else
  {
    int return_sendto=0;
    *fd_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (*fd_descriptor < 0)
    {
        printf("Failed to create socket in %s \n", primitive);
        return *fd_descriptor;
    }
    
    //int one =1;
    //if (setsockopt(*fd_descriptor, SOL_SOCKET, SO_ZEROCOPY, &one, sizeof(one)))
    //      error(1, errno, "setsockopt zerocopy");    
     
    return_sendto = connect(*fd_descriptor, (struct sockaddr*) addr, sizeof (*addr));
   
    if (return_sendto < 0) 
    {   printf("Connection with the server failed in %s %d \n", primitive,errno);
        perror("Critical Error\n");
    }
    
    int val = 1;

    if (setsockopt(*fd_descriptor, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) < 0)
           perror("setsockopt(2) error");

     setsockopt(*fd_descriptor, IPPROTO_TCP, TCP_QUICKACK, (void*) &val, sizeof(int));

     setsockopt(*fd_descriptor, IPPROTO_TCP, TCP_DEFER_ACCEPT, &val, sizeof(val));

     //Surtiu algum efeito.. mas pouco
     int optval=7; // valid values are in the range [1,7]  // 1- low priority, 7 - high priority  
     setsockopt(*fd_descriptor, SOL_SOCKET, SO_PRIORITY, &optval, sizeof(optval));  


/*
    //Estas opcoes pioram bastante o tempo, com isto o auto tunning dos buffers é desligado. 
    int size = 1048576;     
    //int size_w = size/2;

    /*

    setsockopt(*fd_descriptor, IPPROTO_TCP, TCP_WINDOW_CLAMP, &size_w, sizeof(size_w));
	
    setsockopt(*fd_descriptor, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
        
    setsockopt(*fd_descriptor, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
   */
    
  
    //mutuamente exclusivo com o NODELAY poderá funcionar em testes com multiplos clientes
    //setsockopt(*fd_descriptor, SOL_SOCKET, TCP_CORK , &val, sizeof(val));	


    //Default:cubic Nao surtiu efeito e deu erro quando colocado no servidor e no client    

    /*

      char buf[256];
      socklen_t len;
      strcpy(buf, "reno");

      len = sizeof(buf);

      if (setsockopt(*fd_descriptor, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0)
      {
        perror("getsockopt");
        return -1;
      }
   */


      //Aparentemente SO_RCVLOWAT e SO_SNDLOWAT pode ser explorado de outra forma, mas é necessário falar com o professor Rufino.
      //int res=0;
      //setsockopt(*fd_descriptor, SOL_SOCKET, SO_RCVLOWAT, &res, sizeof(res));

       //setsockopt(*fd_descriptor, SOL_SOCKET, SO_SNDLOWAT, &res, sizeof(res));  

      //opcao a nivel da firewall mas como a firewall esta down de prinicipio  foi por isso que nao produziu resultados
      //int mark = 100;
      //setsockopt(*fd_descriptor, SOL_SOCKET, SO_MARK, &mark, sizeof(mark));



    
    register_descriptor(*fd_descriptor,key);
    return return_sendto;
 }
}

void * build_header_udp(char id, int part_of_transaction, int total_packets, int size_total, char *id_transaction, struct sockaddr_in *addr) {

    void *buffer = malloc(SIZE_HEADER_UDP);
    struct sockaddr_in addr_;
    memcpy(&addr_, addr, sizeof (struct sockaddr_in));
    addr_.sin_addr.s_addr = inet_addr(ip_server);
    struct{
		char id;
		int part_of_packets;
        int total_packets; 
        int size_buffer;
        char id_transaction [SIZE_TRANSACTION_ID];
	} cl_header_udp={.id=id, .part_of_packets=part_of_transaction, .total_packets=total_packets, .size_buffer=size_total};

    strcpy(cl_header_udp.id_transaction,id_transaction),
    
    memcpy(buffer, &cl_header_udp, sizeof(cl_header_udp));
    memcpy(buffer+sizeof(cl_header_udp), &addr_, sizeof (struct sockaddr_in));

    return buffer;
}

void * build_header_tcp(char id, int *size_total) {

    *size_total += SIZE_HEADER_TCP;

    void *buffer = malloc(SIZE_HEADER_TCP);
    long offset = 0;

    memcpy(buffer, &id, sizeof (char));
    buffer += sizeof (char);
    offset += sizeof (char);

    memcpy(buffer, size_total, sizeof (int));
    buffer += sizeof (int);
    offset += sizeof (int);

    buffer -= offset;
    return buffer;
}

char * generate_uuid_transaction(char *ip, int pid, int tid) {
    
    char *hash = malloc(sizeof (char)*SIZE_TRANSACTION_ID);
    memset(hash,0,SIZE_TRANSACTION_ID);
    char tid_str[11];
    char pid_str[6];
    char number[33];
    
    //Get the tid of the current thread
    pid_t tid_ = syscall(SYS_gettid);
    snprintf(tid_str, 6, "%d", tid_);
    snprintf(pid_str, 6, "%d", pid);
    pthread_mutex_lock(&_omx_send);
    snprintf(number, 33, "%d", transaction_nr);
    transaction_nr++;
    pthread_mutex_unlock(&_omx_send);
    strcat(hash, ip);
    strcat(hash, tid_str);
    strcat(hash, pid_str);
    strcat(hash, number);

    int ch = '.';

    char *ptr = NULL;
    while (strchr(hash, ch)) {
        ptr = strchr(hash, ch);
        strcpy(ptr, ptr + 1);
    }
    return hash;
}

void setNrPacketHeader(void *header, int number) {

     struct{
		char id;
		int part_of_packets;
        int total_packets; 
        int size_buffer;
        char id_transaction [SIZE_TRANSACTION_ID];
	} cl_header_udp;
    
    memcpy(&cl_header_udp, header, sizeof (cl_header_udp));
    cl_header_udp.part_of_packets=number;
    
    memcpy(header,&cl_header_udp, sizeof (cl_header_udp));
    
}

void send_data_udp(int *fd, void * buffer, void*header, int size, struct sockaddr_in *addr, char * primitive) 
{
    
    int return_sendto = 0, send_complet = 0, miss_send = 0, i = 0, offset_send = 0, offset_total = 0;
    void * block_send = NULL;

    if ((size+SIZE_HEADER_UDP) > mtu) 
    {

        block_send = malloc(mtu);
        send_complet = size / (mtu - SIZE_HEADER_UDP);
        miss_send = size - (send_complet * (mtu - SIZE_HEADER_UDP));
   struct{
		char id;
		int part_of_packets;
        int total_packets; 
        int size_buffer;
        char id_transaction [SIZE_TRANSACTION_ID];
	} cl_header_udp;
    
    memcpy(&cl_header_udp, header, sizeof (cl_header_udp));
    cl_header_udp.total_packets=send_complet;
    memcpy(header,&cl_header_udp, sizeof (cl_header_udp));

        for (i = 0; i < send_complet; i++) {
            setNrPacketHeader(header, i);
            offset_send = 0;

            memcpy(block_send, header, SIZE_HEADER_UDP);
            offset_send += SIZE_HEADER_UDP;
            block_send += SIZE_HEADER_UDP;

            memcpy(block_send, buffer, (mtu - SIZE_HEADER_UDP));
            offset_send += (mtu - SIZE_HEADER_UDP);
            block_send += (mtu - SIZE_HEADER_UDP);

            offset_total += (mtu - SIZE_HEADER_UDP);
            buffer += (mtu - SIZE_HEADER_UDP);

            block_send -= offset_send;

            return_sendto = sendto(*fd, block_send, mtu, 0, (struct sockaddr *) (addr), sizeof ((*addr)));
            sleep(0);
            _ccl_check_size_data(return_sendto, mtu, primitive, 0);
        }

        if (miss_send > 0) {
            return_sendto = 0;
            setNrPacketHeader(header, i);
            offset_send = 0;

            memcpy(block_send, header, SIZE_HEADER_UDP);
            offset_send += SIZE_HEADER_UDP;
            block_send += SIZE_HEADER_UDP;

            memcpy(block_send, buffer, miss_send);
            offset_send += miss_send;
            block_send += miss_send;

            block_send -= offset_send;
            return_sendto = sendto(*fd, block_send, miss_send + SIZE_HEADER_UDP, 0, (struct sockaddr *) (addr), sizeof ((*addr)));
            _ccl_check_size_data(return_sendto, miss_send + SIZE_HEADER_UDP, primitive, 0);
        }
        
    } else 
    {
        block_send = malloc(SIZE_HEADER_UDP + size);
        memcpy(block_send, header, SIZE_HEADER_UDP);
        offset_send += SIZE_HEADER_UDP;
        block_send += SIZE_HEADER_UDP;

        memcpy(block_send, buffer, size);
        offset_send += size;
        block_send += size;

        block_send -= offset_send;
        return_sendto = sendto(*fd, block_send, size + SIZE_HEADER_UDP, 0, (struct sockaddr *) (addr), sizeof ((*addr)));
        _ccl_check_size_data(return_sendto, size + SIZE_HEADER_UDP, primitive, 0);
    }
    free(block_send);
}

void send_data_tcp(int *fd, void * buffer, void * header, int size, char * primitive) {
#if DEBUG ==0
    struct timeval t0,t1;
    gettimeofday(&t0, NULL);
#endif
    int return_sendto = 0;
    
#if DEBUG ==0
    gettimeofday(&t1, NULL);
    long long milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
    long long milisecconds1 = t1.tv_sec*1000LL+t1.tv_usec/1000;
    printf("-- rOpenCL (DEBUG): Size mensagem %d. Tempo gasto no send_data_tcp antes da invocacoa do send %ld (ms) -- \n", size, milisecconds1-milisecconds);
#endif
	//if(size<10000) 
        //    return_sendto = send(*fd, buffer, size, 0);
     	//else
	     return_sendto = send(*fd, buffer, size, 0 );
#if DEBUG ==0      
  gettimeofday(&t0, NULL);
        milisecconds = t0.tv_sec*1000LL+t0.tv_usec/1000;
        printf("-- rOpenCL (DEBUG): Size mensagem %d. timestamp envio %ld (ms) -- \n", size, milisecconds);
#endif
     
    _ccl_check_size_data(return_sendto, size, primitive, 1);
    
}

int recv_data_udp(int *fd, void * buffer, int size, char * primitive) {

    int return_sendto = 0, send_complet = 0, miss_recv = 0, i = 0, offset_total = 0;
    void * block_send = NULL;

    if (size > mtu) 
    {
        block_send = malloc(mtu);
        send_complet = size / (mtu);
        miss_recv = size - (send_complet * (mtu));
        int  tt =  size - miss_recv;
        //for (i = 0; i < send_complet; i++)
        while(1)
        {
            return_sendto = recvfrom(*fd, buffer+offset_total, mtu, 0, NULL, NULL);
            offset_total += mtu;
            if(offset_total==(tt))
                    break;
            //_ccl_check_size_data(return_sendto, mtu, primitive, 0);
            //printf("%d\n", offset_total);
        }
        
        if (errno == 11) {
                return 1;
        }

        if (miss_recv > 0) 
        {
            return_sendto = 0;
            return_sendto = recvfrom(*fd, buffer+offset_total, miss_recv, 0, NULL, NULL);
            if (errno == 11) {
                return 1;
            }
            //memcpy(buffer, block_send, miss_recv);
           // offset_total += miss_recv;
           // buffer += miss_recv;
            _ccl_check_size_data(return_sendto, miss_recv, primitive, 0);
        }

       // buffer -= offset_total;
        free(block_send);
        return 0;
    } else {

        return_sendto = recvfrom(*fd, buffer, size, 0, NULL, NULL);
        if (errno == 11) {
            return 1;
        }
    }
    return 0;
}

int recv_data_tcp(int *fd, void * buffer, int size, char * primitive) {    
    int size_read = 0, return_sendto = 0, size_recv = 0, offset_total = 0;

    return_sendto = recv(*fd, &size_recv, sizeof(int), 0);
    
    size_recv = size_recv-sizeof(int);
    return_sendto = recv(*fd, buffer, size_recv/*-sizeof(int)*/, 0);
    offset_total += return_sendto;
  
    while (offset_total < size_recv) 
    {
         return_sendto = recv(*fd, buffer + offset_total, size_recv-offset_total, 0);
         offset_total += return_sendto;
    }

    return 0;
    
    /**
     *  int size_read = 0, return_sendto = 0, size_recv = 0, offset_total = 0;

    size += sizeof (int);
    return_sendto = recv(*fd, buffer, size, 0);

    memcpy(&size_recv, buffer, sizeof (int));
    size_read += return_sendto;
    
    memcpy(buffer,buffer+sizeof(int), return_sendto-sizeof(int));
    
    while (size_read != size_recv) 
    {
            return_sendto = recv(*fd, buffer+size_read,size, 0);
            size_read += return_sendto;
    }
   
    
    return 0;*/
}

int closeSocketUdp(int *fd)
{
    //close(*fd);
    return 1;
}

int closeSocketTCP(int *fd_socket,int *fd_connect)
{
   /** shutdown(*fd_socket, SHUT_RDWR);
    close(*fd_socket);
   // shutdown(*fd_connect, SHUT_RDWR);
    //close(*fd_connect);*/
    return 1;
}

int compare_desc(const void *pa, const void *pb)
{
    cl_descriptor_object *pa_ = (cl_descriptor_object*)pa;
    cl_descriptor_object *pb_ = (cl_descriptor_object*)pb;
    return strcmp(pa_->key, pb_->key);
}

void * lookup_descriptor(char *key) 
{
    cl_descriptor_object obj;
     memset(obj.key, '\0', sizeof(char)*32);
    memcpy(obj.key,key,strlen(key));
   
    void * ptr = NULL;
    pthread_mutex_lock(&tree_done);
        ptr = tfind((void*) &obj, &tree_descriptores, compare_desc);
    pthread_mutex_unlock(&tree_done);
    return ptr;
}

void * register_descriptor(int des, char * key) 
{
    cl_descriptor_object *object = malloc(sizeof(cl_descriptor_object));
      memset(object->key, '\0', sizeof(char)*32);
    memcpy(object->key,key,strlen(key));
    object->descriptor = des;
    void *ptr = NULL;
    pthread_mutex_lock(&tree_done);
        ptr = tsearch((void*) object, &tree_descriptores, compare_desc);
    pthread_mutex_unlock(&tree_done);
    return ptr;
}


void * lookup_info_udp(char *key) 
{
    cl_info_udp_object obj;
    memset(obj.key, '\0', sizeof(char)*32);
    memcpy(obj.key,key,strlen(key));
   
    void * ptr = NULL;
    pthread_mutex_lock(&tree_done);
        ptr = tfind((void*) &obj, &tree_info_udp, compare_desc);
    pthread_mutex_unlock(&tree_done);
    return ptr;
}

void * register_info_udp(int des, struct sockaddr_in *addr_host, struct sockaddr_in *addr_rDaemon, char * key) 
{
    cl_info_udp_object *object = malloc(sizeof(cl_info_udp_object));
    
    //guardar a key
    memset(object->key, '\0', sizeof(char)*32);
    memcpy(object->key,key,strlen(key));
    
    //guardar o descritor
    object->descriptor = des;
    
    //guardar o struct sockaddr_in host
    memcpy(&object->host_socket, addr_host, sizeof(struct sockaddr_in));
    
    //guardar o struct  sockaddr_in rDaemon
    memcpy(&object->rDaemon_socket, addr_rDaemon, sizeof(struct sockaddr_in));
    
    void *ptr = NULL;
    
    pthread_mutex_lock(&tree_done);
        ptr = tsearch((void*) object, &tree_info_udp, compare_desc);
    pthread_mutex_unlock(&tree_done);
    
    return ptr;
}
int compare_info_udp(const void *pa, const void *pb)
{
    cl_info_udp_object *pa_ = (cl_info_udp_object*)pa;
    cl_info_udp_object *pb_ = (cl_info_udp_object*)pb;
    return strcmp(pa_->key, pb_->key);
}

