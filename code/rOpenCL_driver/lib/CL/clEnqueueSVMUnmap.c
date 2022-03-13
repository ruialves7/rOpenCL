#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueSVMUnmap) (cl_command_queue command_queue, void *svm_ptr,cl_uint num_events_in_wait_list,const cl_event *event_wait_list,cl_event *event) CL_API_SUFFIX__VERSION_2_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueSVMUnmap primitive\n ---");    
#endif
        
#if DEBUG == 0
        puts("--- End execute clEnqueueSVMUnmap primitive\n ---");    
#endif
        
         puts("--- End execute clEnqueueSVMUnmap primitive\n ---");  
         exit(0);
  return CL_INVALID_VALUE;
}
POsym(clEnqueueSVMUnmap)

