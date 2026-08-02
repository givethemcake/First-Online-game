# To Run

# Compile

g++ -o Server gameServer.cpp Game.cpp <br>
g++ -o Client client.cpp

# Local Execution (same machine)
### Start the server first.
./Server <br>
Check the address at which the server is listening (written and flushed to standard out).

### Starting the clients

./Client 0.0.0.0 <br>
./client 0.0.0.0 <br>
Pass the address at which the server is actually listening to your client as a command line argument instead of 0.0.0.0 <br>
Start two instances of the client to start the game. <br>

# Local Execution (Over a LAN)

Grab the local ipv4 address of the machine hosting the server. <br>
Start up the server on the host machine. <br>
Pass the ipv4 address to the clients as the first command line argument on the machine running the clients. <br>

# Note:
The server listens on all interfaces on port 8080 make sure no other application is trying to use the same port. <br>
