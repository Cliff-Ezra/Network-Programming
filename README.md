# Network Programming

The repository contains 2 folders:

1. File Programs : Contains programs that save and retrieve data from files.
2. Server Programs : Contains programs that use sockets to communicate between client and server. {to be updated}

## How to Run

1. Clone the repository.

### File Programs

1. Run the .cpp file
2. The program will run on the terminal. You will be prompted to enter the required data.
3. A .txt file will be created in the File Programs folder which will contain the data entered by the user.

### Server Programs

1. Run the server.cpp file on a separate terminal
2. Run the client.cpp file on a separate terminal
3. The client will be prompted to enter the required data.
4. A .txt file will be created and it will contain the information sent by the client.

## Things to Note

1. The server program does not always terminate on its own. You will have to manually terminate it using the following commands:
   In this program, the port number is hard coded into the code and it is 3000.
     
``` bash
    sudo lsof -i :<port_number>
    sudo kill <PID>
```

## Contributors

[Cliff Ezra](https://github.com/Cliff-Ezra), [Austin Ongwae](https://github.com/AustinOngwae), [Teddy Mwai](https://github.com/TedMwai), [Elijah Ajak](https://github.com/ElijahAjak), [Thon Peter](https://github.com/Thonpeter)

## License

[MIT](https://choosealicense.com/licenses/mit/)

