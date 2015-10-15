# BasicHttpServerClient
This will have basic Http Server and basic Http Client. Both will be developed in language C. 
It will be developed for Http version 1.0 initially and then upgraded to Http 1.1

For each new client request I will be creating a new thread. Current max thread is kept for 512. Got a heads up from my mentor
that it will cause problem. 
For thread synchronization I am using critical section and condition variables. Yes it is being developed in win32.
