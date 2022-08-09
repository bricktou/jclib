#include "SCCPConvert.h"
using namespace sccp;

#ifndef SCT_PLUGIN_API
#   define SCT_PLUGIN_API extern "C"
#endif

#ifndef SCT_EXPORT_API
#   define SCT_EXPORT_API _declspec(dllexport)
#endif

SCT_PLUGIN_API
SCT_EXPORT_API
IConvert *registConvert(IAccessSCCT *accessScct) {
    return new SCCPConvert(accessScct);
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        ghModule = hModule;
        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

