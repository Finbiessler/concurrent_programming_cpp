# Chapter 2 - Managing threads

## 2.1 Basic thread management

- Each C++ program has at least one thread started by the C++ runtime, it is the thread running ```main()```
- Your program can launch additional threads running that has another function as an entry point. These thread run concurrently with each other and the main (initial) thread
- An thread exits when the function associated with it finishes. For example when      ```main()``` finishes the initial thread exits

### 2.1.1 Launching a thread
Starting a thread always boils down to constructing a ```std::thread``` object for example like this:

````cpp
void do_some_work();
std::thread my_thread(do_some_work);
````

Remark: Make sure that you include the ```<thread>``` header so the complier notices the definition of the ```std::thread``` class. In the following code snippets this will be omitted.

As with much of the C++ Standard Library, ```std::thread``` works with any callable type for example:

````cpp
class background_task
{
    public:
    void operator() () const
    {
        do_something();
        do_something_else();
    }
};

background_task f;
std::thread my_thread(f);
````
