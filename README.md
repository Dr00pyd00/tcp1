# First little TCP program for multi chatting

Open the folder with WSL :
=> open a Powershell and type:
```bat
  wsl
```

go in the directory with cmd / powershell or bash:
```bat
$ make
```
=> that's make compilation and create executable 'server'.
Them you can execute the program:
Put the port you want and use the same for clients , if you dont type server port the default one is : 8989
```bat
$ ./server [port]
```

---
### Open mutli client for test the chat:

You need to simulate some clients for see the result on the 'server' console:
Use 'netcat' in separates consoles with localhost and port 8989:

```bat
$ nc localhost 8989
"type your messages here ..."
```
Them you can see in the server console the messages of all clients.

-> For leave each clients or  the server you can :  CTRL + C


