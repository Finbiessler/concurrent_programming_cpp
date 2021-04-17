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

#### Remark

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

#### Remark

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

When a thread is started one needs to explicitly state whether to wait for it to finish (by joining it **TODO: REFERENCE**) or leave it to run on it's own (by detaching it **TODO: Reference** ). When one does not decide before the ```std::thread``` object is destroyed the program is terminated by calling ```std::terminate```. Note that you only have to decide before the ```std::thread``` object is destroyed. When not waiting for the thread to finish one has to insure that the data the thread is working on is still available. 

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
2.  