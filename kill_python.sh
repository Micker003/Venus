PID=$(ps -ef | grep CV.py | grep -v grep | awk '{print $2}')
sudo kill $PID