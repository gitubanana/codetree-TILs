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
std::vector<t_pos> horses;

inline bool isBound(const t_pos &pos)
{
    return ((0 <= pos.y && pos.y < ySize)
            && (0 <= pos.x && pos.x < xSize));
}

void    backTracking(int depth=0, int totalGo=0)
{
    if (depth == horses.size())
    {
        min = std::min(min, totalCnt - totalGo);
        return ;
    }
    
    const int &y = horses[depth].y;
    const int &x = horses[depth].x;
    const int &space = map[y][x];

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

        backTracking(depth + 1, totalGo + plus);

        for (const t_pos &pos : checked)
        {
            cnt[pos.y][pos.x] -= space;
        }
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
            switch (space)
            {
                case ENEMY:
                    break ;
                default:
                    ++totalCnt;
                    if (space != 0)
                    {
                        horses.push_back({y, x});
                    }
                    break ;
            }
        }
    }

    backTracking();

    std::cout << min << '\n';
    return 0;
}