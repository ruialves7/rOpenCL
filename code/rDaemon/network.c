/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "network.h"

#include <sys/time.h>

void send_data_tcp(int *fd, void *buffer, size_t size, char *primitive)
{
    size_t return_sendto = 0;
    return_sendto = send(*fd, buffer, size, 0);
    _ccl_check_size_data(return_sendto, size, primitive, 1);
}
