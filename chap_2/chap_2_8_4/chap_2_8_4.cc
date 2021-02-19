#include "../../Mutex.h"
#include <map>
#include <string>
#include <vector>
#include "../../Thread.h"

#include <boost/shared_ptr.hpp>

using std::string;

typedef std::pair<string, int> Entry;
typedef std::vector<Entry> EntryList;
typedef std::map<string, EntryList> Map;
typedef boost::shared_ptr<Map> MapPtr;

class CustomerData : boost::noncopyable
{
public:
    CustomerData()
        : data_(new Map)
    {
    }

    int query(const string& customer, const string& stock) const;

    void update(const string& customer, const EntryList& entries);
    void update(const string& message);

private:
    static int find_entry(const EntryList& entries, const string& stock);
    static MapPtr parse_data(const string& message);

    MapPtr get_data() const
    {
        muduo::MutexLockGuard lock(mutex_);
        return data_;
    }

    mutable muduo::MutexLock mutex_;
    MapPtr data_;
};

int CustomerData::query(const string& customer, const string& stock) const
{
    MapPtr data = get_data();
    
    Map::const_iterator entries = data->find(customer);
    if (entries != data->end())
    {
        return find_entry(entries->second, stock);
    }
    else
    {
        return -1;
    }
}

void CustomerData::update(const string& customer, const EntryList& entries)
{
    muduo::MutexLockGuard lock(mutex_);
    if (!data_.unique())
    {
        MapPtr new_data(new Map(*data_));
        data_.swap(new_data);
    }
    assert(data_.unique());
    (*data_)[customer] = entries;
}

void CustomerData::update(const string& message)
{
    MapPtr new_data = parse_data(message);
    if (new_data)
    {
        muduo::MutexLockGuard lock(mutex_);
        data_.swap(new_data);
    }
}

int CustomerData::find_entry(const EntryList& entries, const string& stock)
{
    for (int i = 0; i < entries.size(); i++)
    {
        if (entries[i].first == stock)
        {
            return entries[i].second;
        }
    }

    return -1;
}

MapPtr CustomerData::parse_data(const string& message)
{
    MapPtr data(new Map);
    return data;
}

CustomerData data;

void work_thread_func()
{
    while (true)
    {
        data.query("", "");
    }
}

void back_thread_func()
{
    while (true)
    {
        for (int i = 0; i < 10; i++)
        {
            data.update("");
        }
    }
}

int main()
{
    muduo::Thread thread_0(work_thread_func);
    muduo::Thread thread_1(work_thread_func);
    muduo::Thread thread_2(work_thread_func);
    muduo::Thread thread_3(work_thread_func);

    muduo::Thread thread_back(back_thread_func);
    return 0;
}