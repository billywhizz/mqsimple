mcs -target:library -debug$1 -out:bin/mqsimplen.dll src/csharp/mqsimple.cs
mcs -debug$1 -r:bin/mqsimplen.dll -out:bin/mono-test.exe src/csharp/test.cs
cp -f bin/* ../../bin/
