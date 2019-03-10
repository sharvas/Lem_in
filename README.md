# lem_in

This is a pathfinding algorithm project. The goal of the project is to find the fastest way for the number of ants to move through the network of rooms and tunnels.

The program cannot have memory leaks. All errors must be handled carefully. In no way can the program quit in an unexpected manner (Segmentation fault, bus error, double free, etc). Allowed functions for the mandatory part are ```malloc, free, read, write, strerror, perror``` and ```exit```.

The input is received in the following format:
```
number_of_ants
the_rooms
the_links
```
The rooms are defined by ```name coord_x coord_y```. The links are defined by ```name1-name2``` and all is broken by comments, which start with ```#```. Lines starting with ## are commands (for example marking start/end room).

An example map:
```
#number_of_ants
4
#the_rooms
3 2 2
##start
start 4 0
##end
end 4 6
4 0 4
1 4 2
2 4 4
5 8 2
6 8 4
#the_links
start-1
3-4
2-4
1-5
6-5
end-6
1-2
2-end
3-start
```

The schematic represantation of the map:

![map](https://github.com/sharvas/lem_in/raw/master/images/map.png)

## usage

To compile the project - run ```make```. It will compaile an executable ```lem-in```. Running it witht an ```-h``` flag will display the usage:
```
➜ lem_in ./lem-in -h
usage: ./lem-in [-a] [-r] [-l] [-rl] [-p] [-g] [-t] [-all] [-wc] < map.map

    [-a] display number of ants
    [-r] display rooms
    [-l] display links
    [-rl] display rooms with links
    [-p] display paths
    [-g] display path groups
    [-t] display number of turns
    [-all] display all
    [-wc] display map with comments
```

There are many options to display various stages of program execution. For the parsing, ```-a -r -l``` flags will show an essential information that was read and ```-rl``` flag displays what is put in the graph - nodes (rooms) and edges (links).

Flags ```-p -g -t``` display information related to the algorithm - discovered paths, groups of paths and, finally, how many turns did it take to move all the ants through the network.


## algorithm and the output

The output of the map example above uses two paths:
```
start -> 1 -> 5 -> 6 -> end
```
```
start -> 3 -> 4 -> 2 -> end
```
```
➜ lem_in ./lem-in < maps/pdf_example3.map
...

L1-1 L2-3
L1-5 L2-4 L3-1 L4-3
L1-6 L2-2 L3-5 L4-4
L1-end L2-end L3-6 L4-2
L3-end L4-end
```
```
➜ lem_in ./lem-in -g < maps/pdf_example3.map
...

The chosen group:

-- path 1 --
start
1
5
6
end
length = 5

-- path 2 --
start
3
4
2
end
length = 5

depth - 2
```

The output is in the format ```L(ant number)-(room number)```. ```L1-1``` means ant one in the first room. The algorithm chose to take two paths (**depth of 2**) for 4 ants. For 2 ants it would take only one (**depth of 1**) and the shortest path (```start -> 1 -> 2 -> end```).

```
➜ lem_in ./lem-in < maps/pdf_example3_2ants.map
...

L1-1
L1-2 L2-1
L1-end L2-2
L2-end
```

```
➜ lem_in ./lem-in -g < maps/pdf_example3_2ants.map
...

The chosen group:

-- path 1 --
start
1
2
end
length = 4

depth - 1
```

The inspiration for the algorithm was taken from the Edmonds–Karp max flow problem-solving algorithm, using a breadth-first search to find paths.

## test script

To evaluate the algorithm's performance a shell script was written. Randomly gerenated maps have the expected benchmark, and the script compares the performance (```avg moves diff```). If it is 0, the performance is spot on, if a positive number - the algorithm performed worse by this many turns.

```
➜  lem_in ./test_performance.sh
performance test --> place the 'generator' in ./tools/ and run the script

-- 50/50 --
--flow-one
avg moves diff: 0

real    0m0.037s
user    0m0.022s
sys     0m0.019s

-- 50/50 --
--flow-ten
avg moves diff: 0

real    0m0.034s
user    0m0.022s
sys     0m0.018s

-- 50/50 --
--flow-thousand
avg moves diff: 0

real    0m0.080s
user    0m0.060s
sys     0m0.031s

-- 25/25 --
--big
avg moves diff: 0

real    0m0.458s
user    0m0.394s
sys     0m0.103s

-- 25/25 --
--big-superposition
avg moves diff: 6

real    0m1.379s
user    0m1.274s
sys     0m0.159s
```
