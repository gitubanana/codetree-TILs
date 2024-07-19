#include <iostream>
#include <vector>

const int SIZE = 101;
const int dy[] = {0, -1, 0, 1};
const int dx[] = {1, 0, -1, 0};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

bool isCurve[SIZE + 1][SIZE + 1];

struct t_pos
{
    int y, x;
};

struct t_curve
{
    std::vector<int> dirs;
    t_pos cur;

    t_curve(int y, int x, int dir)
        : cur({y, x})
    {
        isCurve[cur.y][cur.x] = true;
        isCurve[cur.y += dy[dir]][cur.x += dx[dir]] = true;
        dirs.push_back(dir);
    }

    inline void makeCurve(void)
    {
        for (int i = dirs.size() - 1; i >= 0; --i)
        {
            int &dir = dirs[i];
            int newDir = (dir + 1) % dirSize;

            cur.y += dy[newDir];
            cur.x += dx[newDir];
            isCurve[cur.y][cur.x] = true;
            dirs.push_back(newDir);
        }
    }
};

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    int curveCnt;

    std::cin >> curveCnt;
    while (curveCnt--)
    {
        int y, x, dir, generation;
        std::cin >> y >> x >> dir >> generation;

        t_curve cur(y, x, dir);
        while (generation--)
        {
            cur.makeCurve();
        }
    }

    int ans = 0;
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            ans += (
                isCurve[y][x]
                && isCurve[y + 1][x]
                && isCurve[y][x + 1]
                && isCurve[y + 1][x + 1]
            );
        }
    }

    std::cout << ans << '\n';
    return 0;
}