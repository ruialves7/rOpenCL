#include "pocl_cl.h"
CL_API_ENTRY void* CL_API_CALL
POname(clSVMAlloc)(cl_context context, cl_svm_mem_flags flags,size_t size,unsigned int alignment) CL_API_SUFFIX__VERSION_2_0
{
#if DEBUG == 0
        puts("--- Start execute clSVMAlloc primitive\n ---");    
#endif
 #if DEBUG == 0
        puts("--- End execute clSVMAlloc primitive\n ---");    
#endif   
        
        puts("---  execute clSVMAlloc primitive\n ---");    
        exit(0);
  return NULL;
}
POsym(clSVMAlloc)

