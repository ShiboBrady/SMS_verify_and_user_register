#include "threadusage.h"

using namespace thread;

CThreadUsage::CThreadUsage(unsigned int printCount, std::string strFilename)
    :printCount_(printCount),
     output_(strFilename.c_str()),
     count_(0)
{
    pthread_mutex_init(&mutex_, NULL);
}

CThreadUsage::~CThreadUsage()
{
    output_.close();
    pthread_mutex_destroy(&mutex_);
}

void CThreadUsage::AddThread(pthread_t pthread_id)
{
    pthread_mutex_lock(&mutex_);
    usage_.insert(std::make_pair<pthread_t, int>(pthread_id, 0));
    pthread_mutex_unlock(&mutex_);
}

void CThreadUsage::Add()
{
    pthread_mutex_lock(&mutex_);
    ++usage_[pthread_self()];
    ++count_;
    if (printCount_ == count_)
    {
        Print();
        Clear();
    }
    pthread_mutex_unlock(&mutex_);
}

void CThreadUsage::Print()
{
    output_ << "========== Begin ========" << "\n"
            << "Thread_ID\t" << "Usage" << std::endl;

    for (std::map<pthread_t, int>::iterator iBegin = usage_.begin();
         iBegin != usage_.end(); ++iBegin)
    {
        output_ << iBegin->first << "\t" << iBegin->second << std::endl;
    }
    output_ << "=========== End =========" << std::endl;
}

void CThreadUsage::Clear()
{
    count_ = 0;
    for (std::map<pthread_t, int>::iterator iBegin = usage_.begin();
         iBegin != usage_.end(); ++iBegin)
    {
        iBegin->second = 0;
    }
}