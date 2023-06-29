#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueNativeKernel)(cl_command_queue command_queue ,void (CL_CALLBACK * user_func)(void *), void * args ,size_t cb_args, cl_uint num_mem_objects, const cl_mem * mem_list, const void ** args_mem_loc ,cl_uint num_events_in_wait_list,
const cl_event * event_wait_list ,cl_event * event ) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueNativeKernel primitive\n ---");    
#endif

#if DEBUG == 0
        puts("--- End execute clEnqueueNativeKernel primitive\n ---");    
#endif
         puts("--- clEnqueueNativeKernel  not implemented \n ---");  
         //exit(0);
  return CL_INVALID_VALUE;
}
POsym(clEnqueueNativeKernel)
