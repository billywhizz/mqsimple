CPU_ARC=64
GCC_VER=3.4
MQ_LIB_PATH=/opt/mqm/lib64/
MQ_INCLUDE_PATH=/opt/mqm/inc/
LIB_PATH=/usr/lib64/
g++ -DLINUX -m$CPU_ARC -shared -fPIC -fsigned-char -o bin/libmqsimple.so ../src/cpp/mqthread.h ../src/cpp/mqsimple.h ../src/cpp/mqthread.cpp ../src/cpp/mqsimple.cpp -I$MQ_INCLUDE_PATH -L$MQ_LIB_PATH$GCC_VER -Wl,-rpath=$MQ_LIB_PATH$GCC_VER -L$MQ_LIB_PATH$GCC_VER -Wl,-rpath=$MQ_LIB_PATH$GCC_VER -Wl,-rpath=$LIB_PATH -limqs23gl -limqb23gl
g++ -DLINUX -m$CPU_ARC -fsigned-char -o bin/qtest ../src/cpp/qtest.cpp -I../src/cpp -L./bin -Wl,-rpath=$MQ_LIB_PATH$GCC_VER -lmqsimple
g++ -DLINUX -m$CPU_ARC -fsigned-char -o bin/qput ../src/cpp/qput.cpp -I../src/cpp -L./bin -Wl,-rpath=$MQ_LIB_PATH$GCC_VER -lmqsimple
g++ -DLINUX -m$CPU_ARC -fsigned-char -o bin/qget ../src/cpp/qget.cpp -I../src/cpp -L./bin -Wl,-rpath=$MQ_LIB_PATH$GCC_VER -lmqsimple
cp -f bin/* ../bin/
