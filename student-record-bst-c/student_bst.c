/*
 * Student Record Search System Using a Binary Search Tree (BST)
 * -------------------------------------------------------------
 * Each record: enrolment ID (unique BST key), name, department.
 *
 * Covers:
 *   - BST insertion (duplicate IDs rejected with a message)
 *   - Search by enrolment ID
 *   - Deletion: leaf, one-child and two-child cases
 *   - Inorder (sorted), preorder and postorder traversals
 *   - Menu-driven interface with 10 sample records
 *   - Comparison count + height: balanced-looking vs sorted insertion order
 *   - 5 built-in test cases (menu option 9)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- data structures ---------- */
typedef struct {
    int  id;                 /* enrolment ID (BST key) */
    char name[30];
    char dept[30];
} Student;

typedef struct Node {
    Student data;
    struct Node *left, *right;
} Node;

/* 10 sample records, listed in a balanced-looking insertion order */
Student samples[10] = {
    {1008, "Aarav",    "CSE"}, {1004, "Priya", "IT"},  {1012, "Rohan",  "ECE"},
    {1002, "Simran",   "CSE"}, {1006, "Karan", "ME"},  {1010, "Neha",   "IT"},
    {1014, "Vikram",   "CSE"}, {1001, "Anjali","ECE"}, {1003, "Harpreet","CE"},
    {1007, "Meera",    "CSE"}
};

/* tells the caller which deletion case happened: 1 leaf, 2 one child, 3 two children */
const char *case_names[] = {"none", "leaf node", "one child", "two children"};

/* ---------- insertion (recursive) ----------
 * *cmp counts the comparisons made; *inserted is 0 for a duplicate ID.
 */
Node *insert(Node *root, Student s, int *cmp, int *inserted) {
    if (root == NULL) {                              /* empty spot found */
        Node *n = (Node *)malloc(sizeof(Node));
        n->data = s;
        n->left = n->right = NULL;
        *inserted = 1;
        return n;
    }
    (*cmp)++;
    if (s.id == root->data.id) {                     /* duplicate key */
        *inserted = 0;
        return root;
    }
    if (s.id < root->data.id)
        root->left = insert(root->left, s, cmp, inserted);
    else
        root->right = insert(root->right, s, cmp, inserted);
    return root;
}

/* ---------- search (recursive) ---------- */
Node *search(Node *root, int id, int *cmp) {
    if (root == NULL) return NULL;
    (*cmp)++;
    if (id == root->data.id) return root;
    if (id < root->data.id) return search(root->left, id, cmp);
    return search(root->right, id, cmp);
}

/* ---------- deletion ---------- */
Node *find_min(Node *root) {                         /* leftmost node */
    while (root->left != NULL) root = root->left;
    return root;
}

/* *found = 1 if the ID existed; *kind = which deletion case was used */
Node *delete_node(Node *root, int id, int *found, int *kind) {
    if (root == NULL) return NULL;                   /* ID not in the tree */

    if (id < root->data.id) {
        root->left = delete_node(root->left, id, found, kind);
    } else if (id > root->data.id) {
        root->right = delete_node(root->right, id, found, kind);
    } else {
        *found = 1;
        if (root->left == NULL && root->right == NULL) {     /* Case 1: leaf */
            *kind = 1;
            free(root);
            return NULL;
        }
        if (root->left == NULL || root->right == NULL) {     /* Case 2: one child */
            Node *child = (root->left != NULL) ? root->left : root->right;
            *kind = 2;
            free(root);
            return child;
        }
        /* Case 3: two children -> copy the inorder successor (smallest ID in the
           right subtree) here, then delete that successor from the right subtree */
        Node *succ = find_min(root->right);
        root->data = succ->data;
        int f2 = 0, k2 = 0;
        root->right = delete_node(root->right, succ->data.id, &f2, &k2);
        *kind = 3;
    }
    return root;
}

/* ---------- traversals ---------- */
void print_student(Student s) {
    printf("  ID: %-5d | Name: %-9s | Dept: %s\n", s.id, s.name, s.dept);
}

void inorder(Node *root) {                           /* Left, Root, Right -> sorted */
    if (root == NULL) return;
    inorder(root->left);
    print_student(root->data);
    inorder(root->right);
}

void preorder(Node *root) {                          /* Root, Left, Right */
    if (root == NULL) return;
    print_student(root->data);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node *root) {                         /* Left, Right, Root */
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    print_student(root->data);
}

/* same as inorder but collects the IDs in a string (used by the test cases) */
void inorder_ids(Node *root, char *buf) {
    if (root == NULL) return;
    inorder_ids(root->left, buf);
    char t[12];
    sprintf(t, "%d ", root->data.id);
    strcat(buf, t);
    inorder_ids(root->right, buf);
}

int height(Node *root) {                             /* empty = 0, one node = 1 */
    if (root == NULL) return 0;
    int l = height(root->left), r = height(root->right);
    return 1 + (l > r ? l : r);
}

void free_tree(Node *root) {                         /* postorder free */
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

/* ---------- balanced-looking vs sorted insertion ---------- */
int compare_ids(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
}

void run_one_order(const char *label, int ids[], int n) {
    Node *root = NULL;
    int insert_cmp = 0, search_cmp = 0;
    for (int i = 0; i < n; i++) {
        Student s = { ids[i], "Student", "CSE" };
        int inserted;
        root = insert(root, s, &insert_cmp, &inserted);
    }
    for (int i = 0; i < n; i++) search(root, ids[i], &search_cmp);
    printf("%-26s%17d%21.2f%8d\n", label, insert_cmp, (double)search_cmp / n, height(root));
    free_tree(root);
}

void compare_orders(void) {
    int balanced[10], sorted[10];
    for (int i = 0; i < 10; i++) balanced[i] = sorted[i] = samples[i].id;
    qsort(sorted, 10, sizeof(int), compare_ids);        /* ascending order */

    printf("\nSame 10 enrolment IDs, two insertion orders:\n");
    printf("%-26s%17s%21s%8s\n", "Insertion order", "Insert compares", "Avg search compares", "Height");
    run_one_order("Balanced-looking order", balanced, 10);
    run_one_order("Sorted (ascending) order", sorted, 10);
}

/* ---------- menu helpers ---------- */
int read_int(const char *prompt, int *out) {
    char line[32], *end;
    printf("%s", prompt);
    if (fgets(line, sizeof line, stdin) == NULL) return 0;
    long v = strtol(line, &end, 10);
    if (end == line) { printf("  Please enter a number.\n"); return 0; }
    *out = (int)v;
    return 1;
}

void read_text(const char *prompt, char *dest, int size) {
    printf("%s", prompt);
    if (fgets(dest, size, stdin) == NULL) dest[0] = '\0';
    dest[strcspn(dest, "\r\n")] = '\0';
}

void show_tree(const char *title, void (*traverse)(Node *), Node *root) {
    printf("\n%s\n", title);
    if (root == NULL) printf("  (tree is empty)\n");
    traverse(root);
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

Node *build_sample_tree(void) {
    Node *root = NULL;
    for (int i = 0; i < 10; i++) {
        int cmp = 0, ins;
        root = insert(root, samples[i], &cmp, &ins);
    }
    return root;
}

void run_tests(void) {
    char actual[200];
    tests_passed = tests_total = 0;
    printf("\n===== TEST CASES =====\n");
    printf("(sample tree: 1008 root; 1004 and 1012 below it; leaves 1001 1003 1007 1010 1014)\n\n");

    /* TC1: delete a leaf */
    Node *root = build_sample_tree();
    int found = 0, kind = 0;
    root = delete_node(root, 1001, &found, &kind);
    actual[0] = '\0'; inorder_ids(root, actual);
    sprintf(actual + strlen(actual), "[%s]", case_names[kind]);
    report("TC1 Delete leaf node 1001",
           "1002 1003 1004 1006 1007 1008 1010 1012 1014 [leaf node]", actual);
    free_tree(root);

    /* TC2: delete a node with one child (1006 has only a right child, 1007) */
    root = build_sample_tree();
    found = kind = 0;
    root = delete_node(root, 1006, &found, &kind);
    actual[0] = '\0'; inorder_ids(root, actual);
    sprintf(actual + strlen(actual), "[%s]", case_names[kind]);
    report("TC2 Delete node with one child 1006",
           "1001 1002 1003 1004 1007 1008 1010 1012 1014 [one child]", actual);
    free_tree(root);

    /* TC3: delete a node with two children (1004 -> replaced by successor 1006) */
    root = build_sample_tree();
    found = kind = 0;
    root = delete_node(root, 1004, &found, &kind);
    actual[0] = '\0'; inorder_ids(root, actual);
    sprintf(actual + strlen(actual), "[%s]", case_names[kind]);
    report("TC3 Delete node with two children 1004",
           "1001 1002 1003 1006 1007 1008 1010 1012 1014 [two children]", actual);
    free_tree(root);

    /* TC4: duplicate enrolment ID is rejected */
    root = build_sample_tree();
    Student dup = {1008, "Duplicate", "CSE"};
    int cmp = 0, inserted = 1;
    root = insert(root, dup, &cmp, &inserted);
    report("TC4 Insert duplicate ID 1008", "rejected", inserted ? "inserted" : "rejected");

    /* TC5: successful and unsuccessful search */
    cmp = 0;
    Node *hit = search(root, 1010, &cmp);
    cmp = 0;
    Node *miss = search(root, 9999, &cmp);
    sprintf(actual, "%s / %s", hit ? hit->data.name : "not found", miss ? "found" : "not found");
    report("TC5 Search 1010 (exists) and 9999 (missing)", "Neha / not found", actual);
    free_tree(root);

    printf("%d/%d test cases passed\n", tests_passed, tests_total);
}

/* ---------- main menu ---------- */
int main(void) {
    Node *root = NULL;
    char line[16];

    for (;;) {
        printf("\n===== STUDENT RECORD SEARCH SYSTEM (BST) =====\n");
        printf("1. Insert student          6. Postorder traversal\n");
        printf("2. Search by enrolment ID  7. Load 10 sample records\n");
        printf("3. Delete student          8. Compare balanced vs sorted insertion\n");
        printf("4. Inorder (sorted)        9. Run test cases\n");
        printf("5. Preorder traversal      0. Exit\n");
        printf("Enter choice: ");
        if (fgets(line, sizeof line, stdin) == NULL) break;

        switch (line[0]) {
        case '1': {
            Student s;
            int cmp = 0, inserted;
            if (!read_int("Enrolment ID: ", &s.id)) break;
            read_text("Name: ", s.name, sizeof s.name);
            read_text("Department: ", s.dept, sizeof s.dept);
            root = insert(root, s, &cmp, &inserted);
            if (inserted) printf("Record inserted (%d comparisons).\n", cmp);
            else printf("Duplicate! Enrolment ID %d already exists. Record rejected.\n", s.id);
            break;
        }
        case '2': {
            int id, cmp = 0;
            if (!read_int("Enrolment ID to search: ", &id)) break;
            Node *n = search(root, id, &cmp);
            if (n) { printf("FOUND (%d comparisons):\n", cmp); print_student(n->data); }
            else   printf("NOT FOUND: no student with ID %d (%d comparisons).\n", id, cmp);
            break;
        }
        case '3': {
            int id, found = 0, kind = 0;
            if (!read_int("Enrolment ID to delete: ", &id)) break;
            root = delete_node(root, id, &found, &kind);
            if (found) printf("Deleted ID %d (case: %s).\n", id, case_names[kind]);
            else       printf("ID %d not found. Nothing deleted.\n", id);
            break;
        }
        case '4': show_tree("Inorder (sorted by enrolment ID):", inorder, root);  break;
        case '5': show_tree("Preorder:", preorder, root);                         break;
        case '6': show_tree("Postorder:", postorder, root);                       break;
        case '7':
            for (int i = 0; i < 10; i++) {
                int cmp = 0, inserted;
                root = insert(root, samples[i], &cmp, &inserted);
                printf("  %s: %d %s\n", inserted ? "Inserted" : "Duplicate, skipped",
                       samples[i].id, samples[i].name);
            }
            break;
        case '8': compare_orders(); break;
        case '9': run_tests();      break;
        case '0': free_tree(root); printf("Goodbye!\n"); return 0;
        default:  printf("Invalid choice.\n");
        }
    }
    free_tree(root);
    return 0;
}
