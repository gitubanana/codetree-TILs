#include <iostream>
#include <climits>

const int MAX_SIZE = 200;
const int DEPTH_LIMIT = 3;
const int dy[] = {-1, 1, 0, 0};
const int dx[] = {0, 0, -1, 1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int maxSum;
int ySize, xSize;
int map[MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE];

void    backTracking(int y, int x, int curSum, int depth=0)
{
    if (depth == DEPTH_LIMIT)
    {
        maxSum = std::max(maxSum, curSum);
        return ;
    }

    visited[y][x] = true;
    for (int dir = 0; dir < dirSize; ++dir)
    {
        int nextY = y + dy[dir];
        int nextX = x + dx[dir];

        if (!(0 <= nextY && nextY < ySize)
            || !(0 <= nextX && nextX < xSize)
            || visited[nextY][nextX])
            continue ;

        backTracking(
            nextY,
            nextX,
            curSum + map[nextY][nextX],
            depth + 1
        );
    }
    visited[y][x] = false;
}

void    calFlower(int y, int x)
{
    int sum = 0;
    int leafCnt = 4;
    int min = INT_MAX;

    for (int dir = 0; dir < dirSize; ++dir)
    {
        int plusY = y + dy[dir];
        int plusX = x + dx[dir];

        if (!(0 <= plusY && plusY < ySize)
            || !(0 <= plusX && plusX < xSize))
        {
            if (leafCnt == 3)
                return ;

            --leafCnt;
            continue ;
        }

        const int &plus = map[plusY][plusX];

        min = std::min(min, plus);
        sum += plus;
    }

    maxSum = std::max(
        maxSum,
        map[y][x] + (leafCnt == 3 ? sum : sum - min)
    );
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    std::cin >> ySize >> xSize;
    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            std::cin >> map[y][x];
        }
    }

    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            backTracking(y, x, map[y][x]);
            calFlower(y, x);
        }
    }

    std::cout << maxSum << '\n';
    return 0;
}