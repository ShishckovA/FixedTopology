for ((i=1; i <= 9; i++))
do
gnome-terminal -e "bash -c \"./bin/fixedTopology\"" > /dev/null
sleep 2
done