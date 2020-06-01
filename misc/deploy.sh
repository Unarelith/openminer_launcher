#!/bin/bash

echo -n "Which version are you deploying? "
read version

rm -rf deploy/win32/bin/openminer_launcher deploy/win64/bin/openminer_launcher deploy/linux64/bin/openminer_launcher
mkdir -p deploy/win32/bin/openminer_launcher deploy/win64/bin/openminer_launcher deploy/linux64/bin/openminer_launcher

rm deploy/*.zip

# Win32
i686-w64-mingw32-cmake -B deploy/win32 . && \
cmake --build deploy/win32 -j8 && \
i686-w64-mingw32-strip deploy/win32/openminer_launcher.exe && \
cp deploy/win32/openminer_launcher.exe deploy/win32/bin/openminer_launcher/openminer_launcher.exe && \
# i686-w64-mingw32-windeployqt-qt5 deploy/win32/bin/openminer_launcher && \
cp /usr/i686-w64-mingw32/bin/libwinpthread-1.dll \
/usr/i686-w64-mingw32/bin/Qt5Core.dll \
/usr/i686-w64-mingw32/bin/Qt5Gui.dll \
/usr/i686-w64-mingw32/bin/Qt5Network.dll \
/usr/i686-w64-mingw32/bin/Qt5Sql.dll \
/usr/i686-w64-mingw32/bin/Qt5Widgets.dll \
/usr/i686-w64-mingw32/bin/libgcc_s_dw2-1.dll \
/usr/i686-w64-mingw32/bin/libstdc++-6.dll \
deploy/win32/external/quazip/quazip/libQuazip.dll \
    deploy/win32/bin/openminer_launcher && \
cd deploy/win32/bin && \
zip -T -r ../../OpenMiner_Launcher-$version-win32.zip openminer_launcher &&
cd ../../..

# Win64
x86_64-w64-mingw32-cmake -B deploy/win64 . && \
cmake --build deploy/win64 -j8 && \
x86_64-w64-mingw32-strip deploy/win64/openminer_launcher.exe && \
cp deploy/win64/openminer_launcher.exe deploy/win64/bin/openminer_launcher/openminer_launcher.exe && \
# x86_64-w64-mingw32-windeployqt-qt5 deploy/win64/bin/openminer_launcher && \
cp /usr/x86_64-w64-mingw64/bin/libwinpthread-1.dll \
/usr/x86_64-w64-mingw64/bin/Qt5Core.dll \
/usr/x86_64-w64-mingw64/bin/Qt5Gui.dll \
/usr/x86_64-w64-mingw64/bin/Qt5Network.dll \
/usr/x86_64-w64-mingw64/bin/Qt5Sql.dll \
/usr/x86_64-w64-mingw64/bin/Qt5Widgets.dll \
/usr/x86_64-w64-mingw64/bin/libgcc_s_dw2-1.dll \
/usr/x86_64-w64-mingw64/bin/libstdc++-6.dll \
deploy/win64/external/quazip/quazip/libQuazip.dll \
    deploy/win64/bin/openminer_launcher && \
cp /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll deploy/win64/bin/openminer_launcher && \
cd deploy/win64/bin && \
zip -T -r ../../OpenMiner_Launcher-$version-win64.zip openminer_launcher &&
cd ../../..

# Linux64
cmake -B deploy/linux64 . && \
cmake --build deploy/linux64 -j8 && \
strip deploy/linux64/openminer_launcher && \
mkdir -p deploy/linux64/bin/openminer_launcher/bin deploy/linux64/bin/openminer_launcher/lib && \
cp deploy/linux64/openminer_launcher deploy/linux64/bin/openminer_launcher/bin && \
cp /usr/lib/ld-linux-x86-64.so.2 \
/usr/lib/libc.so.6 \
/usr/lib/libdl.so.2 \
/usr/lib/libfreetype.so.6 \
/usr/lib/libgcc_s.so.1 \
/usr/lib/libGLdispatch.so.0 \
/usr/lib/libglib-2.0.so.0 \
/usr/lib/libGLU.so.1 \
/usr/lib/libGLX.so.0 \
/usr/lib/libjpeg.so.8 \
/usr/lib/libm.so.6 \
/usr/lib/libpcre.so.1 \
/usr/lib/libpthread.so.0 \
/usr/lib/librt.so.1 \
/usr/lib/libstdc++.so.6 \
/usr/lib/libX11.so.6 \
/usr/lib/libXau.so.6 \
/usr/lib/libxcb.so.1 \
/usr/lib/libXdmcp.so.6 \
/usr/lib/libzstd.so.1 \
/usr/lib/libQt5Core.so.5 \
/usr/lib/libQt5DBus.so.5 \
/usr/lib/libQt5Gui.so.5 \
/usr/lib/libQt5Network.so.5 \
/usr/lib/libQt5Sql.so.5 \
/usr/lib/libQt5Widgets.so.5 \
/usr/lib/libQt5XcbQpa.so.5 \
/usr/lib/libxcb-*.so \
    deploy/linux64/bin/openminer_launcher/lib && \
mkdir deploy/linux64/bin/openminer_launcher/platforms/ && \
cp /usr/lib/qt/plugins/platforms/libqxcb.so deploy/linux64/bin/openminer_launcher/platforms/ && \
echo -e "#!/bin/bash\n./lib/ld-linux-x86-64.so.2 --library-path lib ./bin/openminer_launcher \$*" > deploy/linux64/bin/openminer_launcher/openminer_launcher && \
chmod +x deploy/linux64/bin/openminer_launcher/openminer_launcher && \
cd deploy/linux64/bin && \
zip -T -r ../../OpenMiner_Launcher-$version-linux64.zip openminer_launcher && \
cd ../../..

