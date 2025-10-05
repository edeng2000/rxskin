#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#define WIN32_LEAN_AND_MEAN  
#include <windows.h>

#define lengthof(x) (sizeof(x)/sizeof(*x))

#include "rxskinincludes.h"
#include "fundation/rxlogger.h"

#endif //PCH_H
