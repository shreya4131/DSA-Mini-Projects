/*
 * Campus Route Finder Using Graphs
 * --------------------------------
 * Campus locations = vertices, paths between locations = edges (undirected, unweighted).
 *
 * Covers:
 *   - Graph with 10 locations stored as an ADJACENCY LIST (linked lists)
 *   - Adjacency MATRIX (optional) built from the list, to compare both
 *   - BFS  : route with the minimum number of edges (queue + visited + parent array)
 *   - DFS  : all reachable locations / reachability check (recursion + visited array)
 *   - Disconnected locations (not visited by DFS)
 *   - 5 built-in test cases (menu option 6)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_V 10                         /* number of locations */

/* ---------- adjacency list node ---------- */
typedef struct AdjNode {
    int dest;                            /* neighbouring location */
    struct AdjNode *next;
} AdjNode;

char names[MAX_V][20] = {
    "Main Gate", "Library", "Block 1", "Block 2", "Cafeteria",
    "Auditorium", "Parking", "Lab", "Annex", "Workshop"
};
AdjNode *adj[MAX_V];                     /* adj[i] = list of neighbours of location i */

/* ---------- building the graph ---------- */

/* add edge u -> v at the END of u's list */
void add_one_way(int u, int v) {
    AdjNode *node = (AdjNode *)malloc(sizeof(AdjNode));
    node->dest = v;
    node->next = NULL;
    if (adj[u] == NULL) {
        adj[u] = node;
    } else {
        AdjNode *cur = adj[u];
        while (cur->next != NULL) cur = cur->next;
        cur->next = node;
    }
}

/* undirected path: store the edge in both directions */
void add_path(int u, int v) {
    add_one_way(u, v);
    add_one_way(v, u);
}

void build_graph(void) {
    for (int i = 0; i < MAX_V; i++) adj[i] = NULL;
    add_path(0, 6);   /* Main Gate  - Parking    */
    add_path(0, 1);   /* Main Gate  - Library    */
    add_path(0, 2);   /* Main Gate  - Block 1    */
    add_path(1, 2);   /* Library    - Block 1    */
    add_path(1, 4);   /* Library    - Cafeteria  */
    add_path(2, 3);   /* Block 1    - Block 2    */
    add_path(3, 7);   /* Block 2    - Lab        */
    add_path(3, 5);   /* Block 2    - Auditorium */
    add_path(4, 5);   /* Cafeteria  - Auditorium */
    add_path(8, 9);   /* Annex      - Workshop   (separate from the rest of the campus) */
}

void free_graph(void) {
    for (int i = 0; i < MAX_V; i++) {
        AdjNode *cur = adj[i];
        while (cur != NULL) {
            AdjNode *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        adj[i] = NULL;
    }
}

/* ---------- display: adjacency list and adjacency matrix ---------- */
void display_list(void) {
    printf("\nAdjacency list:\n");
    for (int i = 0; i < MAX_V; i++) {
        printf("  %-10s -> ", names[i]);
        for (AdjNode *e = adj[i]; e != NULL; e = e->next)
            printf("%s%s", names[e->dest], e->next ? ", " : "");
        printf("\n");
    }
}

void display_matrix(void) {
    int matrix[MAX_V][MAX_V] = {0};
    for (int u = 0; u < MAX_V; u++)
        for (AdjNode *e = adj[u]; e != NULL; e = e->next)
            matrix[u][e->dest] = 1;

    printf("\nAdjacency matrix (1 = direct path):\n      ");
    for (int j = 0; j < MAX_V; j++) printf("%3d", j);
    printf("\n");
    for (int i = 0; i < MAX_V; i++) {
        printf("%3d | ", i);
        for (int j = 0; j < MAX_V; j++) printf("%3d", matrix[i][j]);
        printf("   %s\n", names[i]);
    }
}

/* ---------- BFS: minimum number of edges ----------
 * Returns 1 and fills path[] / *len if a route exists, otherwise 0.
 * Time O(V + E), space O(V).
 */
int bfs_route(int src, int dst, int path[], int *len) {
    int visited[MAX_V] = {0};
    int parent[MAX_V];
    int queue[MAX_V];
    int front = 0, rear = 0;

    for (int i = 0; i < MAX_V; i++) parent[i] = -1;

    visited[src] = 1;
    queue[rear++] = src;                       /* enqueue source */

    while (front < rear) {
        int u = queue[front++];                /* dequeue */
        if (u == dst) break;
        for (AdjNode *e = adj[u]; e != NULL; e = e->next) {
            if (!visited[e->dest]) {
                visited[e->dest] = 1;
                parent[e->dest] = u;           /* remember who discovered it */
                queue[rear++] = e->dest;
            }
        }
    }

    if (!visited[dst]) { *len = 0; return 0; } /* destination never reached */

    /* path reconstruction: walk back from dst to src using parent[] */
    int temp[MAX_V], n = 0;
    for (int cur = dst; cur != -1; cur = parent[cur]) temp[n++] = cur;
    for (int i = 0; i < n; i++) path[i] = temp[n - 1 - i];   /* reverse */
    *len = n;
    return 1;
}

/* ---------- DFS (recursive): visit everything reachable from u ----------
 * Time O(V + E), space O(V) (visited array + recursion stack).
 */
void dfs(int u, int visited[], int order[], int *count) {
    visited[u] = 1;
    order[(*count)++] = u;
    for (AdjNode *e = adj[u]; e != NULL; e = e->next)
        if (!visited[e->dest])
            dfs(e->dest, visited, order, count);
}

/* ---------- helpers for the menu ---------- */
void print_locations(void) {
    printf("\nLocations:\n");
    for (int i = 0; i < MAX_V; i++) printf("  %d. %s\n", i, names[i]);
}

/* read a location number (0..MAX_V-1); returns -1 on bad input */
int read_location(const char *prompt) {
    char line[32], *end;
    printf("%s", prompt);
    if (fgets(line, sizeof line, stdin) == NULL) return -1;
    long v = strtol(line, &end, 10);
    if (end == line || v < 0 || v >= MAX_V) {
        printf("  Invalid location number.\n");
        return -1;
    }
    return (int)v;
}

void print_path(const int path[], int len) {
    for (int i = 0; i < len; i++)
        printf("%s%s", names[path[i]], i < len - 1 ? " -> " : "");
    printf("\n");
}

/* ---------- menu actions ---------- */
void action_bfs(void) {
    print_locations();
    int s = read_location("Enter source number: ");      if (s < 0) return;
    int d = read_location("Enter destination number: "); if (d < 0) return;
    int path[MAX_V], len;
    if (bfs_route(s, d, path, &len)) {
        printf("BFS route : ");
        print_path(path, len);
        printf("Edges     : %d\n", len - 1);
    } else {
        printf("No route exists from %s to %s.\n", names[s], names[d]);
    }
}

void action_dfs(void) {
    print_locations();
    int s = read_location("Enter starting location number: "); if (s < 0) return;
    int visited[MAX_V] = {0}, order[MAX_V], count = 0;
    dfs(s, visited, order, &count);

    printf("Reachable from %s (DFS order): ", names[s]);
    print_path(order, count);

    printf("Disconnected (not reachable): ");
    int found = 0;
    for (int i = 0; i < MAX_V; i++)
        if (!visited[i]) { printf("%s%s", found ? ", " : "", names[i]); found = 1; }
    printf("%s\n", found ? "" : "none");
}

void action_reachable(void) {
    print_locations();
    int s = read_location("Enter source number: ");      if (s < 0) return;
    int d = read_location("Enter destination number: "); if (d < 0) return;
    int visited[MAX_V] = {0}, order[MAX_V], count = 0;
    dfs(s, visited, order, &count);
    printf("%s is %s from %s.\n", names[d],
           visited[d] ? "REACHABLE" : "NOT reachable", names[s]);
}

/* ---------- test cases (expected vs actual) ---------- */
int tests_passed = 0, tests_total = 0;

void report(const char *title, const char *expected, const char *actual) {
    int ok = (strcmp(expected, actual) == 0);
    tests_total++;
    if (ok) tests_passed++;
    printf("%s\n  Expected: %s\n  Actual  : %s\n  Result  : %s\n\n",
           title, expected, actual, ok ? "PASS" : "FAIL");
}

/* run BFS and turn the answer into text */
void route_text(int s, int d, char *out) {
    int path[MAX_V], len;
    out[0] = '\0';
    if (!bfs_route(s, d, path, &len)) { strcpy(out, "No route"); return; }
    for (int i = 0; i < len; i++) {
        strcat(out, names[path[i]]);
        if (i < len - 1) strcat(out, " -> ");
    }
}

void run_tests(void) {
    char actual[200];
    tests_passed = tests_total = 0;
    printf("\n===== TEST CASES =====\n\n");

    route_text(0, 1, actual);
    report("TC1 Direct route: Main Gate -> Library", "Main Gate -> Library", actual);

    route_text(0, 7, actual);
    report("TC2 Multi-stop route: Main Gate -> Lab",
           "Main Gate -> Block 1 -> Block 2 -> Lab", actual);

    route_text(0, 9, actual);
    report("TC3 Unreachable destination: Main Gate -> Workshop", "No route", actual);

    int visited[MAX_V] = {0}, order[MAX_V], count = 0;
    dfs(0, visited, order, &count);
    sprintf(actual, "%d", count);
    report("TC4 DFS from Main Gate: number of reachable locations", "8", actual);

    actual[0] = '\0';
    for (int i = 0; i < MAX_V; i++)
        if (!visited[i]) { if (actual[0]) strcat(actual, ", "); strcat(actual, names[i]); }
    report("TC5 Disconnected locations from Main Gate", "Annex, Workshop", actual);

    printf("%d/%d test cases passed\n", tests_passed, tests_total);
}

/* ---------- main menu ---------- */
int main(void) {
    char line[16];
    build_graph();

    for (;;) {
        printf("\n===== CAMPUS ROUTE FINDER =====\n");
        printf("1. Display graph (adjacency list)\n");
        printf("2. Display adjacency matrix\n");
        printf("3. Find route between two places (BFS)\n");
        printf("4. Show all reachable + disconnected locations (DFS)\n");
        printf("5. Check if a destination is reachable (DFS)\n");
        printf("6. Run test cases\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        if (fgets(line, sizeof line, stdin) == NULL) break;

        switch (line[0]) {
            case '1': display_list();      break;
            case '2': display_matrix();    break;
            case '3': action_bfs();        break;
            case '4': action_dfs();        break;
            case '5': action_reachable();  break;
            case '6': run_tests();         break;
            case '0': free_graph(); printf("Goodbye!\n"); return 0;
            default:  printf("Invalid choice.\n");
        }
    }
    free_graph();
    return 0;
}
