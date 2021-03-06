# Condition-Variables-and-Mutexes-


Write a complete Linux program, to run on stoker, that implements a threaded producer-consumer based parallel system with a main thread and four other threads -- three "consumer" threads and a "printing" thread.

The main thread should initialise any data structures needed and instantiate the other threads. It should then enter a loop in which it reads a line of text from the standard input (usually the command line) and passes it, using some kind of shared data structure, to the consumer threads. This should be arranged using condition variables and mutexes and one of the three consumer threads will pick up the string.

Each of the three consumer threads is identical and each one is of the form of a loop which waits for a line of text from the main thread. It should use the above-mentioned shared data structure, condition variable and mutex to do this. When it receives the line of text from the "producer", i.e. the main thread, it should pass it to the printing thread, via another shared data structure, condition variable and mutex, along with an index number identifying itself.

The printing thread should be of the form of a loop which should wait for a line of text and index number from one of the three consumer threads, using the appropriate shared data structure, condition variable and mutex, and should print both items on the standard output.

For example, if the user entered "The quick brown fox jumped over the lazy dog." and it was received by consumer thread 2, the printing thread should output "2. The quick brown fox jumped over the lazy dog."
