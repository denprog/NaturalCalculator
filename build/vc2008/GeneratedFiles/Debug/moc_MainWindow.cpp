/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Fri 28. Oct 13:19:32 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../source/Main/MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      20,   11,   11,   11, 0x08,
      29,   11,   11,   11, 0x08,
      38,   11,   11,   11, 0x08,
      47,   11,   11,   11, 0x08,
      56,   11,   11,   11, 0x08,
      69,   11,   11,   11, 0x08,
      78,   11,   11,   11, 0x08,
      88,   11,   11,   11, 0x08,
     101,   11,   11,   11, 0x08,
     114,   11,   11,   11, 0x08,
     124,   11,   11,   11, 0x08,
     133,   11,   11,   11, 0x08,
     146,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0OnNew()\0OnOpen()\0OnSave()\0"
    "OnUndo()\0OnRedo()\0OnSettings()\0OnPlus()\0"
    "OnMinus()\0OnMultiply()\0OnDivision()\0"
    "OnPower()\0OnSqrt()\0OnEquality()\0OnSin()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnNew(); break;
        case 1: OnOpen(); break;
        case 2: OnSave(); break;
        case 3: OnUndo(); break;
        case 4: OnRedo(); break;
        case 5: OnSettings(); break;
        case 6: OnPlus(); break;
        case 7: OnMinus(); break;
        case 8: OnMultiply(); break;
        case 9: OnDivision(); break;
        case 10: OnPower(); break;
        case 11: OnSqrt(); break;
        case 12: OnEquality(); break;
        case 13: OnSin(); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
