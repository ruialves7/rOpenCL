#include "cl_management.h"
#include <stdio.h>
void *getDispachPointer(int type)
{
    if (type == 0)
    {
        struct _cl_platform_id _platforms[1] = {{&pocl_dispatch}};
        struct _cl_platform_id *platform = malloc(sizeof(struct _cl_platform_id) * 1);
        platform[0] = _platforms[0];
        return platform;
    }
    if (type == 1)
    {
        struct _cl_device_id _device[1] = {{&pocl_dispatch}};
        struct _cl_device_id *device_return = malloc(sizeof(struct _cl_device_id) * 1);
        device_return[0] = _device[0];
        return device_return;
    }
    if (type == 2)
    {
        struct _cl_context_ _context[1] = {{&pocl_dispatch}};
        struct _cl_context_ *context_return = malloc(sizeof(struct _cl_context_) * 1);
        context_return[0] = _context[0];
        return context_return;
    }
    if (type == 3)
    {
        struct _cl_command_queue_ _command_queue[1] = {{&pocl_dispatch}};
        struct _cl_command_queue_ *_command_queue_return = malloc(sizeof(struct _cl_command_queue_) * 1);
        _command_queue_return[0] = _command_queue[0];
        return _command_queue_return;
    }
    if (type == 4)
    {
        struct _cl_mem _cl_buffer_[1] = {{&pocl_dispatch}};
        struct _cl_mem *_cl_buffer_return = malloc(sizeof(struct _cl_mem) * 1);
        _cl_buffer_return[0] = _cl_buffer_[0];
        return _cl_buffer_return;
    }

    if (type == 5)
    {
        struct _cl_program_ _cl_program_[1] = {{&pocl_dispatch}};
        struct _cl_program_ *_cl_program_return = malloc(sizeof(struct _cl_program_) * 1);
        _cl_program_return[0] = _cl_program_[0];
        return _cl_program_return;
    }
    if (type == 6)
    {
        struct _cl_kernel_ _cl_kernel_[1] = {{&pocl_dispatch}};
        struct _cl_kernel_ *_cl_kernel_return = malloc(sizeof(struct _cl_kernel_) * 1);
        _cl_kernel_return[0] = _cl_kernel_[0];
        return _cl_kernel_return;
    }
    if (type == 7)
    {
        struct _cl_event_ _cl_event_[1] = {{&pocl_dispatch}};
        struct _cl_event_ *_cl_event_return = malloc(sizeof(struct _cl_event_) * 1);
        _cl_event_return[0] = _cl_event_[0];
        return _cl_event_return;
    }
    if (type == 8)
    {
        struct _cl_sampler_ _cl_sampler_[1] = {{&pocl_dispatch}};
        struct _cl_sampler_ *_cl_sampler_return = malloc(sizeof(struct _cl_sampler_) * 1);
        _cl_sampler_return[0] = _cl_sampler_[0];
        return _cl_sampler_return;
    }
    return NULL;
}
