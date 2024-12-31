export_all : code.cpp
	g++ code.cpp -o libcode.so -shared -fPIC 

hidden : code.cpp
	g++ code.cpp -o libcode.so -shared -fPIC -fvisibility=hidden

extern : code.cpp
	g++ code.cpp libutil.a -o libcode.so -shared -fPIC -fvisibility=hidden

codeabi : code.cpp
	gcc code.cpp libutil.a -o shared -fPIC -Wl, --version-script=libcode.version

util.o : util.cpp
	g++ util.cpp -o util.o -c -fPIC
