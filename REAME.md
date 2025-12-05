This is the CircleDetector project.

Setup (in powershell):
```
mkdir pkg; cd pkg; git clone https://github.com/Microsoft/vcpkg.git; cmd; .\vcpkg\bootstrap-vcpkg.bat
mkdir build
```

Configure and build
```
cmake -G "Visual Studio 17 2022" -B build/ -S . -DENABLE_TESTING=FALSE -DUSE_VCPKG=TRUE
cmake --build build/ --config Debug
cmake --build build/ --config Release
```

Execute:
```
cp \build\app\<Debug|Release>\CircleDetector.exe  <WORKDIR>
cd <WORKDIR> 
CircleDetector.exe config.json
```

Note: 
In the .json file please provile following entries
1. File name
2. Output directory
3. Number of columns
4. Number of rows
5. Number of threads <OPTIONAL>