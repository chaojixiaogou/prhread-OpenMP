#include<iostream>
#include<cstring>
#include<pthread.h>
#include<Windows.h>
using namespace std;
class TimerCounter
{
public:
    TimerCounter(void);//���캯��
    ~TimerCounter(void);//��������
private:
    LARGE_INTEGER startCount;//��¼��ʼʱ��

    LARGE_INTEGER endCount;//��¼����ʱ��

    LARGE_INTEGER freq;//����CPUʱ��Ƶ��
public:
    double dbTime;//�������е�ʱ�䱣��������

public:
    void Start();//�������ʼ�㴦��ʼ��ʱ
    void Stop();//�����������㴦������ʱ
};
TimerCounter::TimerCounter(void)
{
    QueryPerformanceFrequency(&freq);//��ȡ����CPUʱ��Ƶ��
}
TimerCounter::~TimerCounter(void)
{
}
void TimerCounter::Start()
{
    QueryPerformanceCounter(&startCount);//��ʼ��ʱ
}
void TimerCounter::Stop()
{
    QueryPerformanceCounter(&endCount);//ֹͣ��ʱ

    dbTime = ((double)endCount.QuadPart - (double)startCount.QuadPart) / (double)freq.QuadPart;//��ȡʱ���

}


int n; // ���̴�С
int cnt = 0; // �������
int row1[20][20]; // ���ÿһ�лʺ��λ��
pthread_mutex_t mutex; // ������

struct Task {
    int x; // ��ǰҪ���ûʺ������
};

// �ж��ڵ�x�з��ûʺ��Ƿ�Ϸ�
bool check(int x, int row[]) {
    for (int i = 1; i < x; i++) {
        if (row[i] == row[x] || abs(row[x] - row[i]) == abs(x - i))
            return false;
    }
    return true;
}

// �ݹ麯�������õ�x�еĻʺ�
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

// �̺߳���������ִ��һ��������
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
