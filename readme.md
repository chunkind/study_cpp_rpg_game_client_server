* ServerCore
- 일반 -> 출력 디렉터리
$(SolutionDir)Libraries\ServerCore\$(Configuration)\
- 일반 -> 중간 디렉터리
$(SolutionDir)Intermediate\ServerCore\$(Configuration)\
- C/C++ -> 일반 -> 추가 포함 디렉터리
$(SolutionDir)ServerCore\
- 링커 -> 일반 -> 추가 라이브러리 디렉터리
$(SolutionDir)Libraries\ServerCore\
- pch 아래 코드 추가 ----------------------------------------
#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerCore.lib")
#else
#pragma comment(lib, "Release\\ServerCore.lib")
#endif
---------------------------------------------------------

* Server
- 일반 -> 출력 디렉터리
$(SolutionDir)Binaries\$(Platform)\
- 일반 -> 중간 디렉터리
$(SolutionDir)Intermediate\$(Platform)\