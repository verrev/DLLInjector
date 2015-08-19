# DLLInjector
DLL injector for Microsoft Windows. 

Usage: 
1) run the injector
2) DLLInjector.txt is created
3) edit the file as needed 
   [LINE 1] example.exe - your executables name, can be get from task manager.
   [LINE 2] yourDLL.dll - path to dll, if it is in the same folder as the injector, just the name of it will suffice.
   [LINE 3] 1 - if this line is set to 1, the process will be created automatically when the injector is run.
   [LINE 4] c:/users/username/program files(x86)/game/game.exe - the direct path to the executable which will be
            run when line 3 is set to 1.
   [LINE 5] 0 - the delay between running the injector and the injection happening (in milliseconds, 1000 ms is 1 sec).