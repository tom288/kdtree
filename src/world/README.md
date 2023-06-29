# Constructing Worlds

## Import File

world.txt is an import file, it contains up to 2^32 import lines

each import line specifies a world file to import

Example import lines:

```text
standard.txt
buildings.txt
world2.txt
```

---

## World Files

World files cannot have import lines.

World files contain up to 2^32 node type blocks, they are delimited by an empty line,
they do not need to appear in the order they are used in.

---

## Terminology

indentation level - To indent a line a level start that line with four extra spaces.

color - A color, each color channel is an integer in the range 0 to 255.

---

## Node Type Block

Each node type block starts with the node types name.

```text
world
```

Subsequent lines of text in the block are indented at least one level.

The first line is the node types color.

```text
    0 80 0
```

The block then ends with up to 2^32 replacement rule blocks.

---

## Replacement Rule Line

Replacement rules start with an orientation, orientations are either "left", "up", "right", "down",

followed by an integer denoting the node types split percent (0 to 100).

```text
    left 50
```

The integer optionally ends with a unit, the available units are nm, um, mm, cm, m, km.

```text
    left 20cm
```

An example replacement rule:

```text
    left 50
        sofa
            size: 5
            col: red
        carpet
```

In this example `size` and `col` are parameters being passed to `sofa`,

parameters can be node type names, integers, or colors (R, G, B) in the range 0 to 255.

---

## Builtin Node Types

Some node types are built in, these are the most common:

```text
solid colors:
    red - most common color names are included
    134 - the RGB color (32, 128, 256)

_ - nothing/invalid/unknown/unimplemented - black

dividing into sub-objects on a grid up to 5x5 in size:
    std.h3, std.v23, std.v2.decay.50_left - 1D grid - horizontal/vertical divisions
    std.chess.3, std.chess.23, std.chess.121_311 - like std.h2 and std.h32 but for a 2D grid, up to 5x5
```
