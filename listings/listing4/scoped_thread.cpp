#include <thread> 
#include <string>

class scoped_thread
{
    std::thread t;
public:
    explicit scoped_thread(std::thread t_): t(std::move(t_))
    {
        if(!t.joinable()) {
            std::string msg = "No thread";
            throw std::logic_error(msg);
        }
    }
    ~scoped_thread()
    {
        t.join(); 
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
};