#include "pocl_cl.h"
CL_API_ENTRY cl_mem CL_API_CALL
POname(clCreateImage2D) (cl_context context,cl_mem_flags flags,const cl_image_format * image_format,size_t image_width,size_t image_height,size_t image_row_pitch, void * host_ptr,cl_int * errcode_ret)
CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clCreateImage2D primitive\n ---");    
#endif
#if DEBUG == 0
        puts("--- End execute clCreateImage2D primitive\n ---");    
#endif 
         puts("--- clCreateImage2D not implemented\n ---");    
         exit(0);
  return NULL; 
}
POsym(clCreateImage2D) 
