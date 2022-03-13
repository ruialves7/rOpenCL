#include "pocl_cl.h"
extern CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueCopyBufferToImage)(cl_command_queue  command_queue,cl_mem src_buffer,cl_mem dst_image,size_t            src_offset,const size_t * dst_origin, const size_t * region,cl_uint num_events_in_wait_list,const cl_event * event_wait_list,cl_event * event)CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueCopyBufferToImage primitive\n ---");    
#endif
        
#if DEBUG == 0
        puts("--- End execute clEnqueueCopyBufferToImage primitive\n ---");    
#endif
        
 puts("--- clEnqueueCopyBufferToImage primitive not implemented\n ---");   
 exit(0);
  return CL_INVALID_VALUE;
}
POsym(clEnqueueCopyBufferToImage)
