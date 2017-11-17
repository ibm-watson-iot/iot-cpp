#!/bin/bash
echo "Backing lib"
rm -rf lib_bk
rm -rf build
mkdir build
mv lib lib_bk
mkdir tmp
mkdir lib
cd tmp
echo "Downloading Paho mqtt cpp ...."
curl -LO https://github.com/eclipse/paho.mqtt.cpp/archive/v0.5-prerelease.zip
unzip v0.5-prerelease.zip
echo "Downloading Paho mqtt c..."
curl -LO https://github.com/eclipse/paho.mqtt.c/archive/v1.1.0.tar.gz
tar -xvf v1.1.0.tar.gz
echo "Downloading jsoncpp...."
curl -LO https://github.com/open-source-parsers/jsoncpp/archive/0.10.6.tar.gz
tar -xvf 0.10.6.tar.gz
echo "Downloading log4cpp...."
# git clone https://git.code.sf.net/p/log4cpp/codegit log4cpp-codegit
curl -Lo log4cpp-codegit.tar.gz https://sourceforge.net/projects/log4cpp/files/latest/download
tar -xvzf log4cpp-codegit.tar.gz
mv log4cpp log4cpp-codegit
echo "Copying the necessary files to lib"
cp paho.mqtt.cpp-0.5-prerelease/src/*.cpp ../lib/
cp -r paho.mqtt.cpp-0.5-prerelease/src/mqtt ../lib/
cp paho.mqtt.c-1.1.0/src/*.c ../lib/
cp paho.mqtt.c-1.1.0/src/*.h ../lib/
cd jsoncpp-0.10.6
python amalgamate.py
cp dist/jsoncpp.cpp ../../lib/
cp -R dist/json ../../lib/
cd ../log4cpp-codegit
cp -r include/log4cpp ../../lib/
cp include/config.h.in ../../lib/log4cpp/config.h
cp src/* ../../lib/
cd ../../
cp ./lib_bk/CMakeLists.txt ./lib/
cp ./lib_bk/VersionInfo.h ./lib/
echo "Removing the temporary files"
rm -rf ./tmp/
rm -rf ./lib_bk/
echo "Finished setup"
