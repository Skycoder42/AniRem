#!/bin/sh
# run with git root as pwd
# $1 out-dir
# $2 version
set -e

currdir=$(pwd)
pkgname=anirem
pkgver=${1:?Invalid commands. Syntax: arch.sh <version> <outdir>}
outdir=$(readlink -f ${2:?Invalid commands. Syntax: arch.sh <version> <outdir>})
pkgdir=$(mktemp -d)

echo 'CONFIG += no_mobile no_updater' >> .qmake.conf
./qpminit.sh

pushd $(mktemp -d)

qmake -r "$currdir"
make
make lrelease
make INSTALL_ROOT="$pkgdir" install

popd

install -D -m 644 -p Desktop/${pkgname}.desktop $pkgdir/usr/share/applications/${pkgname}.desktop
for res in 16 22 24 32 48 64 72 96 192 256 512; do
	install -D -m 644 -p "icn/${pkgname}_${res}.png" "$pkgdir/usr/share/icons/hicolor/${res}x${res}/apps/${pkgname}.png"
done

mkdir -p "$outdir"
cd "$pkgdir"
export XZ_OPT=-9
tar cJf "$outdir/${pkgname}-${pkgver}.tar.xz" ./*
