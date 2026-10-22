mkdir package
pushd package
del /q *
rem copy ..\README.md .\
rem copy ..\LICENSE .\
copy ..\bin\Release-COD11MP\x86\stub_qmm_COD11MP.dll .\
copy ..\bin\Release-CODMP\x86\stub_qmm_CODMP.dll .\
copy ..\bin\Release-CODUOMP\x86\stub_qmm_CODUOMP.dll .\
copy ..\bin\Release-JAMP\x86\stub_qmm_JAMP.dll .\
copy ..\bin\Release-JAMP\x64\stub_qmm_x86_64_JAMP.dll .\
copy ..\bin\Release-JASP\x86\stub_qmm_JASP.dll .\
copy ..\bin\Release-JASP\x64\stub_qmm_x86_64_JASP.dll .\
copy ..\bin\Release-JK2MP\x86\stub_qmm_JK2MP.dll .\
copy ..\bin\Release-JK2MP\x64\stub_qmm_x86_64_JK2MP.dll .\
copy ..\bin\Release-JK2SP\x86\stub_qmm_JK2SP.dll .\
copy ..\bin\Release-JK2SP\x64\stub_qmm_x86_64_JK2SP.dll .\
copy ..\bin\Release-MOHAA\x86\stub_qmm_MOHAA.dll .\
copy ..\bin\Release-MOHBT\x86\stub_qmm_MOHBT.dll .\
copy ..\bin\Release-MOHSH\x86\stub_qmm_MOHSH.dll .\
copy ..\bin\Release-Q2R\x64\stub_qmm_x86_64_Q2R.dll .\
copy ..\bin\Release-Q3A\x86\stub_qmm_Q3A.dll .\
copy ..\bin\Release-Q3A\x64\stub_qmm_x86_64_Q3A.dll .\
copy ..\bin\Release-QUAKE2\x86\stub_qmm_QUAKE2.dll .\
copy ..\bin\Release-QUAKE2\x64\stub_qmm_x86_64_QUAKE2.dll .\
copy ..\bin\Release-RTCWMP\x86\stub_qmm_RTCWMP.dll .\
copy ..\bin\Release-RTCWMP\x64\stub_qmm_x86_64_RTCWMP.dll .\
copy ..\bin\Release-RTCWSP\x86\stub_qmm_RTCWSP.dll .\
copy ..\bin\Release-RTCWSP\x64\stub_qmm_x86_64_RTCWSP.dll .\
copy ..\bin\Release-SIN\x86\stub_qmm_SIN.dll .\
copy ..\bin\Release-SOF\x86\stub_qmm_SOF.dll .\
copy ..\bin\Release-SOF2MP\x86\stub_qmm_SOF2MP.dll .\
copy ..\bin\Release-SOF2SP\x86\stub_qmm_SOF2SP.dll .\
copy ..\bin\Release-STEF2\x86\stub_qmm_STEF2.dll .\
copy ..\bin\Release-STVOYHM\x86\stub_qmm_STVOYHM.dll .\
copy ..\bin\Release-STVOYSP\x86\stub_qmm_STVOYSP.dll .\
copy ..\bin\Release-WET\x86\stub_qmm_WET.dll .\
copy ..\bin\Release-WET\x64\stub_qmm_x86_64_WET.dll .\
popd
