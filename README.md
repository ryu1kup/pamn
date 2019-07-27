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
$ ../bin/pamn -i <init>.txt -o <output>.root
```
where, `<init>.txt` is a txt file that list up the full paths to nSort output files,
and `<output>.root` is the pamn output root file.
