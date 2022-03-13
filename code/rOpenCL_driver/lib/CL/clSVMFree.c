#include "pocl_cl.h"
CL_API_ENTRY void CL_API_CALL
POname(clSVMFree)(cl_context context,void *svm_pointer) CL_API_SUFFIX__VERSION_2_0
{
#if DEBUG == 0
        puts("--- Start execute clSVMFree primitive\n ---");    
#endif
#if DEBUG == 0
        puts("--- End execute clSVMFree primitive\n ---");    
#endif
        
        puts("--- execute clSVMFree primitive\n ---");  
        exit(0);
}
POsym(clSVMFree)

