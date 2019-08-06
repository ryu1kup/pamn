# pamn
process and merge nSort output files

## compilation
```
$ mkdir ./build
$ cd ./build
$ cmake ..
$ make && make install
```

## usage
```
$ mkdir ./work
$ cd ./work
$ ../bin/pamn -i input.txt -o output.root
```
where, `input.txt` is a txt file that list up the paths from the working directory to nSort output files,
and `output.root` is the pamn output root file.


Tips: You can make much easily input.txt as a following way,
```
$ ls /path/to/inputdir/*.root > input.txt
```

## branches
pamn output has the following branches.

- ns
- fv
- NR
- Ed
- secondS2
- nhits
