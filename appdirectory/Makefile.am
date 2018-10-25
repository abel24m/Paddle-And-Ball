noinst_HEADERS = Assignment2.h BaseApplication.h ScoreManager.h Simulator.h Ball.h Wall.h Paddle.h GuiManager.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir) -std=c++11 -I/usr/include/SDL
assignment2_SOURCES = Assignment2.cpp BaseApplication.cpp ScoreManager.cpp Simulator.cpp Ball.cpp Wall.cpp Paddle.cpp GuiManager.cpp
assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) -I/lusr/opt/cegui-0.8.7/include/cegui-0
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) -L/lusr/opt/cegui-0.8.7/lib -lCEGUIBase-0 -lCEGUIOgreRenderer-0
assignment2_LDFLAGS = -lOgreOverlay -lboost_system -R/lusr/lib/cegui-0.8
assignment2_LDFLAGS += -lSDL -lSDL_mixer

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
