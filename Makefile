default:
	g++ ${wildcard .cpp} -o game include/*.cpp icon.res -lncursesw -lsfml-audio -lsfml-system -pthread
