#include <iostream>
#include <climits>
#include <cstring>
#include <queue>

enum e_space
{
    EMPTY = 0
};

struct t_pos
{
    int y, x;

    inline bool operator>(const t_pos &other) const
    {
        if (this->y == other.y)
        {
            return (this->x > other.x);
        }

        return (this->y > other.y);
    }
};

const int MAX_SIZE = 20;
const int dy[] = {-1, 1, 0, 0};
const int dx[] = {0, 0, -1, 1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

t_pos car;
int size;
int moved;
int battery;
bool isWall[MAX_SIZE][MAX_SIZE];
bool visited[MAX_SIZE][MAX_SIZE];
int destination[MAX_SIZE][MAX_SIZE];
int customer[MAX_SIZE][MAX_SIZE];

inline bool isBound(const t_pos &pos)
{
    return ((0 <= pos.y && pos.y < size)
            && (0 <= pos.x && pos.x < size));
}

inline std::istream &operator>>(std::istream &in, t_pos &input)
{
    in >> input.y >> input.x;
    --input.y, --input.x;
    return (in);
}

int findClosestCustomer(void)
{
    int &curCustomer = customer[car.y][car.x];
    if (curCustomer != EMPTY)
    {
        int id = curCustomer;

        curCustomer = EMPTY;
        return (id);
    }

    std::queue<t_pos> q;
    int customerId = EMPTY;
    t_pos customerPos = {INT_MAX, INT_MAX};

    memset(visited, false, sizeof(visited));
    visited[car.y][car.x] = true;
    q.push(car);
    while (!q.empty() && battery)
    {
        int qSize = q.size();

        --battery;
        while (qSize--)
        {
            const t_pos cur = q.front();
            q.pop();

            for (int dir = 0; dir < dirSize; ++dir)
            {
                t_pos next = {
                    cur.y + dy[dir],
                    cur.x + dx[dir]
                };

                if (!isBound(next)
                    || isWall[next.y][next.x]
                    || visited[next.y][next.x])
                    continue ;

                visited[next.y][next.x] = true;

                int &nextCustomer = customer[next.y][next.x];
                if (nextCustomer != EMPTY)
                {
                    if (customerPos > next)
                    {
                        customerPos = next;
                        customerId = nextCustomer;
                    }
                    continue ;
                }

                q.push(next);
            }
        }

        if (customerId != EMPTY)
        {
            car = customerPos;
            customer[customerPos.y][customerPos.x] = EMPTY;
            return (customerId);
        }
    }

    return (EMPTY);
}

bool goToDestination(int customerId)
{
    int &curDestination = destination[car.y][car.x];
    if (curDestination != EMPTY)
    {
        curDestination = EMPTY;
        return (true);
    }

    std::queue<t_pos> q;

    memset(visited, false, sizeof(visited));
    visited[car.y][car.x] = true;
    q.push(car);
    while (!q.empty() && battery)
    {
        int qSize = q.size();

        ++moved;
        --battery;
        while (qSize--)
        {
            const t_pos cur = q.front();
            q.pop();

            for (int dir = 0; dir < dirSize; ++dir)
            {
                t_pos next = {
                    cur.y + dy[dir],
                    cur.x + dx[dir]
                };

                if (!isBound(next)
                    || isWall[next.y][next.x]
                    || visited[next.y][next.x])
                    continue ;

                visited[next.y][next.x] = true;

                int &nextDest = destination[next.y][next.x];
                if (nextDest == customerId)
                {
                    car = next;
                    nextDest = EMPTY;
                    return (true);
                }

                q.push(next);
            }
        }
    }

    return (false);
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int customerCnt;

    std::cin >> size >> customerCnt >> battery;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            std::cin >> isWall[y][x];
        }
    }

    std::cin >> car;
    for (int id = 1; id <= customerCnt; ++id)
    {
        t_pos start, end;

        std::cin >> start >> end;
        customer[start.y][start.x] = id;
        destination[end.y][end.x] = id;
    }

    while (customerCnt--)
    {
        moved = 0;

        int id = findClosestCustomer();
        if (id == EMPTY || !goToDestination(id))
        {
            break ;
        }

        battery += moved * 2;
    }

    std::cout << (customerCnt != -1 ? -1 : battery) << '\n';
    return 0;
}