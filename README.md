# Data Structures & Algorithms in C++ 🛠️🚀

A comprehensive repository containing core data structures and foundational computer science algorithms implemented from scratch in C++. This project serves as a personal laboratory for deeply understanding memory management, pointer manipulation, and complex algorithmic efficiency.

## 🌿 Repository Architecture

To keep the production environment clean and well-documented, this repository utilizes a multi-branch workflow:
- **`main`** (Current Branch): Contains only the clean production documentation (`README.md`).
- **`implemented-structures`**: Contains the raw, fully operational C++ source files and automated unit tests.

---

## 📦 What's Inside?

The following core modules are fully implemented on the `implemented-structures` branch:

### 1. Linear Data Structures
- **`DoublyLinkedList.cpp`**: A custom, template-based implementation of a Doubly Linked List ADT with a bidirectional Iterator framework.
- **`StackAndDeque.cpp`**: Custom Stack (sequential element processing) and Double-Ended Queue (Deque) structures, including vector search utilities.

### 2. Advanced Trees & Graphs
- **`AVLTreeMap.cpp`**: A self-balancing binary search tree (AVL Tree) supporting dynamic single/double rotations (Left, Right, Left-Right, Right-Left) to maintain optimal logarithmic time complexity.
- **`GraphAndHashMap.cpp`**: A complete Directed Graph implementation using an adjacency matrix representation, featuring Breadth-First Search (BFS) and Depth-First Search (DFS) traversals, paired with a Chaining Hash Map using custom hashing.

### 3. Sorting & Optimization Algorithms
- **`SortingAndHeap.cpp`**: High-performance linear sorting via Radix Sort (backed by Counting Sort) and a Binary Heap implementation used for building Heap Sort.

---

## 🛠️ Key Concepts Explored

- **Memory Management:** Safe dynamic allocation and deallocation (`new` / `delete`), avoiding memory leaks through proper destructors and deep copying.
- **Object-Oriented Design:** Extensive usage of C++ templates for generic programming, pure virtual interface classes, and operator overloading.
- **Self-Balancing Logic:** Complex pointer manipulation required for tree rotations and restructuring under-the-hood.
