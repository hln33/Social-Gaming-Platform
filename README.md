# Social Gaming

Run server
    bin/chatserver 4000 ../web-socket-networking/webchat.html

In separate terminals, run multiple instances of the chat client using:

    bin/chatclient localhost 4000

once Client is loaded:
------MENU------- 
1 - Create 
2 - Join 
0 - To close game 

1:
will be prompted to enter in path to a json file. If you are in the root folder on terminals then the path starts from there. If my json file is in the bin folder then it will look something like this:

    bin/test.json

2: 
will be prompt to type in a code to join a room

0: 
exits game