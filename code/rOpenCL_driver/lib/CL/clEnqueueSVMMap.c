#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueSVMMap) (cl_command_queue command_queue,cl_bool blocking_map,cl_map_flags map_flags,void *svm_ptr,
size_t size,cl_uint num_events_in_wait_list,const cl_event *event_wait_list,cl_event *event) CL_API_SUFFIX__VERSION_2_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueSVMMap primitive\n ---");    
#endif
#if DEBUG == 0
        puts("--- End execute clEnqueueSVMMap primitive\n ---");    
#endif
        puts("--- execute clEnqueueSVMMap primitive \n ---"); 
        exit(0);
    return CL_INVALID_VALUE;
}
POsym(clEnqueueSVMMap)

