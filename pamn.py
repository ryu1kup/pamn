import argparse
import subprocess

# parse arguments
parser = argparse.ArgumentParser()
parser.add_argument('-i', '--input', default='init.txt', help='init txt file listing paths to Sort output files')
parser.add_argument('-o', '--output', default='output.root', help='pamn output root file')
args = parser.parse_args()

pamn = './bin/pamn'
subprocess.call([pamn, '-i', args.input, '-o', args.utput])
