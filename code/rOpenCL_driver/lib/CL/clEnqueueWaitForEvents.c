#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueWaitForEvents)(cl_command_queue  command_queue,cl_uint num_events,const cl_event * event_list) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueWaitForEvents primitive\n ---");    
#endif
#if DEBUG == 0
        puts("--- End execute clEnqueueWaitForEvents primitive\n ---");    
#endif
        
         puts("--- End execute clEnqueueWaitForEvents primitive\n ---");    
         exit(0);
  return CL_INVALID_VALUE;
}
POsym(clEnqueueWaitForEvents)
