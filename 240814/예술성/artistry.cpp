#include <iostream>
#include <unordered_map>
#include <cstring>
#include <vector>
#define ID first
#define ADJACENT second
#define CURMAP map[curIdx]
#define NEXTMAP map[nextIdx]

struct t_pos
{
    int y, x;
};

const int MAX_SIZE = 29;
const int MAX_NUM = 10;
const int EMPTY = -1;
const int dy[] = {1, 0, -1, 0};
const int dx[] = {0, 1, 0, -1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int size, id;
int curIdx, nextIdx;
int map[2][MAX_SIZE][MAX_SIZE];
int groupId[MAX_SIZE][MAX_SIZE];
std::vector<int> groupSize;
int groupNum[MAX_SIZE * MAX_SIZE];
std::unordered_map<int, int> adjacentCnt; // id, cnt

inline bool inRange(int y, int x)
{
    return ((0 <= y && y < size)
            && (0 <= x && x < size));
}

void    dfs(int y, int x, int &cnt)
{
    ++cnt;
    groupId[y][x] = id;

    for (int dir = 0; dir < dirSize; ++dir)
    {
        int nextY = y + dy[dir];
        int nextX = x + dx[dir];

        if (!inRange(nextY, nextX)
            || CURMAP[y][x] != CURMAP[nextY][nextX]
            || groupId[nextY][nextX] != EMPTY)
            continue ;

        dfs(nextY, nextX, cnt);
    }
}

void countAdjacent(int y, int x)
{
    int curId = groupId[y][x];

    groupId[y][x] = EMPTY;
    for (int dir = 0; dir < dirSize; ++dir)
    {
        int nextY = y + dy[dir];
        int nextX = x + dx[dir];

        if (!inRange(nextY, nextX)
            || groupId[nextY][nextX] == EMPTY)
            continue ;

        int &nextId = groupId[nextY][nextX];
        if (curId == nextId)
        {
            countAdjacent(nextY, nextX);
        }
        else
        {
            ++adjacentCnt[nextId];
        }
    }
}

int getScore(void)
{
    id = 0;
    groupSize.clear();
    memset(groupId, EMPTY, sizeof(groupId));
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            if (groupId[y][x] != EMPTY)
                continue ;

            int cnt = 0;

            dfs(y, x, cnt);
            groupSize.push_back(cnt);
            groupNum[id] = CURMAP[y][x];
            ++id;
        }
    }

    int score = 0;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int curId = groupId[y][x];
            if (curId == EMPTY)
                continue;

            int &aCnt = groupSize[curId];
            int &aNum = groupNum[curId];

            adjacentCnt.clear();   
            countAdjacent(y, x);
            for (std::pair<const int, int> &cur : adjacentCnt)
            {
                int &bCnt = groupSize[cur.ID];
                int &bNum = groupNum[cur.ID];

                score += (aCnt + bCnt) * aNum * bNum * cur.ADJACENT;
            }
        }
    }

    return (score);
}

void    rotateCross(void)
{
    static const int crossSize = size / 2;
    static const t_pos starts[] = {
        {0, crossSize},
        {crossSize, 0},
        {size - 1, crossSize},
        {crossSize, size - 1}
    };

    nextIdx = curIdx ^ 1;

    memcpy(NEXTMAP, CURMAP, sizeof(NEXTMAP));
    for (int dir = 0; dir < dirSize; ++dir)
    {
        int remainSize = crossSize;
        int nextDir = (dir + 1) % dirSize;
        t_pos cur = starts[dir];
        t_pos next = starts[nextDir];

        while (remainSize--)
        {
            NEXTMAP[next.y][next.x] = CURMAP[cur.y][cur.x];
            cur.y += dy[dir];
            cur.x += dx[dir];
            next.y += dy[nextDir];
            next.x += dx[nextDir];
        }
    }

    curIdx = nextIdx;
}

void    rotateSquare(int idx)
{
    static const int squareSize = size / 2;
    static const t_pos starts[] = {
        {0, 0},
        {0, squareSize + 1},
        {squareSize + 1, 0},
        {squareSize + 1, squareSize + 1}
    };
    static const t_pos ends[] = {
        {squareSize - 1, squareSize - 1},
        {squareSize - 1, size - 1},
        {size - 1, squareSize - 1},
        {size - 1, size - 1}
    };

    const t_pos &start = starts[idx];
    const t_pos &end = ends[idx];
    for (int y = 0; y < squareSize; ++y)
    {
        for (int x = 0; x < squareSize; ++x)
        {
            NEXTMAP[start.y + y][start.x + x] = CURMAP[end.y - x][start.x + y];
        }
    }
}

void    rotateSquares(void)
{
    nextIdx = curIdx ^ 1;

    memcpy(NEXTMAP, CURMAP, sizeof(NEXTMAP));
    for (int i = 0; i < dirSize; ++i)
    {
        rotateSquare(i);
    }

    curIdx = nextIdx;
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    std::cin >> size;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cin >> CURMAP[y][x];
        }
    }

    int rotateCnt = 3;
    int totalScore = getScore();

    while (rotateCnt--)
    {
        rotateCross();
        rotateSquares();
        totalScore += getScore();
    }

    std::cout << totalScore << '\n';
    return 0;
}