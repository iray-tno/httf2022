#include <algorithm>
#include <utility>
#include <vector>
#include <iostream>
#include <array>
#include <numeric>
#include <memory>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <sys/time.h>

#ifdef LOCAL
// #define MEASURE_TIME
#define DEBUG
#else
#define NDEBUG
// #define DEBUG
#endif
#include <cassert>

using namespace std;


int main() {
    int n, m, k, r;

    cin >> n >> m >> k >> r;

    vector<vector<int>> tasks(n, vector<int>(k, 0));
    vector<pair<int, int>> taskRelations(r, pair<int, int>());
    for (int i = 0; i < n; ++i) for (int j = 0; j < k; ++j) cin >> tasks[i][j];
    for (int i = 0; i < r; ++i) cin >> taskRelations[i].first >> taskRelations[i].second;

    cout << tasks[0][1] << endl;
    return 0;
}
