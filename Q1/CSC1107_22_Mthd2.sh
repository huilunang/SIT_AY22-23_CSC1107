#!/bin/bash
dir_path=/home/pi/linux/CSC1107_assignment
env_file=$dir_path/CSC1107_22_env.txt
source $env_file

# Increment the counter
((COUNTER++))

# Save the updated counter value to the file
echo "COUNTER=$COUNTER" > "$env_file"
echo "DISABLED=FALSE" >> "$env_file"

# Check if the counter has reached the limit (3 times)
if [ "$COUNTER" -le 3 ]; then
    echo Number of times script has run: $COUNTER
    $dir_path/CSC1107_22.sh
else
    echo Script is disabled after executing 3 times
fi