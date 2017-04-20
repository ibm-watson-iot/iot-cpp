#!/bin/bash
echo "Backing lib"
rm -rf lib_bk
rm -rf build
mkdir build
mv lib lib_bk
mkdir tmp
mkdir lib
cd tmp
echo "downloading paho mqtt cpp ...."
curl -LO https://github.com/eclipse/paho.mqtt.cpp/archive/master.zip
unzip master.zip
echo "downloading paho mqtt c..."
curl -LO https://github.com/eclipse/paho.mqtt.c/archive/v1.1.0.tar.gz
tar -xvf v1.1.0.tar.gz
echo "downloading jsoncpp...."
curl -LO https://github.com/open-source-parsers/jsoncpp/archive/0.10.6.tar.gz
tar -xvf 0.10.6.tar.gz
echo "copying the necessary files to lib"
cp paho.mqtt.cpp-master/src/*.cpp ../lib/
cp -r paho.mqtt.cpp-master/src/mqtt ../lib/
cp paho.mqtt.c-1.1.0/src/*.c ../lib/
cp paho.mqtt.c-1.1.0/src/*.h ../lib/
cd jsoncpp-0.10.6
python amalgamate.py
cp dist/jsoncpp.cpp ../../lib/
cp -R dist/json ../../lib/
cd ../../
cp ./lib_bk/CMakeLists.txt ./lib/
cp ./lib_bk/VersionInfo.h ./lib/
echo "removing the temporary files"
rm -rf ./tmp/
rm -rf ./lib_bk/
echo "finished setup"
