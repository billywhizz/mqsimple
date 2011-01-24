Linux Build

cd gcc-linux/
chmod +x build.sh

# configure build script

./build32.sh
bin/qtest "QMBETFAIRCHANNEL/TCP/centos(1416)/qmbetfair/QMARKET" 10 0 0 1

yum install mono-devel
cd ..
cd bindings/
cd csharp/
chmod +x build-mono.sh
./build-mono.sh -
cd ../../bin/
mono mono-test.exe "QMBETFAIRCHANNEL/TCP/centos(1416)/qmbetfair/QMARKET" 10 0 0 1
