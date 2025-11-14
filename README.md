# Stock Trading Matching System (Client–Server)

![C](https://img.shields.io/badge/language-C-555?style=flat-square)

A **Systems Programming University Project** implemented in **C**.

---

## Overview

This project is a client–server **stock trading matching engine** written entirely in C.  
It simulates a simplified stock exchange where multiple clients can:

- Place **buy and sell orders**
- Use **trigger-based trading rules** (±5% price conditions)
- **Cancel** previously submitted orders
- **Load bulk instructions** from a file
- Retrieve **completed and pending commands** on login

The server stores all commands in custom **queue structures**, matches orders in real time, and reports execution results back to the client.

---

## Features

### Client
- Interactive **menu-driven interface**
- Supports:
  - Sell for fixed price
  - Sell if price increases >5%
  - Buy for fixed price
  - Buy if price drops >5%
  - Bulk load commands from file
  - Cancel order
- Sends **structured binary messages** to the server
- Receives **completed + pending order history** upon login

### Server
- Implements **two queues per stock**:
  - `Q1` → Sell orders
  - `Q2` → Buy orders
- Matching logic supports:
  - **Full fills**
  - **Partial fills**
  - **Queue reorganization** after each fill
- Processes **cancellation requests**
- Maintains **completed and uncompleted command records**
- **Multi-user support**

---

## System Architecture

```
   [ Client ]  <-- TCP Socket -->  [ Server ]
       |                                 |
       |  send commands                  |
       |-------------------------------->|
       |                                 |
       |<--------------------------------|
       |   send results & pending orders |
```

- Both sides exchange **C structs directly over sockets** (binary protocol).
---

## Project Structure

```
stock-trading-system/
├── client.c
├── server.c
├── struct.h
├── enqueue_dequeue_buy_sell.h
├── file.txt          # sample batch input
├── Makefile
└── README.md
```

---

## How It Works

1. **Client Types a Command**  
   The client fills a `commandFromClient` struct and sends it to the server.

2. **Server Places Command in Queue**  
   Each company has two queues (buy/sell).  
   The new command is inserted at the rear.

3. **Matching Engine Runs**  
   The server checks if the new command matches with the opposite queue:
   - Same company
   - Compatible price
   - Available quantity

4. **Partial or Full Fill**  
   The dequeue functions handle:
   - **Equal quantity → remove both**
   - **Buy > Sell → decrease buy**
   - **Sell > Buy → decrease sell**

5. **History is Saved**  
   Completed commands get stored and returned to users when they reconnect.

---

## Example Input File (`file.txt`)

```
1 10 1 50 12.5
1 10 3 20 11.0
2 10 2 40
2 10 4 30
```

**Format:**  
```
user_id company_id command_type number_of_stocks [price]
```

---

## Build & Run

**Build:**
```bash
make
```

**Run Server:**
```bash
./server
```

**Run Client:**
```bash
./client
```

---

## Command Types

| Type | Meaning                     |
|------|-----------------------------|
| 1    | Sell at fixed price          |
| 2    | Sell if price increases >5%  |
| 3    | Buy at fixed price           |
| 4    | Buy if price drops >5%       |
| 5    | Load from file               |
| 6    | Cancel command               |
| 7    | Exit                         |

---

## What I Learned

Through this project I gained hands-on experience with:

- Low-level **C systems programming**
- **TCP socket programming**
- **Queue data structures**
- **Order matching logic**
- **Struct-based binary protocols**
- **State management per user**
- **Manual memory & pointer manipulation**

---

## Author
Developed during my undergraduate studies in **Computer Science & Biomedical Informatics**.



