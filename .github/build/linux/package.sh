#!/bin/sh
mkdir -p package
cd package
rm -f *
cp ../README.md ./
cp ../LICENSE ./

for x in CODMP CODUOMP COD11MP JAMP JASP JK2MP JK2SP MOHAA MOHBT MOHSH QUAKE2 Q3A RTCWMP RTCWSP SIN SOF2MP STEF2 STVOYHM STVOYSP WET; do
  cp ../bin/release-$x/x86/stub_qmm_$x.so ./
  cp ../bin/release-$x/x86_64/stub_qmm_x86_64_$x.so ./
done 

cd ..
