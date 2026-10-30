# Short Report – Campus Route Finder Using Graphs

## 1. Graph representation
Campus locations are **vertices** and walking paths are **edges**. The graph is undirected and unweighted, so every path is stored in both directions. There are 10 locations and 10 paths.

**Adjacency list (main representation).** An array `adj[]` of linked lists; `adj[i]` holds the neighbours of location *i*:
```
Main Gate  -> Parking, Library, Block 1
Library    -> Main Gate, Block 1, Cafeteria
Block 1    -> Main Gate, Library, Block 2
Block 2    -> Block 1, Lab, Auditorium
Cafeteria  -> Library, Auditorium
Auditorium -> Block 2, Cafeteria
Parking    -> Main Gate
Lab        -> Block 2
Annex      -> Workshop
Workshop   -> Annex
```
Annex and Workshop are connected only to each other, so they are the disconnected locations.

**Adjacency matrix (optional).** A 10×10 table where `matrix[i][j] = 1` if a direct path exists. It is built from the list and displayed by menu option 2.

## 2. Algorithms
**BFS (shortest route).** Uses a **queue**, a **visited array** and a **parent array**. Starting from the source, locations are explored level by level, so the first time the destination is reached the route has the minimum number of edges. `parent[v]` records which location discovered *v*; the route is rebuilt by walking from the destination back to the source and reversing it.

**DFS (reachability).** Recursive; marks a location in the **visited array**, then visits each unvisited neighbour before backtracking (the recursion acts as the stack). Every location marked visited is reachable from the start; the unmarked ones are the **disconnected** locations. Reachability of a destination = whether it is marked visited.

## 3. Sample input / output
```
Option 3 – source 0 (Main Gate), destination 7 (Lab)
BFS route : Main Gate -> Block 1 -> Block 2 -> Lab
Edges     : 3

Option 4 – start 0 (Main Gate)
Reachable from Main Gate (DFS order): Main Gate -> Parking -> Library -> Block 1 -> Block 2 -> Lab -> Auditorium -> Cafeteria
Disconnected (not reachable): Annex, Workshop

Option 3 – source 0 (Main Gate), destination 9 (Workshop)
No route exists from Main Gate to Workshop.

Option 5 – source 0, destination 9
Workshop is NOT reachable from Main Gate.
```

## 4. Test cases (menu option 6)
| # | Scenario | Expected | Actual | Result |
|---|---|---|---|---|
| TC1 | Direct route: Main Gate → Library | Main Gate -> Library | Main Gate -> Library | PASS |
| TC2 | Multi-stop: Main Gate → Lab | Main Gate -> Block 1 -> Block 2 -> Lab | same | PASS |
| TC3 | Unreachable: Main Gate → Workshop | No route | No route | PASS |
| TC4 | DFS from Main Gate: reachable count | 8 | 8 | PASS |
| TC5 | Disconnected from Main Gate | Annex, Workshop | Annex, Workshop | PASS |

All 5 test cases passed.

## 5. Time and space complexity
V = number of locations, E = number of paths.

| Algorithm | Time | Space | Reason |
|---|---|---|---|
| BFS (adjacency list) | O(V + E) | O(V) | each location is enqueued once and each edge examined at most twice |
| DFS (adjacency list) | O(V + E) | O(V) | visited array + recursion stack |
| BFS / DFS (adjacency matrix) | O(V²) | O(V) | finding neighbours needs a scan of a whole row |

## 6. Adjacency list vs adjacency matrix
| Criterion | Adjacency list | Adjacency matrix |
|---|---|---|
| Storage | O(V + E) | O(V²) |
| Check if edge (u, v) exists | O(degree of u) | O(1) |
| List neighbours of u | O(degree of u) | O(V) |
| BFS / DFS | O(V + E) | O(V²) |
| Best for | Sparse graphs | Dense graphs |

A campus map is sparse (each location has only a few direct paths), so the adjacency list uses less memory and gives faster traversal.

## 7. Conclusion
BFS finds the route with the fewest edges in an unweighted graph, DFS identifies reachable and disconnected locations, and the adjacency list gives O(V + E) traversal for this sparse graph.
