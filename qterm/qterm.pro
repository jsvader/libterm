QMAKE_LFLAGS += '-Wl,-rpath,\'./app/native/lib\''
QMAKE_CXXFLAGS = -O0 -g3
SOURCES += qterm.cpp
HEADERS += qterm.h
LIBS += -L../lib/ -lterm
INCLUDEPATH=../include
#-devMode -debugToken debugToken.bar
package.target = $${TARGET}.bar
package.depends = $${TARGET}
package.commands = blackberry-nativepackager \
    -devMode \
    -package $${TARGET}.bar -arg -platform -arg blackberry \
    blackberry-tablet.xml $${TARGET} \
    -e icon.png res/icon.png \
    -e splashscreen.png res/splashscreen.png \
    -e ../lib/libterm.so lib/libterm.so \
    -e $$[QT_INSTALL_LIBS]/libQtCore.so.4 lib/libQtCore.so.4 \
    -e $$[QT_INSTALL_LIBS]/libQtGui.so.4 lib/libQtGui.so.4 \
    -e $$[QT_INSTALL_LIBS]/libQtOpenGL.so.4 lib/libQtOpenGL.so.4 \
    -e $$[QT_INSTALL_LIBS]/libQtNetwork.so.4 lib/libQtNetwork.so.4 \
    -e $$[QT_INSTALL_PLUGINS]/platforms/libblackberry.so lib/libblackberry.so

QMAKE_EXTRA_TARGETS += package
