#pragma once
// todo:全部include进来会不会导致程序变大
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace meta_log {

template <typename T, typename Container>
class IterableContainer
{
  public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    IterableContainer() {}

    virtual ~IterableContainer() {}

    iterator begin() { return GetContainer().begin(); }
    iterator end() { return GetContainer().end(); }

  private:
    virtual Container& GetContainer() = 0;
};

template <typename T, typename Container = std::deque<T>>
class IterableQueue : public IterableContainer<T, Container>, public std::queue<T, Container>
{
  public:
    IterableQueue(std::queue<T, Container> q, size_t container_max)
    {
        std::size_t cnt = 0;
        // 多push进去一个，这样能生成省略号
        while (cnt++ < container_max + 1 && !q.empty())
        {
            this->push(q.front());
            q.pop();
        }
    }

  private:
    inline Container& GetContainer() { return this->c; }
};

template <typename T, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class IterablePriorityQueue : public IterableContainer<T, Container>, public std::priority_queue<T, Container, Compare>
{
  public:
    IterablePriorityQueue(std::priority_queue<T, Container, Compare> q, size_t container_max)
    {
        std::size_t cnt = 0;
        while (cnt++ < container_max + 1 && !q.empty())
        {
            this->push(q.top());
            q.pop();
        }
    }

  private:
    inline Container& GetContainer() { return this->c; }
};

template <typename T, typename Container = std::deque<T>>
class IterableStack : public IterableContainer<T, Container>, public std::stack<T, Container>
{
  public:
    IterableStack(std::stack<T, Container> s, size_t container_max)
    {
        std::size_t cnt = 0;
        while (cnt++ < container_max + 1 && !s.empty())
        {
            this->push(s.top());
            s.pop();
        }
    }

  private:
    inline Container& GetContainer() { return this->c; }
};
}  // namespace meta_log