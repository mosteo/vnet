#ifndef vnet_support_h_
#define vnet_support_h_

#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <queue>

namespace vnet {

// Support classes to be used elsewhere

    //  Thread-safe queue
    //  Note that for non-blocking pop, the result will be sliced to the base Datum class!!
    template<typename Datum>
    class QueueMonitor : boost::noncopyable {
    public:
        void  push (const Datum &datum);
        Datum pop_blocking ();
        void  pop_non_blocking (bool &empty, Datum &datum);
        //  if (!empty) datum = pop_blocking();
        
    private:
        std::queue<Datum>         data_;
        boost::mutex              data_mutex_;
        boost::condition_variable data_available_;
    };

}

////////////////////////////////
// IMPLEMENTATION BEGINS HERE //
////////////////////////////////
// can't believe C++ still requires impl in headers...

template <typename Datum>
void vnet::QueueMonitor<Datum>::push(const Datum& datum)
{
    {
        boost::lock_guard<boost::mutex> lock (data_mutex_);
        data_.push (datum);
    }
    data_available_.notify_one ();
}

template <typename Datum>
Datum vnet::QueueMonitor<Datum>::pop_blocking()
{
    boost::unique_lock<boost::mutex> lock (data_mutex_);
    
    while (data_.empty ()) {
        data_available_.wait (lock);
    }
    
    const Datum datum = data_.front ();
    data_.pop ();    
    return datum;
}

template <typename Datum>
void vnet::QueueMonitor<Datum>::pop_non_blocking(bool& empty, Datum& datum)
{
    boost::lock_guard<boost::mutex> lock (data_mutex_);
    
    empty = data_.empty ();
    if (!data_.empty()) {
        datum = data_.front ();
        data_.pop ();
    }
}

#endif