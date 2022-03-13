#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL POname (clUnloadCompiler) (void) CL_API_SUFFIX__VERSION_1_1
{
#if DEBUG == 0
        puts("--- Start execute clUnloadCompiler primitive\n ---");    
#endif
#if DEBUG == 0
        puts("--- End execute clUnloadCompiler primitive\n ---");    
#endif
        puts("--- clUnloadCompiler primitive\n ---");  
        exit(0);
  return CL_INVALID_VALUE;
}
POsym(clUnloadCompiler)
