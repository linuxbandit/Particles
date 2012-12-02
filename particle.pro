TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS += -lGL -lGLU -lglut


SOURCES += \
    ParticleSystem.cpp \
    Particles.cpp \
    Particle.cpp \
    Plane.cpp

HEADERS += \
    ParticleSystem.h \
    Particle.h \
    Vector3f.h \
    Plane.h

