#! /bin/bash

PKG_DIR=$(pwd)
apt-get download $1
find . -name "*.deb" -exec dpkg-deb --extract '{}' . \;
ALLLINKS=$(find . -xtype l)
for LINK in ${ALLLINKS}
do
	TARGET=$(readlink ${LINK})
	if [ ! -f ${TARGET} ]; then
		echo "Updating symlink ${LINK} to ${PKG_DIR}${TARGET}"
		ln -fs ${PKG_DIR}${TARGET} ${LINK}
	fi
done
