#!/usr/bin/env python3

import os
import re
import glob
import sys
import subprocess

def is_executable(filepath):
    """Check if the file is executable."""
    return os.path.isfile(filepath) and os.access(filepath, os.X_OK)

# Directories
input_dir = 'benchmarks/doitgen'      # Path to input directory

# Desired output file extension (e.g., .din for DineroIV)
output_extension = '.txt'

# Fetch all subdirectories from input_dir
subdirs = [f.path for f in os.scandir(input_dir) if f.is_dir()]

for root, dirs, files in os.walk(input_dir):
    for file in files:
        filepath = os.path.join(root, file)
        
        if is_executable(filepath):
            print(f"Getting trace from : {filepath}...")

            input_filename = os.path.basename(filepath) # Get the filename without directory
            output_filepath = os.path.join(root, input_filename.split('.')[1]+output_extension)  # Output file path

            with open(output_filepath, 'w') as output_file:

                process = subprocess.Popen(
                    ['../spike', '-l', '../pk', filepath],  # Command split into a list
                    stdout=subprocess.DEVNULL,  # Capture the output
                    stderr=subprocess.PIPE,  # Ignore errors if needed, or log separately
                    bufsize=1,  # Line buffering
                    universal_newlines=True  # Ensure text output, not bytes
                )

                # Read input directly form process stderr
                trace_file = process.stderr

                # Open the Spike log file for reading and the DineroIV trace file for writing
                collecting = False

                with open(output_filepath, 'w') as output_file:

                    for line in trace_file:
                        if 'instret' in line:
                            if not collecting:
                                # First instret line, start collecting
                                collecting = True
                            else:
                                # Second instret line, stop collecting
                                break
                        if collecting:
                            # Write the line to the output file
                            output_file.write(line)