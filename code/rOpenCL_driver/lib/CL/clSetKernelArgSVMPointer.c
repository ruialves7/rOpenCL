#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clSetKernelArgSVMPointer)(cl_kernel kernel,cl_uint arg_index,const void *arg_value) CL_API_SUFFIX__VERSION_2_0
{
#if DEBUG == 0
        puts("--- Start execute clSetKernelArgSVMPointer primitive\n ---");    
#endif
#if DEBUG == 0
        puts("--- End execute clSetKernelArgSVMPointer primitive\n ---");    
#endif
        puts("--- clSetKernelArgSVMPointer primitive\n ---");  
        exit(0);
  return CL_INVALID_VALUE;
}
POsym(clSetKernelArgSVMPointer)
