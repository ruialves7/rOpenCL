#ifndef CL_MANAGEMENT_H
#define CL_MANAGEMENT_H 
#include"pocl_cl.h"
#ifdef BUILD_ICD
struct _cl_icd_dispatch pocl_dispatch = 
{
  &POname(clGetPlatformIDs),
  &POname(clGetPlatformInfo),
  &POname(clGetDeviceIDs),
  &POname(clGetDeviceInfo),
  &POname(clCreateContext),
  &POname(clCreateContextFromType),
  &POname(clRetainContext),
  &POname(clReleaseContext),
  &POname(clGetContextInfo),
  &POname(clCreateCommandQueue),
  &POname(clRetainCommandQueue), /* 10 */
  &POname(clReleaseCommandQueue),
  &POname(clGetCommandQueueInfo),
  NULL /*clSetCommandQueueProperty*/,
  &POname(clCreateBuffer),
  &POname(clCreateImage2D),
  &POname(clCreateImage3D),
  &POname(clRetainMemObject),
  &POname(clReleaseMemObject),
  &POname(clGetSupportedImageFormats),
  &POname(clGetMemObjectInfo), /* 20 */
  &POname(clGetImageInfo),
  &POname(clCreateSampler),
  &POname(clRetainSampler),
  &POname(clReleaseSampler),
  &POname(clGetSamplerInfo),
  &POname(clCreateProgramWithSource),
  &POname(clCreateProgramWithBinary),
  &POname(clRetainProgram),
  &POname(clReleaseProgram),
  &POname(clBuildProgram), /* 30 */
  &POname(clUnloadCompiler),
  &POname(clGetProgramInfo),
  &POname(clGetProgramBuildInfo),
  &POname(clCreateKernel),
  &POname(clCreateKernelsInProgram),
  &POname(clRetainKernel),
  &POname(clReleaseKernel),
  &POname(clSetKernelArg),
  &POname(clGetKernelInfo),
  &POname(clGetKernelWorkGroupInfo), /* 40 */
  &POname(clWaitForEvents),
  &POname(clGetEventInfo),
  &POname(clRetainEvent),
  &POname(clReleaseEvent),
  &POname(clGetEventProfilingInfo),
  &POname(clFlush),
  &POname(clFinish),
  &POname(clEnqueueReadBuffer),
  &POname(clEnqueueWriteBuffer),
  &POname(clEnqueueCopyBuffer), /* 50 */
  &POname(clEnqueueReadImage),
  &POname(clEnqueueWriteImage),
  &POname(clEnqueueCopyImage),
  &POname(clEnqueueCopyImageToBuffer),
  &POname(clEnqueueCopyBufferToImage),
  &POname(clEnqueueMapBuffer),
  &POname(clEnqueueMapImage),
  &POname(clEnqueueUnmapMemObject),
  &POname(clEnqueueNDRangeKernel),
  &POname(clEnqueueTask), /* 60 */
  &POname(clEnqueueNativeKernel),
  &POname(clEnqueueMarker),
  &POname(clEnqueueWaitForEvents),
  &POname(clEnqueueBarrier),
  &POname(clGetExtensionFunctionAddress),
  &POname(clCreateFromGLBuffer),
  &POname(clCreateFromGLTexture2D),
  &POname(clCreateFromGLTexture3D),
  &POname(clCreateFromGLRenderbuffer),
  &POname(clGetGLObjectInfo),
  &POname(clGetGLTextureInfo),
  &POname(clEnqueueAcquireGLObjects),
  &POname(clEnqueueReleaseGLObjects),
  &POname(clGetGLContextInfoKHR),
  NULL, /* &clUnknown75 */
  NULL, /* &clUnknown76 */
  NULL, /* &clUnknown77 */
  NULL, /* &clUnknown78 */
  NULL, /* &clUnknown79 */
  NULL, /* &clUnknown80 */
  &POname(clSetEventCallback),
  &POname(clCreateSubBuffer),
  &POname(clSetMemObjectDestructorCallback),
  &POname(clCreateUserEvent),
  &POname(clSetUserEventStatus),
  &POname(clEnqueueReadBufferRect),
  &POname(clEnqueueWriteBufferRect),
  &POname(clEnqueueCopyBufferRect),
  NULL, /* &POname(clCreateSubDevicesEXT),     */
  &POname(clRetainDevice), /* &POname(clRetainDeviceEXT),         */
  &POname(clReleaseDevice), /* &POname(clReleaseDeviceEXT),        */
  NULL, /* &clUnknown92 */
  &POname(clCreateSubDevices),
  &POname(clRetainDevice),
  &POname(clReleaseDevice),
  &POname(clCreateImage),
  &POname(clCreateProgramWithBuiltInKernels),
  &POname(clCompileProgram),
  &POname(clLinkProgram),
  &POname(clUnloadPlatformCompiler),
  &POname(clGetKernelArgInfo),
  &POname(clEnqueueFillBuffer),
  &POname(clEnqueueFillImage),
  &POname(clEnqueueMigrateMemObjects),
  &POname(clEnqueueMarkerWithWaitList),
  &POname(clEnqueueBarrierWithWaitList),
  &POname(clGetExtensionFunctionAddressForPlatform),
  &POname(clCreateFromGLTexture),
  NULL, /* &clUnknown109 */
  NULL, /* &clUnknown110 */
  NULL, /* &clUnknown111 */
  NULL, /* &clUnknown112 */
  NULL, /* &clUnknown113 */
  NULL, /* &clUnknown114 */
  NULL, /* &clUnknown115 */
  NULL, /* &clUnknown116 */
  NULL, /* &clUnknown117 */
  NULL, /* &clUnknown118 */
  NULL, /* &clUnknown119 */
  NULL, /* &clUnknown120 */
  NULL, /* &clUnknown121 */
  NULL, /* &clUnknown122 */
#if (OCL_ICD_IDENTIFIED_FUNCTIONS > 110)
  &POname(clCreateCommandQueueWithProperties),
  NULL, /* &POname(clCreatePipe),*/
  NULL, /* &POname(clGetPipeInfo),*/
  &POname(clSVMAlloc),
  &POname(clSVMFree),
  &POname(clEnqueueSVMFree),
  &POname(clEnqueueSVMMemcpy),
  &POname(clEnqueueSVMMemFill),
  &POname(clEnqueueSVMMap),
  &POname(clEnqueueSVMUnmap),
  NULL, /* clCreateSamplerWithProperties */
  &POname(clSetKernelArgSVMPointer),
  &POname(clSetKernelExecInfo),
  NULL, /* &clUnknown136 */
  NULL, /* &clUnknown137 */
  NULL, /* &clUnknown138 */
  NULL, /* &clUnknown139 */
  NULL, /* &clUnknown140 */
  NULL, /* &clUnknown141 */
  NULL, /* &clUnknown142 */
  NULL, /* &clUnknown143 */
  NULL, /* &clUnknown144 */
  NULL, /* &clUnknown145 */
  NULL, /* &clUnknown146 */
  NULL, /* &clUnknown147 */
  NULL, /* &clUnknown148 */
  NULL, /* &clUnknown149 */
  NULL, /* &clUnknown150 */
  NULL, /* &clUnknown151 */
  NULL, /* &clUnknown152 */
  NULL, /* &clUnknown153 */
  NULL, /* &clUnknown154 */
  NULL, /* &clUnknown155 */
  NULL, /* &clUnknown156 */
  NULL, /* &clUnknown157 */
  NULL, /* &clUnknown158 */
  NULL, /* &clUnknown159 */
  NULL, /* &clUnknown160 */
  NULL, /* &clUnknown161 */
  NULL, /* &clUnknown162 */
  NULL, /* &clUnknown163 */
  NULL, /* &clUnknown164 */
  NULL, /* &clUnknown165 */
#endif
#if (OCL_ICD_IDENTIFIED_FUNCTIONS > 127)
  NULL, /* &clUnknown166 */
  NULL, /* &clUnknown167 */
  NULL, /* &clUnknown168 */
  NULL, /* &clUnknown169 */
  NULL, /* &clUnknown170 */
  NULL, /* &clUnknown171 */
  NULL, /* &clUnknown172 */
  NULL, /* &clUnknown173 */
  NULL, /* &clUnknown174 */
  NULL, /* &clUnknown175 */
  NULL, /* &clUnknown176 */
  NULL, /* &clUnknown177 */
#endif
#if (OCL_ICD_IDENTIFIED_FUNCTIONS > 129)
  NULL, /* &clUnknown178 */
  NULL, /* &clUnknown179 */
#endif
};
#endif
void *getDispachPointer(int type);

#endif
