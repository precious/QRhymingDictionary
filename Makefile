QT_INCLUDE_DIR=/usr/local/Trolltech/Qt-4.7.0/include
QT_LIB_DIR=/usr/local/Trolltech/Qt-4.7.0/lib
CXX=g++

all:
	moc mainwindow.h -o moc_mainwindow.cpp
	$(CXX) moc_mainwindow.cpp -I$(QT_INCLUDE_DIR) -I$(QT_INCLUDE_DIR)/QtGui -I$(QT_INCLUDE_DIR)/QtCore -I$(QT_INCLUDE_DIR)/QtSql -I. -c -o moc_mainwindow.o
	$(CXX) dbapi.cpp -I$(QT_INCLUDE_DIR) -I$(QT_INCLUDE_DIR)/QtGui -I$(QT_INCLUDE_DIR)/QtSql -I$(QT_INCLUDE_DIR)/QtCore -I. -c -o dbapi.o
	$(CXX) mainwindow.cpp -I$(QT_INCLUDE_DIR) -I$(QT_INCLUDE_DIR)/QtGui -I$(QT_INCLUDE_DIR)/QtCore -I$(QT_INCLUDE_DIR)/QtSql -I. -c -o mainwindow.o
	$(CXX) main.cpp -I$(QT_INCLUDE_DIR) -I$(QT_INCLUDE_DIR)/QtGui -I$(QT_INCLUDE_DIR)/QtCore -I$(QT_INCLUDE_DIR)/QtSql -I. -c -o main.o
	$(CXX) -Wl,-rpath,/usr/local/Trolltech/Qt-4.7.0/lib main.o dbapi.o moc_mainwindow.o mainwindow.o -I$(QT_INCLUDE_DIR) -I$(QT_INCLUDE_DIR)/QtGui -I$(QT_INCLUDE_DIR)/QtSql -I$(QT_INCLUDE_DIR)/QtCore -I. -L$(QT_LIB_DIR) -lQtCore -lQtGui -lQtSql -o QRhymingDictionary
	
clear:
	rm -f moc_mainwindow.cpp
	rm -f *.o
