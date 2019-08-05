import argparse
import subprocess

# parse arguments
parser = argparse.ArgumentParser()
parser.add_argument('-i', '--input', help='init txt file listing paths to Sort output files')
parser.add_argument('-o', '--output', help='pamn output root file')
args = parser.parse_args()

# init
if args.input is not None:
    init = args.input
else:
    init = 'init.txt'

# output
if args.output is not None:
    output = args.output
else:
    output = 'output.root'

pamn = './bin/pamn'
subprocess.call([pamn, '-i', init, '-o', output])
