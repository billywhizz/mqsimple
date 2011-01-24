rem build library
cl.exe /O1 /Ob1 /I "D:\Program Files\IBM\WebSphere MQ\Tools\c\include" /I "D:\Program Files\IBM\WebSphere MQ\Tools\cplus\include" /D "WINDOWS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_VC80_UPGRADE=0x0710" /D "_WINDLL" /D "_MBCS" /GF /FD /EHsc /MT /Gy /Fp".\Release/mqsimple.pch" /Fo".\Release/" /Fd".\Release/" /FR".\Release\\" /W3 /c ..\src\cpp\mqthread.cpp ..\src\cpp\mqsimple.cpp
link.exe /OUT:".\Release/mqsimple.dll" /INCREMENTAL:NO /LIBPATH:"D:\Program Files\IBM\WebSphere MQ\Tools\Lib" /DLL /MANIFEST /MANIFESTFILE:".\Release\mqsimple.dll.intermediate.manifest" /DEF:"..\src\def\mqsimple.def" /PDB:".\Release/mqsimple.pdb" /SUBSYSTEM:CONSOLE /MACHINE:X86 mqic32.lib mqm.lib imqc23vn.Lib imqs23vn.Lib imqb23vn.Lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ".\Release\mqsimple.obj" ".\Release\mqthread.obj"
copy /Y Release\mqsimple.dll ..\bin\

rem build test app
cl.exe /O2 /I "D:\Program Files\IBM\WebSphere MQ\Tools\c\include" /I "D:\Program Files\IBM\WebSphere MQ\Tools\cplus\include" /D "WINDOWS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_VC80_UPGRADE=0x0710" /D "_MBCS" /FD /EHsc /MT /Fo"Release\\" /Fd"Release\vc80.pdb" /W3 /c /Wp64 /Zi /TP ..\src\cpp\qtest.cpp
link.exe /OUT:".\Release/qtest.exe" /INCREMENTAL:NO /LIBPATH:"D:\Program Files\IBM\WebSphere MQ\Tools\Lib" /LIBPATH:".\Release" /MANIFEST /MANIFESTFILE:"Release\qtest.exe.intermediate.manifest" /DEBUG /PDB:"D:\SVN\dev\centos\mqsimple\VStudio2005\Release\qtest.pdb" /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /MACHINE:X86 mqic32.lib mqm.lib imqc23vn.Lib imqs23vn.Lib imqb23vn.Lib mqsimple.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ".\Release\qtest.obj"
copy /Y Release\qtest.exe ..\bin\

rem build qget
cl.exe /O2 /I "D:\Program Files\IBM\WebSphere MQ\Tools\c\include" /I "D:\Program Files\IBM\WebSphere MQ\Tools\cplus\include" /D "WINDOWS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_VC80_UPGRADE=0x0710" /D "_MBCS" /FD /EHsc /MT /Fo"Release\\" /Fd"Release\vc80.pdb" /W3 /c /Wp64 /Zi /TP ..\src\cpp\qget.cpp
link.exe /OUT:".\Release/qget.exe" /INCREMENTAL:NO /LIBPATH:"D:\Program Files\IBM\WebSphere MQ\Tools\Lib" /LIBPATH:".\Release" /MANIFEST /MANIFESTFILE:"Release\qtest.exe.intermediate.manifest" /DEBUG /PDB:"D:\SVN\dev\centos\mqsimple\VStudio2005\Release\qget.pdb" /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /MACHINE:X86 mqic32.lib mqm.lib imqc23vn.Lib imqs23vn.Lib imqb23vn.Lib mqsimple.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ".\Release\qget.obj"
copy /Y Release\qget.exe ..\bin\

rem build qput
cl.exe /O2 /I "D:\Program Files\IBM\WebSphere MQ\Tools\c\include" /I "D:\Program Files\IBM\WebSphere MQ\Tools\cplus\include" /D "WINDOWS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_VC80_UPGRADE=0x0710" /D "_MBCS" /FD /EHsc /MT /Fo"Release\\" /Fd"Release\vc80.pdb" /W3 /c /Wp64 /Zi /TP ..\src\cpp\qput.cpp
link.exe /OUT:".\Release/qput.exe" /INCREMENTAL:NO /LIBPATH:"D:\Program Files\IBM\WebSphere MQ\Tools\Lib" /LIBPATH:".\Release" /MANIFEST /MANIFESTFILE:"Release\qtest.exe.intermediate.manifest" /DEBUG /PDB:"D:\SVN\dev\centos\mqsimple\VStudio2005\Release\qput.pdb" /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /MACHINE:X86 mqic32.lib mqm.lib imqc23vn.Lib imqs23vn.Lib imqb23vn.Lib mqsimple.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ".\Release\qput.obj"
copy /Y Release\qput.exe ..\bin\
