Adding and Testing Custom Code
====================================



## Purpose
You can quickly add your custom code into this folder so that quick prototypes can be easily tested without having to create a whoel new project just for it. This repo is inteded to be cloned into `[openpose root directory]/examples/user_code`



## Building Custom Code
1. [Install/compile OpenPose as usual](https://github.com/CMU-Perceptual-Computing-Lab/openpose/blob/master/doc/installation.md#installation)
2. Clone this repo into `examples/user_code`
```
#from open pose root directory
cd examples
#remove the user_code folder if it already exists
rm -rf user_code

git clone https://github.com/Max-Mobility/OpenPoseUserCode.git user_code
```

3. Re-compile OpenPose.
```
#from openpose root directory
cd build/
make -j`nproc`
```

## Adding New Code

1. Add all `*.cpp/*.hpp` files into the user_code directory
2. Modify the local CMakelists.txt to include the custom code. Be sure to add any necessary link libraries

## Running Your Custom Code
Run:
```
#from openpose root directory
./build/examples/user_code/{your_custom_file_name}
```
