QT += testlib
QT -= gui
DESTDIR = ../Debug


CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_configtest.cpp

INCLUDEPATH +=  $$PWD/../sdk/linphone-sdk/desktop/include \
				$$PWD/../

LIBS +=  -L$$PWD/../sdk/linphone-sdk/desktop/lib/ -lzlib \
				 -lxml2 \
				 -lliblinphone++ \
				 -lliblinphone \
				 -lbctoolbox \
				 -lortp \
				 -lmediastreamer \
				 -lbelcard \
				 -lbelr \
				 -lsqlite3 \
				 -lavcodec \
				 -lavutil \
				 -lbctoolbox-tester \
				 -lbcunit \
				 -lbellesip \
				 -lbv16 \
				 -lbzrtp \
				 -ldecaf \
				 -lgcc \
				 -lgsm \
				 -llime \
				 -lmbedcrypto \
				 -lmbedtls \
				 -lmbedx509 \
				 -lmingwex \
				 -lopenh264_dll \
				 -lopus \
				 -lortp \
				 -lsoci_core_4_0 \
				 -lsoci_sqlite3_4_0 \
				 -lspeex \
				 -lspeexdsp \
				 -lsrtp2 \
				 -lswresample \
				 -lswscale \
				 -lturbojpeg-static \
				 -lxerces-c \
				 -ladvapi32 \
				 -lUser32
