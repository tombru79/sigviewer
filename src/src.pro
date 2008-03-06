TEMPLATE = app
QT += qt3support
TARGET = sigviewer
DESTDIR = ..\bin
MOC_DIR = ..\tmp
OBJECTS_DIR = ..\tmp
RCC_DIR = ..\tmp
INCLUDEPATH += ../extern
LIBS += -L../extern -lbiosig
CONFIG +=     warn_on \
    qt \
    static \
    thread \
    debug

RESOURCES = src.qrc
win32:RC_FILE = src.rc
TRANSLATIONS += translations\sigviewer_de.ts \
    translations\sigviewer_en.ts \
    translations\sigviewer_es.ts \
    translations\sigviewer_fr.ts
HEADERS += *h \
	base\*h \
	base\gdf\*h \
	base\bkr\*h \
  base\cnt\*h \ 
	smart_canvas\*h 
SOURCES += *.cpp \
    base\*.cpp \
    base\gdf\*.cpp \
    base\bkr\*.cpp  \
    base\cnt\*.cpp \
    smart_canvas\*.cpp