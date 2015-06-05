#ifndef __THREAD_USAGE__
#define __THREAD_USAGE__

#include <fstream>
#include <string>
#include <map>
#include <pthread.h>

namespace thread
{
class CThreadUsage
{
public:
    CThreadUsage(unsigned int printCount, std::string strFilename);
    ~CThreadUsage();

    void AddThread(pthread_t pthread_id);
    void Add();

private:
    std::map<pthread_t, int> usage_;
    unsigned int printCount_;
    std::ofstream output_;
    unsigned int count_;
    pthread_mutex_t mutex_;

    void Print();
    void Clear();

};


}

#endif //__THREAD_USAGE__

