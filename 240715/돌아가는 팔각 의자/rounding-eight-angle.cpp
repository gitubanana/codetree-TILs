#include <iostream>

enum e_space
{
    NORTH = '0',
    SOUTH
};

enum e_idx
{
    UP = 0,
    RIGHT = 2,
    LEFT = 6
};

struct t_chair
{
    static const int SIZE = 8;

    int start;
    char arr[SIZE + 1];

    t_chair(void) : start(0) { }

    inline void clockWise(void)
    {
        start = (start == 0 ? SIZE - 1 : start - 1);
    }

    inline void counterClockWise(void)
    {
        start = (start + 1) % SIZE;
    }

    inline char &operator[](int idx)
    {
        return (arr[(start + idx) % SIZE]);
    }
};

using t_func = void (t_chair::*)(void);

const int CHAIR_CNT = 4;
t_func rotate[] = {
    &t_chair::counterClockWise,
    &t_chair::clockWise
};

t_chair chairs[CHAIR_CNT];

void    dfs(int curIdx, int dx, bool dir)
{
    static const int chIdx[2][2] = {
        {LEFT, RIGHT},
        {RIGHT, LEFT}
    };

    if (!(0 <= curIdx && curIdx < CHAIR_CNT))
        return ;

    int y = (dx == -1);
    t_chair &cur = chairs[curIdx];
    t_chair &prev = chairs[curIdx - dx];

    if (cur[chIdx[y][0]] == prev[chIdx[y][1]])
        return ;

    dfs(curIdx + dx, dx, !dir);
    (cur.*rotate[dir])();
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    for (int i = 0; i < CHAIR_CNT; ++i)
    {
        std::cin >> chairs[i].arr;
    }

    int cmdCnt;

    std::cin >> cmdCnt;
    while (cmdCnt--)
    {
        int moveIdx, dir;

        std::cin >> moveIdx >> dir;
        --moveIdx;
        dir = (dir == 1);

        dfs(moveIdx + 1, 1, !dir);
        dfs(moveIdx - 1, -1, !dir);

        t_chair &cur = chairs[moveIdx];

        (cur.*rotate[dir])();
    }

    int ans = 0;
    for (int i = 0; i < CHAIR_CNT; ++i)
    {
        ans += ((chairs[i][UP] == SOUTH) << i);
    }

    std::cout << ans << '\n';
    return 0;
}