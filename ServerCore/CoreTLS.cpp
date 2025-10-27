#include "pch.h"
#include "CoreTLS.h"

// 코드		X
// 스택		X
// 힙		O
// 데이터	O
// TLS		X

thread_local uint32 LThreadId = 0;