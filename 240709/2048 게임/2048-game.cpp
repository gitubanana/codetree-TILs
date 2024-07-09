#include <iostream>
#include <cstring>

const int EMPTY = 0;
const int MAX_SIZE = 20;
const int DEPTH_LIMIT = 5;

int size;
int maxBlock = 0;

struct t_board
{
    int map[MAX_SIZE][MAX_SIZE + 1];

    inline void rotate(void)
    {
        static int temp[MAX_SIZE][MAX_SIZE + 1];

        memcpy(temp, map, sizeof(map));
        for (int y = 0; y < size; ++y)
        {
            for (int x = 0; x < size; ++x)
            {
                map[y][x] = temp[size - x - 1][y];
            }
        }
    }

    inline void left(void)
    {
        for (int y = 0; y < size; ++y)
        {
            int plusX = 0;

            for (; plusX < size; ++plusX)
            {
                int &cur = map[y][plusX];
                if (cur == EMPTY)
                    continue ;

                int cmpX = plusX + 1;
                while (cmpX < size)
                {
                    if (map[y][cmpX] != EMPTY)
                        break ;

                    ++cmpX;
                }

                int &cmp = map[y][cmpX];
                if (cur == cmp)
                {
                    cur <<= 1;
                    cmp = 0;
                    plusX = cmpX;
                }
                else
                {
                    plusX = cmpX - 1;
                }
            }

            int leftX = 0;
            while (leftX < size && map[y][leftX] != EMPTY)
            {
                ++leftX;
            }

            for (int x = leftX + 1; x < size; ++x)
            {
                if (map[y][x] == EMPTY)
                    continue ;

                map[y][leftX++] = map[y][x];
                map[y][x] = EMPTY;
            }
        }
    }
};

int getMax(const t_board &cur)
{
    int ret = 0;

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            ret = std::max(ret, cur.map[y][x]);
        }
    }
    return (ret);
}

void    backTracking(t_board &cur, int depth=0)
{
    if (depth == DEPTH_LIMIT)
    {
        maxBlock = std::max(maxBlock, getMax(cur));
        return ;
    }


    int dirCnt = 4;
    while (dirCnt--)
    {
        t_board next = cur;

        next.left();
        backTracking(next, depth + 1);
        cur.rotate();
    }
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    t_board init;

    std::cin >> size;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cin >> init.map[y][x];
        }
    }

    backTracking(init);
    std::cout << maxBlock << '\n';
    return 0;
}