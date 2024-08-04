#include <iostream>
#include <vector>

const int dirSize[] = {0, 0, 1, 1};
const std::vector<int> dy[] = {
    {},
    {},
    {0},
    {1}
};
const std::vector<int> dx[] = {
    {},
    {},
    {1},
    {0}
};

struct t_map
{
    static const int Y_SIZE = 6;
    static const int X_SIZE = 4;

    int score;
    bool isBlock[Y_SIZE][X_SIZE];

    inline bool isOk(int type, int y, int x)
    {
        if (y >= Y_SIZE)
            return (false);

        if (isBlock[y][x])
            return (false);

        for (int dir = 0; dir < dirSize[type]; ++dir)
        {
            int nextY = y + dy[type][dir];
            int nextX = x + dx[type][dir];
            if (nextY >= Y_SIZE)
                return (false);

            if (isBlock[nextY][nextX])
                return (false);
        }
        return (true);
    }

    void    putBlock(int type, int x)
    {
        int y = 1;

        while (isOk(type, y, x))
        {
            ++y;
        }
        --y;

        isBlock[y][x] = true;
        for (int dir = 0; dir < dirSize[type]; ++dir)
        {
            isBlock[y + dy[type][dir]][x + dx[type][dir]] = true;
        }
    }

    void    removeRow(int y)
    {
        while (y > 0)
        {
            for (int x = 0; x < X_SIZE; ++x)
            {
                isBlock[y][x] = isBlock[y - 1][x];
            }
            --y;
        }

        for (int x = 0; x < X_SIZE; ++x)
        {
            isBlock[0][x] = false;
        }
    }

    void    doCmd(int type, int x)
    {
        putBlock(type, x);

        for (int y = Y_SIZE - 1; y >= 2; --y)
        {
            bool completed = true;
            for (int x = 0; x < X_SIZE; ++x)
            {
                if (!isBlock[y][x])
                {
                    completed = false;
                    break ;
                }
            }

            if (!completed)
                continue ;

            ++score;
            removeRow(y);
        }

        int removeCnt = 0;
        for (int y = 0; y < 2; ++y)
        {
            for (int x = 0; x < X_SIZE; ++x)
            {
                if (isBlock[y][x])
                {
                    ++removeCnt;
                    break ;
                }
            }
        }

        while (removeCnt--)
        {
            removeRow(Y_SIZE - 1);
        }
    }

    inline int countBlock(void) const
    {
        int cnt = 0;

        for (int y = 2; y < Y_SIZE; ++y)
        {
            for (int x = 0; x < X_SIZE; ++x)
            {
                cnt += isBlock[y][x];
            }
        }
        return (cnt);
    }
};

t_map yellow, red;

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int cmdCnt;
    int changeType[] = {0, 1, 3, 2};

    std::cin >> cmdCnt;
    while (cmdCnt--)
    {
        int type, y, x;

        std::cin >> type >> y >> x;
        yellow.doCmd(type, x);
        red.doCmd(changeType[type], t_map::X_SIZE - y - 1 - (type == 3));
    }

    std::cout << yellow.score + red.score << '\n';
    std::cout << yellow.countBlock() + red.countBlock() << '\n';
    return 0;
}