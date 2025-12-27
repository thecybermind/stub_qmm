mkdir package
pushd package
del /q *
rem copy ..\README.md .\
rem copy ..\LICENSE .\

for /f %%x in (..\games.lst) do (
         copy ..\bin\Release-%%x\x86\stub_qmm_%%x.dll .\
         copy ..\bin\Release-%%x\x64\stub_qmm_x86_64_%%x.dll .\     
       )
copy ..\bin\Release-Q2R\x64\stub_qmm_x86_64_Q2R.dll .\
popd
