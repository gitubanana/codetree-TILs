#include <iostream>
#include <vector>
#include <bitset>

const int MAX_SIZE = 100 * 2;

int start;
int size, isOk, walkSize;

struct t_life
{
    std::vector<int> life;

    t_life(int size) : life(size)
    {
        for (int i = 0; i < life.size(); ++i)
        {
            std::cin >> life[i];
        }
    }

    inline int &operator[](int idx)
    {
        return (life[(start + idx) % life.size()]);
    }
};

struct t_board
{
    bool board[MAX_SIZE];

    inline bool &operator[](int idx)
    {
        return (board[(start + idx) % walkSize]);
    }
};

t_board board;

int main(void)
{
    std::cin.tie(0)->sync_with_stdio(0);

    std::cin >> size >> isOk;
    walkSize = size * 2;

    int ans = 0;
    t_life life(walkSize);

    while (isOk > 0)
    {
        board[size - 1] = false;
        if (start == 0)
            start = walkSize - 1;
        else
            --start;

        board[size - 1] = false;
        for (int cur = size - 2; cur >= 0; --cur)
        {
            if (!board[cur])
                continue ;

            int next = cur + 1;
            if (board[next] || !life[next])
                continue ;

            board[cur] = false;
            board[next] = true;
            isOk -= !(--life[next]);
        }

        if (!board[0] && life[0])
        {
            board[0] = true;
            isOk -= !(--life[0]);
        }
        ++ans;
    }

    std::cout << ans << '\n';
    return 0;
}