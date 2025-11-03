#include <iostream>
using namespace std;
//#include <unordered_map>
//#include <algorithm>
#include <windows.h>

int cache[50][50];

int combination(int n, int r)
{
    // 기저 사례
    if (r == 0 || n == r)
        return 1;

    return combination(n - 1, r - 1) + combination(n - 1, r);
}
int combination2(int n, int r)
{
    // 기저 사례
    if (r == 0 || n == r)
        return 1;

    // 이미 답을 구한 적 있으면 바로 반환
    int& ret = cache[n][r];
    if (ret != -1)
        return ret;

    return ret = combination(n - 1, r - 1) + combination(n - 1, r);
}

// +0 집행검
// 무기 강화 주문서 -> +1, +2, +3 중 하나

// +9 집행검 뜨는 경우 수는?
// ex) +1 +2 +3 +4 ... +9
// ex) +3 +6 +9
// ex) +1 +3 +4 ... +9
int N = 9;
int cache2[100];

int Enchant(int num)
{
    // 기저 사례
    if (num > N)
        return 0;
    if (num == N)
        return 1;

    // 캐시
    int& ret = cache2[num];
    if (ret != -1)
        return ret;

    // 적용
    return ret = Enchant(num + 1) + Enchant(num + 2) + Enchant(num + 3);
}

int main()
{
    __int64 start = GetTickCount64();

    // ** 동적계획법 사용법 **
    // 1. 그냥...
    //int lotto = combination(45, 6);
    // 2. 동적계획법 적용
    //::memset(cache, -1, sizeof(cache));
    //int lotto = combination2(45, 6);

    // ** 집행검 예제 **
    ::memset(cache, -1, sizeof(cache));
    int ret = Enchant(8);
    cout << ret << endl;

    __int64 end = GetTickCount64();
    cout << end - start << "ms" << endl;
}