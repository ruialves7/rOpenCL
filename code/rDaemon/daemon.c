#include "common.h"
#include <time.h>
#include <sys/time.h>
// Array to avoid the if/else struct when is needed send data.
char func[128][128] = {"_connect", " _cclGetPlatformIDs", "NULL", " _cclGetPlatformInfo", " _cclGetDeviceIDs", " _cclGetDeviceInfo", " _cclCreateContext", " _cclCreateCommandQueue", " _cclCreateBuffer", " _cclEnqueueReadBuffer", " _cclEnqueueWriteBuffer", " _cclCreateProgramWithSource", " _cclBuildProgram", " _cclCreateKernel", " _cclSetKernelArg", " _cclEnqueueNDRangeKernel", " _cclGetEventProfilingInfo", " _cclCreateImage", " _cclEnqueueReadImage", " _cclEnqueueWriteImage", " _cclEnqueueFillImage", " _cclEnqueueCopyImage", " _cclEnqueueCopyImageToBuffer", " _cclEnqueueCopyBufferToImage", " NULL", " _cclCreateSubBuffer", " _cclEnqueueReadBufferRect", " _cclEnqueueWriteBufferRect", " _cclEnqueueFillBuffer", " _cclEnqueueCopyBuffer", " _cclEnqueueCopyBufferRect", " _cclEnqueueMapImage", " NULL", " _cclEnqueueMapBuffer", " _cclCreateSubDevices", " NULL", " _cclRetainDevice", " _cclReleaseDevice", " _cclCreateContextFromType", " _cclRetainContext", " _cclReleaseContext", " _cclGetContextInfo", " NULL", " _cclGetExtensionFunctionAddressForPlatform", " NULL", " _cclRetainCommandQueue", " _cclReleaseCommandQueue", " _cclGetCommandQueueInfo", " _cclGetSupportedImageFormats", " _cclGetImageInfo", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " NULL", " _cclFlush", " _cclFinish", " _cclRetainMemObject", " _cclSetMemObjectDestructorCallback", " _cclEnqueueUnmapMemObject", " _cclReleaseMemObject", " _cclCreateSampler", " _cclRetainSampler", " _cclReleaseSampler", " _cclGetSamplerInfo", " NULL", " _cclCreateProgramWithBinary", " _cclCreateProgramWithBuiltInKernels", " _cclRetainProgram", " _cclReleaseProgram", " _cclEnqueueMigrateMemObjects", " NULL", " NULL", " _cclGetMemObjectInfo", " _cclCompileProgram", " _cclLinkProgram", " _cclUnloadPlatformCompiler", " _cclGetProgramInfo", " _cclGetProgramBuildInfo", " _cclCreateKernelsInProgram", " _cclRetainKernel", " _cclReleaseKernel", " NULL", " NULL", " _cclCreateUserEvent", " _cclSetUserEventStatus", " _cclWaitForEvents", " _cclGetEventInfo", " NULL", " _cclGetKernelInfo", " _cclGetKernelWorkGroupInfo", " _cclGetKernelArgInfo", " _cclRetainEvent", " _cclReleaseEvent", " _cclSetEventCallback", " _cclEnqueueMarkerWithWaitList", " _cclEnqueueBarrierWithWaitList", " _cclEnqueueNativeKernel", " NULL", " _cclEnqueueTask", " _cclCreateFromGLBuffer", " _cclCreateFromGLTexture", " _cclCreateFromGLRenderbuffer", " _cclGetGLObjectInfo", " _cclGetGLTextureInfo", " _cclEnqueueAcquireGLObjects", " _cclEnqueueReleaseGLObjects", " _cclCreateEventFromGLsyncKHR", " _cclGetGLContextInfoKHR", " _cclEnqueueMarker", " _cclEnqueueBarrier"};

void _connect(void *_request)
{
    char id = 0x00;
    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    size_t size_buffer_reply = sizeof(size_t) + sizeof(char), offset_total = 0;
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &id, sizeof(char), &offset_total);
    buffer_data_reply += sizeof(char);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_connect");
    free(buffer_data_reply);
}

void _cclGetPlatformIDs(void *_request)
{
    int fd_tcp_client_primitive = 0;
    size_t size_buffer_reply = 0, offset_total = 0;
    void *buffer_data_reply = NULL;
    cl_uint num_entries;
    cl_int result;
    cl_uint num_platforms;
    cl_platform_id platforms[100];
    void *data = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client_primitive = request->fd;

    _ccl_memcpy(&num_entries, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    data -= offset_total;


    // find nr platforms
    result = clGetPlatformIDs(0, NULL, &num_platforms);

    num_entries = num_platforms;
    num_platforms = 0;
    result = clGetPlatformIDs(num_entries, platforms, &num_platforms);
    if (num_entries > 0 && num_platforms != 0)
    {
        size_buffer_reply += (sizeof(cl_platform_id) * num_entries);
    }

    size_buffer_reply += (sizeof(size_t)+sizeof(cl_int) + sizeof(cl_uint));
    buffer_data_reply = malloc(size_buffer_reply);

    offset_total = 0;

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &num_platforms, sizeof(cl_uint), &offset_total);
    buffer_data_reply += sizeof(cl_uint);

    if (num_entries > 0 && num_platforms != 0)
    {
        _ccl_memcpy(buffer_data_reply, platforms, sizeof(cl_platform_id) * num_platforms, &offset_total);
        buffer_data_reply += sizeof(cl_platform_id) * num_platforms;
    }

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetPlatformIDs");
    free(buffer_data_reply);
}

void _cclGetPlatformInfo(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client_primitive = 0;
    size_t size_buffer_reply = 0, offset_total = 0;
    cl_int result = 0;
    size_t param_value_size_ret = 0;
    char *param_value = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client_primitive = request->fd;

    cl_platform_id platform;
    _ccl_memcpy(&platform, data, sizeof(cl_platform_id), &offset_total);
    data += sizeof(cl_platform_id);
    cl_platform_info param_name;
    _ccl_memcpy(&param_name, data, sizeof(cl_platform_info), &offset_total);
    data += sizeof(cl_platform_info);

    size_t param_value_size;
    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;
    offset_total = 0;
    if (param_value_size > 0)
        param_value = malloc(param_value_size + 1);

    if (param_name == CL_PLATFORM_HOSTNAME && param_value_size > 0)
    {
        gethostname(param_value, param_value_size);
        param_value[strlen(param_value)] = '\0';
        param_value_size_ret = strlen(param_value) + 1;
    }
    else
    {

        result = clGetPlatformInfo(platform, param_name, param_value_size, param_value, &param_value_size_ret);
    }

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t) + param_value_size_ret;

    if (param_value_size <= 0)
        size_buffer_reply -= param_value_size_ret;

    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    if (param_value_size > 0)
    {
        _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
        buffer_data_reply += param_value_size_ret;
    }

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetPlatformInfo");
    free(buffer_data_reply);

    if (param_value_size > 0)
        free(param_value);
}

void _cclGetDeviceIDs(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;
    cl_int result;
    cl_uint num_devices;
    cl_device_id *device = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    cl_platform_id platform;
    _ccl_memcpy(&platform, data, sizeof(cl_platform_id), &offset_total);
    data += sizeof(cl_platform_id);

    cl_device_type device_type;
    _ccl_memcpy(&device_type, data, sizeof(cl_device_type), &offset_total);
    data += sizeof(cl_device_type);

    cl_uint num_entries;
    _ccl_memcpy(&num_entries, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    data -= offset_total;
    offset_total = 0;

    if (num_entries > 0)
        device = malloc(sizeof(cl_device_id) * num_entries);

    result = clGetDeviceIDs( platform, device_type, num_entries, device, &num_devices);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_uint) + sizeof(cl_device_id) * num_entries;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &num_devices, sizeof(cl_uint), &offset_total);
    buffer_data_reply += sizeof(cl_uint);

    if (num_entries > 0)
    {
        _ccl_memcpy(buffer_data_reply, device, sizeof(cl_device_id) * num_entries, &offset_total);
        buffer_data_reply += sizeof(cl_device_id) * num_entries;
    }

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetDeviceIDs");
    free(buffer_data_reply);

    if (num_entries > 0)
        free(device);
}

void _cclGetDeviceInfo(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    size_t size_buffer_reply = 0, offset_total = 0;
    int fd_tcp_client = 0;
    cl_int result;
    size_t param_value_size_ret = 0;
    char *param_value = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    cl_device_id device;
    _ccl_memcpy(&device, data, sizeof(cl_device_id), &offset_total);
    data += sizeof(cl_device_id);

    cl_device_info param_name;
    _ccl_memcpy(&param_name, data, sizeof(cl_device_info), &offset_total);
    data += sizeof(cl_device_info);

    size_t param_value_size;
    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;
    offset_total = 0;

    if (param_value_size > 0)
        param_value = malloc(param_value_size);

    result = clGetDeviceInfo(device, param_name, param_value_size, param_value, &param_value_size_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t);

    if (param_value_size > 0)
        size_buffer_reply += param_value_size_ret;

    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    if (param_value_size > 0)
    {

        _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
        buffer_data_reply += param_value_size_ret;
    }

    if (param_value_size > 0)
        free(param_value);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetDeviceInfo");
    free(buffer_data_reply);
}

void _cclCreateContext(void *_request)
{
    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0;int  offset_total = 0;
    cl_context result;
    cl_int errcode_ret;
    cl_context_properties *properties = NULL;
    cl_device_id *devices = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    int size_properties = 0;
    _ccl_memcpy(&size_properties, data, sizeof(int), &offset_total);
    data += sizeof(int);
    cl_uint num_devices;
    _ccl_memcpy(&num_devices, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);
    if (size_properties > 0)
    {
        properties = malloc(sizeof(cl_context_properties) * size_properties);
        _ccl_memcpy(properties, data, sizeof(cl_context_properties) * size_properties, &offset_total);
        data += sizeof(cl_context_properties) * size_properties;
    }

    if (num_devices > 0)
    {
        devices = malloc(sizeof(cl_device_id) * num_devices);
        _ccl_memcpy(devices, data, sizeof(cl_device_id) * num_devices, &offset_total);
        data += sizeof(cl_device_id) * num_devices;
    }

    data -= offset_total;
    offset_total = 0;
    result = clCreateContext(properties, num_devices, devices, NULL, NULL, &errcode_ret);
    size_buffer_reply = sizeof(size_t) + sizeof(cl_context) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_context), &offset_total);
    buffer_data_reply += sizeof(cl_context);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    if (size_properties > 0)
        free(properties);

    if (num_devices > 0)
        free(devices);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateContext");
    free(buffer_data_reply);
}

void _cclCreateCommandQueue(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_context context;
        cl_device_id device;
        cl_command_queue_properties properties;

    } ccl_request;

    struct
    {
        cl_command_queue result;
        cl_int errcode_ret;

    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    ccl_reply.result = clCreateCommandQueue(ccl_request.context, ccl_request.device, ccl_request.properties, &ccl_reply.errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateCommandQueue");
    free(buffer_data_reply);
}

void _cclCreateBuffer(void *_request)
{

    void *data = NULL, *host_ptr = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    struct
    {
        cl_context context;
        cl_mem_flags flags;
        size_t size;
        char host_ptr_is_null;
    } ccl_request;

    struct
    {
        cl_mem result;
        cl_int errcode_ret;
        uintptr_t ptr_remote;
    } ccl_reply;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&ccl_request, data, sizeof(ccl_request), &offset_total);
    data += sizeof(ccl_request);

    if (!ccl_request.host_ptr_is_null)
    {
        host_ptr = malloc(ccl_request.size);
        _ccl_memcpy(host_ptr, data, ccl_request.size, &offset_total);

        data += ccl_request.size;
    }

    data -= offset_total;
    offset_total = 0;

    ccl_reply.result = clCreateBuffer(ccl_request.context, ccl_request.flags, ccl_request.size, host_ptr, &ccl_reply.errcode_ret);

    if (!ccl_request.host_ptr_is_null)
    {
        ccl_reply.ptr_remote = (uintptr_t)host_ptr;
    }

    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateBuffer");
    free(buffer_data_reply);
}

void _cclCreateProgramWithSource(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    cl_program result;
    cl_int errcode_ret;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    cl_context context;
    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    cl_uint count;
    _ccl_memcpy(&count, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    size_t *lengths = malloc(sizeof(size_t) * count);
    _ccl_memcpy(lengths, data, sizeof(size_t) * count, &offset_total);
    data += sizeof(size_t) * count;

    char **strings = malloc(sizeof(char *) * count);

    for (int s = 0; s < count; s++)
    {
        (strings)[s] = (char *)malloc(sizeof(char) * lengths[s]);
        _ccl_memcpy((strings)[s], data, lengths[s], &offset_total);
        data += lengths[s];
    }
    data -= offset_total;
    offset_total = 0;

    result = clCreateProgramWithSource(context, count, (const char **)strings, lengths, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_program) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_program), &offset_total);
    buffer_data_reply += sizeof(cl_program);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    free(lengths);

    for (int s = 0; s < count; s++)
        free((strings)[s]);
    free(strings);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateProgramWithSource");
    free(buffer_data_reply);
}

void _cclBuildProgram(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;
    cl_device_id *device_list = NULL;

    struct
    {
        cl_program program;
        cl_uint num_devices;
    } ccl_request;

    struct
    {
        cl_int result;
    } ccl_reply;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&ccl_request, data, sizeof(ccl_request), &offset_total);
    data += sizeof(ccl_request);

    int ss = 0;
    _ccl_memcpy(&ss, data, sizeof(int), &offset_total);
    data += sizeof(int);

    if (ccl_request.num_devices > 0)
    {
        device_list = malloc(sizeof(cl_device_id) * ccl_request.num_devices);
        _ccl_memcpy(device_list, data, sizeof(cl_device_id) * ccl_request.num_devices, &offset_total);
        data += sizeof(cl_device_id) * ccl_request.num_devices;
    }

    char *options = NULL;
    if (ss > 0)
    {
        options = malloc(ss);
        _ccl_memcpy(options, data, ss, &offset_total);
        data += ss;
    }
    data -= offset_total;
    offset_total = 0;

    ccl_reply.result = clBuildProgram(ccl_request.program, ccl_request.num_devices, device_list, options, NULL, NULL);

    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclBuildProgram");

    free(buffer_data_reply);

    if (ccl_request.num_devices > 0)
        free(device_list);

    if (ss > 0)
        free(options);
}

void _cclCreateKernel(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    int ss = 0;
    _ccl_memcpy(&ss, data, sizeof(int), &offset_total);
    data += sizeof(int);

    cl_program program;
    _ccl_memcpy(&program, data, sizeof(cl_program), &offset_total);
    data += sizeof(cl_program);

    char *kernel_name = NULL;
    if (ss > 0)
    {
        kernel_name = malloc(ss);
        _ccl_memcpy(kernel_name, data, ss, &offset_total);
        data += ss;
        // kernel_name[ss]='\0';
    }
    data -= offset_total;
    struct
    {
        cl_kernel result;
        cl_int errcode_ret;
    } ccl_reply;

    ccl_reply.result = clCreateKernel(program, kernel_name, &ccl_reply.errcode_ret);

    offset_total=0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateKernel");

    free(buffer_data_reply);

    if (ss > 0)
        free(kernel_name);
}

void _cclSetKernelArg(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_kernel kernel;
        cl_uint arg_index;
        size_t arg_size;
        char arg_value_is_null;
        cl_mem ptr;
        char is_ptr;
    } ccl_request;

    struct
    {
        cl_int result;
    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    void *arg_value = NULL;
    if (ccl_request.arg_value_is_null == '0')
    {
        if (ccl_request.is_ptr == '0')
        {
            arg_value = malloc(ccl_request.arg_size);
            memcpy(arg_value, data + sizeof(ccl_request), ccl_request.arg_size);
        }
        else
        {
            arg_value = (void *)&ccl_request.ptr;
        }
    }

    pthread_mutex_lock(&mutex_ts);
    ccl_reply.result = clSetKernelArg(ccl_request.kernel, ccl_request.arg_index, ccl_request.arg_size, ccl_request.arg_value_is_null == '1' ? NULL : arg_value);
    pthread_mutex_unlock(&mutex_ts);

    if (ccl_request.is_ptr == '0' && ccl_request.arg_size > 0)
        free(arg_value);

    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclSetKernelArg");
    free(buffer_data_reply);
}

void _cclEnqueueWriteBuffer(void *_request)
{
    size_t size_buffer_reply = 0, offset_total = 0;
    int fd_tcp_client = 0;
    void *data = NULL, *ptr = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    cl_event *event_wait_list = NULL;

    struct
    {
        cl_event event;
        cl_int result;
    } ccl_reply;

    struct
    {
        cl_command_queue command_queue;
        cl_mem buffer;
        cl_bool blocking_write;
        size_t offset;
        size_t size;
        cl_uint num_events_in_wait_list;
        char event_is_null;
    } ccl_request;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    int aux = 0;
    if (ccl_request.num_events_in_wait_list > 0)
    {
        aux = ccl_request.num_events_in_wait_list * sizeof(cl_event);
        event_wait_list = (data + sizeof(ccl_request));
    }

    if (ccl_request.size > 0)
    {
        ptr = malloc(ccl_request.size);
        memcpy(ptr, data + sizeof(ccl_request) + aux, ccl_request.size);
    }

    ccl_reply.result = clEnqueueWriteBuffer(ccl_request.command_queue, ccl_request.buffer, ccl_request.blocking_write, ccl_request.offset, ccl_request.size, ptr, ccl_request.num_events_in_wait_list, (ccl_request.num_events_in_wait_list == 0) ? NULL : event_wait_list, ccl_request.event_is_null ? NULL : &ccl_reply.event);

    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueWriteBuffer");

    free(buffer_data_reply);
}

void _cclEnqueueNDRangeKernel(void *_request)
{
    size_t size_buffer_reply = 0, offset_total = 0;
    void *data = NULL;
    int fd_tcp_client = 0;
    
    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_event event;
        cl_int result;
    } ccl_reply;

    struct
    {
        cl_command_queue command_queue;
        cl_kernel kernel;
        cl_uint work_dim;
        cl_uint num_events_in_wait_list;
        char event_is_null;
        char local_work_size_is_null;
        char global_work_offset_is_null;
    } ccl_request;

    _ccl_memcpy(&ccl_request, data, sizeof(ccl_request), &offset_total);
    data += sizeof(ccl_request);

    size_t *global_work_offset = NULL;
    if (!ccl_request.global_work_offset_is_null)
    {
        global_work_offset = malloc(sizeof(size_t) * ccl_request.work_dim);
        _ccl_memcpy(global_work_offset, data, sizeof(size_t) * ccl_request.work_dim, &offset_total);
        data += sizeof(size_t) * ccl_request.work_dim;
        // global_work_offset = data;
        // data += sizeof (size_t) * ccl_request.work_dim;
    }
    size_t *global_work_size = malloc(sizeof(size_t) * ccl_request.work_dim);
    _ccl_memcpy(global_work_size, data, sizeof(size_t) * ccl_request.work_dim, &offset_total);
    data += sizeof(size_t) * ccl_request.work_dim;

    // size_t *global_work_size =data;
    // data += sizeof (size_t) * ccl_request.work_dim;

    size_t *local_work_size = NULL;

    if (!ccl_request.local_work_size_is_null)
    {
        local_work_size = malloc(sizeof(size_t) * ccl_request.work_dim);
        _ccl_memcpy(local_work_size, data, sizeof(size_t) * ccl_request.work_dim, &offset_total);
        data += sizeof(size_t) * ccl_request.work_dim;
        // local_work_size = data;
        // data+= sizeof (size_t) * ccl_request.work_dim;
    }

    cl_event *event_wait_list = NULL;

    if (ccl_request.num_events_in_wait_list > 0)
    {

        event_wait_list = malloc(sizeof(cl_event) * ccl_request.num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * ccl_request.num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * ccl_request.num_events_in_wait_list;
        // event_wait_list = data;
    }

    data -= offset_total;
    offset_total = 0;
    ccl_reply.result = clEnqueueNDRangeKernel(ccl_request.command_queue, ccl_request.kernel, ccl_request.work_dim, ccl_request.global_work_offset_is_null ? NULL : global_work_offset, global_work_size, ccl_request.local_work_size_is_null ? NULL : local_work_size, ccl_request.num_events_in_wait_list, (ccl_request.num_events_in_wait_list == 0) ? NULL : event_wait_list, ccl_request.event_is_null ? NULL : &ccl_reply.event);

    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueNDRangeKernel");

    free(buffer_data_reply);

    if (ccl_request.num_events_in_wait_list > 0)
        free(event_wait_list);

    if (!ccl_request.global_work_offset_is_null)
        free(global_work_offset);

    if (!ccl_request.local_work_size_is_null)
        free(local_work_size);
}

void _cclEnqueueReadBuffer(void *_request)
{
    void *ptr = NULL, *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;
    struct
    {
        cl_event event;
        cl_int result;
    } ccl_reply;

    struct
    {
        cl_command_queue command_queue;
        cl_mem buffer;
        cl_bool blocking_read;
        size_t offset;
        size_t size;
        cl_uint num_events_in_wait_list;
        char event_is_null;
    } ccl_request;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&ccl_request, data, sizeof(ccl_request), &offset_total);
    data += sizeof(ccl_request);

    cl_event *event_wait_list = NULL;
    if (ccl_request.num_events_in_wait_list > 0)
    {
        event_wait_list = malloc(sizeof(cl_event) * ccl_request.num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * ccl_request.num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * ccl_request.num_events_in_wait_list;
    }

    data -= offset_total;
    offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply) + sizeof(int);
    if (ccl_request.size > 0)
    {
        ptr = malloc(ccl_request.size);
        size_buffer_reply += ccl_request.size;
    }

    ccl_reply.result = clEnqueueReadBuffer(ccl_request.command_queue, ccl_request.buffer, ccl_request.blocking_read, ccl_request.offset, ccl_request.size, ptr, ccl_request.num_events_in_wait_list, (ccl_request.num_events_in_wait_list == 0) ? NULL : event_wait_list, ccl_request.event_is_null ? NULL : &ccl_reply.event);

    buffer_data_reply = malloc(size_buffer_reply);
    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    if (ccl_request.size > 0)
    {
        _ccl_memcpy(buffer_data_reply, ptr, ccl_request.size, &offset_total);
        buffer_data_reply += ccl_request.size;
    }

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueReadBuffer");

    free(buffer_data_reply);

    if (ccl_request.num_events_in_wait_list > 0)
        free(event_wait_list);

    if (ccl_request.size > 0)
        free(ptr);
}

void _cclGetEventProfilingInfo(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    cl_int result;
    size_t param_value_size_ret;
    char *param_value = NULL;
    cl_event event;
    cl_profiling_info param_name;
    size_t param_value_size;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&event, data, sizeof(cl_event), &offset_total);
    data += sizeof(cl_event);

    _ccl_memcpy(&param_name, data, sizeof(cl_profiling_info), &offset_total);
    data += sizeof(cl_profiling_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    param_value = malloc(param_value_size);
    data -= offset_total;
    offset_total = 0;
    result = clGetEventProfilingInfo(event, param_name, param_value_size, param_value, &param_value_size_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t) + param_value_size_ret;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
    buffer_data_reply += param_value_size_ret;

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetEventProfilingInfo");

    free(buffer_data_reply);

    free(param_value);
}

void _cclCreateImage(void *_request)
{

    cl_context context;
    cl_mem_flags flags;
    cl_image_format image_format;
    cl_image_desc image_desc;
    cl_mem result;
    cl_int errcode_ret;
    size_t size_htr = 0;
    void *buffer_data_reply = NULL, *host_ptr = NULL, *data = NULL;

    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&size_htr, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    _ccl_memcpy(&flags, data, sizeof(cl_mem_flags), &offset_total);
    data += sizeof(cl_mem_flags);

    _ccl_memcpy(&image_format, data, sizeof(cl_image_format), &offset_total);
    data += sizeof(cl_image_format);

    _ccl_memcpy(&image_desc, data, sizeof(cl_image_desc), &offset_total);
    data += sizeof(cl_image_desc);

    if (size_htr > 0)
    {
        host_ptr = malloc(size_htr);
        memcpy(host_ptr, data, size_htr);
        data += size_htr;
    }

    data -= offset_total;
    offset_total = 0;
    result = clCreateImage(context, flags, &image_format, &image_desc, host_ptr, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_mem) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_mem), &offset_total);
    buffer_data_reply += sizeof(cl_mem);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    if (size_htr > 0)
        free(host_ptr);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateImage");

    free(buffer_data_reply);
}

void _cclEnqueueReadImage(void *_request)
{

    cl_command_queue command_queue;
    cl_mem image;
    cl_bool blocking_read;
    size_t origin[3];
    size_t region[3];
    size_t row_pitch;
    size_t slice_pitch;
    cl_uint num_events_in_wait_list;
    cl_event *event_wait_list = NULL;
    char event_is_null;
    cl_int result;
    cl_event event;

    void *ptr = NULL;
    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&image, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&blocking_read, data, sizeof(cl_bool), &offset_total);
    data += sizeof(cl_bool);

    _ccl_memcpy(origin, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(region, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(&row_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&slice_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&num_events_in_wait_list, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_events_in_wait_list > 0)
    {
        event_wait_list = malloc(sizeof(cl_event) * num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * num_events_in_wait_list;
    }

    _ccl_memcpy(&event_is_null, data, sizeof(char), &offset_total);
    data += sizeof(char);

    data -= offset_total;
    offset_total = 0;
    ptr = malloc(sizeof(float) * region[0] * region[1]);

    result = clEnqueueReadImage(command_queue, image, /*blocking_read*/ CL_TRUE, origin, region, row_pitch, slice_pitch, ptr, num_events_in_wait_list, (num_events_in_wait_list == 0) ? NULL : event_wait_list, event_is_null ? NULL : &event);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event) + sizeof(float) * region[0] * region[1];
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    _ccl_memcpy(buffer_data_reply, ptr, sizeof(float) * region[0] * region[1], &offset_total);
    buffer_data_reply += sizeof(float) * region[0] * region[1];

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueReadImage");
    free(buffer_data_reply);

    if (num_events_in_wait_list > 0)
        free(event_wait_list);

    free(ptr);
}

void _cclEnqueueWriteImage(void *_request)
{

    cl_command_queue command_queue;
    cl_mem image;
    cl_bool blocking_write;
    size_t origin[3];
    size_t region[3];
    size_t input_row_pitch;
    size_t input_slice_pitch;
    cl_uint num_events_in_wait_list;
    cl_event *event_wait_list = NULL;
    char event_is_null;

    cl_int result;
    cl_event event;

    void *buffer_data_reply = NULL, *ptr = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&image, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&blocking_write, data, sizeof(cl_bool), &offset_total);
    data += sizeof(cl_bool);

    _ccl_memcpy(origin, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(region, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(&input_row_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&input_slice_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&num_events_in_wait_list, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_events_in_wait_list > 0)
    {
        event_wait_list = malloc(sizeof(cl_event) * num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * num_events_in_wait_list;
    }

    _ccl_memcpy(&event_is_null, data, sizeof(char), &offset_total);
    data += sizeof(char);

    if (8 * region[0] * region[1] > 0)
    {
        ptr = malloc(8 * region[0] * region[1]);
        _ccl_memcpy(ptr, data, 8 * region[0] * region[1], &offset_total);
        data += 8 * region[0] * region[1];
    }
    data -= offset_total;
    offset_total = 0;

    result = clEnqueueWriteImage(command_queue, image, /*blocking_write*/ CL_TRUE, origin, region, input_row_pitch, input_slice_pitch, ptr, num_events_in_wait_list, (num_events_in_wait_list == 0) ? NULL : event_wait_list, event_is_null ? NULL : &event);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);
    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueWriteImage");
    free(buffer_data_reply);
    if (num_events_in_wait_list > 0)
        free(event_wait_list);
    free(ptr);
}

void _cclEnqueueFillImage(void *_request)
{
    cl_command_queue command_queue;
    cl_mem image;
    size_t origin[3];
    size_t region[3];
    cl_uint num_events_in_wait_list;
    cl_event *event_wait_list = NULL;
    void *fill_color = malloc(FILL_COLOR), *buffer_data_reply = NULL, *data = NULL;
    char event_is_null;
    cl_int result;
    cl_event event;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&image, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(fill_color, data, FILL_COLOR, &offset_total);
    data += FILL_COLOR;

    _ccl_memcpy(origin, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(region, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(&num_events_in_wait_list, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_events_in_wait_list > 0)
    {
        event_wait_list = malloc(sizeof(cl_event) * num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * num_events_in_wait_list;
    }

    _ccl_memcpy(&event_is_null, data, sizeof(char), &offset_total);
    data += sizeof(char);

    data -= offset_total;
    offset_total = 0;

    result = clEnqueueFillImage(command_queue, image, fill_color, origin, region, num_events_in_wait_list, (num_events_in_wait_list == 0) ? NULL : event_wait_list, event_is_null ? NULL : &event);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueFillImage");
    free(buffer_data_reply);

    free(fill_color);
}

void _cclEnqueueCopyImage(void *_request)
{

    // not implementated yet
}

void _cclEnqueueCopyImageToBuffer(void *_request)
{

    // not implementated yet
}

void _cclEnqueueCopyBufferToImage(void *_request)
{

    // not implementated yet
}

void _cclEnqueueMapImage(void *_request)
{

    cl_command_queue command_queue;
    cl_mem image;
    cl_bool blocking_map;
    cl_map_flags map_flags;
    size_t origin[3];
    size_t region[3];
    size_t image_row_pitch;
    size_t image_slice_pitch;
    cl_uint num_events_in_wait_list;
    cl_event *event_wait_list = NULL;
    char event_is_null;
    void *result = NULL, *data = NULL, *buffer_data_reply = NULL;
    cl_event event;
    cl_int errcode_ret;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&image, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&blocking_map, data, sizeof(cl_bool), &offset_total);
    data += sizeof(cl_bool);

    _ccl_memcpy(&map_flags, data, sizeof(cl_map_flags), &offset_total);
    data += sizeof(cl_map_flags);

    _ccl_memcpy(origin, data, 3 * sizeof(size_t), &offset_total);
    data += 3 * sizeof(size_t);

    _ccl_memcpy(region, data, 3 * sizeof(size_t), &offset_total);
    data += 3 * sizeof(size_t);

    _ccl_memcpy(&image_row_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&image_slice_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&num_events_in_wait_list, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_events_in_wait_list > 0)
    {
        event_wait_list = malloc(sizeof(cl_event) * num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * num_events_in_wait_list;
    }

    _ccl_memcpy(&event_is_null, data, sizeof(char), &offset_total);
    data += sizeof(char);

    data -= offset_total;
    offset_total = 0;

    result = clEnqueueMapImage(command_queue, image, blocking_map, map_flags, origin, region, &image_row_pitch, &image_slice_pitch, num_events_in_wait_list, (num_events_in_wait_list == 0) ? NULL : event_wait_list, event_is_null ? NULL : &event, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event) + sizeof(void *);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, result, sizeof(void *), &offset_total);
    buffer_data_reply += sizeof(void *);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueMapImage");
    free(buffer_data_reply);

    if (num_events_in_wait_list > 0)
        free(event_wait_list);
}

void _cclCreateSubBuffer(void *_request)
{

    cl_mem buffer;
    cl_mem_flags flags;
    cl_buffer_create_type buffer_create_type;
    cl_buffer_region buffer_create_info;
    cl_mem result;
    cl_int errcode_ret;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;

    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&buffer, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&flags, data, sizeof(cl_map_flags), &offset_total);
    data += sizeof(cl_map_flags);

    _ccl_memcpy(&buffer_create_type, data, sizeof(cl_buffer_create_type), &offset_total);
    data += sizeof(cl_buffer_create_type);

    _ccl_memcpy(&buffer_create_info, data, sizeof(cl_buffer_region), &offset_total);
    data += sizeof(cl_buffer_region);

    data -= offset_total;
    offset_total = 0;

    result = clCreateSubBuffer(buffer, flags, buffer_create_type, (void *)&buffer_create_info, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_mem);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_mem), &offset_total);
    buffer_data_reply += sizeof(cl_mem);
    buffer_data_reply -= offset_total;

    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateSubBuffer");
    free(buffer_data_reply);
}

void _cclEnqueueReadBufferRect(void *_request)
{

    cl_command_queue command_queue;
    cl_mem buffer;
    cl_bool blocking_read;
    size_t buffer_origin[3];
    size_t host_origin[3];
    size_t region[3];
    size_t buffer_row_pitch;
    size_t buffer_slice_pitch;
    size_t host_row_pitch;
    size_t host_slice_pitch;
    cl_uint num_events_in_wait_list;
    cl_event *event_wait_list = NULL;
    char event_is_null;
    cl_int result;
    cl_event event;
    void *ptr = NULL, *buffer_data_reply = NULL, *data = NULL;
    int size_ptr = 0, x = 0, y = 0, fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&buffer, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&blocking_read, data, sizeof(cl_bool), &offset_total);
    data += sizeof(cl_bool);

    _ccl_memcpy(buffer_origin, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(host_origin, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(region, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(&buffer_row_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&buffer_slice_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&host_row_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&host_slice_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&num_events_in_wait_list, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_events_in_wait_list > 0)
    {
        event_wait_list = malloc(sizeof(cl_event) * num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * num_events_in_wait_list;
    }

    _ccl_memcpy(&event_is_null, data, sizeof(char), &offset_total);
    data += sizeof(char);

    data -= offset_total;
    offset_total = 0;

    if (host_row_pitch != 0)
        x = host_row_pitch;
    else
        x = region[0];

    if (host_slice_pitch != 0)
        y = host_slice_pitch;
    else
        y = region[1];
    size_ptr = x * y;
    ptr = malloc(size_ptr);

    result = clEnqueueReadBufferRect(command_queue, buffer, /*blocking_read*/ CL_TRUE, buffer_origin, host_origin, region, buffer_row_pitch, buffer_slice_pitch, host_row_pitch, host_slice_pitch, ptr, num_events_in_wait_list, (num_events_in_wait_list == 0) ? NULL : event_wait_list, event_is_null ? NULL : &event);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event) + size_ptr;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    _ccl_memcpy(buffer_data_reply, ptr, size_ptr, &offset_total);
    buffer_data_reply += size_ptr;

    buffer_data_reply -= offset_total;

    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueReadBufferRect");
    free(buffer_data_reply);

    if (num_events_in_wait_list > 0)
        free(event_wait_list);
    free(ptr);
}

void _cclEnqueueWriteBufferRect(void *_request)
{
    cl_command_queue command_queue;
    cl_mem buffer;
    cl_bool blocking_write;
    size_t buffer_origin[3];
    size_t host_origin[3];
    size_t region[3];
    size_t buffer_row_pitch;
    size_t buffer_slice_pitch;
    size_t host_row_pitch;
    size_t host_slice_pitch;
    cl_uint num_events_in_wait_list;
    cl_event *event_wait_list = NULL;
    char event_is_null;
    cl_int result;
    cl_event event;
    void *ptr = NULL, *buffer_data_reply = NULL, *data = NULL;
    int size_ptr = 0, x = 0, y = 0, fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&buffer, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&blocking_write, data, sizeof(cl_bool), &offset_total);
    data += sizeof(cl_bool);

    _ccl_memcpy(buffer_origin, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(host_origin, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(region, data, sizeof(size_t) * 3, &offset_total);
    data += sizeof(size_t) * 3;

    _ccl_memcpy(&buffer_row_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&buffer_slice_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&host_row_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&host_slice_pitch, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&num_events_in_wait_list, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_events_in_wait_list > 0)
    {
        event_wait_list = malloc(sizeof(cl_event) * num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * num_events_in_wait_list;
    }

    _ccl_memcpy(&event_is_null, data, sizeof(char), &offset_total);
    data += sizeof(char);

    if (host_row_pitch != 0)
        x = host_row_pitch;
    else
        x = region[0];

    if (host_slice_pitch != 0)
        y = host_slice_pitch;
    else
        y = region[1];
    size_ptr = x * y;

    if (size_ptr > 0)
    {

        ptr = malloc(size_ptr);
        _ccl_memcpy(ptr, data, size_ptr, &offset_total);
        data += size_ptr;
    }

    data -= offset_total;
    offset_total = 0;

    result = clEnqueueWriteBufferRect(command_queue, buffer, /*blocking_write*/ CL_TRUE, buffer_origin, host_origin, region, buffer_row_pitch, buffer_slice_pitch, host_row_pitch, host_slice_pitch, ptr, num_events_in_wait_list, (num_events_in_wait_list == 0) ? NULL : event_wait_list, event_is_null ? NULL : &event);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    buffer_data_reply -= offset_total;

    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueWriteBufferRect");
    free(buffer_data_reply);
    if (num_events_in_wait_list > 0)
        free(event_wait_list);

    if (size_ptr > 0)
        free(ptr);
}

void _cclEnqueueFillBuffer(void *_request)
{

    cl_command_queue command_queue;
    cl_mem buffer;
    size_t pattern_size;
    size_t offset;
    size_t size;
    cl_uint num_events_in_wait_list;
    cl_event *event_wait_list = NULL;
    char event_is_null;

    void *buffer_data_reply = NULL, *ptr = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    cl_int result;
    cl_event event;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&buffer, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&pattern_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&offset, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    _ccl_memcpy(&num_events_in_wait_list, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_events_in_wait_list > 0)
    {
        event_wait_list = malloc(sizeof(cl_event) * num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * num_events_in_wait_list;
    }

    _ccl_memcpy(&event_is_null, data, sizeof(char), &offset_total);
    data += sizeof(char);

    if (pattern_size > 0)
    {
        ptr = malloc(pattern_size);
        _ccl_memcpy(ptr, data, pattern_size, &offset_total);
        data += pattern_size;
    }

    data -= offset_total;
    offset_total = 0;

    result = clEnqueueFillBuffer(command_queue, buffer, ptr, pattern_size, offset, size, num_events_in_wait_list, (num_events_in_wait_list == 0) ? NULL : event_wait_list, event_is_null ? NULL : &event);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueFillBuffer");
    free(buffer_data_reply);

    if (num_events_in_wait_list > 0)
        free(event_wait_list);
    if (pattern_size > 0)
        free(ptr);
}

void _cclEnqueueCopyBuffer(void *_request)
{

    int fd_tcp_client = 0;
    void *data = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    cl_event *event_wait_list = NULL;

    struct
    {
        cl_event event;
        cl_int result;

    } ccl_reply;

    struct
    {
        cl_command_queue command_queue;
        cl_mem src_buffer;
        cl_mem dst_buffer;
        size_t src_offset;
        size_t dst_offset;
        size_t size;
        cl_uint num_events_in_wait_list;
        char event_is_null;

    } ccl_request;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    if (ccl_request.num_events_in_wait_list > 0)
    {
        event_wait_list = data + sizeof(ccl_request);
    }

    ccl_reply.result = clEnqueueCopyBuffer(ccl_request.command_queue, ccl_request.src_buffer, ccl_request.dst_buffer, ccl_request.src_offset, ccl_request.dst_offset, ccl_request.size, ccl_request.num_events_in_wait_list, (ccl_request.num_events_in_wait_list == 0) ? NULL : event_wait_list, ccl_request.event_is_null ? NULL : &ccl_reply.event);

    size_t size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply), offset_total = 0;
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueCopyBuffer");
    free(buffer_data_reply);
}

void _cclEnqueueCopyBufferRect(void *_request)
{

    // Not implemented yet
}

void _cclEnqueueMapBuffer(void *_request)
{
    void *result = NULL, *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_command_queue command_queue;
        cl_mem buffer;
        cl_bool blocking_map;
        cl_map_flags map_flags;
        size_t offset;
        size_t cb;
        cl_uint num_events_in_wait_list;
        char event_is_null;
    } ccl_request;

    struct
    {
        cl_event event;
        cl_int errcode_ret;
        uintptr_t ptr_remote;
    } ccl_reply;

    cl_event *event_wait_list = NULL;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    if (ccl_request.num_events_in_wait_list > 0)
    {
        event_wait_list = data += sizeof(ccl_request);
    }

    result = clEnqueueMapBuffer(ccl_request.command_queue, ccl_request.buffer, ccl_request.blocking_map, ccl_request.map_flags, ccl_request.offset, ccl_request.cb, ccl_request.num_events_in_wait_list, (ccl_request.num_events_in_wait_list == 0) ? NULL : event_wait_list, ccl_request.event_is_null ? NULL : &ccl_reply.event, &ccl_reply.errcode_ret);

    ccl_reply.ptr_remote = (uintptr_t)result;

    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply) + ccl_request.cb;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    _ccl_memcpy(buffer_data_reply, result, ccl_request.cb, &offset_total);
    buffer_data_reply += ccl_request.cb;

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueMapBuffer");
    free(buffer_data_reply);
}

void _cclCreateSubDevices(void *_request)
{

    cl_device_id in_device;
    cl_device_partition_property *properties = NULL;
    cl_uint num_devices, num_devices_ret;
    cl_int result;
    cl_device_id *out_devices = NULL;

    void *buffer_data_reply = NULL, *data = NULL;
    int num_properties = 0, fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&num_properties, data, sizeof(int), &offset_total);
    data += sizeof(int);

    _ccl_memcpy(&num_devices, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_devices > 0)
        out_devices = malloc(sizeof(cl_device_id) * num_devices);

    if (num_properties > 0)
    {

        properties = malloc(sizeof(cl_device_partition_property) * num_properties);
        _ccl_memcpy(properties, data, sizeof(cl_device_partition_property) * num_properties, &offset_total);
        data += sizeof(cl_device_partition_property) * num_properties;
    }

    _ccl_memcpy(&in_device, data, sizeof(cl_device_id), &offset_total);
    data += sizeof(cl_device_id);
    data -= offset_total;

    offset_total = 0;

    result = clCreateSubDevices(in_device, properties, num_devices, out_devices, &num_devices_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_uint) + sizeof(cl_device_id) * num_devices_ret;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &num_devices_ret, sizeof(cl_uint), &offset_total);
    buffer_data_reply += sizeof(cl_uint);

    _ccl_memcpy(buffer_data_reply, out_devices, sizeof(cl_device_id) * num_devices_ret, &offset_total);
    buffer_data_reply += sizeof(cl_device_id) * num_devices_ret;

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateSubDevices");
    free(buffer_data_reply);

    if (num_properties > 0)
        free(properties);

    if (num_devices > 0)
        free(out_devices);
}

void _cclRetainDevice(void *_request)
{
    void *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    struct
    {
        cl_device_id device;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    ccl_reply.errcode_ret = clRetainDevice(ccl_request.device);

    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclRetainDevice");
    free(buffer_data_reply);
}

void _cclReleaseDevice(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_device_id device;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));
    ccl_reply.errcode_ret = clReleaseDevice(ccl_request.device);

    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclReleaseDevice");
    free(buffer_data_reply);
}

void _cclCreateContextFromType(void *_request)
{
    void *buffer_data_reply = NULL, *data = NULL;
    char *user_data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;
    cl_context result;
    cl_int errcode_ret;
    cl_context_properties *properties = NULL;
    cl_device_type device_type;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    int size_properties = 0;
    _ccl_memcpy(&size_properties, data, sizeof(int), &offset_total);
    data += sizeof(int);

    if (size_properties > 0)
    {
        properties = malloc(sizeof(cl_context_properties) * size_properties);
        _ccl_memcpy(properties, data, sizeof(cl_context_properties) * size_properties, &offset_total);
        data += sizeof(cl_context_properties) * size_properties;
    }

    _ccl_memcpy(&device_type, data, sizeof(cl_device_type), &offset_total);
    data += sizeof(cl_device_type);

    int size_user_data = 0;

    _ccl_memcpy(&size_user_data, data, sizeof(int), &offset_total);
    data += sizeof(int);

    if (size_user_data > 0)
    {
        user_data = malloc(size_user_data + 1);
        _ccl_memcpy(user_data, data, size_user_data, &offset_total);
        data += size_user_data;
        int len = strlen(user_data);
        user_data[len + 1] = '\0';
    }

    data -= offset_total;
    offset_total = 0;

    result = clCreateContextFromType(properties, device_type, NULL, user_data, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_context) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_context), &offset_total);
    buffer_data_reply += sizeof(cl_context);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    if (size_properties > 0)
        free(properties);

    if (size_user_data > 0)
        free(user_data);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateContextFromType");
    free(buffer_data_reply);
}

void _cclRetainContext(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    struct
    {
        cl_context context;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    ccl_reply.errcode_ret = clRetainContext(ccl_request.context);

    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclRetainContext");
    free(buffer_data_reply);
}

void _cclReleaseContext(void *_request)
{
    void *data = NULL;
    int fd_tcp_client = 0;

    struct
    {
        cl_context context;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    ccl_reply.errcode_ret = clReleaseContext(ccl_request.context);
    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclReleaseContext");
    free(buffer_data_reply);
}

void _cclGetContextInfo(void *_request)
{
    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    void *param_value = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {

        cl_context context;
        cl_context_info param_name;
        size_t param_value_size;

    } ccl_request;

    struct
    {
        cl_int result;
        size_t param_value_size_ret;
    } ccl_reply = {.param_value_size_ret = 0};

    memcpy(&ccl_request, data, sizeof(ccl_request));

    if (ccl_request.param_value_size > 0)
        param_value = malloc(ccl_request.param_value_size);

    ccl_reply.result = clGetContextInfo(ccl_request.context, ccl_request.param_name, ccl_request.param_value_size, param_value, &ccl_reply.param_value_size_ret);

    size_buffer_reply = sizeof(size_t)+sizeof(ccl_reply) + ccl_reply.param_value_size_ret;

    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    if (ccl_reply.result == CL_SUCCESS && ccl_request.param_value_size > 0)
    {
        _ccl_memcpy(buffer_data_reply, param_value, ccl_reply.param_value_size_ret, &offset_total);
        buffer_data_reply += ccl_reply.param_value_size_ret;
        free(param_value);
    }

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetContextInfo");
    free(buffer_data_reply);
}

void _cclGetExtensionFunctionAddressForPlatform(void *_request)
{
    cl_platform_id platform;
    char *funcname = NULL;
    void *buffer_data_reply = NULL, *result = malloc(sizeof(void *)), *data = NULL;
    int size = 0, fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&platform, data, sizeof(cl_platform_id), &offset_total);
    data += sizeof(cl_platform_id);

    _ccl_memcpy(&size, data, sizeof(int), &offset_total);
    data += sizeof(int);

    if (size > 0)
    {
        funcname = malloc(size + 1);
        _ccl_memcpy(funcname, data, size, &offset_total);
        data += size;
    }

    data -= offset_total;
    offset_total = 0;

    result = clGetExtensionFunctionAddressForPlatform(platform, funcname);

    size_buffer_reply = sizeof(size_t) + sizeof(void *);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, result, sizeof(void *), &offset_total);
    buffer_data_reply += sizeof(void *);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetExtensionFunctionAddressForPlatform");
    free(buffer_data_reply);

    if (size > 0)
        free(funcname);
}

void _cclRetainCommandQueue(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_command_queue command_queue;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));
    ccl_reply.errcode_ret = clRetainCommandQueue(ccl_request.command_queue);

    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclRetainCommandQueue");
    free(buffer_data_reply);
}

void _cclReleaseCommandQueue(void *_request)
{
    void *data = NULL;
    int fd_tcp_client = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_command_queue command_queue;

    } ccl_request;

    struct
    {
        cl_int errcode_ret;

    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));
    ccl_reply.errcode_ret = clReleaseCommandQueue(ccl_request.command_queue);

    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclReleaseCommandQueue");
    free(buffer_data_reply);
}

void _cclGetCommandQueueInfo(void *_request)
{

    cl_command_queue command_queue;
    cl_command_queue_info param_name;
    size_t param_value_size;
    cl_int result;
    size_t param_value_size_ret;
    char *param_value = NULL;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;

    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&param_name, data, sizeof(cl_command_queue_info), &offset_total);
    data += sizeof(cl_command_queue_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;
    offset_total = 0;

    param_value = malloc(param_value_size + 1);
    result = clGetCommandQueueInfo(command_queue, param_name, param_value_size, param_value, &param_value_size_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t) + param_value_size_ret;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
    buffer_data_reply += param_value_size_ret;

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetCommandQueueInfo");
    free(buffer_data_reply);
    free(param_value);
}

void _cclGetSupportedImageFormats(void *_request)
{

    cl_context context;
    cl_mem_flags flags;
    cl_mem_object_type image_type;
    cl_uint num_entries;
    cl_int result;
    cl_image_format *image_formats = NULL;
    cl_uint num_image_formats;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    _ccl_memcpy(&flags, data, sizeof(cl_mem_flags), &offset_total);
    data += sizeof(cl_mem_flags);

    _ccl_memcpy(&image_type, data, sizeof(cl_mem_object_type), &offset_total);
    data += sizeof(cl_mem_object_type);

    _ccl_memcpy(&num_entries, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    data -= offset_total;
    offset_total = 0;

    if (num_entries > 0)
        image_formats = malloc(sizeof(cl_image_format) * num_entries);

    result = clGetSupportedImageFormats(context, flags, image_type, num_entries, image_formats, &num_image_formats);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_uint) + sizeof(cl_image_format) * num_image_formats;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &num_image_formats, sizeof(cl_uint), &offset_total);
    buffer_data_reply += sizeof(cl_uint);

    if (num_entries > 0)
    {
        _ccl_memcpy(buffer_data_reply, image_formats, sizeof(cl_image_format) * num_image_formats, &offset_total);
        buffer_data_reply += sizeof(cl_image_format) * num_image_formats;
    }
    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetSupportedImageFormats");
    free(buffer_data_reply);

    if (num_entries > 0)
        free(image_formats);
}

void _cclGetImageInfo(void *_request)
{
    cl_mem image;
    cl_image_info param_name;
    size_t param_value_size;
    cl_int result;
    size_t param_value_size_ret;
    char *param_value = NULL;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&image, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&param_name, data, sizeof(cl_image_info), &offset_total);
    data += sizeof(cl_image_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;
    offset_total = 0;

    param_value = malloc(param_value_size + 1);
    result = clGetImageInfo(image, param_name, param_value_size, param_value, &param_value_size_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t) + param_value_size_ret;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
    buffer_data_reply += param_value_size_ret;

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetImageInfo");
    free(buffer_data_reply);

    free(param_value);
}

void _cclFlush(void *_request)
{
    void *data = NULL;
    int fd_tcp_client = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_command_queue command_queue;

    } ccl_request;

    struct
    {
        cl_int errcode_ret;

    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    ccl_reply.errcode_ret = clFlush(ccl_request.command_queue);
    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclFlush");
    free(buffer_data_reply);
}

void _cclFinish(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_command_queue command_queue;

    } ccl_request;

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));
    ccl_reply.errcode_ret = clFinish(ccl_request.command_queue);
    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclFinish");
    free(buffer_data_reply);
}

void _cclRetainMemObject(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    struct
    {
        cl_mem memobj;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    ccl_reply.errcode_ret = clRetainMemObject(ccl_request.memobj);
    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclRetainMemObject");
    free(buffer_data_reply);
}

void _cclSetMemObjectDestructorCallback(void *_request)
{
    cl_mem memobj;
    int size_user = 0;
    cl_int errcode_ret;
    void *buffer_data_reply = NULL, *user_data = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&memobj, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&size_user, data, sizeof(int), &offset_total);
    data += sizeof(int);

    if (size_user > 0)
    {
        user_data = malloc(size_user);
        _ccl_memcpy(&size_user, data, size_user, &offset_total);
        data += size_user;
    }
    data -= offset_total;

    offset_total = 0;
    errcode_ret = clSetMemObjectDestructorCallback(memobj, NULL, user_data);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclSetMemObjectDestructorCallback");
    free(buffer_data_reply);
}

void _cclEnqueueUnmapMemObject(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;
    void *mapped_ptr = NULL;
    cl_event *event_wait_list = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_command_queue command_queue;
        cl_mem memobj;
        cl_uint num_events_in_wait_list;
        char event_is_null;
        uintptr_t str_pointer;
    } ccl_request;

    struct
    {
        cl_int result;
        cl_event event;
    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    mapped_ptr = (void *)ccl_request.str_pointer;

    if (ccl_request.num_events_in_wait_list > 0)
    {
        event_wait_list = data + sizeof(ccl_request);
    }

    ccl_reply.result = clEnqueueUnmapMemObject(ccl_request.command_queue, ccl_request.memobj, mapped_ptr, ccl_request.num_events_in_wait_list, (ccl_request.num_events_in_wait_list == 0) ? NULL : event_wait_list, ccl_request.event_is_null ? NULL : &ccl_reply.event);
    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueUnmapMemObject");
    free(buffer_data_reply);
}

void _cclReleaseMemObject(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_mem memobj;
        char isnull;
        uintptr_t ptr_remote;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    if (!ccl_request.isnull)
    {
        void *mapped_ptr = (void *)ccl_request.ptr_remote;
        free(mapped_ptr);
    }

    ccl_reply.errcode_ret = clReleaseMemObject(ccl_request.memobj);
    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclReleaseMemObject");
    free(buffer_data_reply);
}

void _cclCreateSampler(void *_request)
{

    cl_context context;
    cl_bool normalized_coords;
    cl_addressing_mode addressing_mode;
    cl_filter_mode filter_mode;
    cl_int errcode_ret;
    cl_sampler result;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    _ccl_memcpy(&normalized_coords, data, sizeof(cl_bool), &offset_total);
    data += sizeof(cl_bool);

    _ccl_memcpy(&addressing_mode, data, sizeof(cl_addressing_mode), &offset_total);
    data += sizeof(cl_addressing_mode);

    _ccl_memcpy(&filter_mode, data, sizeof(cl_filter_mode), &offset_total);
    data += sizeof(cl_filter_mode);

    data -= offset_total;
    offset_total = 0;

    result = clCreateSampler(context, normalized_coords, addressing_mode, filter_mode, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_sampler) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_sampler), &offset_total);
    buffer_data_reply += sizeof(cl_sampler);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateSampler");
    free(buffer_data_reply);
}

void _cclRetainSampler(void *_request)
{

    cl_sampler sampler;
    cl_int errcode_ret;
    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;
    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&sampler, data, sizeof(cl_sampler), &offset_total);
    data += sizeof(cl_sampler);

    data -= offset_total;
    offset_total = 0;
    errcode_ret = clRetainSampler(sampler);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclRetainSampler");
    free(buffer_data_reply);
}

void _cclReleaseSampler(void *_request)
{

    cl_sampler sampler;
    cl_int errcode_ret;
    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&sampler, data, sizeof(cl_sampler), &offset_total);
    data += sizeof(cl_sampler);
    data -= offset_total;

    offset_total = 0;
    errcode_ret = clReleaseSampler(sampler);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclReleaseSampler");
    free(buffer_data_reply);
}

void _cclGetSamplerInfo(void *_request)
{
    cl_sampler sampler;
    cl_sampler_info param_name;
    size_t param_value_size;
    cl_int result;
    size_t param_value_size_ret;
    char *param_value = NULL;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&sampler, data, sizeof(cl_sampler), &offset_total);
    data += sizeof(cl_sampler);

    _ccl_memcpy(&param_name, data, sizeof(cl_sampler_info), &offset_total);
    data += sizeof(cl_sampler_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;
    offset_total = 0;

    param_value = malloc(param_value_size);
    result = clGetSamplerInfo(sampler, param_name, param_value_size, param_value, &param_value_size_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t) + param_value_size_ret;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
    buffer_data_reply += param_value_size_ret;

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetSamplerInfo");
    free(buffer_data_reply);

    free(param_value);
}

void _cclCreateProgramWithBinary(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    cl_program result;
    cl_int errcode_ret;
    cl_int binary_status = -1;
    cl_context context;
    cl_uint num_devices;
    cl_device_id *device_list = NULL;
    size_t *lengths = NULL;
    unsigned char **binaries = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    _ccl_memcpy(&num_devices, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_devices > 0)
    {
        device_list = malloc(sizeof(cl_device_id) * num_devices);
        _ccl_memcpy(device_list, data, sizeof(cl_device_id) * num_devices, &offset_total);
        data += sizeof(cl_device_id) * num_devices;
    }

    lengths = malloc(sizeof(size_t) * num_devices);
    _ccl_memcpy(lengths, data, sizeof(size_t) * num_devices, &offset_total);
    data += sizeof(size_t) * num_devices;

    binaries = malloc(sizeof(char *) * num_devices);

    for (int s = 0; s < num_devices; s++)
    {
        (binaries)[s] = (unsigned char *)malloc(sizeof(char) * lengths[s]);
        _ccl_memcpy((binaries)[s], data, lengths[s], &offset_total);
        data += lengths[s];
    }
    data -= offset_total;
    offset_total = 0;

    result = clCreateProgramWithBinary(context, num_devices, device_list, lengths, (const unsigned char **)binaries, &binary_status, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_program) + sizeof(cl_int) + sizeof(int *);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_program), &offset_total);
    buffer_data_reply += sizeof(cl_program);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &binary_status, sizeof(int), &offset_total);
    buffer_data_reply += sizeof(int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateProgramWithBinary");
    free(buffer_data_reply);

    free(lengths);

    for (int s = 0; s < num_devices; s++)
        free((binaries)[s]);
    free(binaries);

    if (num_devices > 0)
        free(device_list);
}

void _cclCreateProgramWithBuiltInKernels(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    cl_program result;
    cl_int errcode_ret;
    cl_context context;
    cl_uint num_devices;
    cl_device_id *device_list = NULL;

    char *kernel_names = NULL;
    int size_kernel = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    _ccl_memcpy(&num_devices, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_devices > 0)
    {

        device_list = malloc(sizeof(cl_device_id) * num_devices);
        _ccl_memcpy(device_list, data, sizeof(cl_device_id) * num_devices, &offset_total);
        data += sizeof(cl_device_id) * num_devices;
    }

    _ccl_memcpy(&size_kernel, data, sizeof(int), &offset_total);
    data += sizeof(int);

    if (size_kernel > 0)
    {
        kernel_names = malloc(size_kernel);
        _ccl_memcpy(kernel_names, data, size_kernel, &offset_total);
        data += size_kernel;
    }
    data -= offset_total;
    offset_total = 0;

    result = clCreateProgramWithBuiltInKernels(context, num_devices, device_list, kernel_names, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_program) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_program), &offset_total);
    buffer_data_reply += sizeof(cl_program);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateProgramWithBuiltInKernels");
    free(buffer_data_reply);

    if (num_devices > 0)
        free(device_list);
    if (size_kernel > 0)
        free(kernel_names);
}

void _cclRetainProgram(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_int errcode_ret;

    } ccl_reply;

    struct
    {
        cl_program program;

    } ccl_request;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    ccl_reply.errcode_ret = clRetainProgram(ccl_request.program);
    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclRetainProgram");
    free(buffer_data_reply);
}

void _cclReleaseProgram(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_int errcode_ret;

    } ccl_reply;

    struct
    {
        cl_program program;

    } ccl_request;

    memcpy(&ccl_request, data, sizeof(ccl_request));
    ccl_reply.errcode_ret = clReleaseProgram(ccl_request.program);

    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclReleaseProgram");
    free(buffer_data_reply);
}

void _cclEnqueueMigrateMemObjects(void *_request)
{

    // TODO: not implemented yet
}

void _cclGetMemObjectInfo(void *_request)
{

    cl_mem memobj;
    cl_mem_info param_name;
    size_t param_value_size;

    cl_int result;
    size_t param_value_size_ret;
    char *param_value = NULL;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&memobj, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&param_name, data, sizeof(cl_mem_info), &offset_total);
    data += sizeof(cl_mem_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;
    offset_total = 0;

    param_value = malloc(param_value_size);

    result = clGetMemObjectInfo(memobj, param_name, param_value_size, param_value, &param_value_size_ret);
    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t) + param_value_size_ret;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
    buffer_data_reply += param_value_size_ret;

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetMemObjectInfo");
    free(buffer_data_reply);
    free(param_value);
}

void _cclCompileProgram(void *_request)
{

    cl_program program;
    cl_uint num_devices = 0;
    cl_device_id *device_list = NULL;
    char *options = NULL;
    cl_uint num_input_headers = 0;
    cl_program *input_headers = NULL;
    char **header_include_names = NULL;
    void *user_data = NULL;
    cl_uint size_options = 0;
    size_t *size_array = NULL;
    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    cl_int result;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&program, data, sizeof(cl_program), &offset_total);
    data += sizeof(cl_program);

    _ccl_memcpy(&num_devices, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    _ccl_memcpy(&num_input_headers, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    _ccl_memcpy(&size_options, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_devices > 0)
    {
        device_list = malloc(sizeof(cl_device_id) * num_devices);
        _ccl_memcpy(device_list, data, sizeof(cl_device_id) * num_devices, &offset_total);
        data += sizeof(cl_device_id) * num_devices;
    }

    if (size_options > 0)
    {
        options = malloc(size_options);
        _ccl_memcpy(options, data, size_options, &offset_total);
        data += size_options;
    }

    if (num_input_headers > 0)
    {

        input_headers = malloc(sizeof(cl_program) * num_input_headers);
        _ccl_memcpy(input_headers, data, sizeof(cl_program) * num_input_headers, &offset_total);
        data += sizeof(cl_program) * num_input_headers;

        size_array = malloc(sizeof(size_t) * num_input_headers);
        _ccl_memcpy(input_headers, data, sizeof(size_t) * num_input_headers, &offset_total);
        data += sizeof(size_t) * num_input_headers;

        header_include_names = malloc(sizeof(char *) * num_input_headers);
        for (int i = 0; i < num_input_headers; i++)
        {
            (header_include_names)[i] = (char *)malloc(sizeof(char) * size_array[i]);
            _ccl_memcpy((header_include_names)[i], data, size_array[i], &offset_total);
            data += size_array[i];
        }
    }
    data -= offset_total;
    offset_total = 0;
    result = clCompileProgram(program, num_devices, device_list, options, num_input_headers, input_headers, (const char **)header_include_names, NULL, user_data);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCompileProgram");
    free(buffer_data_reply);
    if (num_devices > 0)
        free(device_list);

    if (size_options > 0)
        free(options);

    if (num_input_headers > 0)
    {
        free(input_headers);
        free(size_array);
        for (int i = 0; i < num_input_headers; i++)
            free((header_include_names)[i]);
        free(header_include_names);
    }
}

void _cclLinkProgram(void *_request)
{

    cl_context context;
    cl_uint num_devices = 0;
    cl_device_id *device_list = NULL;
    char *options = NULL;
    cl_uint num_input_programs = 0;
    cl_program *input_programs = NULL;
    cl_uint size_options = 0;
    void *buffer_data_reply = NULL, *user_data = NULL, *data = NULL;
    int fd_tcp_client = 0;

    size_t size_buffer_reply = 0, offset_total = 0;

    cl_program result;
    cl_int errcode_ret;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    _ccl_memcpy(&num_devices, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    _ccl_memcpy(&num_input_programs, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    _ccl_memcpy(&size_options, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_devices > 0)
    {
        device_list = malloc(sizeof(cl_device_id) * num_devices);
        _ccl_memcpy(device_list, data, sizeof(cl_device_id) * num_devices, &offset_total);
        data += sizeof(cl_device_id) * num_devices;
    }

    if (size_options > 0)
    {
        options = malloc(size_options);
        _ccl_memcpy(options, data, size_options, &offset_total);
        data += size_options;
    }

    if (num_input_programs > 0)
    {
        input_programs = malloc(sizeof(cl_program) * num_input_programs);
        _ccl_memcpy(input_programs, data, sizeof(cl_program) * num_input_programs, &offset_total);
        data += sizeof(cl_program) * num_input_programs;
    }
    data -= offset_total;
    offset_total = 0;

    result = clLinkProgram(context, num_devices, device_list, options, num_input_programs, input_programs, NULL, user_data, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_program) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_program), &offset_total);
    buffer_data_reply += sizeof(cl_program);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclLinkProgram");
    free(buffer_data_reply);

    if (num_devices > 0)
        free(device_list);

    if (size_options > 0)
        free(options);

    if (num_input_programs > 0)
    {
        free(input_programs);
    }
}

void _cclUnloadPlatformCompiler(void *_request)
{
    cl_platform_id platform;
    cl_int errcode_ret;
    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;

    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    memcpy(&platform, data, sizeof(cl_platform_id));
    data += sizeof(cl_platform_id);

    data -= offset_total;
    offset_total = 0;

    errcode_ret = clUnloadPlatformCompiler(platform);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclUnloadPlatformCompiler");
    free(buffer_data_reply);
}

void _cclGetProgramInfo(void *_request)
{

    cl_program program;
    cl_program_info param_name;
    size_t param_value_size;
    cl_int result;
    size_t param_value_size_ret;
    char *param_value = NULL;
    void *buffer_data_reply = NULL, *data = NULL;

    int fd_tcp_client = 0, nr_position = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    char **p = NULL;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&program, data, sizeof(cl_program), &offset_total);
    data += sizeof(cl_program);

    _ccl_memcpy(&param_name, data, sizeof(cl_program_info), &offset_total);
    data += sizeof(cl_program_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;
    offset_total = 0;

    if (param_name == CL_PROGRAM_BINARIES)
    {

        nr_position = param_value_size / sizeof(char *);
        size_t *ss = malloc(sizeof(size_t) * nr_position);
        result = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t) * nr_position, ss, NULL);

        p = malloc(sizeof(char *));
        for (int i = 0; i < nr_position; i++)
            p[i] = malloc(ss[i]);

        result = clGetProgramInfo(program, param_name, sizeof(p), p, &param_value_size_ret);
    }
    else
    {
        if (param_value_size > 0)
            param_value = malloc(param_value_size);
        result = clGetProgramInfo(program, param_name, param_value_size, param_value, &param_value_size_ret);
    }
    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t);

    if (param_name == CL_PROGRAM_BINARIES)
    {
        for (int i = 0; i < nr_position; i++)
            size_buffer_reply += (strlen(p[i]) + 1);
    }
    else
    {
        if (param_value_size > 0)
            size_buffer_reply += param_value_size_ret;
    }

    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    if (param_name != CL_PROGRAM_BINARIES)
    {
        if (param_value_size > 0)
        {
            _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
            buffer_data_reply += param_value_size_ret;
        }
    }
    else
    {
        for (int i = 0; i < nr_position; i++)
        {
            _ccl_memcpy(buffer_data_reply, p[i], strlen(p[i]) + 1, &offset_total);
            buffer_data_reply += (strlen(p[i]) + 1);
        }
    }

    if (param_name != CL_PROGRAM_BINARIES)
    {
        if (param_value_size > 0)
            free(param_value);
    }
    else
    {
        for (int i = 0; i < nr_position; i++)
            free(p[i]);
        free(p);
        // if(param_value_size>0)
        // free(param_value);
    }
    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetProgramInfo");
    free(buffer_data_reply);
}

void _cclGetProgramBuildInfo(void *_request)
{

    cl_program program;
    cl_device_id device;
    cl_program_build_info param_name;
    size_t param_value_size;
    cl_int result;
    size_t param_value_size_ret;
    char *param_value = NULL;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&program, data, sizeof(cl_program), &offset_total);
    data += sizeof(cl_program);

    _ccl_memcpy(&device, data, sizeof(cl_device_id), &offset_total);
    data += sizeof(cl_device_id);

    _ccl_memcpy(&param_name, data, sizeof(cl_program_build_info), &offset_total);
    data += sizeof(cl_program_build_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    if (param_value_size > 0)
    {
        param_value = malloc(param_value_size);
    }
    data -= offset_total;
    offset_total = 0;

    result = clGetProgramBuildInfo(program, device, param_name, param_value_size, param_value, &param_value_size_ret);
    char *ss = (char *)param_value;

    if (param_value != NULL)
        printf("Size: %ld Log:%s\n", param_value_size_ret, ss);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t);
    if (param_value_size > 0)
    {
        size_buffer_reply += param_value_size_ret;
    }
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);
    if (param_value_size > 0)
    {
        _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
        buffer_data_reply += param_value_size_ret;
    }

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetProgramBuildInfo");
    free(buffer_data_reply);
    if (param_value_size > 0)
        free(param_value);
}

void _cclCreateKernelsInProgram(void *_request)
{

    cl_program program;
    cl_uint num_kernels;
    cl_int result;
    cl_uint num_kernels_ret;
    cl_kernel *kernels = NULL;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&program, data, sizeof(cl_program), &offset_total);
    data += sizeof(cl_program);

    _ccl_memcpy(&num_kernels, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    data -= offset_total;
    offset_total = 0;

    if (num_kernels > 0)
        kernels = malloc(sizeof(cl_kernel) * num_kernels);

    result = clCreateKernelsInProgram(program, num_kernels, kernels, &num_kernels_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_uint) + sizeof(cl_kernel) * num_kernels;

    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &num_kernels_ret, sizeof(cl_uint), &offset_total);
    buffer_data_reply += sizeof(cl_uint);

    _ccl_memcpy(buffer_data_reply, kernels, sizeof(cl_kernel) * num_kernels, &offset_total);
    buffer_data_reply += sizeof(cl_kernel) * num_kernels;

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateKernelsInProgram");
    free(buffer_data_reply);
    if (num_kernels > 0)
        free(kernels);
}

void _cclRetainKernel(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_kernel kernel;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;

    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    ccl_reply.errcode_ret = clRetainKernel(ccl_request.kernel);
    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclRetainKernel");
    free(buffer_data_reply);
}

void _cclReleaseKernel(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_kernel kernel;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;

    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));
    ccl_reply.errcode_ret = clReleaseKernel(ccl_request.kernel);

    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclReleaseKernel");
    free(buffer_data_reply);
}

void _cclCreateUserEvent(void *_request)
{

    cl_context context;
    cl_event result;
    cl_int errcode_ret;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;

    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    data -= offset_total;

    offset_total = 0;
    result = clCreateUserEvent(context, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_event) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateUserEvent");
    free(buffer_data_reply);
}

void _cclSetUserEventStatus(void *_request)
{

    cl_event event;
    cl_int execution_status;
    cl_int result;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&event, data, sizeof(cl_event), &offset_total);
    data += sizeof(cl_event);

    _ccl_memcpy(&execution_status, data, sizeof(cl_int), &offset_total);
    data += sizeof(cl_int);

    data -= offset_total;

    offset_total = 0;
    result = clSetUserEventStatus(event, execution_status);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclSetUserEventStatus");
    free(buffer_data_reply);
}

void _cclWaitForEvents(void *_request)
{

    cl_event *event_list = NULL;
    void *data = NULL;
    int fd_tcp_client = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    struct
    {
        cl_uint num_events;
    } ccl_request;

    struct
    {
        cl_int result;
    } ccl_reply;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    event_list = data + (sizeof(ccl_request));

    ccl_reply.result = clWaitForEvents(ccl_request.num_events, event_list);

    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclWaitForEvents");
    free(buffer_data_reply);
}

void _cclGetEventInfo(void *_request)
{

    cl_event event;
    cl_event_info param_name;
    size_t param_value_size;
    cl_int result;
    size_t param_value_size_ret;
    char *param_value = NULL;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;

    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&event, data, sizeof(cl_event), &offset_total);
    data += sizeof(cl_event);

    _ccl_memcpy(&param_name, data, sizeof(cl_event_info), &offset_total);
    data += sizeof(cl_event_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;

    if (param_value_size > 0)
        param_value = malloc(param_value_size);

    result = clGetEventInfo(event, param_name, param_value_size, param_value, &param_value_size_ret);

    offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t);
    if (param_value_size > 0)
        size_buffer_reply += param_value_size_ret;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    if (param_value_size > 0)
    {
        _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
        buffer_data_reply += param_value_size_ret;
    }

    if (param_value_size > 0)
        free(param_value);
    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetEventInfo");
    free(buffer_data_reply);
}

void _cclGetKernelInfo(void *_request)
{

    cl_kernel kernel;
    cl_kernel_info param_name;
    size_t param_value_size;
    cl_int result;
    size_t param_value_size_ret;
    char *param_value = NULL;
    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&kernel, data, sizeof(cl_kernel), &offset_total);
    data += sizeof(cl_kernel);

    _ccl_memcpy(&param_name, data, sizeof(cl_kernel_info), &offset_total);
    data += sizeof(cl_kernel_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;
    offset_total = 0;

    if (param_value_size > 0)
        param_value = malloc(param_value_size);

    result = clGetKernelInfo(kernel, param_name, param_value_size, param_value, &param_value_size_ret);
    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t) + param_value_size_ret;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    if (param_value_size > 0)
    {
        _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
        buffer_data_reply += param_value_size_ret;
    }

    free(param_value);
    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetKernelInfo");
    free(buffer_data_reply);
}

void _cclGetKernelWorkGroupInfo(void *_request)
{

    cl_kernel kernel;
    cl_device_id device;
    cl_kernel_work_group_info param_name;
    size_t param_value_size;

    cl_int result;
    size_t param_value_size_ret;
    void *param_value = NULL;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&kernel, data, sizeof(cl_kernel), &offset_total);
    data += sizeof(cl_kernel);

    _ccl_memcpy(&device, data, sizeof(cl_device_id), &offset_total);
    data += sizeof(cl_device_id);

    _ccl_memcpy(&param_name, data, sizeof(cl_kernel_work_group_info), &offset_total);
    data += sizeof(cl_kernel_work_group_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);
    data -= offset_total;
    offset_total = 0;

    if (param_value_size > 0)
        param_value = malloc(param_value_size);

    result = clGetKernelWorkGroupInfo(kernel, device, param_name, param_value_size, param_value, &param_value_size_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t);
    if (param_value_size > 0)
        size_buffer_reply += param_value_size_ret;

    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    if (param_value_size > 0)
    {
        _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
        buffer_data_reply += param_value_size_ret;
    }

    if (param_value_size > 0)
        free(param_value);
    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetKernelWorkGroupInfo");
    free(buffer_data_reply);
}

void _cclGetKernelArgInfo(void *_request)
{

    cl_kernel kernel;
    cl_uint arg_indx;
    cl_kernel_arg_info param_name;
    size_t param_value_size;
    size_t param_value_size_ret;
    char *param_value = NULL;
    cl_int result;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&kernel, data, sizeof(cl_kernel), &offset_total);
    data += sizeof(cl_kernel);

    _ccl_memcpy(&arg_indx, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    _ccl_memcpy(&param_name, data, sizeof(cl_kernel_arg_info), &offset_total);
    data += sizeof(cl_kernel_arg_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;

    param_value = malloc(param_value_size + 1);

    result = clGetKernelArgInfo(kernel, arg_indx, param_name, param_value_size, param_value, &param_value_size_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t) + param_value_size_ret;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
    buffer_data_reply += param_value_size_ret;

    free(param_value);
    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetKernelArgInfo");
    free(buffer_data_reply);
}

void _cclRetainEvent(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    struct
    {
        cl_event event;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    memcpy(&ccl_request.event, data, sizeof(ccl_request));

    ccl_reply.errcode_ret = clRetainEvent(ccl_request.event);

    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclRetainEvent");
    free(buffer_data_reply);
}

void _cclReleaseEvent(void *_request)
{

    void *data = NULL;
    int fd_tcp_client = 0;

    struct
    {
        cl_event event;
    } ccl_request;

    struct
    {
        cl_int errcode_ret;
    } ccl_reply;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    memcpy(&ccl_request.event, data, sizeof(ccl_request));
    ccl_reply.errcode_ret = clReleaseEvent(ccl_request.event);

    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclReleaseEvent");
    free(buffer_data_reply);
}

void callbacksetEvent(cl_event event, cl_int event_command_status, void *user_data)
{
    struct sockaddr_in repl_callback;

    struct
    {
        cl_event event;
        uintptr_t callback;
        cl_int event_command_status;

    } ccl_request = {.event = event, .event_command_status = event_command_status};

    memcpy(&ccl_request.callback, user_data, sizeof(uintptr_t));
    memcpy(&repl_callback, user_data + sizeof(uintptr_t), sizeof(struct sockaddr_in));
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }

    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr_in *)&repl_callback, sizeof(repl_callback)) != 0)
    {
        perror("Error:");
        printf("connection with the server failed... %d\n", errno);
        exit(0);
    }

    send(sockfd, &ccl_request, sizeof(ccl_request), 0);

    free(user_data);
    close(sockfd);
}

void _cclSetEventCallback(void *_request)
{

    void *func = NULL, *data = NULL;
    int fd_tcp_client = 0;
    void *callback = NULL;

    struct
    {
        cl_event event;
        char isnull;
        uintptr_t callback;
        cl_int command_exec_callback_type;

    } ccl_request;

    struct
    {
        cl_int result;

    } ccl_reply;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    memcpy(&ccl_request, data, sizeof(ccl_request));

    if (!ccl_request.isnull)
    {
        func = &callbacksetEvent;

        callback = malloc(sizeof(uintptr_t) + sizeof(struct sockaddr_in));
        memcpy(callback, &ccl_request.callback, sizeof(uintptr_t));
        memcpy(callback + sizeof(uintptr_t), data + sizeof(ccl_request), sizeof(struct sockaddr_in));
    }

    ccl_reply.result = clSetEventCallback(ccl_request.event, ccl_request.command_exec_callback_type, func, callback);

    size_t size_buffer_reply = 0, offset_total = 0;
    size_buffer_reply = sizeof(size_t) + sizeof(ccl_reply);
    void *buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &ccl_reply, sizeof(ccl_reply), &offset_total);
    buffer_data_reply += sizeof(ccl_reply);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclSetEventCallback");
    free(buffer_data_reply);
}

void _cclEnqueueMarkerWithWaitList(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    cl_uint num_events_in_wait_list;
    cl_command_queue command_queue;
    cl_int result;
    cl_event event;
    cl_event *event_wait_list = NULL;
    char event_is_null;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&num_events_in_wait_list, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_events_in_wait_list > 0)
    {

        event_wait_list = malloc(sizeof(cl_event) * num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * num_events_in_wait_list;
    }
    _ccl_memcpy(&event_is_null, data, sizeof(char), &offset_total);
    data += sizeof(char);

    data -= offset_total;
    offset_total = 0;
    result = clEnqueueMarkerWithWaitList(command_queue, num_events_in_wait_list, (num_events_in_wait_list == 0) ? NULL : event_wait_list, event_is_null ? NULL : &event);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueMarkerWithWaitList");
    free(buffer_data_reply);
    if (num_events_in_wait_list > 0)
        free(event_wait_list);
}

void _cclEnqueueBarrierWithWaitList(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;
    cl_uint num_events_in_wait_list;
    cl_command_queue command_queue;
    cl_int result;
    cl_event event;
    cl_event *event_wait_list = NULL;
    char event_is_null;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&num_events_in_wait_list, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_events_in_wait_list > 0)
    {
        event_wait_list = malloc(sizeof(cl_event) * num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * num_events_in_wait_list;
    }

    _ccl_memcpy(&event_is_null, data, sizeof(char), &offset_total);
    data += sizeof(char);
    data -= offset_total;
    offset_total = 0;

    result = clEnqueueBarrierWithWaitList(command_queue, num_events_in_wait_list, (num_events_in_wait_list == 0) ? NULL : event_wait_list, event_is_null ? NULL : &event);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclEnqueueBarrierWithWaitList");
    free(buffer_data_reply);
    if (num_events_in_wait_list > 0)
        free(event_wait_list);
}

void _cclEnqueueNativeKernel(void *_request)
{
    // TODO not implmented yet
}

void _cclEnqueueTask(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    cl_uint num_events_in_wait_list;
    cl_command_queue command_queue;
    cl_int result;
    cl_event event;
    cl_kernel kernel;
    cl_event *event_wait_list = NULL;
    char event_is_null;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    _ccl_memcpy(&kernel, data, sizeof(cl_kernel), &offset_total);
    data += sizeof(cl_kernel);

    _ccl_memcpy(&num_events_in_wait_list, data, sizeof(cl_uint), &offset_total);
    data += sizeof(cl_uint);

    if (num_events_in_wait_list > 0)
    {
        event_wait_list = malloc(sizeof(cl_event) * num_events_in_wait_list);
        _ccl_memcpy(event_wait_list, data, sizeof(cl_event) * num_events_in_wait_list, &offset_total);
        data += sizeof(cl_event) * num_events_in_wait_list;
    }

    _ccl_memcpy(&event_is_null, data, sizeof(char), &offset_total);
    data += sizeof(char);
    data -= offset_total;
    offset_total = 0;

    result = clEnqueueTask(command_queue, kernel, num_events_in_wait_list, (num_events_in_wait_list == 0) ? NULL : event_wait_list, event_is_null ? NULL : &event);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "clEnqueueTask");
    free(buffer_data_reply);
    if (num_events_in_wait_list > 0)
        free(event_wait_list);
}

void _cclCreateFromGLBuffer(void *_request)
{

    cl_context context;
    cl_mem_flags flags;
    GLuint bufobj;
    cl_int errcode_ret;
    cl_mem result;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    _ccl_memcpy(&flags, data, sizeof(cl_mem_flags), &offset_total);
    data += sizeof(cl_mem_flags);

    _ccl_memcpy(&bufobj, data, sizeof(GLuint), &offset_total);
    data += sizeof(GLuint);

    data -= offset_total;
    offset_total = 0;

    result = clCreateFromGLBuffer(context, flags, bufobj, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_mem) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_mem), &offset_total);
    buffer_data_reply += sizeof(cl_mem);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateFromGLBuffer");
    free(buffer_data_reply);
}

void _cclCreateFromGLTexture(void *_request)
{

    cl_context context;
    cl_mem_flags flags;
    GLenum texture_target;
    GLint miplevel;
    GLuint texture;
    cl_int errcode_ret;
    cl_mem result;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    _ccl_memcpy(&flags, data, sizeof(cl_mem_flags), &offset_total);
    data += sizeof(cl_mem_flags);

    _ccl_memcpy(&texture_target, data, sizeof(GLenum), &offset_total);
    data += sizeof(GLenum);

    _ccl_memcpy(&miplevel, data, sizeof(GLint), &offset_total);
    data += sizeof(GLint);

    _ccl_memcpy(&texture, data, sizeof(GLuint), &offset_total);
    data += sizeof(GLuint);

    data -= offset_total;
    offset_total = 0;

    result = clCreateFromGLTexture(context, flags, texture_target, miplevel, texture, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_mem) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_mem), &offset_total);
    buffer_data_reply += sizeof(cl_mem);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateFromGLTexture");
    free(buffer_data_reply);
}

void _cclCreateFromGLRenderbuffer(void *_request)
{

    cl_context context;
    cl_mem_flags flags;
    GLuint renderbuffer;
    cl_int errcode_ret;
    cl_mem result;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&context, data, sizeof(cl_context), &offset_total);
    data += sizeof(cl_context);

    _ccl_memcpy(&flags, data, sizeof(cl_mem_flags), &offset_total);
    data += sizeof(cl_mem_flags);

    _ccl_memcpy(&renderbuffer, data, sizeof(GLuint), &offset_total);
    data += sizeof(GLuint);

    data -= offset_total;
    offset_total = 0;

    result = clCreateFromGLRenderbuffer(context, flags, renderbuffer, &errcode_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_mem) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_mem), &offset_total);
    buffer_data_reply += sizeof(cl_mem);

    _ccl_memcpy(buffer_data_reply, &errcode_ret, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclCreateFromGLRenderbuffer");
    free(buffer_data_reply);
}

void _cclGetGLObjectInfo(void *_request)
{

    cl_mem memobj;
    cl_gl_object_type gl_object_type;
    GLuint gl_object_name;
    cl_int result;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&memobj, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    data -= offset_total;
    offset_total = 0;

    result = clGetGLObjectInfo(memobj, &gl_object_type, &gl_object_name);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_gl_object_type) + sizeof(GLuint);

    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &gl_object_type, sizeof(cl_gl_object_type), &offset_total);
    buffer_data_reply += sizeof(cl_gl_object_type);

    _ccl_memcpy(buffer_data_reply, &gl_object_name, sizeof(GLuint), &offset_total);
    buffer_data_reply += sizeof(GLuint);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetGLObjectInfo");
    free(buffer_data_reply);
}

void _cclGetGLTextureInfo(void *_request)
{

    cl_mem memobj;
    cl_gl_texture_info param_name;
    size_t param_value_size;
    cl_int result;
    size_t param_value_size_ret;
    char *param_value = NULL;

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&memobj, data, sizeof(cl_mem), &offset_total);
    data += sizeof(cl_mem);

    _ccl_memcpy(&param_name, data, sizeof(cl_gl_texture_info), &offset_total);
    data += sizeof(cl_gl_texture_info);

    _ccl_memcpy(&param_value_size, data, sizeof(size_t), &offset_total);
    data += sizeof(size_t);

    data -= offset_total;
    offset_total = 0;

    param_value = malloc(param_value_size);
    result = clGetGLTextureInfo(memobj, param_name, param_value_size, param_value, &param_value_size_ret);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(size_t) + param_value_size_ret;
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &param_value_size_ret, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, param_value, param_value_size_ret, &offset_total);
    buffer_data_reply += param_value_size_ret;

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "_cclGetGLTextureInfo");
    free(buffer_data_reply);
    free(param_value);
}

void _cclEnqueueAcquireGLObjects(void *_request)
{

    // TODO not implmented yet
}

void _cclEnqueueReleaseGLObjects(void *_request)
{

    // TODO not implmented yet
}

void _cclCreateEventFromGLsyncKHR(void *_request)
{

    // TODO not implmented yet
}

void _cclGetGLContextInfoKHR(void *_request)
{
    // TODO not implmented yet
}

void _cclEnqueueMarker(void *_request)
{

    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;
    cl_command_queue command_queue;
    cl_int result;
    cl_event event;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    data -= offset_total;
    offset_total = 0;

    result = clEnqueueMarker(command_queue, &event);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int) + sizeof(cl_event);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    _ccl_memcpy(buffer_data_reply, &event, sizeof(cl_event), &offset_total);
    buffer_data_reply += sizeof(cl_event);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "clEnqueueMarker");
    free(buffer_data_reply);
}

void _cclEnqueueBarrier(void *_request)
{
    void *buffer_data_reply = NULL, *data = NULL;
    int fd_tcp_client = 0;
    size_t size_buffer_reply = 0, offset_total = 0;

    cl_command_queue command_queue;
    cl_int result;

    ropencl_primitive_request *request = (ropencl_primitive_request *)_request;
    data = request->data;
    fd_tcp_client = request->fd;

    _ccl_memcpy(&command_queue, data, sizeof(cl_command_queue), &offset_total);
    data += sizeof(cl_command_queue);

    data -= offset_total;
    offset_total = 0;

    result = clEnqueueBarrier(command_queue);

    size_buffer_reply = sizeof(size_t) + sizeof(cl_int);
    buffer_data_reply = malloc(size_buffer_reply);

    _ccl_memcpy(buffer_data_reply, &size_buffer_reply, sizeof(size_t), &offset_total);
    buffer_data_reply += sizeof(size_t);

    _ccl_memcpy(buffer_data_reply, &result, sizeof(cl_int), &offset_total);
    buffer_data_reply += sizeof(cl_int);

    buffer_data_reply -= offset_total;
    send_data_tcp(&(request->fd), buffer_data_reply, size_buffer_reply, "clEnqueueBarrier");
    free(buffer_data_reply);
}
// Array of the primitives rOpenCL
void (*handler[])(void *) = {_connect, _cclGetPlatformIDs, NULL, _cclGetPlatformInfo, _cclGetDeviceIDs, _cclGetDeviceInfo, _cclCreateContext, _cclCreateCommandQueue, _cclCreateBuffer, _cclEnqueueReadBuffer, _cclEnqueueWriteBuffer, _cclCreateProgramWithSource, _cclBuildProgram, _cclCreateKernel, _cclSetKernelArg, _cclEnqueueNDRangeKernel, _cclGetEventProfilingInfo, _cclCreateImage, _cclEnqueueReadImage, _cclEnqueueWriteImage, _cclEnqueueFillImage, _cclEnqueueCopyImage, _cclEnqueueCopyImageToBuffer, _cclEnqueueCopyBufferToImage, NULL, _cclCreateSubBuffer, _cclEnqueueReadBufferRect, _cclEnqueueWriteBufferRect, _cclEnqueueFillBuffer, _cclEnqueueCopyBuffer, _cclEnqueueCopyBufferRect, _cclEnqueueMapImage, NULL, _cclEnqueueMapBuffer, _cclCreateSubDevices, NULL, _cclRetainDevice, _cclReleaseDevice, _cclCreateContextFromType, _cclRetainContext, _cclReleaseContext, _cclGetContextInfo, NULL, _cclGetExtensionFunctionAddressForPlatform, NULL, _cclRetainCommandQueue, _cclReleaseCommandQueue, _cclGetCommandQueueInfo, _cclGetSupportedImageFormats, _cclGetImageInfo, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, _cclFlush, _cclFinish, _cclRetainMemObject, _cclSetMemObjectDestructorCallback, _cclEnqueueUnmapMemObject, _cclReleaseMemObject, _cclCreateSampler, _cclRetainSampler, _cclReleaseSampler, _cclGetSamplerInfo, NULL, _cclCreateProgramWithBinary, _cclCreateProgramWithBuiltInKernels, _cclRetainProgram, _cclReleaseProgram, _cclEnqueueMigrateMemObjects, NULL, NULL, _cclGetMemObjectInfo, _cclCompileProgram, _cclLinkProgram, _cclUnloadPlatformCompiler, _cclGetProgramInfo, _cclGetProgramBuildInfo, _cclCreateKernelsInProgram, _cclRetainKernel, _cclReleaseKernel, NULL, NULL, _cclCreateUserEvent, _cclSetUserEventStatus, _cclWaitForEvents, _cclGetEventInfo, NULL, _cclGetKernelInfo, _cclGetKernelWorkGroupInfo, _cclGetKernelArgInfo, _cclRetainEvent, _cclReleaseEvent, _cclSetEventCallback, _cclEnqueueMarkerWithWaitList, _cclEnqueueBarrierWithWaitList, _cclEnqueueNativeKernel, NULL, _cclEnqueueTask, _cclCreateFromGLBuffer, _cclCreateFromGLTexture, _cclCreateFromGLRenderbuffer, _cclGetGLObjectInfo, _cclGetGLTextureInfo, _cclEnqueueAcquireGLObjects, _cclEnqueueReleaseGLObjects, _cclCreateEventFromGLsyncKHR, _cclGetGLContextInfoKHR, _cclEnqueueMarker, _cclEnqueueBarrier};

void *worker_tcp(void *arg)
{

#if DEBUG == 1
    struct timeval t0, t1;
    char ip[16];
#endif

    int fd_client = (long)((int *)arg);
    size_t total_size = 0, return_sendto = 0, size_recv = 0, offset_total = 0;
    char id;

    ropencl_primitive_request request;

    request.fd = fd_client;

    while (1)
    {

#if DEBUG == 1
        printf("-- TCP: Waiting for messages --\n");
#endif

        return_sendto = recv(request.fd, &id, sizeof(char), 0);

        if (return_sendto == 0)
        {


            puts("-- TCP: Thread exit --\n");

            close(fd_client);
            pthread_exit(0);
        }

        return_sendto = recv(request.fd, &size_recv, sizeof(size_t), 0);

        request.id = id;
        total_size += size_recv;
        // Remove the header data size.
        size_recv -= (sizeof(char) + sizeof(size_t));

#if DEBUG == 1
        gettimeofday(&t0, NULL);
        printf("-- TCP: Primitive 0x%02X (%s)  Data Size: %d Data Already Read: %d --\n", id, func[(int)id], size_recv, offset_total);
#endif

        request.data = malloc(size_recv);

        while (offset_total < size_recv)
        {
            return_sendto = recv(request.fd, request.data + offset_total, size_recv - offset_total, 0);
            offset_total += return_sendto;
        }

#if DEBUG == 1
        inet_ntop(AF_INET, &((cli_tcp.sin_addr)), ip, 16);
        printf("-- TCP: Theorical size %d bytes : Real size %d bytes from %s [reqid=0x%02X] -- \n", size_recv, offset_total, ip, id);
#endif

        handler[((int)(request.id))](&request);

#if DEBUG == 1
        gettimeofday(&t1, NULL);
        printf("-- TCP: Primitive 0x%02X time to processed :%ld --\n", id, (t1.tv_usec - t0.tv_usec));
#endif
        size_recv = 0;
        return_sendto = 0;
        offset_total = 0;
        free(request.data);
    }
}

void *get_primitves_network_tcp(void *arg)
{
    int len = sizeof(cli_tcp), fd_client, threads = 2;
    fd_tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd_tcp == -1)
    {
        _ccl_perror_and_exit("Failed to create TCP socket");
    }
    memset(&addr_tcp, 0, sizeof(addr_tcp));
    addr_tcp.sin_family = AF_INET;
    addr_tcp.sin_addr.s_addr = inet_addr(LOCAL_INTERFACE);
    addr_tcp.sin_port = htons(PORT);

    if (bind(fd_tcp, (struct sockaddr *)&addr_tcp, sizeof(addr_tcp)) == -1)
    {
        _ccl_perror_and_exit("Failed to bind TCP socket");
    }
    if ((listen(fd_tcp, SIZE_HOST_PROGRAM)) != 0)
    {
        _ccl_perror_and_exit("Listen TCP Socket Failed");
    }

    printf("-- TCP: rOpenCLDaemon is listening on port %d --\n", PORT);

    while (1)
    {
        fd_client = accept(fd_tcp, (struct sockaddr *)&cli_tcp, (socklen_t *)&len);
        if (fd_client < 0)
        {
            _ccl_perror_and_exit(" -- TCP: Server acccept failed -- \n");
        }

        int val = 1;

        if (setsockopt(fd_client, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) < 0)
            perror("setsockopt(2) error");

        setsockopt(fd_client, IPPROTO_TCP, TCP_QUICKACK, (void *)&val, sizeof(int));

        setsockopt(fd_client, IPPROTO_TCP, TCP_DEFER_ACCEPT, &val, sizeof(val));

        int optval = 7; // valid values are in the range [1,7]   // 1- low priority, 7 - high priority
        setsockopt(fd_client, SOL_SOCKET, SO_PRIORITY, &optval, sizeof(optval));

        if (pthread_create(&dispatcher_thread[threads++], NULL, worker_tcp, (int *)(long)fd_client) != 0)
            _ccl_perror_and_exit("pthread_create");

        pthread_detach(dispatcher_thread[threads - 1]);

#if DEBUG == 1
        printf("-- TCP: Descriptor allocated %d --\n", fd_client);
        printf("-- TCP: Connection accept --\n");
#endif
    }
}

int main(int argc, char **argv)
{

    init_daemon();

    if (pthread_create(&dispatcher_thread[0], NULL, get_primitves_network_tcp, (int *)(long)1) != 0)
        _ccl_perror_and_exit("pthread_create");

    pthread_join(dispatcher_thread[0], NULL);

    exit(0);
}
