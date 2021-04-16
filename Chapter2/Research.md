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

NOTE: Continue with last paragraph of p. 18 