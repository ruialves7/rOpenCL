#include"common.h"

//void * tree_transaction_pendents = NULL;
//void * tree_transaction_done = NULL;
void * tree_command_queues = NULL;
int lock_primitives[256] = {0/*_connect*/, 0/*_cclGetPlatformIDs*/, 0/*NULL*/, 0/*_cclGetPlatformInfo*/, 0/*_cclGetDeviceIDs*/, 0/*_cclGetDeviceInfo*/, 0/*_cclCreateContext*/, 0/*_cclCreateCommandQueue*/, 0/*_cclCreateBuffer*/, 1/*_cclEnqueueReadBuffer*/, 1/*_cclEnqueueWriteBuffer*/, 0/* _cclCreateProgramWithSource*/, 0/*_cclBuildProgram*/, 0/*_cclCreateKernel*/, 0/*_cclSetKernelArg*/, 1/*_cclEnqueueNDRangeKernel*/, 0/*_cclGetEventProfilingInfo*/, 0/* _cclCreateImage*/, 1/*_cclEnqueueReadImage*/, 1/*_cclEnqueueWriteImage*/, 1/*_cclEnqueueFillImage*/, 1/* _cclEnqueueCopyImage*/, 1/*_cclEnqueueCopyImageToBuffer*/, 1/*_cclEnqueueCopyBufferToImage*/, 0/* NULL*/, 0/*_cclCreateSubBuffer*/, 1/*_cclEnqueueReadBufferRect*/, 1/*_cclEnqueueWriteBufferRect*/, 1/*_cclEnqueueFillBuffer*/, 1/*_cclEnqueueCopyBuffer*/, 1/*_cclEnqueueCopyBufferRect*/, 1/*_cclEnqueueMapImage*/, 0/*NULL*/, 1/*_cclEnqueueMapBuffer*/, 0/*_cclCreateSubDevices*/, 0/*NULL*/, 0/*_cclRetainDevice*/, 0/*_cclReleaseDevice*/, 0/*_cclCreateContextFromType*/, 0/*_cclRetainContext*/, 0/*_cclReleaseContext*/, 0/*_cclGetContextInfo*/, 0/*NULL*/, 0/*_cclGetExtensionFunctionAddressForPlatform*/, 0/*NULL*/, 0/*_cclRetainCommandQueue*/, 0/*_cclReleaseCommandQueue*/, 0/*_cclGetCommandQueueInfo*/, 0/*_cclGetSupportedImageFormats*/, 0/*_cclGetImageInfo*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/* NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*NULL*/, 0/*_cclFlush*/, 0/*_cclFinish*/, 0/*_cclRetainMemObject*/, 0/*_cclSetMemObjectDestructorCallback*/, 1/*_cclEnqueueUnmapMemObject*/, 0/*_cclReleaseMemObject*/, 0/*_cclCreateSampler*/, 0/*_cclRetainSampler*/, 0/*_cclReleaseSampler*/, 0/*_cclGetSamplerInfo*/, 0/*NULL*/, 0/*_cclCreateProgramWithBinary*/, 0/*_cclCreateProgramWithBuiltInKernels*/, 0/* _cclRetainProgram*/, 0/*_cclReleaseProgram*/, 1/*_cclEnqueueMigrateMemObjects*/, 0/* NULL*/, 0/*NULL*/, 0/*_cclGetMemObjectInfo*/, 0/*_cclCompileProgram*/, 0/*_cclLinkProgram*/, 0/*_cclUnloadPlatformCompiler*/, 0/*_cclGetProgramInfo*/, 0/*_cclGetProgramBuildInfo*/, 0/*_cclCreateKernelsInProgram*/, 0/*_cclRetainKernel*/, 0/*_cclReleaseKernel*/, 0/*NULL*/, 0 /* NULL*/, 0/*_cclCreateUserEvent*/, 0/*_cclSetUserEventStatus*/, 0/*_cclWaitForEvents*/, 0/*_cclGetEventInfo*/, 0/* NULL*/, 0/*_cclGetKernelInfo*/, 0/* _cclGetKernelWorkGroupInfo*/, 0/*_cclGetKernelArgInfo*/, 0/*_cclRetainEvent*/, 0/*_cclReleaseEvent*/, 0/*_cclSetEventCallback*/, 1/*_cclEnqueueMarkerWithWaitList*/, 1/*_cclEnqueueBarrierWithWaitList*/, 1/*_cclEnqueueNativeKernel*/, 0/*NULL*/, 1/*_cclEnqueueTask*/, 0/*_cclCreateFromGLBuffer*/, 0/*_cclCreateFromGLTexture*/, 0/*_cclCreateFromGLRenderbuffer*/, 0/* _cclGetGLObjectInfo*/, 0/*_cclGetGLTextureInfo*/, 0/*_cclEnqueueAcquireGLObjects*/, 0/*_cclEnqueueReleaseGLObjects*/, 0/*_cclCreateEventFromGLsyncKHR*/, 0/*_cclGetGLContextInfoKHR*/, 1/*_cclEnqueueMarker*/, 1/*_cclEnqueueBarrier*/};

int fd, primitive_available_udp = 0, fd_tcp;
struct sockaddr_in addr, addr_tcp, cli_tcp;
pthread_mutex_t mutex_udp = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_udp = PTHREAD_COND_INITIALIZER;
pthread_rwlock_t lock_mutex = PTHREAD_RWLOCK_INITIALIZER;
pthread_t dispatcher_thread[MAX_NUM_THREADS];
pthread_rwlock_t *mutexs = NULL;
cl_device_id * devices = NULL;
cl_uint totaldevices = 0;
pthread_mutex_t mutex_ts=PTHREAD_MUTEX_INITIALIZER;

void init_daemon() {

   cl_uint platformCount;
    cl_platform_id* platforms;
    cl_int ret_code;
    cl_uint deviceCount;
    ret_code = clGetPlatformIDs(0, NULL, &platformCount);
    if ((ret_code != CL_SUCCESS) || (platformCount == 0))
        _ccl_perror_and_exit("OpenCL");

    platforms = (cl_platform_id*) malloc(sizeof (cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);

    for (int i = 0; i < platformCount; i++) {

        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
        totaldevices += deviceCount;
        devices = (cl_device_id*) realloc(devices, sizeof (cl_device_id) * totaldevices);
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, (devices + (totaldevices - deviceCount)), NULL);
    }
    mutexs = malloc(sizeof (pthread_rwlock_t) * totaldevices);
    for (int i = 0; i < totaldevices; i++)
        pthread_rwlock_init(&mutexs[i], NULL);
}

void _ccl_perror_and_exit(char*str) {
    perror(str);
    exit(1);
}

void _ccl_check_size_data(int real, int theorical, char *primitives, int operation) {
    if (real >= 0) {
        if (real != theorical) {
            printf("An error has occurred\nReal data:%d\n Theorical data:%d\n Primitive: %s\n Operation:%d\n", real, theorical, primitives, operation);
            perror("Fatal error");
            exit(0);
        }
    } else {
        printf("Primitive: %s\n Operation:%d\n", primitives, operation);
        _ccl_perror_and_exit("Error");
    }
}

int _ccl_equals_addr(struct sockaddr_in * s1, struct sockaddr_in * s2) {

    if ((s1->sin_addr.s_addr == s2->sin_addr.s_addr) && (s1->sin_family == s2->sin_family)&&(s1->sin_port == s2->sin_port))
        return 0;
    else
        return 1;
}

/*void _ccl_memcpy(void * dst, void *src, int size_data, int * total_offset) {
    memcpy(dst, src, size_data);
    *total_offset += size_data;
}*/

int compare_network(const void *pa, const void *pb) {
    available_primitive_udp *pa_ = (available_primitive_udp*) pa;
    available_primitive_udp *pb_ = (available_primitive_udp*) pb;
    return strcmp(pa_->id_transaction, pb_->id_transaction);
}

int findDevice(cl_device_id device) {
    for (int i = 0; i < totaldevices; i++)
        if (devices[i] == device)
            return i;
    return -1;
}

int compare_commandqueue(const void *pa, const void *pb) {
    cl_command_queue_object *pa_ = (cl_command_queue_object*) pa;
    cl_command_queue_object *pb_ = (cl_command_queue_object*) pb;

    if (pa_->queue < pb_->queue)
        return -1;
    
    else if (pa_->queue > pb_->queue)
        return 1;
    else
        return 0;

}





