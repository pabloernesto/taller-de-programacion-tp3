# Compilation flags.
#CPPFLAGS = -std=c++11 -O0 -ggdb -DDEBUG -fno-inline
CPPFLAGS = -std=c++11 -O3
LDFLAGS = -pthread

# Setting path.
VPATH = ../src
CPPFLAGS += -I../src

all : db_test

db_test : db_test.cpp server_database.cpp
