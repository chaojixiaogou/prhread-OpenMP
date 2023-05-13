#include<iostream>
#include<cstring>
#include<omp.h>
#include<Windows.h>
#include <immintrin.h>
using namespace std;
class TimerCounter
{
public:
    TimerCounter(void);//构造函数
    ~TimerCounter(void);//析构函数
private:
    LARGE_INTEGER startCount;//记录开始时间

    LARGE_INTEGER endCount;//记录结束时间

    LARGE_INTEGER freq;//本机CPU时钟频率
public:
    double dbTime;//程序运行的时间保存在这里

public:
    void Start();//被测程序开始点处开始计时
    void Stop();//被测程序结束点处结束计时
};
TimerCounter::TimerCounter(void)
{
    QueryPerformanceFrequency(&freq);//获取主机CPU时钟频率
}
TimerCounter::~TimerCounter(void)
{
}
void TimerCounter::Start()
{
    QueryPerformanceCounter(&startCount);//开始计时
}
void TimerCounter::Stop()
{
    QueryPerformanceCounter(&endCount);//停止计时

    dbTime = ((double)endCount.QuadPart - (double)startCount.QuadPart) / (double)freq.QuadPart;//获取时间差

}

int n; // 棋盘大小
int cnt = 0; // 解的数量
int row1[20][20]; // 存放每一行皇后的位置

// 判断在第x行放置皇后是否合法
bool check(int x, int row[]) {
    if (x <= 8) {
        for (int i = 1; i < x; i++) {
            if (row[i] == row[x] || abs(row[x] - row[i]) == abs(x - i))
                return false;
        }
        return true;
    }
    else {
        __m256i xmm1, xmm2, xmm3, xmm4;
        xmm2 = _mm256_set1_epi32(row[x]);
        xmm4 = _mm256_set1_epi32(x);
        xmm1 = _mm256_loadu_si256((__m256i*) & row[1]);
        xmm3 = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
        __m256i xmm5 = _mm256_cmpeq_epi32(xmm1, xmm2);
        int test1 = _mm256_testz_si256(xmm5, xmm5);
        if (test1 == 0)
            return false;
        xmm3 = _mm256_sub_epi32(xmm4, xmm3);
        xmm1 = _mm256_sub_epi32(xmm2, xmm1);
        xmm1 = _mm256_abs_epi32(xmm1);
        xmm1 = _mm256_cmpeq_epi32(xmm1, xmm3);
        int test = _mm256_testz_si256(xmm1, xmm1);
        if (test == 0)
            return false;
        for (int i = 9; i < x; i++) {
            if (row[i] == row[x] || abs(row[x] - row[i]) == abs(x - i))
                return false;
        }
        return true;
    }
}

// 递归函数，放置第x行的皇后
void dfs(int x, int row[]) {
    if (x > n) {
#pragma omp atomic
        cnt++;
        return;
    }
    for (int i = 1; i <= n; i++) {
        row[x] = i;
        if (check(x, row)) {
            dfs(x + 1, row);
        }
    }
}

int main() {
    cin >> n;
    TimerCounter tc;
    tc.Start();
#pragma omp parallel for num_threads(n)
    for (int i = 0; i < n; i++) {
        row1[i + 1][1] = i + 1;
        //row1[i + 2][1] = i + 2;
        dfs(2, row1[i + 1]);
        //dfs(2, row1[i + 2]);
        /*for (int j = 0; j < n / 2; j++) {
            row1[i + 1 + j][1] = i + 1 + j;
            dfs(2, row1[i + 1 + j]);
        }*/
    }
    tc.Stop();
    cout << cnt << endl;
    cout << tc.dbTime;
    return 0;
}
