/****************************************************************************
** Meta object code from reading C++ file 'MutiTableWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../MutiTableWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MutiTableWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BaseDataTable_t {
    QByteArrayData data[17];
    char stringdata0[191];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseDataTable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseDataTable_t qt_meta_stringdata_BaseDataTable = {
    {
QT_MOC_LITERAL(0, 0, 13), // "BaseDataTable"
QT_MOC_LITERAL(1, 14, 11), // "pageChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 11), // "currentPage"
QT_MOC_LITERAL(4, 39, 10), // "totalPages"
QT_MOC_LITERAL(5, 50, 8), // "goToPage"
QT_MOC_LITERAL(6, 59, 4), // "page"
QT_MOC_LITERAL(7, 64, 12), // "goToNextPage"
QT_MOC_LITERAL(8, 77, 16), // "goToPreviousPage"
QT_MOC_LITERAL(9, 94, 13), // "goToFirstPage"
QT_MOC_LITERAL(10, 108, 12), // "goToLastPage"
QT_MOC_LITERAL(11, 121, 16), // "onSectionResized"
QT_MOC_LITERAL(12, 138, 12), // "logicalIndex"
QT_MOC_LITERAL(13, 151, 7), // "oldSize"
QT_MOC_LITERAL(14, 159, 7), // "newSize"
QT_MOC_LITERAL(15, 167, 11), // "applyFilter"
QT_MOC_LITERAL(16, 179, 11) // "clearFilter"

    },
    "BaseDataTable\0pageChanged\0\0currentPage\0"
    "totalPages\0goToPage\0page\0goToNextPage\0"
    "goToPreviousPage\0goToFirstPage\0"
    "goToLastPage\0onSectionResized\0"
    "logicalIndex\0oldSize\0newSize\0applyFilter\0"
    "clearFilter"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseDataTable[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   64,    2, 0x0a /* Public */,
       7,    0,   67,    2, 0x0a /* Public */,
       8,    0,   68,    2, 0x0a /* Public */,
       9,    0,   69,    2, 0x0a /* Public */,
      10,    0,   70,    2, 0x0a /* Public */,
      11,    3,   71,    2, 0x0a /* Public */,
      15,    1,   78,    2, 0x0a /* Public */,
      16,    1,   81,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   12,   13,   14,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,

       0        // eod
};

void BaseDataTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BaseDataTable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pageChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->goToPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->goToNextPage(); break;
        case 3: _t->goToPreviousPage(); break;
        case 4: _t->goToFirstPage(); break;
        case 5: _t->goToLastPage(); break;
        case 6: _t->onSectionResized((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->applyFilter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->clearFilter((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BaseDataTable::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseDataTable::pageChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BaseDataTable::staticMetaObject = { {
    QMetaObject::SuperData::link<QTableWidget::staticMetaObject>(),
    qt_meta_stringdata_BaseDataTable.data,
    qt_meta_data_BaseDataTable,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BaseDataTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseDataTable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BaseDataTable.stringdata0))
        return static_cast<void*>(this);
    return QTableWidget::qt_metacast(_clname);
}

int BaseDataTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void BaseDataTable::pageChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
