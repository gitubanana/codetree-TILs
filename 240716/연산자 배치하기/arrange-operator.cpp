#include <iostream>
#include <climits>

const int MAX_SIZE = 11;
const int OP_SIZE = 3;

int size;
int min = INT_MAX;
int max = INT_MIN;
int num[MAX_SIZE];
int opCnt[OP_SIZE];

int cal(int a, int op, int b)
{
    switch (op)
    {
        case 0:
            return (a + b);
        case 1:
            return (a - b);
    }
    return (a * b);
}

void    backTracking(int cur, int depth=1)
{
    if (depth == size)
    {
        min = std::min(min, cur);
        max = std::max(max, cur);
        return ;
    }

    for (int op = 0; op < 3; ++op)
    {
        int &curCnt = opCnt[op];

        if (curCnt == 0)
            continue ;

        --curCnt;
        backTracking(
            cal(cur, op, num[depth]),
            depth + 1
        );
        ++curCnt;
    }
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    std::cin >> size;
    for (int i = 0; i < size; ++i)
    {
        std::cin >> num[i];
    }
    for (int i = 0; i < OP_SIZE; ++i)
    {
        std::cin >> opCnt[i];
    }

    backTracking(num[0]);

    std::cout << min << ' ' << max << '\n';
    return 0;
}