<h1 align="center">
  <br>
  Multiplayer Tic-Tac-Toe
</h1>

<h4 align="center">Multiplayer Tic-Tac-Toe Game written in C</h4>

<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#license">License</a>
</p>

## Key Features

- **Multi-Session Support**: Allows multiple Tic-Tac-Toe games to be played simultaneously, with each game session isolated from the others.
- **Two-Player Matchmaking**: Automatically pairs clients into game sessions, allowing them to play against each other in real-time.
- **Real-Time Game State Updates**: Ensures that all players receive immediate updates on the game state after every move, keeping the gameplay synchronized.
- **Threaded Client Management**: Utilizes multi-threading to handle multiple clients efficiently, ensuring smooth gameplay even with several active sessions.
- **Session Persistence**: Manages each game session independently, maintaining the game board and player states until the game concludes.
- **Turn-Based Gameplay Enforcement**: Ensures that players take turns according to the game rules, preventing invalid moves or simultaneous actions.
- **Player Disconnection Handling**: Detects and gracefully handles player disconnections, allowing the game session to end or offer reconnection options.
- **Simple Command Protocol**: Implements a straightforward text-based protocol for communication between the client and server, making it easy to extend or modify.

## How To Use

To clone and run this application, you'll need [Git](https://git-scm.com) and [CMake](https://cmake.org/)
installed on your computer. From your command line:

```bash
# Clone this repository
$ git clone https://github.com/alessandrofoglia07/multiplayer-tic-tac-toe

# Go into the repository
$ cd multiplayer-tic-tac-toe
```

The app is divided into two main components: the server and the client. The server is responsible for handling game
sessions, while the client is used to connect to the server and handle the user commands. (Note: The program is WIP and does not yet fully support multiplayer games.)

You can run the server like this:

```bash
# Build the server
$ cmake --build ./cmake-build-debug --target server -- -j 6
# Run the server
$ ./build/server
```

Once the server is running, you can connect to it using the client. You can run multiple instances of the client to
simulate multiple users chatting with each other.

You can run the client like this:

```bash
# Build the client
$ cmake --build ./cmake-build-debug --target game -- -j 6
# Run the client
$ ./build/game
```

## License

MIT

---

> GitHub [@alessandrofoglia07](https://github.com/alessandrofoglia07) &nbsp;&middot;&nbsp;
> StackOverflow [@Alexxino](https://stackoverflow.com/users/21306952/alexxino) &nbsp;&middot;&nbsp;
> NPM [alessandrofoglia07](https://www.npmjs.com/~alessandrofoglia07)
