TARGET = guildWars
OBJECTS = main.o Point.o BezierSurface.o Truck.o Light.o Color.o notsure.o Hero.o Material.o SirVive.o Camera.o CameraController.o ArcballController.o FreeCamController.o
OBJECTS += FirstPersonController.o MogdinBaramud.o Viewport.o BezierCurve.o


LOCAL_INC_PATH = /Users/jpaone/Desktop/include
LOCAL_LIB_PATH = /Users/jpaone/Desktop/lib
LOCAL_BIN_PATH = /Users/jpaone/Desktop/bin

BUILDING_IN_LAB = 1

USING_GLUI = 0
USING_OPENAL = 1
USING_OPENGL = 1


CXX    = g++
CFLAGS = -Wall -Wno-deprecated-declarations -g -std=c++11

#############################
## SETUP GLUI
##      We must link against
##  GLUI before we link against
##  GLUT.
##
##     If you want to build
##  on your own machine, you
##  need to change the
##  appropriate paths.
#############################

LAB_INC_PATH = Z:/CSCI441GFx/include
LAB_LIB_PATH = Z:/CSCI441GFx/lib
LAB_BIN_PATH = Z:/CSCI441GFx/bin

# if we are not building in the Lab
ifeq ($(BUILDING_IN_LAB), 0)
    # and we are running Windows
    ifeq ($(OS), Windows_NT)
        # then set our lab paths to our local paths
        # so the Makefile will still work seamlessly
        LAB_INC_PATH = $(LOCAL_INC_PATH)
        LAB_LIB_PATH = $(LOCAL_LIB_PATH)
        LAB_BIN_PATH = $(LOCAL_BIN_PATH)
    endif
endif



# Windows Lab builds 
ifeq ($(OS), Windows_NT)
	INCPATH += -ID:/CSCI441GFx/include/
	LIBPATH += -LD:/CSCI441GFx/lib/

# Non-Windows build
else
	INCPATH += -I/Users/jpaone/Desktop/include/
	LIBPATH += -L/Users/jpaone/Desktop/lib

	INCPATH += -I/home/martin/Documents/csci441/include
	LIBPATH += -L/home/martin/Documents/csci441/lib
endif



#############################
## SETUP OpenGL & GLUT 
#############################

# Windows builds 
ifeq ($(OS), Windows_NT)
	INCPATH += -IC:/Strawberry/c/include/
	LIBPATH += -LC:/Strawberry/c/lib/
      LIBS += -lglut -lopengl32 -lglu32

# Mac builds
else ifeq ($(shell uname), Darwin)
	LIBS += -framework GLUT -framework OpenGL

# Linux and all other builds
else
	LIBS += -lglut -lGL -lGLU
endif




#############################
## SETUP OpenAL & ALUT
#############################

# if we are using OpenAL & GLUT in this program
ifeq ($(USING_OPENAL), 1)
    # Windows builds
    ifeq ($(OS), Windows_NT)
        INCPATH += -I$(LAB_INC_PATH)
        LIBPATH += -L$(LAB_LIB_PATH)
        LIBS += -lalut.dll -lOpenAL32.dll
	WINDOWS_AL = 1

    # Mac builds
    else ifeq ($(shell uname), Darwin)
        INCPATH += -I$(LOCAL_INC_PATH)
        LIBPATH += -L$(LOCAL_LIB_PATH)
        LIBS += -framework OpenAL
	WINDOWS_AL = 0

    # Linux and all other builds
    else
        INCPATH += -I$(LOCAL_INC_PATH)
        LIBPATH += -L$(LOCAL_LIB_PATH)
        LIBS += -lalut -lopenal
	WINDOWS_AL = 0
    endif
endif






#############################
## COMPILATION INSTRUCTIONS 
#############################

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
	if [ $(USING_OPENAL) -eq 1 ]; \
	then \
		if [ $(WINDOWS_AL) -eq 1 ]; \
		then \
			rm OpenAL32.dll; \
			rm libalut.dll; \
		fi \
	fi 

depend:
	rm -f Makefile.bak
	mv Makefile Makefile.bak
	sed '/^# DEPENDENCIES/,$$d' Makefile.bak > Makefile
	echo '# DEPENDENCIES' >> Makefile
	$(CXX) $(INCPATH) -MM *.cpp >> Makefile

.c.o: 	
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

.cc.o: 	
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

.cpp.o: 	
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

$(TARGET): $(OBJECTS) 
	$(CXX) $(CFLAGS) $(INCPATH) -o $@ $^ $(LIBPATH) $(LIBS)
	if [ $(USING_OPENAL) -eq 1 ]; \
	then \
		if [ $(WINDOWS_AL) -eq 1 ]; \
		then \
			cp $(LAB_BIN_PATH)/OpenAL32.dll .; \
			cp $(LAB_BIN_PATH)/libalut.dll .; \
		fi \
	fi 

# DEPENDENCIES
main.o: main.cpp Point.h
Point.o: Point.cpp Point.h
Light.o: Light.cpp Light.h
BezierSurface.o: BezierSurface.cpp BezierSurface.h
Hero.o: Hero.cpp Hero.h
notsure.o: notsure.cpp notsure.h
Truck.o: Truck.cpp Truck.h
Material.o: Material.cpp Material.h
SirVive.o: SirVive.cpp SirVive.h
MogdinBaramud.o: MogdinBaramud.cpp
CameraController.o: CameraController.cpp
ArcballController.o: ArcballController.cpp
FreeCamController.o: FreeCamController.cpp
FirstPersonController.o: FirstPersonController.cpp
Viewport.o: Viewport.cpp
BezierCurve.o: BezierCurve.cpp
