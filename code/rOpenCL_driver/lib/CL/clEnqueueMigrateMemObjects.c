#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueMigrateMemObjects) (cl_command_queue command_queue,cl_uint num_mem_objects,const cl_mem *mem_objects,cl_mem_migration_flags flags,cl_uint num_events_in_wait_list,const cl_event *event_wait_list,cl_event *event)CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueMigrateMemObjects primitive\n ---");    
#endif
        
#if DEBUG == 0
        puts("--- End execute clEnqueueMigrateMemObjects primitive\n ---");    
#endif
        
         puts("---  clEnqueueMigrateMemObjects primitive not implemented\n ---");    
         exit(0);
  return CL_INVALID_VALUE;
}
POsym(clEnqueueMigrateMemObjects)
