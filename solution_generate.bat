@echo off

set start_time=%DATE% %TIME%
set PRODUCT_VERSION=0.2.0

set BUILD_DIR=build
mkdir %~dp0%BUILD_DIR%
del %~dp0%BUILD_DIR%\*.sln

cmake ^
--no-warn-unused-cli ^
-DCMAKE_GENERATOR_PLATFORM=x64 ^
-DCMAKE_CONFIGURATION_TYPES=Debug;Release ^
-DPRODUCT_VERSION=%PRODUCT_VERSION% ^
-S %~dp0% ^
-B %~dp0%BUILD_DIR%\ ^
-G "Visual Studio 16 2019"

echo -- Start: %start_time%
echo -- Stop:  %DATE% %TIME%

:end
