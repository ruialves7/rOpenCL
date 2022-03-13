#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueCopyImage)(cl_command_queue command_queue ,cl_mem src_image ,cl_mem dst_image,const size_t *        src_origin,const size_t * dst_origin ,const size_t * region, cl_uint  num_events_in_wait_list ,const cl_event *     event_wait_list , cl_event * event ) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueCopyImage primitive\n ---");    
#endif

#if DEBUG == 0
        puts("--- End execute clEnqueueCopyImage primitive\n ---");    
#endif
        
        puts("--- not clEnqueueCopyImage primitive\n ---");  
        exit(0);
  return CL_INVALID_VALUE;
}
POsym(clEnqueueCopyImage)
