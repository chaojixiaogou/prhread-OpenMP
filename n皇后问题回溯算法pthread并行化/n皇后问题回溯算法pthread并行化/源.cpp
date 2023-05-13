#include<iostream>
#include<cstring>
#include<pthread.h>
#include<Windows.h>
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
pthread_mutex_t mutex; // 互斥锁

struct Task {
    int x; // 当前要放置皇后的行数
};

// 判断在第x行放置皇后是否合法
bool check(int x, int row[]) {
    for (int i = 1; i < x; i++) {
        if (row[i] == row[x] || abs(row[x] - row[i]) == abs(x - i))
            return false;
    }
    return true;
}

// 递归函数，放置第x行的皇后
void dfs(int x, int row[]) {
    if (x > n) {
        pthread_mutex_lock(&mutex);
        cnt++;
        pthread_mutex_unlock(&mutex);
        return;
    }
    for (int i = 1; i <= n; i++) {
        row[x] = i;
        if (check(x, row)) {
            dfs(x + 1, row);
        }
    }
}

// 线程函数，用来执行一部分任务
void* worker(void* arg) {
    Task* task = (Task*)arg;
    row1[task->x][1] = task->x;
    //row1[task->x+1][1] = task->x+1;
    dfs(2, row1[task->x]);
    //dfs(2, row1[task->x + 1]);
    /*for (int i = 0; i < n / 2; i++) {
        row1[task->x + i][1] = task->x + i;
        dfs(2, row1[task->x + i]);
    }*/
    delete task;
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    cin >> n;
    int num_threads = n;
    pthread_t threads[20];
    TimerCounter tc;
    tc.Start();
    for (int i = 0; i < n; i++) {
        Task* task = new Task();
        task->x = i + 1;
        pthread_create(&threads[i], NULL, worker, task);
    }
    for (int i = 0; i < n; i+=n/2) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    tc.Stop();
    cout << cnt << endl;
    cout << tc.dbTime;
    return 0;
}
