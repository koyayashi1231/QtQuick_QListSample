#ifndef ELEMENTLIST_H
#define ELEMENTLIST_H

#include <QAbstractListModel>
#include "element.h"
#include <QQmlEngine>
#include <QTime>

class ElementList : public QAbstractListModel {
    Q_OBJECT
    QList<Element*> m_ElementList;

private:
    static const int m_allowedRisekiMinutes = 5;

public:
    ElementList(QObject * parent = nullptr): QAbstractListModel(parent){}
    ~ElementList() Q_DECL_OVERRIDE { qDeleteAll(m_ElementList); }

    int rowCount(const QModelIndex &) const Q_DECL_OVERRIDE
    {
        return m_ElementList.size();
    }

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE
    {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return QVariant::fromValue<QObject*>(m_ElementList.at(index.row()));
        }
        return QVariant();
    }
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE
    {
        Q_UNUSED(role);
        Element* element = value.value<Element*>();
        if (!element) return false;
        if (element == m_ElementList.at(index.row())) return true;
        delete m_ElementList.at(index.row());
        m_ElementList[index.row()] = element;
        QVector<int> roles;
        roles << role;
        return true;
    }

    Q_INVOKABLE bool addElement(QString name, const QModelIndex &parent = QModelIndex())
    {
        Q_UNUSED(parent);
        int row = m_ElementList.size();
        beginInsertRows(QModelIndex(), row, row);
        Element* element = new Element(name, this);
        QObject::connect(element, SIGNAL(killMe()), this, SLOT(removeDieElements()));
        m_ElementList.insert(row, element);
        QQmlEngine::setObjectOwnership(element, QQmlEngine::CppOwnership);
        endInsertRows();
        return true;
    }

    Q_INVOKABLE bool randRemoveRow(const QModelIndex &parent = QModelIndex())
    {
        Q_UNUSED(parent);
        if(m_ElementList.size()<1)
            return false;
        qsrand(static_cast<uint>(QTime::currentTime().msec()));
        int ra = qrand() % m_ElementList.size();
        beginRemoveRows(QModelIndex(), ra, ra);
        delete m_ElementList.takeAt(ra);
        endRemoveRows();
        return true;
    }

    Q_SLOT void removeDieElements() {
        if(m_ElementList.size()<1)
            return;
        for (int i=0; i<m_ElementList.size(); i++)
        {
            if(m_ElementList[i]->lifeSpan()<=0)
            {
                beginRemoveRows(QModelIndex(), i, i);
                delete m_ElementList.takeAt(i);
                endRemoveRows();
            }
        }
    }

};

#endif // ELEMENTLIST_H
