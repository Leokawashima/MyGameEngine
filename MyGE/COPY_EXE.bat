@echo off
setlocal

REM bat�t�@�C���̂���f�B���N�g�����擾
for %%I in ("%~dp0") do set "bat_dir=%%~dpI"

REM �R�s�[���̃t�H���_�p�X���w��
set "source_folder=%bat_dir%x64\Debug"

REM �t�H���_���̂��ׂĂ� .exe �t�@�C�����R�s�[���ăy�[�X�g����
for /R "%source_folder%" %%F in (*.exe) do (
    copy /Y "%%F" "%bat_dir%" > nul
)

echo �R�s�[���������܂����B
pause