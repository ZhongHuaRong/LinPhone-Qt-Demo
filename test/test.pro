QT += testlib
QT -= gui
DESTDIR = ../Debug


CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_configtest.cpp

INCLUDEPATH +=  $$PWD/../sdk/linphone-sdk/desktop/include \
				$$PWD/../

LIBS +=  -L$$PWD/../sdk/linphone-sdk/desktop/lib/ -lz \
                                 -lxml2 \
                                 -llinphone++ \
                                 -llinphone \
                                 -lbctoolbox \
                                 -lortp \
                                 -lmediastreamer \
				 -lbelr \
                                 -lsqlite3 \
				 -lbellesip \
				 -lbv16 \
                                 -lbzrtp \
				 -lmbedcrypto \
				 -lmbedtls \
                                 -lmbedx509 \
                                 -lopenh264 \
				 -lopus \
                                 -lortp \
				 -lspeex \
				 -lspeexdsp \
                                 -lsrtp2 \
                                 -lturbojpeg\
                                 -lvpx
