fixedTopology:
	g++ -std=c++17 -O3 fixedTopology.cpp expressionses.cpp -o ./bin/fixedTopology
fixedTopology2:
	g++ -std=c++17 -O3 fixedTopology2.cpp expressionses.cpp -o ./bin/fixedTopology2
bar:
	g++ -std=c++17 -O3 fixedTopology2.cpp expressionses.cpp -o ./bin/fixedTopology2 && ./bin/fixedTopology2