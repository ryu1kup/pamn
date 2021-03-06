# pamn
process and merge nSort output files

## compilation
ROOT package is required.
```
$ mkdir build
$ cd build
$ cmake .. && make -j && make install
```

## usage
```
$ mkdir work
$ cd work
$ ../bin/pamn -l input.txt -o output.root
```
where, `input.txt` is a txt file that list up the paths from the working directory to nSort output files,
and `output.root` is the pamn output root file.
Tips: You can make much easily input.txt as a following way,
```
$ ls /path/to/inputdir/*.root > input.txt
```

You can also specify the files in the following format.
```
$ ../bin/pamn -i /path/to/file/output*Sort.root
```

You can check all option by
```
$ ../bin/pamn -?
```




## branches
pamn output has the following branches.

| Branch Name | Unit | Description |
|-------------|------|-------------|
| ns          |      | Scatter times of the incident particle. |
| fv          |      | Fiducial volume parameter, which is less than 1 if the largest S2 scatter in TPC is in 4t fiducial volume. |
| NR          |      | Nuclear recoil parameter, which is 1 if the largest S2 scatter in TPC is nuclear recoil. |
| Ed          | keV  | Energy deposition by the largest S2 scatter in TPC.  |
| secondS2    | p.e. | the second largest S2.  |
| nhits       |      | Number of PMTs which have hit over threshold.  |


