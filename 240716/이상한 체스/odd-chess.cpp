#include <iostream>
#include <vector>
#include <climits>

struct t_pos
{
    int y, x;
};

template <typename T>
using t_vec = std::vector<T>;

using t_2d = t_vec<t_vec<int>>;

const int MAX_SIZE = 8;
const int ENEMY = 6;
const int dy[] = {-1, 0, 1, 0};
const int dx[] = {0, 1, 0, -1};
t_2d dirsByType[] = {
    {},
    {{0}, {1}, {2}, {3}},
    {{0, 2}, {1, 3}},
    {{0, 1}, {1, 2}, {2, 3}, {3, 0}},
    {{3, 0, 1}, {0, 1, 2}, {1, 2, 3}, {2, 3, 0}},
    {{0, 1, 2, 3}},
};

int min = INT_MAX;
int ySize, xSize, totalCnt;
int map[MAX_SIZE][MAX_SIZE];
int cnt[MAX_SIZE][MAX_SIZE];

inline bool isMyHorse(const int &space)
{
    return (1 <= space && space <= 5);
}

inline bool isBound(const t_pos &pos)
{
    return ((0 <= pos.y && pos.y < ySize)
            && (0 <= pos.x && pos.x < xSize));
}

void    backTracking(int y=0, int x=0, int totalGo=0)
{
    min = std::min(min, totalCnt - totalGo);

    while (true)
    {
        if (x == xSize)
        {
            x = 0;
            ++y;
            if (y == ySize)
                break ;
        }

        int &space = map[y][x];
        if (isMyHorse(space))
        {
            std::vector<t_pos> checked;

            for (const t_vec<int> &dirs : dirsByType[space])
            {
                checked.clear();
                int plus = (cnt[y][x] == 0);

                cnt[y][x] += space;
                checked.push_back({y, x});
                for (const int &dir : dirs)
                {
                    t_pos cur = {
                        y + dy[dir],
                        x + dx[dir]
                    };

                    while (isBound(cur))
                    {
                        int &curSpace = map[cur.y][cur.x];
                        int &curCnt = cnt[cur.y][cur.x];
                        if (curSpace == ENEMY)
                            break ;

                        if (curCnt == 0)
                        {
                            ++plus;
                            curCnt += space;
                            checked.push_back(cur);
                        }
                        cur.y += dy[dir];
                        cur.x += dx[dir];
                    }
                }

                backTracking(y, x + 1, totalGo + plus);

                for (const t_pos &pos : checked)
                {
                    cnt[pos.y][pos.x] -= space;
                }
            }
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
            int &space = map[y][x];

            std::cin >> space;
            totalCnt += (space != ENEMY);
        }
    }

    backTracking();

    std::cout << min << '\n';
    return 0;
}