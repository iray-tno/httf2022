#include <algorithm>
#include <utility>
#include <vector>
#include <iostream>
#include <array>
#include <queue>
#include <numeric>
#include <map>
#include <memory>
#include <cmath>
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

    // 隣接リスト
    vector<vector<int>> graph(tasks.size());
    vector<int> indegree(tasks.size(), 0);

    for (const auto& r : taskRelations) {
        int first = r.first - 1, second = r.second - 1;
        graph[first].push_back(second);
        indegree[second] += 1;
    }

    queue<int> readyTasks;
    for (int taskId = 0; taskId < indegree.size(); taskId++) {
        if (indegree[taskId] == 0) readyTasks.push(taskId + 1);
    }

    int startedTasks = 0, finishedTasks = 0;
    queue<int> availableMembers;
    for (int i = 0; i < 20; ++i) availableMembers.push(i + 1);

    map<int, int> assignedTaskMap;
    while (finishedTasks < tasks.size()) {
        int startingTasks = min(readyTasks.size(), availableMembers.size());

        cout << startingTasks;
        for (int i = 0; i < startingTasks; ++i) {
            int memberId = availableMembers.front(); availableMembers.pop();
            int taskId = readyTasks.front(); readyTasks.pop();

            assignedTaskMap[memberId] = taskId;
            startedTasks++;
            cout << " " << memberId << " " << taskId;
        }
        cout << endl;

        int finishedMembers;
        cin >> finishedMembers;
        if (finishedMembers < 0) break;

        for (int i = 0; i < finishedMembers; ++i) {
            int finishedMemberId;
            cin >> finishedMemberId;
            int finishedTaskId = assignedTaskMap[finishedMemberId];

            availableMembers.push(finishedMemberId);
            for (auto& u : graph[finishedTaskId - 1]) {
                indegree[u] -= 1;
                if (indegree[u] == 0) readyTasks.push(u + 1);
            }
            finishedTasks++;
        }
    }

    return 0;
}
