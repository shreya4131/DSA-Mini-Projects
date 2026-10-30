# Campus Route Finder Using Graphs (C)

DSA Mini Project (PPF Week 9) – CGC University, Mohali

## What it does
- Graph of **10 campus locations** stored as an **adjacency list** (linked lists); adjacency **matrix** displayed for comparison
- **BFS** finds the route with the minimum number of edges (queue + visited array + parent array)
- **DFS** (recursive) lists all reachable locations and identifies disconnected ones
- User enters a source and destination (by number) and the BFS route is displayed
- **5 built-in test cases** with expected vs actual output (menu option 6)

## Build and run
```bash
gcc -Wall -Wextra -std=c99 -o campus_route_finder campus_route_finder.c
./campus_route_finder          # Windows: campus_route_finder.exe
```
Any C99 compiler works (gcc / clang / MinGW / Code::Blocks). No extra libraries.

## Menu
| Option | Action |
|---|---|
| 1 | Display graph (adjacency list) |
| 2 | Display adjacency matrix |
| 3 | Find route between two places (BFS) |
| 4 | Show all reachable + disconnected locations (DFS) |
| 5 | Check if a destination is reachable (DFS) |
| 6 | Run test cases |

## Campus map
```
Parking - Main Gate - Library - Cafeteria
              \         |           |
               `-- Block 1        Auditorium
                      |            /
                   Block 2 ------'
                      |
                     Lab

Annex - Workshop      (disconnected from the rest)
```

## Complexity (V = locations, E = paths)
| | Adjacency list | Adjacency matrix |
|---|---|---|
| Space | O(V + E) | O(V²) |
| BFS / DFS time | O(V + E) | O(V²) |

## Screenshots to take
1. Option 1 (graph) and option 2 (matrix)
2. Option 3 – BFS route (e.g. Main Gate → Lab)
3. Option 4 – DFS traversal and disconnected locations
4. Option 3 or 5 with Main Gate → Workshop (unreachable)
5. Option 6 – test case output
