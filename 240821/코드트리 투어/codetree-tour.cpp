#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>

enum e_cmd
{
    CREATE = 200,
    CANCEL = 300,
    BEST = 400,
    CHANGE_START = 500
};

struct t_edge
{
    int v;
    int cost;

    inline bool operator<(const t_edge &other) const
    {
        return (this->cost > other.cost);
    }
};

struct t_sale
{
    int revenue;
    int dest;
};

struct t_cmp
{
    int id;
    int money;

    inline bool operator<(const t_cmp &other) const
    {
        if (this->money == other.money)
            return (this->id > other.id);

        return (this->money < other.money);
    }
};

const int MAX_ID = 30000;
const int MAX_V = 2000;
const int INF = 1e9;

int vCnt;
int dists[MAX_V];
bool deleted[MAX_ID + 1];
std::vector<t_edge> edges[MAX_V];
std::priority_queue<t_cmp> bestSales;
std::unordered_map<int, t_sale> sales;

void    dijkstra(int start=0)
{
    std::priority_queue<t_edge> pq;

    for (int v = 0; v < vCnt; ++v)
    {
        dists[v] = INF;
    }

    dists[start] = 0;
    pq.push({start, 0});
    while (!pq.empty())
    {
        const t_edge cur = pq.top();
        pq.pop();

        if (dists[cur.v] != cur.cost)
            continue ;

        for (const t_edge &next : edges[cur.v])
        {
            int &cmpDist = dists[next.v];
            int nextDist = dists[cur.v] + next.cost;
            if (cmpDist <= nextDist)
                continue ;

            cmpDist = nextDist;
            pq.push({next.v, nextDist});
        }
    }
}

inline void    createSale(void)
{
    int id;
    std::cin >> id;

    t_sale &cur = sales[id];
    std::cin >> cur.revenue >> cur.dest;
    deleted[id] = false;

    int money = cur.revenue - dists[cur.dest];
    if (money >= 0)
    {
        bestSales.push({id, money});
    }
}

inline void    cancelSale(void)
{
    int id;

    std::cin >> id;
    deleted[id] = true;
    sales.erase(id);
}

inline void    printBestSale(void)
{
    int bestId = -1;

    while (!bestSales.empty())
    {
        const t_cmp cur = bestSales.top();
        bestSales.pop();

        if (!deleted[cur.id])
        {
            bestId = cur.id;
            sales.erase(cur.id);
            break ;
        }
    }

    std::cout << bestId << '\n';
}

void    changeStart(void)
{
    int start;
    std::cin >> start;

    dijkstra(start);
    bestSales = std::priority_queue<t_cmp>();
    for (std::pair<const int, t_sale> &cur : sales)
    {
        const int &id = cur.first;
        const t_sale &sale = cur.second;
        int money = sale.revenue - dists[sale.dest];

        if (money >= 0)
        {
            bestSales.push({id, money});
        }
    }
}

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int cmd;
    int eCnt, cmdCnt;

    std::cin >> cmdCnt;
    std::cin >> cmd >> vCnt >> eCnt;
    while (eCnt--)
    {
        int from, to, cost;

        std::cin >> from >> to >> cost;
        edges[from].push_back({to, cost});
        edges[to].push_back({from, cost});
    }

    dijkstra();
    while (--cmdCnt)
    {
        std::cin >> cmd;
        switch (cmd)
        {
            case CREATE:
                createSale();
                break;
            case CANCEL:
                cancelSale();
                break;
            case BEST:
                printBestSale();
                break;
            case CHANGE_START:
                changeStart();
                break;
        }
    }
    return (0);
}