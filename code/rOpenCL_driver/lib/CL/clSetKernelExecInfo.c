#include "pocl_cl.h"
CL_API_ENTRY cl_int CL_API_CALL
POname(clSetKernelExecInfo)(cl_kernel  kernel ,cl_kernel_exec_info  param_name ,size_t  param_value_size ,const void  *param_value) CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clSetKernelExecInfo primitive\n ---");    
#endif
#if DEBUG == 0
        puts("--- End execute clSetKernelExecInfo primitive\n ---");    
#endif
         puts("---  execute clSetKernelExecInfo primitive\n ---");    
         exit(0);
  return CL_INVALID_VALUE;
}
POsym(clSetKernelExecInfo)
