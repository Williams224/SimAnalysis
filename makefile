Analyse.exe: IncidentEnergy.o Analyse.cpp
	g++ -c -std=c++11 `root-config --cflags` Analyse.cpp
	g++ -o Analyse.exe `root-config --glibs` Analyse.o IncidentEnergy.o

IncidentEnergy.o: IncidentEnergy.cpp
	g++ -c -std=c++11 `root-config --cflags` IncidentEnergy.cpp
