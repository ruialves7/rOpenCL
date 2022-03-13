#include "pocl_cl.h"
/* Note - this is deprecated in 1.1, but (some of) the ICD loaders are built
 * against OCL 1.1, so we need it.
 */ 
CL_API_ENTRY void * CL_API_CALL 
POname(clGetExtensionFunctionAddress)(const char * func_name ) CL_EXT_SUFFIX__VERSION_1_0
{
#if DEBUG == 0
        puts("--- Start execute clGetExtensionFunctionAddress primitive\n ---");    
#endif
#ifdef BUILD_ICD
  if( strcmp(func_name, "clIcdGetPlatformIDsKHR")==0 )
  {
#if DEBUG == 0
        puts("--- End execute clGetExtensionFunctionAddress primitive\n ---");    
#endif
      return (void *)&POname(clIcdGetPlatformIDsKHR);
  }
#endif
  if( strcmp(func_name, "clGetPlatformInfo")==0 )
  {
    #if DEBUG == 0
        puts("--- End execute clGetExtensionFunctionAddress primitive\n ---");    
    #endif
    return (void *)&POname(clGetPlatformInfo);
  }
#if DEBUG == 0
        puts("--- End execute clGetExtensionFunctionAddress primitive\n ---");    
#endif
  return NULL;
}
POsymAlways(clGetExtensionFunctionAddress)
