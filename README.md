Adding and Testing Custom Code
====================================



## Purpose
You can quickly add your custom code into this folder so that quick prototypes can be easily tested without having to create a whoel new project just for it. This repo is inteded to be cloned into `[openpose root directory]/examples/user_code`



## How-to
1. Install/compile OpenPose as usual.
2. Add your custom *.cpp / *.hpp files here.
3. Add the name of your custom *.cpp / *.hpp files at the top of the [CMakeLists.txt](./CMakeLists.txt) file. Add any external libraries that are needed here as well.
4. Commit and push new files
5. Clone this repo into `examples/user_code`
6. Re-compile OpenPose.
```
#from openpose root directory
cd build/
make -j`nproc`
```


## Running your Custom Code
Run:
```
#from openpose root directory
./build/examples/user_code/{your_custom_file_name}
```
