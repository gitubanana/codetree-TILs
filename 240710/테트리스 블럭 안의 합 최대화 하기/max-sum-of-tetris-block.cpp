// #include <iostream>
// #include <climits>

// const int MAX_SIZE = 200;
// const int DEPTH_LIMIT = 3;
// const int dy[] = {-1, 1, 0, 0};
// const int dx[] = {0, 0, -1, 1};
// const int dirSize = sizeof(dy) / sizeof(dy[0]);

// int maxSum;
// int ySize, xSize;
// int map[MAX_SIZE][MAX_SIZE];
// bool visited[MAX_SIZE][MAX_SIZE];

// void    backTracking(int y, int x, int curSum, int depth=0)
// {
//     if (depth == DEPTH_LIMIT)
//     {
//         maxSum = std::max(maxSum, curSum);
//         return ;
//     }

//     visited[y][x] = true;
//     for (int dir = 0; dir < dirSize; ++dir)
//     {
//         int nextY = y + dy[dir];
//         int nextX = x + dx[dir];

//         if (!(0 <= nextY && nextY < ySize)
//             || !(0 <= nextX && nextX < xSize)
//             || visited[nextY][nextX])
//             continue ;

//         backTracking(
//             nextY,
//             nextX,
//             curSum + map[nextY][nextX],
//             depth + 1
//         );
//     }
//     visited[y][x] = false;
// }

// void    calFlower(int y, int x)
// {
//     int sum = 0;
//     int leafCnt = 4;
//     int min = INT_MAX;

//     for (int dir = 0; dir < dirSize; ++dir)
//     {
//         int plusY = y + dy[dir];
//         int plusX = x + dx[dir];

//         if (!(0 <= plusY && plusY < ySize)
//             || !(0 <= plusX && plusX < xSize))
//         {
//             if (leafCnt == 3)
//                 return ;

//             --leafCnt;
//             continue ;
//         }

//         const int &plus = map[plusY][plusX];

//         min = std::min(min, plus);
//         sum += plus;
//     }

//     maxSum = std::max(
//         maxSum,
//         map[y][x] + (leafCnt == 3 ? sum : sum - min)
//     );
// }

// int main()
// {
//     std::cin.tie(0)->sync_with_stdio(0);

//     std::cin >> ySize >> xSize;
//     for (int y = 0; y < ySize; ++y)
//     {
//         for (int x = 0; x < xSize; ++x)
//         {
//             std::cin >> map[y][x];
//         }
//     }

//     for (int y = 0; y < ySize; ++y)
//     {
//         for (int x = 0; x < xSize; ++x)
//         {
//             backTracking(y, x, map[y][x]);
//             calFlower(y, x);
//         }
//     }

//     std::cout << maxSum << '\n';
//     return 0;
// }

#include <iostream>
#include <algorithm>
#include <vector>

#define MAX_NUM 200
#define DIR_NUM 4

using namespace std;

int n, m;
int grid[MAX_NUM][MAX_NUM];
int max_sum;    

vector<pair<int, int> > visited_pos;

int dx[DIR_NUM] = {-1, 1, 0, 0};
int dy[DIR_NUM] = {0, 0, -1, 1};

// 주어진 위치가 격자를 벗어나는지 여부를 반환합니다.
int InRange(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m;
}

// 주어진 위치로 이동할 수 있는지 여부를 확인합니다.
int CanGo(int x, int y) {
    if(!InRange(x, y))
        return false;
    
    pair<int, int> pos = make_pair(x, y);
    for(int i = 0; i < (int) visited_pos.size(); i++)
        if(visited_pos[i] == pos)
            return false;

    return true;
}

// 주어진 위치에 대하여 가능한 모든 모양을 탐색하며 최대 합을 반환합니다.
void GetMaxSum(int cnt, int sum) {
    if(cnt == 4) {
        max_sum = max(max_sum, sum);
        return;
    }

    // 지금까지 방문한 위치들을 탐색하면서 인접한 위치로 이동할 수 있는지를 확인합니다.
    for(int i = 0; i < (int) visited_pos.size(); i++) {
        int x = visited_pos[i].first;
        int y = visited_pos[i].second;
        for(int dir = 0; dir < DIR_NUM; dir++) {
            int new_x = x + dx[dir];
            int new_y = y + dy[dir];

            if(CanGo(new_x, new_y)) {
                visited_pos.push_back(make_pair(new_x, new_y));
                GetMaxSum(cnt + 1, sum + grid[new_x][new_y]);
                visited_pos.pop_back();
            }
        }
    }

    return;
}

int main() {              
                          
    // 입력:
    cin >> n >> m;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            cin >> grid[i][j];

    // 격자의 각 위치에 대하여 탐색하여줍니다.
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            visited_pos.push_back(make_pair(i, j));
            GetMaxSum(1, grid[i][j]);
            visited_pos.pop_back();
        }
    }

    cout << max_sum;
}