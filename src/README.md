Use 
```bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```
To produce a `compile_commands.json` that helps the LSP. 

Do Memory analysis can be done on M1 MacOS with leaks
```bash
leaks --atExit -- ./OpenGL_0.app/Contents/MacOS/OpenGL_0
```
