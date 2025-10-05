
#include "pch.h"
#include "resize/resizablemsgsupport.h"

/////////////////////////////////////////////////////////////////////////////
// Registered message to communicate with the library

// static intializer must be called before user code
#pragma warning(disable:4073)
#pragma init_seg(lib)

const UINT WMU_RESIZESUPPORT = ::RegisterWindowMessage(TEXT("WMU_RESIZESUPPORT"));

