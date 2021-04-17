# Managing threads

## 1 Basic thread management

- Each C++ program has at least one thread started by the C++ runtime, it is the thread running ```main()```
- Your program can launch additional threads that has another function as an entry point. These thread run concurrently to each other and the main (initial) thread
- An thread exits when the function associated with it finishes. For example when      ```main()``` finishes the initial thread exits

### 1.1 Launching a thread

Starting a thread always boils down to constructing a ```std::thread``` object for example like this:

````cpp
    void do_some_work();
    std::thread my_thread(do_some_work);
````

#### Remark 1.1.1

Make sure that you include the ```<thread>``` header so the complier notices the definition of the ```std::thread``` class. **In the following code snippets includes will be omitted.**

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

The function object is copied to the storage belonging to the thread where it is executed and invoked from there. Therefore it is essential that the copy behaves exactly like the original otherwise results might be unexpected.

#### Remark 1.1.2

When passing a temporary rather than a named variable the syntax can be the same as a function declaration and the compiler will interpret it as such:

````cpp
    std::thread my_thread(background_task()); // function declaration
````

One avoids that by using one of the following syntaxes

````cpp
    std::thread my_thread((background_task())); // extra pair of parentheses 
    std::thread my_thread{background_task()}; // uniform initialization syntax
````

Another way achieving the correct interpretation would be to use lambda expressions (local/anonymous function) in the following way

````cpp
    std::thread my_thread([]{
        do_something();
        do_something_else();
    });
````

### 1.2 Issues arising from using threads

When a thread is started one needs to explicitly state whether to wait for it to finish (by joining it - see section 1.3) or leave it to run on it's own (by detaching it **TODO: Reference** ). When one does not decide before the ```std::thread``` object is destroyed the program is terminated by calling ```std::terminate```. Note that you only have to decide before the ```std::thread``` object is destroyed. When not waiting for the thread to finish one has to insure that the data the thread is working on is still available.

Here is one example of a program where the thread function holds a local variable that is destroyed before the thread finishes:

````cpp
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
    }

    void oops() {
        int some_local_state=0;
        func my_func(some_local_state);
        std::thread my_thread(my_func);
        my_thread.detach();
    }
````

Here the new thread associated with ```my_thread``` will most likely still be running when ```oops``` exits, since one calls ```detach()``` on the object. Like this in some call of ```do_something(i)``` will result in accessing an already destroyed variable.

In the following the two main ways to avoid this are presented:

1. Make the thread self-contained and copy the data into the thread rather than sharing it
2. By *joining* with the thread

### 1.3 Joining with threads

If one needs to wait for a thread to complete, this is done by calling ```join()``` on the associated ```std::thread``` object. When looking into the code example from before it is sufficient to replace the call of  ```my_thread.detach()``` with ```my_thread.join()``` to insure that the thread was finished before ```oops()``` was exited and the local variables were destroyed.

When you need a more fine grained control over waiting for a thread one can use mechanisms such as condition variables and futures but these will be elaborated in another presentation.

The act of *joining* a ```thread``` cleans up any storage associated to the thread, so the joined thread object is no longer associated with any other running thread. From this it follows that one can only call ```join()```on a given thread once you have called it on it the method ```joinable()``` will return false.

#### 1.3.1 Waiting in exceptional circumstances

- When one plans to detach a thread this can usually be done right after the thread has been started
- But when one plans to join the thread it is not that easy, one has to carefully pick the place where to call ```join()``` on the thread

Joining threads when exceptions are raised:

````cpp
struct func;
void f()
{
    int some_local_state=0;
        func my_func(some_local_state);
        std::thread t(my_func);
        try
        {
            do_something_in_current_thread();
        }
    catch(...) {
        t.join();
        throw; 
    }
    t.join();
}
````

The ```try/catch```block insures that the thread is joined correctly even if a exception is raised during execution.

But this technique is error-prone, though using the standard **Resource Acquisition Is Initialization (RAII)** idiom provides a class and thus a simple mechanism to do exactly that.

A rewritten version using RAII of the listing above is given in the following:

````cpp
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

struct func;
void f()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
    do_something_in_current_thread();
}
````

- Execution of current thread reaches the end of ```f``` -> local objects are destroyed in reversed order of construction
- So the ```thread_guard```object is destroyed first and the thread is joined with in the destructor
- This even happens if the function exits because ```do_something_in_current_thread```
throws an exception
- Also note the precondition in the destructor of ```thread_guard````
- Copy-constructor and copy-assignment are marked as ```=delete```to avoid the object outliving the scope of the thread it was joining
- *Detaching* the thread would not raise these issues and would thus not require such an exception safe handling of the thread

### 1.4 Detaching threads

- *Detaching* a thread by calling ```detach()``` on the associated ```std::thread``` object on it leaves the thread to run in the background
- For *detached* threads there are no means of direct communication with it anymore
  - not possible to join with anymore
  - not possible to obtain a ```std::thread``` object that references to it
  - truly run in the background
  - ownership and control are passed to the C++ Runtime Library
- These threads are often called *daemon threads* that run in the background with no user interface
- When a thread is detached it is no longer joinable

## 2 Passing arguments to a thread function
