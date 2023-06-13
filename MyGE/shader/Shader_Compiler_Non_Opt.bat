@echo off
set VULKAN_SDK=C:\VulkanSDK\1.3.250.0
set GLSLC="%VULKAN_SDK%\Bin\glslc.exe"

for %%f in (*.vert) do (
    %GLSLC% -o "%%~nf.vert.spv" "%%f"
)

for %%f in (*.frag) do (
    %GLSLC% -o "%%~nf.frag.spv" "%%f"
)

echo Compilation completed.
pause