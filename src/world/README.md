# World File Construction

World rule files begin with a blank line followed by up to 2^32 import lines.

import lines look like this

```text
> (filename)
```

for example:

```text
> buildings.txt
```

Imported files may not have import lines.

Other world files are text files containing a series of node type blocks,
up to 2^32 node types can be included, they are delimited by an empty line,
they do not need to appear in order.

A node type uses this format:

```text
string: type name
integer 0 to 100: red, green, blue
0 to 2^32 replacement rules
```

The first node type is the whole world, node types are delimited by at least one blank line.

Replacement rules use this format:

```text
| (up/left/up/down: split direction) (integer 0 to 100: split percent) (node type name) (node type name)
```

The integers can be as is to be a proportion of the parent,
or have the following units (um, mm, cm, m, km), such as 50cm. For example:

```text
world
0 80 0
| up 50 room garden
| up 50cm garden garden

room
0 20 80

garden
0 200 0
```

Some node types are built in, these are the most common:

```text
_ - nothing/invalid/unknown/unimplemented - black

solid colors:
    red - most common color names are included
    134 - the RGB color (32, 128, 256)

dividing into sub-objects on a grid up to 5x5 in size:
    std.h3, std.v23, std.v2.decay.50_left - 1D grid - horizontal/vertical divisions
    std.chess.3, std.chess.23, std.chess.121_311 - like std.h2 and std.h32 but for a 2D grid, up to 5x5
```
