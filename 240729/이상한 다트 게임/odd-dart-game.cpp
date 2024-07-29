#include <iostream>
#include <vector>

struct t_circle
{
    int start;
    std::vector<int> arr;

    t_circle(void) : start(0) { }

    inline void    push(int num)
    {
        arr.push_back(num);
    }

    inline void rotateClockWise(int move)
    {
        start -= move;
        if (start < 0)
        {
            start = arr.size() - start;
        }
    }

    inline void rotateCounterClockWise(int move)
    {
        start = (start + move) % arr.size();
    }

    inline int &operator[](int idx)
    {
        return (arr[(start + idx) % arr.size()]);
    }
};

using t_func = void (t_circle::*)(int);

const int MAX_SIZE = 50;
const int REMOVED = -1;

int ySize, xSize;
int maxDepth;
t_circle map[MAX_SIZE];
t_func rotate[] = {
    &t_circle::rotateClockWise,
    &t_circle::rotateCounterClockWise
};

void    dfs(int y, int x, int depth=0)
{
    static const int dy[] = {-1, 1, 0, 0};
    static const int dx[] = {0, 0, -1, 1};
    static const int dirSize = sizeof(dy) / sizeof(dy[0]);

    int &cur = map[y][x];
    int originNum = cur;

    cur = REMOVED;
    maxDepth = depth;
    for (int dir = 0; dir < dirSize; ++dir)
    {
        int nextY = y + dy[dir];
        int nextX = x + dx[dir];
        if (nextX == -1)
            nextX = xSize - 1;
        else if (nextX == xSize)
            nextX = 0;

        if (!(0 <= nextY && nextY < ySize)
            || !(0 <= nextX && nextX < xSize)
            || map[nextY][nextX] != originNum)
            continue ;

        dfs(nextY, nextX, depth + 1);
    }

    if (maxDepth == 0)
    {
        cur = originNum;
    }
}

int getSum(void)
{
    int sum = 0;

    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            int &num = map[y][x];
            if (num == REMOVED)
                continue ;

            sum += num;
        }
    }
    return (sum);
}

void    removeSameAdjacent(void)
{
    int numCnt = 0;
    bool removed = false;

    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            if (map[y][x] == REMOVED)
                continue ;

            maxDepth = 0;
            dfs(y, x);

            ++numCnt;
            removed |= (maxDepth != 0);
        }
    }

    if (removed)
        return ;

    int avg = getSum() / numCnt;
    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            int &num = map[y][x];
            if (num == REMOVED)
                continue ;

            if (num > avg)
                --num;
            else if (num < avg)
                ++num;
        }
    }
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int cmdCnt;

    std::cin >> ySize >> xSize >> cmdCnt;
    for (int y = 0; y < ySize; ++y)
    {
        for (int x = 0; x < xSize; ++x)
        {
            int num;

            std::cin >> num;
            map[y].push(num);
        }
    }

    while (cmdCnt--)
    {
        int mul, dir, move;

        std::cin >> mul >> dir >> move;
        for (int y = mul - 1; y < ySize; y += mul)
        {
            t_circle &toMove = map[y];
            t_func &rotateFunc = rotate[dir];

            (toMove.*rotateFunc)(move);
        }
        // std::cout << "====== after rotate ======" << '\n';
        // for (int y = 0; y < ySize; ++y)
        // {
        //     for (int x = 0; x < xSize; ++x)
        //     {
        //         std::cout << map[y][x] << ' ';
        //     }
        //     std::cout << '\n';
        // }

        removeSameAdjacent();
    }

    // std::cout << "====== circles ======" << '\n';
    // for (int y = 0; y < ySize; ++y)
    // {
    //     for (int x = 0; x < xSize; ++x)
    //     {
    //         std::cout << map[y][x] << ' ';
    //     }
    //     std::cout << '\n';
    // }
    std::cout << getSum() << '\n';
    return 0;
}