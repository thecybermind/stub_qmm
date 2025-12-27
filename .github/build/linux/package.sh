#!/bin/sh
mkdir -p package
cd package
rm -f *
cp ../README.md ./
cp ../LICENSE ./

while read f; do
  cp ../bin/release-$f/x86/stub_qmm_$f.so ./
  cp ../bin/release-$f/x86_64/stub_qmm_x86_64_$f.so ./
done < ../games.lst

cd ..
