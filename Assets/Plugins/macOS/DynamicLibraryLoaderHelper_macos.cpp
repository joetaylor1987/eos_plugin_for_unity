// DynamicLibraryLoaderHelper.cpp : Defines the functions for the static library.
//

#include <assert.h>
#include <dlfcn.h>

#define STATIC_EXPORT(return_type) extern "C" return_type

struct DLLHContext
{
};

//-------------------------------------------------------------------------
STATIC_EXPORT(void*) LoadLibrary(const char *library_path)
{
    return dlopen(library_path, RTLD_NOW);
}

//-------------------------------------------------------------------------
// pretend windows like function
STATIC_EXPORT(bool) FreeLibrary(void *library_handle)
{
    dlclose(library_handle);    
    return true;
}

//-------------------------------------------------------------------------
STATIC_EXPORT(void*) GetModuleHandle(const char *library_path)
{
    void *to_return = dlopen(library_path, RTLD_NOLOAD);

    // dlopen increments the ref handle, so make sure to 
    // release the ref handle. See `man dlopen`
    if (to_return)
    {
        dlclose(to_return);
    }

    return to_return;
}

//-------------------------------------------------------------------------
STATIC_EXPORT(void*) GetProcAddress(void *library_handle, const char *function_name)
{
    return dlsym(library_handle, function_name);
}

//-------------------------------------------------------------------------
void * DLLH_macOS_load_library_at_path(DLLHContext *ctx, const char *library_path)
{
    void *to_return = dlopen(library_path, RTLD_NOW);
   
    return to_return; 
}

//-------------------------------------------------------------------------
// TODO: Handle the actual module instead of all symbols
void * DLLH_macOS_load_function_with_name(DLLHContext *ctx, void *library_handle, const char *function)
{
    void *output_ptr = nullptr;

    output_ptr = dlsym(library_handle, function);

    return output_ptr;
}

//-------------------------------------------------------------------------
// Create heap data for storing random things, if need be on a given platform
STATIC_EXPORT(void *) DLLH_create_context()
{
    return new DLLHContext();
}

//-------------------------------------------------------------------------
STATIC_EXPORT(void) DLLH_destroy_context(void *context)
{
    delete static_cast<DLLHContext *>(context);
}

//-------------------------------------------------------------------------
STATIC_EXPORT(void *) DLLH_load_library_at_path(void *ctx, const char *library_path)
{
    if (ctx == nullptr) {
        return nullptr;
    }

    DLLHContext *dllh_ctx = static_cast<DLLHContext*>(ctx);
    void *to_return = nullptr;
    
    to_return = DLLH_macOS_load_library_at_path(dllh_ctx, library_path);

    return to_return;
}

//-------------------------------------------------------------------------
// This returns a bare function pointer that is only valid as long as the library_handle and context are
// valid
STATIC_EXPORT(void *) DLLH_load_function_with_name(void *ctx, void *library_handle, const char *function)
{
    void *to_return = nullptr;
    DLLHContext *dllh_ctx = static_cast<DLLHContext*>(ctx);

    to_return = DLLH_macOS_load_function_with_name(dllh_ctx, library_handle, function);

    return to_return;
}

//-------------------------------------------------------------------------
// TODO: unload the library correct? I don't know if that's actually a good
// idea on macos or not
STATIC_EXPORT(void) DLLH_unload_library_at_path(const char *library_path)
{
}
