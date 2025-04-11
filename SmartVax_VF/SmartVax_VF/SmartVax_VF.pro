QT       += sql
QT += printsupport
QT += charts
QT += multimedia multimediawidgets
CONFIG += c++17
LIBS += -lole32 -loleaut32 -luuid -lwinmm -lsapi
CONFIG += lrelease embed_translations
QT += core gui sql multimedia multimediawidgets printsupport charts widgets
win32 {
    # Existing Windows Kits config
    LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/um/64" -lsapi
    LIBS += -lole32 -loleaut32 -luuid -lwinmm
    LIBS += -lole32
    # OpenCV config
    INCLUDEPATH += "C:/opencv-4.11.0/build/install/include"
    LIBS += -L"C:/opencv-4.11.0/build/install/x64/mingw/lib" \
            -lopencv_core4110 \
            -lopencv_highgui4110 \
            -lopencv_imgproc4110 \
            -lopencv_video4110 \
            -lopencv_videoio4110 \
            -lopencv_objdetect4110
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += core gui widgets
QT += charts



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    equipements.cpp \
    evenement.cpp \
    main.cpp \
    mainwindow.cpp \
    vaccin.cpp

HEADERS += \
    connection.h \
    equipements.h \
    evenement.h \
    mainwindow.h \
    vaccin.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    projet_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

DISTFILES += \
    ../projet/save.png \
    216658.png \
    7398464.png \
    dataset/classes \
    dataset/images/WIN_20250326_13_54_39_Pro.jpg \
    dataset/images/WIN_20250326_13_54_40_Pro.jpg \
    dataset/images/WIN_20250326_13_54_45_Pro.jpg \
    dataset/images/syringe1.jpg \
    dataset/images/syringe11.xml \
    dataset/images/syringe2.jpg.jpg \
    dataset/images/syringe3.jpg.jpg \
    dataset/labels/syringe1.txt \
    down arrow (2).jpg \
    haarcascade_frontalface_default.xml \
    icon.png \
    icon2.png \
    icon4.png \
    icon5.png \
    icon6.png \
    icon7.png \
    icon8.png \
    logo.png \
    logo2.png.png \
    up arrow.jpg \
    voice.mp3 \
    voice.mp3


DISTFILES += \
    ../projet/save.png \
    216658.png \
    7398464.png \
    dataset/classes \
    dataset/images/WIN_20250326_13_54_39_Pro.jpg \
    dataset/images/WIN_20250326_13_54_40_Pro.jpg \
    dataset/images/WIN_20250326_13_54_45_Pro.jpg \
    dataset/images/syringe1.jpg \
    dataset/images/syringe11.xml \
    dataset/images/syringe2.jpg.jpg \
    dataset/images/syringe3.jpg.jpg \
    dataset/labels/syringe1.txt \
    down arrow (2).jpg \
    haarcascade_frontalface_default.xml \
    icon.png \
    icon2.png \
    icon4.png \
    icon5.png \
    icon6.png \
    icon7.png \
    icon8.png \
    logo.png \
    logo2.png.png \
    up arrow.jpg \
    voice.mp3


DISTFILES += \
    ../projet/save.png \
    216658.png \
    7398464.png \
    dataset/classes \
    dataset/images/WIN_20250326_13_54_39_Pro.jpg \
    dataset/images/WIN_20250326_13_54_40_Pro.jpg \
    dataset/images/WIN_20250326_13_54_45_Pro.jpg \
    dataset/images/syringe1.jpg \
    dataset/images/syringe11.xml \
    dataset/images/syringe2.jpg.jpg \
    dataset/images/syringe3.jpg.jpg \
    dataset/labels/syringe1.txt \
    down arrow (2).jpg \
    haarcascade_frontalface_default.xml \
    icon.png \
    icon2.png \
    icon4.png \
    icon5.png \
    icon6.png \
    icon7.png \
    icon8.png \
    logo.png \
    logo2.png.png \
    up arrow.jpg \
    voice.mp3


DISTFILES += \
    ../projet/save.png \
    216658.png \
    7398464.png \
    dataset/classes \
    dataset/images/WIN_20250326_13_54_39_Pro.jpg \
    dataset/images/WIN_20250326_13_54_40_Pro.jpg \
    dataset/images/WIN_20250326_13_54_45_Pro.jpg \
    dataset/images/syringe1.jpg \
    dataset/images/syringe11.xml \
    dataset/images/syringe2.jpg.jpg \
    dataset/images/syringe3.jpg.jpg \
    dataset/labels/syringe1.txt \
    down arrow (2).jpg \
    haarcascade_frontalface_default.xml \
    icon.png \
    icon2.png \
    icon4.png \
    icon5.png \
    icon6.png \
    icon7.png \
    icon8.png \
    logo.png \
    logo2.png.png \
    up arrow.jpg \
    voice.mp3


DISTFILES += \
    ../projet/save.png \
    216658.png \
    7398464.png \
    dataset/classes \
    dataset/images/WIN_20250326_13_54_39_Pro.jpg \
    dataset/images/WIN_20250326_13_54_40_Pro.jpg \
    dataset/images/WIN_20250326_13_54_45_Pro.jpg \
    dataset/images/syringe1.jpg \
    dataset/images/syringe11.xml \
    dataset/images/syringe2.jpg.jpg \
    dataset/images/syringe3.jpg.jpg \
    dataset/labels/syringe1.txt \
    down arrow (2).jpg \
    haarcascade_frontalface_default.xml \
    icon.png \
    icon2.png \
    icon4.png \
    icon5.png \
    icon6.png \
    icon7.png \
    icon8.png \
    logo.png \
    logo2.png.png \
    up arrow.jpg \
    voice.mp3


DISTFILES += \
    ../projet/save.png \
    216658.png \
    7398464.png \
    dataset/classes \
    dataset/images/WIN_20250326_13_54_39_Pro.jpg \
    dataset/images/WIN_20250326_13_54_40_Pro.jpg \
    dataset/images/WIN_20250326_13_54_45_Pro.jpg \
    dataset/images/syringe1.jpg \
    dataset/images/syringe11.xml \
    dataset/images/syringe2.jpg.jpg \
    dataset/images/syringe3.jpg.jpg \
    dataset/labels/syringe1.txt \
    down arrow (2).jpg \
    haarcascade_frontalface_default.xml \
    icon.png \
    icon2.png \
    icon4.png \
    icon5.png \
    icon6.png \
    icon7.png \
    icon8.png \
    logo.png \
    logo2.png.png \
    up arrow.jpg \
    voice.mp3


DISTFILES += \
    ../projet/save.png \
    216658.png \
    7398464.png \
    dataset/classes \
    dataset/images/WIN_20250326_13_54_39_Pro.jpg \
    dataset/images/WIN_20250326_13_54_40_Pro.jpg \
    dataset/images/WIN_20250326_13_54_45_Pro.jpg \
    dataset/images/syringe1.jpg \
    dataset/images/syringe11.xml \
    dataset/images/syringe2.jpg.jpg \
    dataset/images/syringe3.jpg.jpg \
    dataset/labels/syringe1.txt \
    down arrow (2).jpg \
    haarcascade_frontalface_default.xml \
    icon.png \
    icon2.png \
    icon4.png \
    icon5.png \
    icon6.png \
    icon7.png \
    icon8.png \
    logo.png \
    logo2.png.png \
    up arrow.jpg \
    voice.mp3


DISTFILES += \
    ../projet/save.png \
    216658.png \
    7398464.png \
    dataset/classes \
    dataset/images/WIN_20250326_13_54_39_Pro.jpg \
    dataset/images/WIN_20250326_13_54_40_Pro.jpg \
    dataset/images/WIN_20250326_13_54_45_Pro.jpg \
    dataset/images/syringe1.jpg \
    dataset/images/syringe11.xml \
    dataset/images/syringe2.jpg.jpg \
    dataset/images/syringe3.jpg.jpg \
    dataset/labels/syringe1.txt \
    down arrow (2).jpg \
    haarcascade_frontalface_default.xml \
    icon.png \
    icon2.png \
    icon4.png \
    icon5.png \
    icon6.png \
    icon7.png \
    icon8.png \
    logo.png \
    logo2.png.png \
    up arrow.jpg \
    voice.mp3
