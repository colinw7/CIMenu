CC = g++
RM = rm

CDEBUG = -g
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = .
LIB_DIR = ../lib
BIN_DIR = ../bin

all: $(BIN_DIR)/CIMenuTest

SRC = \
CIMenuTest.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

LIBS = \
-lCIMenu -lCFile -lCStrUtil -lCOS \
-lcurses

CPPFLAGS = \
-std=c++14 \
-I$(INC_DIR) \
-I../../CIMenu/include \
-I../../CMath/include \
-I../../CUtil/include \
-I../../COS/include \
-I.

LFLAGS = \
-L$(LIB_DIR) \
-L../../CIMenu/lib \
-L../../CFile/lib \
-L../../CStrUtil/lib \
-L../../COS/lib \

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(BIN_DIR)/CIMenuTest

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(BIN_DIR)/CIMenuTest: $(OBJS) $(LIB_DIR)/libCIMenu.a
	$(CC) $(LDEBUG) -o $(BIN_DIR)/CIMenuTest $(OBJS) $(LFLAGS) $(LIBS)
