#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <chrono>
#include <random>
#include <conio.h>
#include <iomanip>

using namespace std;
using namespace chrono;

// x, y = 1,2,3
// SET의 세 번째 값
//
// 1,2,3을 각각 F_3의 1,2,0으로 대응시킨다고 생각하면
// z = 6 - x - y (mod 3)
// 단, mod 결과 0은 SET의 값 3으로 표시해야 한다.

int solve(int x, int y) {
    int r = (6 - x - y) % 3;

    if (r == 0) return 3;
    if (r == 1) return 1;
    return 2;
}

int main() {

    const int N = 20;

    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(1, 3);

    vector<double> times;

    int correct = 0;

    cout << "========================================\n";
    cout << "       SET THIRD-VALUE SPEED TEST\n";
    cout << "========================================\n\n";

    cout << "x, y는 1~3 중 하나입니다.\n";
    cout << "정답인 세 번째 값을 1, 2, 3 중 하나로 누르세요.\n";
    cout << "Enter는 필요 없습니다.\n\n";

    cout << "시작하려면 아무 키나 누르세요.";
    _getch();

    cout << "\n\nSTART!\n\n";

    for (int i = 0; i < N; ++i) {

        int x = dist(rng);
        int y = dist(rng);

        int ans = solve(x, y);

        // 문제 표시
        cout << "[" << i + 1 << "/" << N << "] "
             << x << "  " << y << "  ->  ?   ";
        cout.flush();

        // 문제 표시 이후부터 측정
        auto start = steady_clock::now();

        char key = _getch();

        auto end = steady_clock::now();

        double ms =
            duration<double, milli>(end - start).count();

        int input = key - '0';

        // 1~3 이외의 키
        if (input < 1 || input > 3) {
            cout << "INVALID\n";

            // 이 문제는 다시 출제
            --i;
            continue;
        }

        bool ok = (input == ans);

        if (ok)
            ++correct;

        times.push_back(ms);

        cout << input
             << "    "
             << fixed << setprecision(2)
             << ms << " ms"
             << (ok ? "    OK" : "    X")
             << "\n";
    }

    // -------------------------
    // 통계
    // -------------------------

    sort(times.begin(), times.end());

    double mean =
        accumulate(times.begin(), times.end(), 0.0)
        / times.size();

    double median;

    if (times.size() % 2 == 1) {
        median = times[times.size() / 2];
    }
    else {
        median =
            (times[times.size() / 2 - 1]
           + times[times.size() / 2])
           / 2.0;
    }

    double variance = 0.0;

    for (double t : times) {
        variance += (t - mean) * (t - mean);
    }

    variance /= times.size();

    double sd = sqrt(variance);

    cout << "\n";
    cout << "================================\n";
    cout << "RESULT\n";
    cout << "================================\n";

    cout << fixed << setprecision(2);

    cout << "정답률   : "
         << correct << "/" << N
         << " ("
         << 100.0 * correct / N
         << "%)\n";

    cout << "평균     : " << mean   << " ms\n";
    cout << "중앙값   : " << median << " ms\n";
    cout << "표준편차 : " << sd     << " ms\n";
    cout << "최솟값   : " << times.front() << " ms\n";
    cout << "최댓값   : " << times.back()  << " ms\n";

    cout << "================================\n";

    return 0;
}
