#ifndef CONCURRENT_MAP
#define CONCURRENT_MAP
 
#include <boost/thread.hpp>
#include <map>
 
template<typename Key, typename Value>
class ConcurrentMap
{
    typedef boost::shared_lock shared_lock;
    typedef boost::shared_mutex shared_mutex;
     
public:
    ConcurrentMap();
    bool has(Key k) const
    {
        shared_lock<shared_mutex> lock(schemaAccess);
        return m.find(k) == m.end();
    }
     
    void erase(Key k)
    {
        upgrade_lock<shared_mutex> schemaLock(schemaAccess);
        upgrade_to_unique_lock<shared_mutex> schemaUniqueLock(schemaLock);
 
        valueAccess.erase(k);
        m.erase(k);
    }
     
 
    void set(Key k, Value v)
    {
        shared_lock<shared_mutex> lock(schemaAccess);
 
        // set k, v
        if(m.find(k) == m.end()) {
 
            upgrade_lock<shared_mutex> valueLock(*valueAccess[k]);
            upgrade_to_unique_lock<shared_mutex> valueUniqueLock(valueLock);
             
            m.at(k) = v;
             
        }
        // insert k, v
        else {
            upgrade_lock<shared_mutex> schemaLock(schemaAccess);
            lock.unlock();
            upgrade_to_unique_lock<shared_mutex> schemaUniqueLock(schemaLock);
             
            valueAccess.insert(k, new shared_mutext());
            m.insert(std::pair(k, v));
        }
    }
     
 
    Value get(Key k) const
    {
        shared_lock<shared_mutex> lock(schemaAccess);
        return m.at(k);
    }
 
 
    void insert(Key k, Value v)
    {
        upgrade_lock<shared_mutex> schemaLock(schemaAccess);
        upgrade_to_unique_lock<shared_mutex> schemaUniqueLock(schemaLock);
         
        valueAccess.insert(k, new shared_mutext());
        m.insert(std::pair(k, v));
    }
 
 
private:
    std::map m;
    std::map<Key, std::shared_ptr<shared_mutex> > valueAccess;
    shared_mutex schemaAccess;
}
 
 
#endif
