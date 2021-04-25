#include <thread> 
#include <iostream>

class thread_guard
{
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_): t(t_){}
    ~thread_guard()
    {
        if(t.joinable())
        {
            t.join(); 
        }
    }

    thread_guard(thread_guard const&)=delete;
    thread_guard& operator=(thread_guard const&)=delete;
};

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

void do_something(int i) {
    std::cout << i << std::endl;
}

struct func {
    int& i;
    func(int& i_):i(i_){}
    void operator()()
    {
        for(unsigned j=0;j<1000000;++j)
        {
            do_something(i);
        }
    }
};

int main() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t1(my_func);
    thread_guard g1(t1);
    //scoped_thread s1{std::thread(func(some_local_state))};
    return 0;
}