#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueSVMMemFill) (cl_command_queue command_queue,void *svm_ptr,const void *pattern,size_t pattern_size,size_t size,cl_uint num_events_in_wait_list,const cl_event *event_wait_list,cl_event *event) CL_API_SUFFIX__VERSION_2_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueSVMMemFill primitive\n ---");    
#endif
        
#if DEBUG == 0
        puts("--- End execute clEnqueueSVMMemFill primitive\n ---");    
#endif
        
         puts("--- execute clEnqueueSVMMemFill primitive\n ---");  
         exit(0);
  return CL_INVALID_VALUE;
}
POsym(clEnqueueSVMMemFill)
