# -------------------------------------------------
# Project created by QtCreator
# -------------------------------------------------
QT += openglwidgets
TARGET = VizMol
TEMPLATE = app
SOURCES += atom.cpp \
    glmolecule.cpp \
    main.cpp \
    mainwindow.cpp \
    molecule.cpp \
    util.cpp
HEADERS += atom.h \
    point3d.h \
    glmolecule.h \
    mainwindow.h \
    molecule.h \
    util.h
FORMS += mainwindow.ui
LIBS += -lopengl32 -lglu32
