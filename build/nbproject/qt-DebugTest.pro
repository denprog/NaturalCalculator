# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/DebugTest/GNU-Linux-x86
TARGET = build
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui
SOURCES += ../source/Editor/Caret.cpp ../source/Editor/CaretState.cpp ../source/Editor/Command.cpp ../source/Editor/CommandManager.cpp ../source/FormulaNodes/BracesFormulaNode.cpp ../source/FormulaNodes/CommaFormulaNode.cpp ../source/FormulaNodes/CompoundFormulaNode.cpp ../source/FormulaNodes/DivisionFormulaNode.cpp ../source/FormulaNodes/DocumentFormulaNode.cpp ../source/FormulaNodes/EmptyFormulaNode.cpp ../source/FormulaNodes/EquationFormulaNode.cpp ../source/FormulaNodes/FormulaNode.cpp ../source/FormulaNodes/FormulaNodesCollection.cpp ../source/FormulaNodes/GroupFormulaNode.cpp ../source/FormulaNodes/MinusFormulaNode.cpp ../source/FormulaNodes/MultiplyFormulaNode.cpp ../source/FormulaNodes/NthRootFormulaNode.cpp ../source/FormulaNodes/PlusFormulaNode.cpp ../source/FormulaNodes/PowerFormulaNode.cpp ../source/FormulaNodes/ResultFormulaNode.cpp ../source/FormulaNodes/ResultItemFormulaNode.cpp ../source/FormulaNodes/RootFormulaNode.cpp ../source/FormulaNodes/ShapeFormulaNode.cpp ../source/FormulaNodes/SquareRootFormulaNode.cpp ../source/FormulaNodes/TextFormulaNode.cpp ../source/Main/FormulaWnd.cpp ../source/Main/MainWindow.cpp ../source/Main/NaturalCalculatorApp.cpp ../source/Main/Settings.cpp ../source/Main/SettingsDialog.cpp ../source/Main/SettingsDialogPages.cpp ../source/Main/main.cpp ../source/ParserThread/ParserExpression.cpp ../source/ParserThread/ParserThread.cpp ../source/Util/QRectEx.cpp ../test/BracesTest.cpp ../test/DocumentTest.cpp ../test/EquationTest.cpp ../test/NcTestBase.cpp ../test/NcTests.cpp ../test/ParseTest.cpp ../test/PowerTest.cpp ../test/SquareRootTest.cpp ../test/TextTest.cpp
HEADERS += ../source/Editor/Caret.h ../source/Editor/CaretState.h ../source/Editor/Command.h ../source/Editor/CommandManager.h ../source/FormulaNodes/BracesFormulaNode.h ../source/FormulaNodes/CommaFormulaNode.h ../source/FormulaNodes/CompoundFormulaNode.h ../source/FormulaNodes/DivisionFormulaNode.h ../source/FormulaNodes/DocumentFormulaNode.h ../source/FormulaNodes/EmptyFormulaNode.h ../source/FormulaNodes/EquationFormulaNode.h ../source/FormulaNodes/FormulaNode.h ../source/FormulaNodes/FormulaNodesCollection.h ../source/FormulaNodes/GroupFormulaNode.h ../source/FormulaNodes/MinusFormulaNode.h ../source/FormulaNodes/MultiplyFormulaNode.h ../source/FormulaNodes/NthRootFormulaNode.h ../source/FormulaNodes/PlusFormulaNode.h ../source/FormulaNodes/PowerFormulaNode.h ../source/FormulaNodes/ResultFormulaNode.h ../source/FormulaNodes/ResultItemFormulaNode.h ../source/FormulaNodes/RootFormulaNode.h ../source/FormulaNodes/ShapeFormulaNode.h ../source/FormulaNodes/SquareRootFormulaNode.h ../source/FormulaNodes/TextFormulaNode.h ../source/Main/FormulaWnd.h ../source/Main/MainWindow.h ../source/Main/NaturalCalculatorApp.h ../source/Main/Settings.h ../source/Main/SettingsDialog.h ../source/Main/SettingsDialogPages.h ../source/ParserThread/ParserExpression.h ../source/ParserThread/ParserThread.h ../source/Util/QRectEx.h ../test/BracesTest.h ../test/DocumentTest.h ../test/EquationTest.h ../test/NcTestBase.h ../test/NcTests.h ../test/ParseTest.h ../test/PowerTest.h ../test/SquareRootTest.h ../test/TextTest.h
FORMS += ../source/Forms/OptionsDialog.ui ../source/Forms/SettingsDialog.ui
RESOURCES += ../source/Resources/naturalcalculator.qrc
TRANSLATIONS += ../source/Translation/naturalcalculator_ru.ts
OBJECTS_DIR = build/DebugTest/GNU-Linux-x86
MOC_DIR = ./nbproject/moc
RCC_DIR = ./nbproject/rcc
UI_DIR = ./nbproject/ui
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += TEST 
INCLUDEPATH += ../../BigNumbersParser/source 
LIBS += -Wl,-rpath,../../BigNumbersParser/build/nbproject/build/Debug_shared ../../BigNumbersParser/build/nbproject/build/Debug_shared/libBigNumbersParser.so -lboost_thread -lboost_serialization -lQtTest -lboost_regex  
QMAKE_CXXFLAGS+=-Wno-unused-parameter
QMAKE_CXXFLAGS+=-std=c++0x
QMAKE_FLAGS+=-sj4
