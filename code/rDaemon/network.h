/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   network.h
 * Author: noname
 *
 * Created on June 27, 2019, 11:31 PM
 */

#ifndef NETWORK_H
#define NETWORK_H
#include"common.h"
void send_data_udp(int * fd, void * buffer, int size, struct sockaddr_in *addr, char * primitive);
void send_data_tcp(int * fd, void * buffer, int size,struct sockaddr_in *addr, char * primitive);

#endif /* NETWORK_H */

