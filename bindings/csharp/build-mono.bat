call mcs /target:library /debug%1 /out:bin\mqsimplen.dll src\csharp\mqsimple.cs
call mcs /debug%1 /r:bin\mqsimplen.dll /out:bin\mono-test.exe src\csharp\Test.cs
copy /Y bin\*.* ..\..\bin\