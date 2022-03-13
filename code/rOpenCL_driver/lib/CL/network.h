/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   network.h
 * Author: noname
 *
 * Created on June 24, 2019, 11:15 PM
 */

#ifndef NETWORK_H
#define NETWORK_H
#include"common.h"

void init_udp(int * fd_descriptor, struct sockaddr_in *addr_host, struct sockaddr_in *addr_rDaemon, char * primitive) ;
int  init_tcp(int * fd_descriptor, struct sockaddr_in *addr, char * primitive);
void * build_header_udp(char id, int part_of_transaction, int total_packets, int size_total, char *id_transaction, struct sockaddr_in *addr);
void * build_header_tcp(char id, int *size_total);
char * generate_uuid_transaction(char *ip, int pid, int tid);
void setNrPacketHeader(void *header, int number);
void send_data_udp(int * fd, void * buffer, void * header, int size, struct sockaddr_in *addr, char * primitive);
void send_data_tcp(int * fd, void * buffer, void * header, int size, char * primitive);
int recv_data_udp(int * fd, void * buffer, int size, char * primitive);
int recv_data_tcp(int * fd, void * buffer, int size, char * primitive);
int closeSocketUdp(int *fd);
int closeSocketTCP(int *fd_socket,int *fd_connect);
int compare_desc(const void *pa, const void *pb);
void * lookup_descriptor(char *key);
void * register_descriptor(int des, char * key) ;
void * lookup_info_udp(char *key);
void * register_info_udp(int des, struct sockaddr_in *addr_host, struct sockaddr_in *addr_rDaemon, char * key) ;
int compare_info_udp(const void *pa, const void *pb);

#endif 
