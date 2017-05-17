#!/bin/bash
# $1 Qt lib dir
# $2 Qt plugin dir
# $3 Qt translations dir

lib=$1
plugin=$2
translation=$3

rm -rf deployment
set -e

mkdir deployment
cd deployment
cp ../seasonproxer ./

mkdir lib
cd lib
cp $lib/libicudata.so* ./
cp $lib/libicui18n.so* ./
cp $lib/libicuuc.so* ./
cp $lib/libQt5Core.so* ./
cp $lib/libQt5DBus.so* ./
cp $lib/libQt5XcbQpa.so* ./
cp $lib/libQt5Gui.so* ./
cp $lib/libQt5Widgets.so* ./
cp $lib/libQt5Network.so* ./
cp $lib/libQt5WebSockets.so* ./
cp $lib/libQt5Sql.so* ./
cp $lib/libQt5RestClient.so* ./
cp $lib/libQt5DataSync.so* ./
cp $lib/libQt5AutoUpdaterCore.so* ./
cp $lib/libQt5AutoUpdaterGui.so* ./
cd ..

mkdir plugins
cd plugins

mkdir platforms
cp $plugin/platforms/libqxcb.so ./platforms/

mkdir platforminputcontexts
cp $plugin/platforminputcontexts/libcomposeplatforminputcontextplugin.so ./platforminputcontexts/

mkdir xcbglintegrations
cp $plugin/xcbglintegrations/libqxcb-glx-integration.so ./xcbglintegrations/

cp -r $plugin/bearer ./

mkdir imageformats
cp $plugin/imageformats/libqgif.so ./imageformats/
cp $plugin/imageformats/libqico.so ./imageformats/

mkdir sqldrivers
cp $plugin/sqldrivers/libqsqlite.so ./sqldrivers/

cd ..

mkdir translations
cd translations
cp $translation/qt_*.qm ./
cp $translation/qtbase_*.qm ./
cp $translation/qtwebsockets_*.qm ./
cd ..

echo "[Paths]" > qt.conf
echo "Prefix=." >> qt.conf

chrpath -r "./lib" seasonproxer

exit 0



