#include "pocl_cl.h"
CL_API_ENTRY cl_mem CL_API_CALL
POname(clCreateFromGLTexture3D)(cl_context context,cl_mem_flags flags,cl_GLenum target,cl_GLint  miplevel,cl_GLuint texture,cl_int *errcode_ret) 
CL_API_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clCreateFromGLTexture3D primitive\n ---");    
#endif 

#if DEBUG == 0
        puts("--- End execute clCreateFromGLTexture3D primitive\n ---");    
#endif 
    
  return NULL;
}
POsym(clCreateFromGLTexture3D)
