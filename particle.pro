TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS += -lGL -lGLU -lglut


SOURCES += \
    ParticleSystem.cpp \
    Particles.cpp \
    Particle.cpp

HEADERS += \
    Vector.h \
    ParticleSystem.h \
    Particle.h

