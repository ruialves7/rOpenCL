#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueCopyBufferRect)(cl_command_queue command_queue,cl_mem src_buffer,cl_mem dst_buffer,const size_t *src_origin,const size_t *dst_origin, const size_t *region,size_t src_row_pitch,size_t src_slice_pitch,size_t dst_row_pitch,size_t dst_slice_pitch, cl_uint num_events_in_wait_list,const cl_event *event_wait_list,cl_event *event) CL_API_SUFFIX__VERSION_1_1
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueCopyBufferRect primitive\n ---");    
#endif
        
#if DEBUG == 0
        puts("--- End execute clEnqueueCopyBufferRect primitive\n ---");    
#endif
         puts("--- clEnqueueCopyBufferRect not implemented\n ---");    
         exit(0);
  return CL_INVALID_VALUE;
}
POsym(clEnqueueCopyBufferRect)
