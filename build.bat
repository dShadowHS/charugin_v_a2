set PATH="C:\CMake\bin\";%PATH%
call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" x64

pushd build.win64.vc2012
call cmake.exe -G"Visual Studio 14 Win64" -DOpenCV_DIR="C:/opencv/build" ..
msbuild charugin_v_a.sln /property:Configuration=Debug /m
@rem msbuild polevoy_d_v.sln /property:Configuration=Release /m
popd