#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#define CURMAP map[curMapIdx]
#define NEXTMAP map[nextMapIdx]

enum e_space
{
    EMPTY = 0
};

struct t_pos
{
    int y, x;
};

const int SIZE = 5;
const int LINK_CNT = 3;
const int dy[] = {-1, 1, 0, 0};
const int dx[] = {0, 0, -1, 1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int curMapIdx, nextMapIdx;
int map[2][SIZE][SIZE];
bool visited[SIZE][SIZE];
std::queue<int> relics;

inline bool inRange(const t_pos &pos)
{
    return ((0 <= pos.y && pos.y < SIZE)
            && (0 <= pos.x && pos.x < SIZE));
}

void    printMap(std::string str)
{
    std::cout << "=== " << str << " ===" << '\n';
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            std::cout << CURMAP[y][x] << ' ';
        }
        std::cout << '\n';
    }
}

void    dfs(const t_pos &cur, std::vector<t_pos> &allPos)
{
    allPos.push_back(cur);
    visited[cur.y][cur.x] = true;

    for (int dir = 0; dir < dirSize; ++dir)
    {
        t_pos next = {
            cur.y + dy[dir],
            cur.x + dx[dir]
        };

        if (!inRange(next)
            || visited[next.y][next.x]
            || CURMAP[cur.y][cur.x] != CURMAP[next.y][next.x])
            continue ;

        dfs(next, allPos);
    }
}

void    rotate90(int startY, int startX)
{
    static const int SQAURE_SIZE = 3;

    nextMapIdx = curMapIdx ^ 1;

    memcpy(NEXTMAP, CURMAP, sizeof(NEXTMAP));
    for (int y = 0; y < SQAURE_SIZE; ++y)
    {
        for (int x = 0; x < SQAURE_SIZE; ++x)
        {
            NEXTMAP[startY + y][startX + x] =
                CURMAP[startY + SQAURE_SIZE - 1 - x][startX + y];
        }
    }

    curMapIdx = nextMapIdx;
}

void    getMapRelics(std::vector<t_pos> &removePos)
{
    memset(visited, false, sizeof(visited));
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            if (visited[y][x])
                continue ;

            size_t prevSize = removePos.size();

            dfs({y, x}, removePos);

            size_t addedCnt = removePos.size() - prevSize;
            if (addedCnt < LINK_CNT)
            {
                while (addedCnt--)
                {
                    removePos.pop_back();
                }
            }
        }
    }
}

int removeBestMap(void)
{
    static int cur, next;
    static std::vector<t_pos> toRemove[2];

    int rY, rX;
    int rotateCnt = -1;

    toRemove[cur].clear();
    for (int x = 0; x < SIZE - 2; ++x)
    {
        for (int y = 0; y < SIZE - 2; ++y)
        {
            for (int rCnt = 1; rCnt <= 4; ++rCnt)
            {
                rotate90(y, x);
                if (rCnt == 4)
                    continue ;

                next = cur ^ 1;
                std::vector<t_pos> &curRemove = toRemove[cur];
                std::vector<t_pos> &nextRemove = toRemove[next];

                nextRemove.clear();
                getMapRelics(nextRemove);
                if (curRemove.size() < nextRemove.size()
                    || (curRemove.size() == nextRemove.size() && rotateCnt > rCnt))
                {
                    // std::cout << "cur : " << y << ", " << x << '\n';
                    // std::cout << "size : " << nextRemove.size() << '\n';
                    // printMap("after rotate90");

                    rotateCnt = rCnt;
                    rY = y, rX = x;
                    cur = next;
                }
            }
        }
    }

    if (rotateCnt == -1)
        return (0);

    while (rotateCnt--)
    {
        rotate90(rY, rX);
    }
    for (const t_pos &pos : toRemove[cur])
    {
        CURMAP[pos.y][pos.x] = EMPTY;
    }

    return (toRemove[cur].size());
}

void    fillEmpty(void)
{
    for (int x = 0; x < SIZE; ++x)
    {
        for (int y = SIZE - 1; y >= 0; --y)
        {
            int &curSpace = CURMAP[y][x];
            if (curSpace != EMPTY)
                continue ;

            if (relics.empty())
            {
                std::cout << "error" << '\n';
                exit(0);
            }

            curSpace = relics.front();
            relics.pop();
        }
    }
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    int searchCnt, relicCnt;
    std::vector<t_pos> removePos;

    std::cin >> searchCnt >> relicCnt;
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            std::cin >> CURMAP[y][x];
        }
    }
    while (relicCnt--)
    {
        int relic;

        std::cin >> relic;
        relics.push(relic);
    }

    while (searchCnt--)
    {
        int curValue = removeBestMap();
        if (curValue == 0)
            break ;

        // printMap("after removeBestMap");
        while (true)
        {
            fillEmpty();
            // printMap("after fillEmpty");

            removePos.clear();
            getMapRelics(removePos);
            if (removePos.empty())
                break ;

            curValue += removePos.size();
            for (const t_pos &pos : removePos)
            {
                CURMAP[pos.y][pos.x] = EMPTY;
            }
        }

        std::cout << curValue << ' ';
    }
    std::cout << '\n';

    return 0;
}