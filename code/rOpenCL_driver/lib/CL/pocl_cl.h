#ifndef POCL_CL_H
#define POCL_CL_H

#include <assert.h>
#include <stdio.h>
#include <CL/opencl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include"common.h"
#include"cl_management.h" 


#define BUILD_ICD
#  define POname(name) PO##name
#  define POdeclsym(name)                      \
  __typeof__(name) PO##name __attribute__((visibility("hidden")));
  
#  define POCL_ALIAS_OPENCL_SYMBOL(name)                                \
  __typeof__(name) name __attribute__((alias ("PO" #name), visibility("default")));
  
#  define POsymAlways(name) POCL_ALIAS_OPENCL_SYMBOL(name)

#  if !defined(BUILD_ICD)
#    define POsym(name) POCL_ALIAS_OPENCL_SYMBOL(name)
#  else
#    define POsym(name)
#  endif


#ifdef __cplusplus
extern "C"
{
  CL_API_ENTRY cl_int CL_API_CALL POname (clReleaseEvent) (cl_event event) CL_API_SUFFIX__VERSION_1_0;
}
#endif

#ifdef BUILD_ICD
#  define POCL_ICD_OBJECT struct _cl_icd_dispatch *dispatch;
#  define POCL_ICD_OBJECT_PLATFORM_ID POCL_ICD_OBJECT
#  define POsymICD(name) POsym(name)
#  define POdeclsymICD(name) POdeclsym(name)
#else
#  define POCL_ICD_OBJECT
#  define POCL_ICD_OBJECT_PLATFORM_ID unsigned long id;
#  define POsymICD(name)
#  define POdeclsymICD(name)
#endif

POdeclsym(clBuildProgram)
POdeclsym(clLinkProgram)
POdeclsym(clCompileProgram)
POdeclsym(clCreateBuffer)
POdeclsym(clCreateCommandQueue)
POdeclsym(clCreateContext)
POdeclsym(clCreateContextFromType)
POdeclsym(clCreateImage2D) 
POdeclsym(clCreateImage3D)
POdeclsym(clCreateImage)
POdeclsym(clCreateKernel)
POdeclsym(clCreateKernelsInProgram)
POdeclsym(clCreatePipe)
POdeclsym(clCreateProgramWithBinary)
POdeclsym(clCreateProgramWithBuiltInKernels)
POdeclsym(clCreateProgramWithSource)
POdeclsym(clCreateSampler)
POdeclsym(clCreateSubBuffer)
POdeclsym(clCreateSubDevices)
POdeclsym(clCreateUserEvent)
POdeclsym(clEnqueueBarrier)
POdeclsym(clEnqueueBarrierWithWaitList)
POdeclsym(clEnqueueCopyBuffer)
POdeclsym(clEnqueueCopyBufferRect)
POdeclsym(clEnqueueCopyBufferToImage) 
POdeclsym(clEnqueueCopyImage)
POdeclsym(clEnqueueCopyImageToBuffer)
POdeclsym(clEnqueueMapBuffer)
POdeclsym(clEnqueueMapImage)
POdeclsym(clEnqueueMarker) 
POdeclsym(clEnqueueMigrateMemObjects)
POdeclsym(clEnqueueNativeKernel)
POdeclsym(clEnqueueNDRangeKernel)
POdeclsym(clEnqueueReadBuffer)
POdeclsym(clEnqueueReadBufferRect)
POdeclsym(clEnqueueReadImage)
POdeclsym(clEnqueueTask)
POdeclsym(clEnqueueUnmapMemObject)
POdeclsym(clEnqueueWaitForEvents)
POdeclsym(clEnqueueMarkerWithWaitList)
POdeclsym(clEnqueueWriteBuffer)
POdeclsym(clEnqueueWriteBufferRect)
POdeclsym(clEnqueueWriteImage)
POdeclsym(clEnqueueFillImage)
POdeclsym(clEnqueueFillBuffer)
POdeclsym(clFinish)
POdeclsym(clFlush)
POdeclsym(clGetCommandQueueInfo)
POdeclsym(clGetContextInfo)
POdeclsym(clGetDeviceIDs)
POdeclsym(clGetDeviceInfo)
POdeclsym(clGetEventInfo)
POdeclsym(clGetEventProfilingInfo)
POdeclsym(clGetExtensionFunctionAddress)
POdeclsym(clGetExtensionFunctionAddressForPlatform)
POdeclsym(clGetImageInfo)
POdeclsym(clGetKernelInfo)
POdeclsym(clGetKernelArgInfo)
POdeclsym(clGetKernelWorkGroupInfo)
POdeclsym(clGetMemObjectInfo)
POdeclsym(clGetPlatformIDs)
POdeclsym(clGetPlatformInfo)
POdeclsym(clGetProgramBuildInfo)
POdeclsym(clGetProgramInfo)
POdeclsym(clGetSamplerInfo)
POdeclsym(clGetSupportedImageFormats)
POdeclsymICD(clIcdGetPlatformIDsKHR)
POdeclsym(clReleaseCommandQueue)
POdeclsym(clReleaseContext)
POdeclsym(clReleaseDevice)
POdeclsym(clReleaseEvent)
POdeclsym(clReleaseKernel)
POdeclsym(clReleaseMemObject)
POdeclsym(clReleaseProgram)
POdeclsym(clReleaseSampler)
POdeclsym(clRetainCommandQueue)
POdeclsym(clRetainContext)
POdeclsym(clRetainDevice)
POdeclsym(clRetainEvent)
POdeclsym(clRetainKernel)
POdeclsym(clRetainMemObject)
POdeclsym(clRetainProgram)
POdeclsym(clRetainSampler)
POdeclsym(clSetEventCallback)
POdeclsym(clSetKernelArg)
POdeclsym(clSetMemObjectDestructorCallback)
POdeclsym(clSetUserEventStatus)
POdeclsym(clUnloadCompiler)
POdeclsym(clUnloadPlatformCompiler)
POdeclsym(clWaitForEvents)
POdeclsym(clEnqueueSVMFree)
POdeclsym(clEnqueueSVMMap)
POdeclsym(clEnqueueSVMMemcpy)
POdeclsym(clEnqueueSVMMemFill)
POdeclsym(clEnqueueSVMUnmap)
POdeclsym(clSVMFree)
POdeclsym(clSVMAlloc)
POdeclsym(clSetKernelArgSVMPointer)
POdeclsym(clSetKernelExecInfo)
POdeclsym(clCreateCommandQueueWithProperties)
POdeclsym(clCreateFromGLBuffer)
POdeclsym(clCreateFromGLTexture)
POdeclsym(clCreateFromGLTexture2D)
POdeclsym(clCreateFromGLTexture3D)
POdeclsym(clCreateFromGLRenderbuffer)
POdeclsym(clGetGLObjectInfo)
POdeclsym(clGetGLTextureInfo)
POdeclsym(clEnqueueAcquireGLObjects)
POdeclsym(clEnqueueReleaseGLObjects)
POdeclsym(clGetGLContextInfoKHR)
//clCreateEventFromGLsyncKHR not  append 

extern struct _cl_icd_dispatch pocl_dispatch;
struct _cl_icd_dispatch 
{
        void *funcptr[166];
};
struct _cl_platform_id 
{
  POCL_ICD_OBJECT_PLATFORM_ID
}; 

struct _cl_device_id {
  POCL_ICD_OBJECT
};

struct _cl_context_{
  POCL_ICD_OBJECT

};

struct _cl_kernel_{
  POCL_ICD_OBJECT
};

struct _cl_program_{
  POCL_ICD_OBJECT
};

struct _cl_command_queue_
{
  POCL_ICD_OBJECT
};

struct _cl_mem{
  POCL_ICD_OBJECT
};

struct _cl_event_{
  POCL_ICD_OBJECT
};

struct _cl_sampler_{
  POCL_ICD_OBJECT
};



#define POCL_RETURN_GETINFO_STR(__STR__)                                \
  do                                                                    \
    {                                                                   \
      size_t const value_size = strlen(__STR__) + 1;                    \
      POCL_RETURN_GETINFO_INNER(value_size,                             \
                  memcpy(param_value, __STR__, value_size));            \
    }                                                                   \
  while (0)
  
#define POCL_RETURN_GETINFO_INNER(__SIZE__, MEMASSIGN)                        \
  do                                                                          \
    {                                                                         \
      if (param_value)                                                        \
        {                                                                     \
          MEMASSIGN;                                                          \
        }                                                                     \
      if (param_value_size_ret)                                               \
        *param_value_size_ret = __SIZE__;                                     \
      return CL_SUCCESS;                                                      \
    }                                                                         \
  while (0)
      
#endif /* POCL_CL_H */
