@echo off
setlocal

REM batファイルのあるディレクトリを取得
for %%I in ("%~dp0") do set "bat_dir=%%~dpI"

REM コピー元のフォルダパスを指定
set "source_folder=%bat_dir%x64\Debug"

REM フォルダ内のすべての .exe ファイルをコピーしてペーストする
for /R "%source_folder%" %%F in (*.exe) do (
    copy /Y "%%F" "%bat_dir%" > nul
)

echo コピーが完了しました。
pause