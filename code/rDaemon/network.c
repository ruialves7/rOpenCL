/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include"network.h"


#include <sys/time.h>



void send_data_udp(int *fd, void * buffer, int size, struct sockaddr_in *addr, char * primitive) 
{
    int return_sendto = 0, send_complet = 0, miss_send = 0, i = 0, offset_send = 0, offset_total = 0;
    void * block_send = NULL;
    
   if (size > MAX_UDP_PACKET_SIZE) 
    {
        block_send = malloc(MAX_UDP_PACKET_SIZE);
        send_complet = size / (MAX_UDP_PACKET_SIZE);
        miss_send = size - (send_complet * (MAX_UDP_PACKET_SIZE));

        for (i = 0; i < send_complet; i++) 
        {
            offset_send = 0;
            memcpy(block_send, buffer, MAX_UDP_PACKET_SIZE);
            offset_send += MAX_UDP_PACKET_SIZE;
            block_send += MAX_UDP_PACKET_SIZE;

            offset_total += MAX_UDP_PACKET_SIZE;
            buffer += MAX_UDP_PACKET_SIZE;

            block_send -= offset_send;

            return_sendto = sendto(*fd, block_send, MAX_UDP_PACKET_SIZE, 0, (struct sockaddr *) (addr), sizeof ((*addr)));
            sleep(0);
            
	    _ccl_check_size_data(return_sendto, MAX_UDP_PACKET_SIZE, primitive, 0);
        }

        if (miss_send > 0) {
            return_sendto = 0;
            offset_send = 0;
            memcpy(block_send, buffer, miss_send);
            offset_send += miss_send;
            block_send += miss_send;
            
            buffer += miss_send;

            block_send -= offset_send;
            return_sendto = sendto(*fd, block_send, miss_send, 0, (struct sockaddr *) (addr), sizeof ((*addr)));
            _ccl_check_size_data(return_sendto, miss_send, primitive, 0);
        }
    } else {

        block_send = malloc(size);

        memcpy(block_send, buffer, size);
        offset_send += size;
        block_send += size;

        block_send -= offset_send;
        return_sendto = sendto(*fd, block_send, size, 0, (struct sockaddr *) (addr), sizeof ((*addr)));
        _ccl_check_size_data(return_sendto, size, primitive, 0);
    }
    free(block_send);
}

void send_data_tcp(int *fd, void * buffer, int size, struct sockaddr_in *addr, char * primitive) {

   int return_sendto = 0, offset_send = 0;

if(strcmp(primitive,"_cclEnqueueMapBuffer")!=0 && strcmp(primitive,"_cclEnqueueReadBuffer")!=0)
{    
    size += sizeof (int);
    
    void * block_send = malloc(size);

    memcpy(block_send, &size, sizeof (int));
    offset_send += sizeof (int);
    block_send += sizeof (int);

    memcpy(block_send, buffer, (size - sizeof (int)));
    offset_send += (size - sizeof (int));
    block_send += (size - sizeof (int));

    
    block_send -= offset_send;
     return_sendto = send(*fd, block_send, size, 0);
 } else {
//struct timeval t0, t1;
//gettimeofday(&t0, NULL);
return_sendto = send(*fd, buffer, size, 0); 
//gettimeofday(&t1, NULL);
//unsigned long long tu0 = (unsigned long long)t0.tv_sec*1000000L+(unsigned long long)t0.tv_usec;
//unsigned long long      tu1 = (unsigned long long)t1.tv_sec*1000000L+(unsigned long long)t1.tv_usec;
  //      printf("send=%.3lfs\n", (tu1 - tu0) / 1000000.0);  
}
    _ccl_check_size_data(return_sendto, size, primitive, 1);
}


