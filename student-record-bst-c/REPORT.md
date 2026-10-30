# Short Report – Student Record Search System Using BST

## 1. BST structure
Each node stores a `Student` (enrolment ID, name, department) and pointers to its left and right children. **BST property:** all IDs in the left subtree are smaller than the node's ID and all IDs in the right subtree are larger. The enrolment ID is the key and must be unique.

Tree built from the 10 sample records (inserted in the order 1008, 1004, 1012, 1002, 1006, 1010, 1014, 1001, 1003, 1007):
```
                 1008
            /            \
         1004            1012
        /    \          /    \
     1002    1006    1010    1014
     /  \       \
  1001  1003    1007
```

## 2. Algorithms
- **Insert (recursive):** compare the new ID with the current node; go left if smaller, right if larger; place the record in the first empty position. If an equal ID is found, the record is rejected with a message.
- **Search (recursive):** same path as insertion; return the record if the ID matches, or "not found" at an empty position.
- **Delete (recursive):**
  1. *Leaf node:* remove it.
  2. *One child:* replace the node with its only child.
  3. *Two children:* copy the **inorder successor** (smallest ID in the right subtree) into the node, then delete that successor from the right subtree.
- **Traversals:** inorder (Left, Root, Right) gives sorted IDs; preorder (Root, Left, Right); postorder (Left, Right, Root).

## 3. Traversal output for the sample tree
| Traversal | Enrolment IDs |
|---|---|
| Inorder | 1001, 1002, 1003, 1004, 1006, 1007, 1008, 1010, 1012, 1014 |
| Preorder | 1008, 1004, 1002, 1001, 1003, 1006, 1007, 1012, 1010, 1014 |
| Postorder | 1001, 1003, 1002, 1007, 1006, 1004, 1010, 1014, 1012, 1008 |

## 4. Test cases (menu option 9)
| # | Scenario | Expected inorder IDs afterwards / result | Actual | Result |
|---|---|---|---|---|
| TC1 | Delete **leaf** 1001 | 1002 1003 1004 1006 1007 1008 1010 1012 1014, case "leaf node" | same | PASS |
| TC2 | Delete node with **one child** 1006 | 1001 1002 1003 1004 1007 1008 1010 1012 1014, case "one child" | same | PASS |
| TC3 | Delete node with **two children** 1004 | 1001 1002 1003 1006 1007 1008 1010 1012 1014, case "two children" | same | PASS |
| TC4 | Insert duplicate ID 1008 | rejected | rejected | PASS |
| TC5 | Search 1010 and 9999 | Neha / not found | Neha / not found | PASS |

All 5 test cases passed.

## 5. Balanced-looking vs sorted insertion (menu option 8)
The same 10 IDs were inserted in two orders. Comparisons are counted for inserting all 10 records and then searching for each one.

| Insertion order | Insert comparisons | Avg search comparisons | Height |
|---|---|---|---|
| Balanced-looking (1008, 1004, 1012, 1002, …) | 19 | 2.90 | 4 |
| Sorted ascending (1001, 1002, 1003, …) | 45 | 5.50 | 10 |

With sorted input every new ID is larger than all existing ones, so it becomes the right child of the previous node. The tree turns into a chain of height n, and each insertion or search must walk down the whole chain.

## 6. Time and space complexity
| Operation | Average case | Worst case |
|---|---|---|
| Insert | O(log n) | O(n) |
| Search | O(log n) | O(n) |
| Delete | O(log n) | O(n) |
| Inorder / preorder / postorder | O(n) | O(n) |
| Space | O(n) | O(n) |

Recursive operations use O(h) call-stack space, where h is the tree height.

## 7. Why a BST can degrade to O(n), and how an AVL tree helps
Every BST operation follows one path from the root, so its cost is proportional to the tree height h. The BST property does not control the shape of the tree: inserting IDs in sorted or reverse-sorted order creates a skewed tree with h = n, so search, insert and delete become O(n).

An **AVL tree** is a self-balancing BST. It keeps the *balance factor* (height of left subtree − height of right subtree) of every node at −1, 0 or +1. After an insertion or deletion, if a node becomes unbalanced, the tree restores balance using **rotations** (left, right, left-right, right-left). This keeps the height at O(log n), so search, insert and delete stay O(log n) even in the worst case.

## 8. Conclusion
The BST supports insertion, search, deletion and sorted display with O(log n) average cost, but its speed depends on the tree shape. The experiment shows that sorted insertion is much slower, which is why self-balancing trees such as AVL are used.
