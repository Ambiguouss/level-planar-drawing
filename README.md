# Proper Planar Graph Embedding

This repo consists of implementation of $O(n^4)$ algorithm for embedding proper planar graphs.

Algorithm details can be found in `algo_description.pdf`.

## Usage

### Random graphs

To run the algorithm on some random graphs just run:

```bash
./random_graphs.sh
```

### Custom graphs

To run the algorithm on a custom graph you need to create an input file that represents the graph.

#### File format:

```
n              # number of levels
v_1            # number of vertices on 1st level
v_2            # number of vertices on 2nd level
...
v_n            # number of vertices on nth level

e_1            # number of edges between 1st and 2nd level
a_1 b_1        # edge between a_1st vertex on level 1 and b_1st vertex on level 2 (0-indexed)
a_2 b_2
...
a_e1 b_e1

e_2            # number of edges between 2nd and 3rd level
a_1 b_1
a_2 b_2
...
a_e2 b_e2

...

e_{n-1}        # number of edges between level n-1 and n
a_1 b_1
a_2 b_2
...
a_e_{n-1} b_e_{n-1}
```

- Vertices are **0 indexed**.
- For each edge `a b` between levels `i` and `i+1`, the following must hold:
  - `0 <= a < v_i`
  - `0 <= b < v_{i+1}`

Example input is present in `1.in`.

---

### Compile and Run

#### Compile:

```bash
g++ src/main.cpp -o bin/main
```

#### Run:

```bash
./bin/main < 1.in
```

---

### Output

- If the graph is **not planar**, the program outputs:
  - `res.tex`

- If the graph **is planar**, it outputs:
  - `res.tex`
  - `res_planar.tex`

To see the drawings, compile the `.tex` files using your favourite LaTeX compiler.
