# Nairobi Matatu Management System — Thika Road (Route 45)

A C++ console application that simulates a public transport management system for matatus operating along \*\*Thika Road\*\*, Nairobi. Built as a demonstration of four core data structures working together in a real-world Kenyan context.

Developed as a Data Structures assignment — University project, Kenya context.

---
## Group Members

**Sydney Karanja** - SCT221-0212/2024
**Nyambura Baraka Mugu** - SCT221-0320/2024
**Arnold Mosoti** - SCT221-0353/2024 

## Data Structures Used


| Data Structure | Role in System |

|---|---|

| **Queue** (`std::queue`) | Passengers waiting at a stage board in FIFO order |

| **Stack** (`std::stack`) | Route history for the matatu driver; supports backtracking when roads close |

| **Tree** (custom `TreeNode`) | Hierarchical fare structure: County → Hub → Route → Stage |

| **Graph** (adjacency list + Dijkstra) | Stage network; finds shortest km path between any two stages |

---

## Route Coverage

This version covers **Thika Road (Route 45)** only:



```

CBD → Ngara → Allsops → Garden City → Roysambu → Githurai →

Kahawa Wendani → Kahawa Sukari → Bypass → Ruiru → Kimbo →

Toll → Kenyatta Road → Juja → Witeithe → Thika → Makongeni

```

Stages outside this route are rejected gracefully with an error message — the program will not crash.

---

## Features



\- **Boarding stage selection** is required before any other feature can be used

\- **Passenger Queue** — add passengers, board them FIFO, view the queue

\- **Route History (Stack)** — mark stages visited, backtrack from a stage

\- **Fare Lookup (Tree)** — calculates trip fare relative to your boarding stage

\- **Shortest Path (Graph)** — Dijkstra's algorithm returns the shortest route in km

\- **Input validation** — all invalid stages and bad inputs are caught without crashing

\- **Change boarding stage** — resets the queue and updates all stage-aware features

---

## Getting Started

### Requirements

\- **IDE:** \[Code::Blocks](https://www.codeblocks.org/) (recommended) or any C++11-capable compiler

\- **Compiler:** GCC / MinGW with C++11 support

\- **OS:** Windows, Linux, or macOS

### Building in Code::Blocks

1\. Open Code::Blocks

2\. Go to \*\*File → New → Project → Console Application → C++\*\*

3\. Replace the contents of `main.cpp` with `matatu\_system.cpp`

4\. Enable C++11:

&#x20;  - Go to \*\*Project → Build Options\*\*

&#x20;  - Select \*\*Compiler Settings → Other options\*\*

&#x20;  - Add: `-std=c++11`

5\. Press \*\*F9\*\* (Build and Run)

### Building from the terminal



```bash

g++ -std=c++11 -Wall -o matatu matatu\_system.cpp

./matatu

```

---

## How to Use

1\. \*\*Launch the program\*\* — you will be prompted to enter your boarding stage first

2\. \*\*Type a valid stage name\*\* (case-insensitive, e.g. `juja`, `Garden City`, `CBD`)

3\. \*\*Select from the main menu:\*\*

```

1\. Passenger Queue        (Queue)

2\. Route History          (Stack)

3\. Fare Lookup            (Tree)

4\. Shortest Path          (Graph)

5\. View all route stages

6\. Change boarding stage

0\. Exit

```

### Example session

```

Enter your boarding stage: roysambu

\[OK] Boarding stage set to: Roysambu



Select option: 3 (Fare Lookup)

Destination stage: thika



Fare CBD → Roysambu  : KES 50

Fare CBD → Thika     : KES 130

─────────────────────────────────

Trip fare (Roysambu → Thika): KES 80

```



\---

## Project Structure

```

matatu-system/

├── matatu\_system.cpp   # Full C++ source — all logic in one file

├── README.md           # This file

└── .gitignore          # Files excluded from version control

```

---

## Future Improvements

\- Add more routes (Ngong Road, Jogoo Road, Langata Road)

\- Persist passenger and trip data to a file

\- Add vehicle scheduling across multiple matatus

\- GUI front-end using Qt or a web interface
