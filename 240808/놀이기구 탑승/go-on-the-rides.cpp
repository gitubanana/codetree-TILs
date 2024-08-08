#include <iostream>
#include <cmath>
#include <unordered_set>

enum e_space
{
    EMPTY = 0
};

struct t_seat
{
    int y, x;
    int likeCnt;
    int emptyCnt;

    inline bool operator<(const t_seat &other) const
    {
        if (this->likeCnt == other.likeCnt)
        {
            if (this->emptyCnt == other.emptyCnt)
            {
                if (this->y == other.y)
                {
                    return (this->x > other.x);
                }

                return (this->y > other.y);
            }

            return (this->emptyCnt < other.emptyCnt);
        }

        return (this->likeCnt < other.likeCnt);
    }
};

using t_uset = std::unordered_set<int>;

const int MAX_SIZE = 20;
const int LIKE_CNT = 4;
const int dy[] = {-1, 1, 0, 0};
const int dx[] = {0, 0, -1, 1};
const int dirSize = sizeof(dy) / sizeof(dy[0]);

int size;
int map[MAX_SIZE][MAX_SIZE];
t_uset likes[MAX_SIZE * MAX_SIZE + 1];

inline bool inRange(int y, int x)
{
    return ((0 <= y && y < size)
            && (0 <= x && x < size));
}

t_seat getSeatInfo(int y, int x, int pupilNum)
{
    t_seat seat = {y, x, 0, 0};
    t_uset &myLikes = likes[pupilNum];

    for (int dir = 0; dir < dirSize; ++dir)
    {
        int nextY = y + dy[dir];
        int nextX = x + dx[dir];

        if (!inRange(nextY, nextX))
            continue ;

        int &nextSpace = map[nextY][nextX];
        if (myLikes.find(nextSpace) != myLikes.end())
        {
            ++seat.likeCnt;
        }
        else if (nextSpace == EMPTY)
        {
            ++seat.emptyCnt;
        }
    }
    return (seat);
}

void takeAseat(int pupilNum)
{
    t_seat seat = {-1, -1, -1, -1};

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            if (map[y][x] != EMPTY)
                continue ;

            seat = std::max(seat, getSeatInfo(y, x, pupilNum));
        }
    }

    map[seat.y][seat.x] = pupilNum;
}

int    getTotalScore(void)
{
    int totalScore = 0;

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int likeCnt = 0;
            int &curPupil = map[y][x];
            t_uset &curLikes = likes[curPupil];

            for (int dir = 0; dir < dirSize; ++dir)
            {
                int nextY = y + dy[dir];
                int nextX = x + dx[dir];

                if (!inRange(nextY, nextX))
                    continue ;

                int &nextPupil = map[nextY][nextX];
                likeCnt += (curLikes.find(nextPupil) != curLikes.end());
            }

            if (likeCnt > 0)
            {
                totalScore += std::pow(10, likeCnt - 1);
            }
        }
    }
    return (totalScore);
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    std::cin >> size;

    int pupilCnt = size * size;
    while (pupilCnt--)
    {
        int pupilNum;

        std::cin >> pupilNum;
        for (int i = 0; i < LIKE_CNT; ++i)
        {
            int like;

            std::cin >> like;
            likes[pupilNum].insert(like);
        }
        takeAseat(pupilNum);
    }

    std::cout << getTotalScore() << '\n';
    return 0;
}