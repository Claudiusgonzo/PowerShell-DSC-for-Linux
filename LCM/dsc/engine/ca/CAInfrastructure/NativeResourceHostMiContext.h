/**************************************************************************************************/
/*                                                                                                */
/* Implementation of the MI_Context interface for native (non-WMI) resource providers.            */
/*                                                                                                */
/* MI_Context is used to get the output and results from the resource provider, amongst other     */
/* things. The MI interfaces are documented at                                                    */
/*                                                                                                */
/*     https://msdn.microsoft.com/en-us/library/hh449203(v=vs.85).aspx                            */
/*                                                                                                */
/**************************************************************************************************/

#ifndef _NATIVE_RESOURCE_HOST_MI_MODULE_H
#define _NATIVE_RESOURCE_HOST_MI_MODULE_H

#include <MI.h>
#include "NativeResourceProviderMiModule.h"

/**************************************************************************************************/
/*                                                                                                */
/* NativeResourceHostMiContext extends MI_Context with properties specific to the native          */
/* resource providers.                                                                            */
/*                                                                                                */
/**************************************************************************************************/

typedef struct {
    // Base class
    MI_Context _base;

    // Path to the resource provider implementation
    MI_Char* resourceProviderPath;

    struct {
        // The implementation of MI_Context simply forwards the calls to the engine via a set
        // of callbacks. 'callbackContext' is the context needed by those callbacks.
        ProviderCallbackContext* callbackContext;

        // MI_Module for the resource provider
        NativeResourceProviderMiModule* resourceProviderMiModule;

        // MI_Module_Self resulting from loading the module
        MI_Module_Self* resourceProviderMiModuleSelf;

        // Indicates that the Load method in the resource provider MI_Module has been successfully invoked
        BOOL resourceProviderMiModuleLoaded;

        // Class declaration of the resource provider
        // NOTE: the class declaration is managed by the provider's implementation of MI_Module and should not be deallocated
        MI_ClassDecl* resourceClassDecl;

        // "Self" reference to the class declaration of the resource provider
        void *resourceClassDeclSelf;

        // Indicates that the Load method in the resource provider class has been successfully invoked
        BOOL resourceProviderClassLoaded;

        // This is a copy (clone) of the output resource posted by the resource provider via
        // PostInstance. It will be NULL if the provider hasn't posted a result yet, or
        // after the value has been retrieved with NativeResourceHostMiContext_TakeResourceProviderOutputResource
        MI_Instance* outputResource;

        // Value returned by the resource provider via PostResult. The value will be MI_RESULT_OK
        // if the provider hasn't posted a result yet
        MI_Result result;
    } _private;

} NativeResourceProvider;

//
// Creates an instance of NativeResourceProvider.
//
MI_Result NativeResourceProvider_New(const _In_z_ MI_Char* resourceProviderPath, 
                                            const _In_z_ MI_Char* resourceProviderClassName, 
                                            _In_opt_ void *callbackContext, 
                                            _Outptr_ NativeResourceProvider** nativeResourceProvider);

//
// Destroys the MI_Context created with NativeResourceHostMiContext_Delete.
//
MI_Result NativeResourceProvider_Delete(_In_ NativeResourceProvider* nativeResourceProvider);

//
// Invokes GetTargetResource on the resource provider
//
MI_Result NativeResourceProvider_GetTargetResource(
    _In_ NativeResourceProvider* nativeResourceProvider,
    _In_ MI_Application *miApplication,
    _In_ MI_Session *miSession,
    _In_ MI_Instance *nativeResource,
    _In_ const MI_Instance *resourceProviderRegistration,
    _Inout_ MI_Instance *outputInstance,
    // _Inout_ MI_InstanceA *outputInstance,
    _Outptr_result_maybenull_ MI_Instance **extendedError
);

//
// Invokes GetTargetResource on the resource provider
//
MI_Result NativeResourceProvider_TestTargetResource(
    _In_ NativeResourceProvider* nativeResourceProvider,
    _In_ MI_Application *miApplication,
    _In_ MI_Session *miSession,
    _In_ MI_Instance *nativeResource,
    _In_ const MI_Instance *resourceProviderRegistration,
    _Outptr_ MI_Uint32 *testOperationResult,
    _Outptr_result_maybenull_ MI_Instance **extendedError
);


//
// Invokes GetInventory on the resource provider
//
MI_Result NativeResourceProvider_GetInventory(
    _In_ NativeResourceProvider* nativeResourceProvider,
    _In_ MI_Application *miApplication,
    _In_ MI_Session *miSession,
    _In_ MI_Instance *nativeResource,
    _In_ const MI_Instance *resourceProviderRegistration,
    _Inout_ MI_InstanceA *outputInstance,
    _Outptr_result_maybenull_ MI_Instance **extendedError
);

#endif _NATIVE_RESOURCE_HOST_MI_MODULE_H
