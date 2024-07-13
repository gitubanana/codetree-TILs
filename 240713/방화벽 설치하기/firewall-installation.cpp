#include <iostream>
#include <cstring>
#include <vector>
#include <queue>

enum e_space
{
    EMPTY = 0,
    FIREWALL,
    FIRE
};

struct t_pos
{
    int y, x;
};

const int MAX_SIZE = 8;
const int DEPTH_LIMIT = 3;
const int dy[] = {-1, 1, 0, 0};
const int dx[] = {0, 0, -1, 1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int ans, ySize, xSize;
int totalEmptyCnt;
int map[MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE];
std::vector<t_pos> firePos;

int bfs(void)
{
    int curEmptyCnt = totalEmptyCnt - DEPTH_LIMIT;
    std::queue<t_pos> q;

    memset(visited, false, sizeof(visited));
    for (const t_pos &pos : firePos)
    {
        visited[pos.y][pos.x] = true;
        q.push(pos);
    }
    while (!q.empty())
    {
        const t_pos cur = q.front();
        q.pop();

        for (int dir = 0; dir < dirSize; ++dir)
        {
            t_pos next = {
                cur.y + dy[dir],
                cur.x + dx[dir]
            };

            if (!(0 <= next.y && next.y < ySize)
                || !(0 <= next.x && next.x < xSize)
                || map[next.y][next.x] != EMPTY
                || visited[next.y][next.x])
                continue ;

            --curEmptyCnt;
            visited[next.y][next.x] = true;
            q.push(next);
        }
    }

    return (curEmptyCnt);
}

void    backTracking(int y=0, int x=0, int depth=0)
{
    if (depth == DEPTH_LIMIT)
    {
        ans = std::max(ans, bfs());
        return ;
    }

    while (true)
    {
        if (x >= xSize)
        {
            x = 0;
            ++y;
            if (y >= ySize)
                break ;
        }

        int &curSpace = map[y][x];
        if (curSpace == EMPTY)
        {
            curSpace = FIREWALL;
            backTracking(y, x + 1, depth + 1);
            curSpace = EMPTY;
        }

        ++x;
    }
}

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    std::cin >> ySize >> xSize;
    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            int &curSpace = map[y][x];

            std::cin >> curSpace;
            switch (curSpace)
            {
                case FIRE:
                    firePos.push_back({y, x});
                    break ;
                case EMPTY:
                    ++totalEmptyCnt;
                    break ;
            }
        }
    }

    backTracking();
    std::cout << ans << '\n';
    return 0;
}