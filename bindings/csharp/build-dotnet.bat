csc /target:library /debug%1 /out:bin\mqsimplen.dll src\csharp\mqsimple.cs
csc /debug%1 /r:bin\mqsimplen.dll /out:bin\mono-test.exe src\csharp\Test.cs
copy /Y bin\*.* ..\..\bin\