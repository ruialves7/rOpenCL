#include <assert.h>
#include <string.h>
#include "pocl_cl.h"

#ifdef BUILD_ICD
CL_API_ENTRY cl_int CL_API_CALL
POname(clIcdGetPlatformIDsKHR)(cl_uint num_entries, cl_platform_id * platforms,cl_uint * num_platforms)CL_API_SUFFIX__VERSION_1_0
{	
#if DEBUG == 0
        puts("--- Start execute clIcdGetPlatformIDsKHR primitive\n ---");    
#endif

#if DEBUG == 0
        puts("--- End execute clIcdGetPlatformIDsKHR primitive\n ---");    
#endif
  return POname(clGetPlatformIDs)( num_entries, platforms, num_platforms );
}
POsymICD(clIcdGetPlatformIDsKHR)
#endif

