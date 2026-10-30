# Student Record Search System Using a BST (C)

DSA Mini Project (PPF Week 9) – CGC University, Mohali

## What it does
- Student record: enrolment ID (BST key), name, department
- **Insert** (duplicate IDs rejected with a message), **search** by ID, **delete** (leaf, one-child, two-child)
- **Inorder** (sorted), **preorder**, **postorder** traversals
- Menu-driven; option 7 loads **10 sample records**
- Compares comparisons and height for **balanced-looking vs sorted** insertion (option 8)
- **5 built-in test cases** with expected vs actual output (option 9)

## Build and run
```bash
gcc -Wall -Wextra -std=c99 -o student_bst student_bst.c
./student_bst                  # Windows: student_bst.exe
```
Any C99 compiler works. No extra libraries.

## Menu
| Option | Action | Option | Action |
|---|---|---|---|
| 1 | Insert student | 6 | Postorder traversal |
| 2 | Search by ID | 7 | Load 10 sample records |
| 3 | Delete student | 8 | Compare balanced vs sorted insertion |
| 4 | Inorder (sorted) | 9 | Run test cases |
| 5 | Preorder traversal | 0 | Exit |

## Sample tree (after option 7)
```
                 1008
            /            \
         1004            1012
        /    \          /    \
     1002    1006    1010    1014
     /  \       \
  1001  1003    1007
```

## Complexity
| Operation | Average | Worst (skewed tree) |
|---|---|---|
| Insert / Search / Delete | O(log n) | O(n) |
| Traversals | O(n) | O(n) |
| Space | O(n) | O(n) |

## Screenshots to take
1. Option 7 (insertion) and a duplicate rejection (option 1 with ID 1008)
2. Option 2 – successful (1010) and unsuccessful (9999) search
3. Options 4, 5, 6 – all three traversals
4. Option 3 – deleting a leaf (1001), one-child node (1006), two-child node (1004)
5. Option 8 – comparison output; option 9 – test cases
