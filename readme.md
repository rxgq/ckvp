## C KVP (Key-Value-Pair) Store

This is a repository for a simple, lightweight, and persistent key-value store written in C. It provides an interactive command-line interface (CLI) for managing key-value pairs, with features like persistent storage, authentication, and basic data management commands.

<br/>

### Setup

1. Clone the Repository

```
git clone https://github.com/rxgq/ckvp-store.git
cd ckvp-store
```

2. Build the program
   - Usage `make` (if available):
        ```
        make
        ```

    - Without `make`:\
        Compile manually with your compiler. For instance, with GCC:
        ```
        gcc -o ckvp main.c src/kvp_store.c src/kvp_tests.c src/parser.c src/kvp_engine.c -I./src
        ```

3. Run the program
```
./ckvp.exe
```

4. Setup

When you first run the program, you'll be prompted to set up the KVP Store. Follow the steps in the terminal to complete the configuration. You may also set an optional security passcode.

5. Quit

To exit the cvkp console, type `quit` or `q`.

<br/>

### Features

1. Persistent data storage
2. Password authentication

Supports the following commands:
- `set` `<key>` `<value>`: Add or update a key-value pair.
- `get` `<key>`: Retrieve the value associated with a key.
- `del` `<key>`: Delete a key-value pair.
- `quit` or `q`: Exit the program.

<br/>


#### Example Session

```
PS: .\ckvp.exe
ckvp path: \path\to\ckvp\kvp.dat

C KVP setup required
  perform setup? (y/n): y
  ckvp.dat does not already exist
  created ckvp.dat
  
create authorisation password? (recommended) (y/n): n

ckvp console (type "help")
> set x test
x: test

> get x
test

> set y 1
y: 1

> get y
1

> q
```