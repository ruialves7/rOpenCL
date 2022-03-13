#include "pocl_cl.h"
CL_API_ENTRY cl_command_queue CL_API_CALL
POname(clCreateCommandQueueWithProperties)(cl_context context,cl_device_id device,const cl_queue_properties *properties,cl_int *errcode_ret) CL_API_SUFFIX__VERSION_1_0
{
    #if DEBUG == 0
        puts("--- Start execute clCreateCommandQueueWithProperties primitive\n ---");    
    #endif
        
    #if DEBUG == 0
        puts("--- End execute clCreateCommandQueueWithProperties primitive\n ---");    
    #endif 
       puts("--- clCreateCommandQueueWithProperties not implemented\n ---");  
            exit(0);
  return NULL;
}
POsym(clCreateCommandQueueWithProperties)
