* ServerCore
- �Ϲ� -> ��� ���͸�
$(SolutionDir)Libraries\ServerCore\$(Configuration)\
- �Ϲ� -> �߰� ���͸�
$(SolutionDir)Intermediate\ServerCore\$(Configuration)\
- C/C++ -> �Ϲ� -> �߰� ���� ���͸�
$(SolutionDir)ServerCore\
- ��Ŀ -> �Ϲ� -> �߰� ���̺귯�� ���͸�
$(SolutionDir)Libraries\ServerCore\
- pch �Ʒ� �ڵ� �߰� ----------------------------------------
#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerCore.lib")
#else
#pragma comment(lib, "Release\\ServerCore.lib")
#endif
---------------------------------------------------------

* Server
- �Ϲ� -> ��� ���͸�
$(SolutionDir)Binaries\$(Platform)\
- �Ϲ� -> �߰� ���͸�
$(SolutionDir)Intermediate\$(Platform)\