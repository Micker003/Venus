# Start the files
sudo python ./CV.py &
make run &

# # Wait until a key is pressed to stop execution
# echo "Press any key to stop execution"
# read -n 1 -s

# # Find the process id of the python script and ./main file
# PID=$(ps -ef | grep CV.py | grep -v grep | awk '{print $2}')
# PID2=$(ps -ef | grep main | grep -v grep | awk '{print $2}')
# # Kill the process
# sudo kill $PID
# sudo kill $PID2

