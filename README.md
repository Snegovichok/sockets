# Implementations
## The dynamic library (in the lib folder) contains three functions:
1. sortAndReplace – accepts a string, sorts its characters in descending order and replaces each character that is an even digit (0, 2, 4, 6, 8), on the line "KV".
2. sumNumericValues – goes through the line and sums up all the numbers found.
3. analyzeString – counts the number of alphanumeric characters in a string and returns true if this number is greater than 2 and a multiple of 32, otherwise false.

## Program No. 1 (Client) (client folder) consists of two streams:
The first stream accepts user input, checks it (not empty, only numbers, less than 64 characters), processes it through the sortAndReplace function and writes the result to a shared buffer.
The second thread waits for the data to appear in the buffer, outputs it to the screen, receives the sum of the digits through the sumNumericValues function and tries to send the result (as a string) to the server over TCP. If the connection is not possible, a second attempt is made with a pause.

## Program No. 2 (Server) (server folder): 
Is a TCP server that is waiting for a connection from the client. After receiving the data, it passes it to the analyzeString function and outputs a message about correctness or error. When the connection is disconnected, the server continues to work, trying to accept a new connection.

# Build and launch
Note: The source code must be compiled using "cmake" or "make" using "gcc" to run in a Linux environment. Make sure that you have everything installed for the build.
1. Download the "sockets" folder and open a terminal in it.
2. Run the following commands in order.
- mkdir build && cd build
- cmake ..
- make
3. Open the folder named "bin" in the "build" folder. (you will find the "client" and "server" files there)
4. Run the server and client in different terminals. The client will wait for the input string, process it, and send the result to the server, which analyzes the received data.
