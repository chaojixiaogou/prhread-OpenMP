#include<iostream>
#include<cstring>
#include<omp.h>
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
