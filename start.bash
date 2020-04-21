for ((i=1; i <= 9; i++))
do
gnome-terminal -e "bash -c \"cd ~/CLionProjects/FixedTopology && ./bin/fixedTopology\""
sleep 2
done