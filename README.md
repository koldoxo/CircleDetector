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
1. input_file: File name
2. output_path: Output directory
3. threshold: Threshold for binarisation [PREPROCESSING]
4. sigma: Sigma for blurring [PREPROCESSING]
5. min_length: Minimum length of a contour 
6. window_size: Observation window wize (if -1 it takes the window ratio of the contour for further processing) 
7. window_ratio: Window ratio chooses a proper window size for local curvature computation
8. min_radius: minimum radius of found circles for detection 
9. max_radius: maximum radius of found circles for detection
10. debug_mode: whether we print intermediate results (0 mean deactivated, otherwise is activated)