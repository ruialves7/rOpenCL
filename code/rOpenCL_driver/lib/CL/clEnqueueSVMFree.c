#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clEnqueueSVMFree) (cl_command_queue command_queue,cl_uint  num_svm_pointers,void  *svm_pointers[],void (CL_CALLBACK  *pfn_free_func) ( cl_command_queue queue,cl_uint num_svm_pointers,void *svm_pointers[],void  *user_data),void *user_data,cl_uint num_events_in_wait_list,const cl_event *event_wait_list,cl_event *event) CL_API_SUFFIX__VERSION_2_0
{
#if DEBUG == 0
        puts("--- Start execute clEnqueueSVMFree primitive\n ---");    
#endif

#if DEBUG == 0
        puts("--- End execute clEnqueueSVMFree primitive\n ---");    
#endif
        
           puts("--- execute  clEnqueueSVMFree primitive not implemeented\n ---");    
           exit(0);
  return CL_INVALID_VALUE;
}
POsym(clEnqueueSVMFree)

