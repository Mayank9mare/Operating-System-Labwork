LAB-3   
                     Mayank Raj (B19CSE053)


Steps to run the script:
*  Need a gcc compiler and a linux environment.
* For question 1 :
 a)gcc merge_sort.c -o merge_sort.out; ./merge_sort.out file_name
 b)If the file is empty then the system will generate 100 random numbers and sort then otherwise it will sort the given numbers.
* For question 2:
 a)gcc sever.c -o server.out; ./server.out  => For running server 
 b)gcc client.c -o client.out; ./client.out =>For running server


For question 1:
* Full merge sort is implemented using fork system call using two id rpid and lpid using an external file and then sorting.
* Fully implemented as described in the assignment.
For question 2:
* Single server and client functionality is implemented.
* Few bugs as it doesn’t support multiple clients.
* We were able to implement only two way communication  between client and server. (Really tried for multiple clients communication but were facing errors as only one client was active at a particular instance).