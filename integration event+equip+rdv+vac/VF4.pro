QT       += sql
QT += printsupport
QT += charts
LIBS += -lole32 -loleaut32 -luuid -lwinmm -lsapi
QT += core gui location qml quickwidgets network
QT += serialport
QT += multimedia multimediawidgets       # Multimedia (for sound alerts)
LIBS += -lwinmm -lsapi
  LIBS += -luser32 -lgdi32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#elloumi_Cam
#win32 {
 #   LIBS += -luser32 -lgdi32
  #  QMAKE_CXXFLAGS += -DUNICODE -D_UNICODE
   # LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x64" -lsapi
    #LIBS += -lole32 -loleaut32 -luuid -lwinmm

    #INCLUDEPATH += "C:/opencv-4.11.0/build/install/include"
    #LIBS += -L"C:/opencv-4.11.0/build/install/x64/mingw/lib" \
     #       -lopencv_core4110 \
      #      -lopencv_highgui4110 \
       #     -lopencv_imgproc4110 \
        #   -lopencv_videoio4110 \
         #   -lopencv_objdetect4110 \
          #  -lopencv_imgcodecs4110
#}
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    brasrobotique.cpp \
    chatbot.cpp \
    equipements.cpp \
    evenement.cpp \
    main.cpp \
    mainwindow.cpp \
    newsfetcher.cpp \
    rendez_vous.cpp \
    vaccin.cpp

HEADERS += \
    arduino.h \
    brasrobotique.h \
    chatbot.h \
    connection.h \
    equipements.h \
    evenement.h \
    mainwindow.h \
    newsfetcher.h \
    rendez_vous.h \
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
    image.qrc \
    resources.qrc

DISTFILES += \
    ../projet/save.png \
    216658.png \
    7398464.png \
    build/Desktop_Qt_6_7_3_MinGW_64_bit-Debug/debug/voice.mp3 \
    icon.png \
    icon2.png \
    icon4.png \
    icon5.png \
    icon6.png \
    icon7.png \
    icon8.png \
    logo.png \
    mail.png \
    notification.wav \
    pin.png \
    pin2.png \
    rate.png \
    signature.png \
    stat.png \
    voice.mp3 \
    voice.mp3
