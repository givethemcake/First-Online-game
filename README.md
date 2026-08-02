# Multiplayer Game

## Building

Compile the server:

```bash
g++ -o Server gameServer.cpp Game.cpp
```

Compile the client:

```bash
g++ -o Client client.cpp
```

---

## Running (same machine)

Start the server first:

```bash
./Server
```

The server will print the address it is listening on.

Open two terminals and start two clients:

```bash
./Client 127.0.0.1
```

```bash
./Client 127.0.0.1
```

Replace `127.0.0.1` with the address printed by the server if it differs.

---

## Running over a LAN

1. Find the IPv4 address of the machine running the server.

   ```bash
   ip addr
   ```

   Example:

   ```
   192.168.1.64
   ```

2. Start the server on the host machine.

3. On each client machine, connect using the server's IPv4 address:

   ```bash
   ./Client 192.168.1.64
   ```

4. Start two clients to begin the game.

---

## Notes

- The server listens on port **8080**.
- Make sure no other application is using port **8080**.
- Both the server and clients must be on the same LAN when connecting over a network.
