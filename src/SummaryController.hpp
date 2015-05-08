/*
 * SummaryController.hpp
 *
 *  Created on: 3 mai 2015
 *      Author: pierre
 */

#ifndef SUMMARYCONTROLLER_HPP_
#define SUMMARYCONTROLLER_HPP_


#include <bb/cascades/ListView>



class SummaryController : public QObject {
    Q_OBJECT;

    Q_PROPERTY( QString date       READ getDate       NOTIFY dateChanged)
    Q_PROPERTY( QString stats      READ getStats      NOTIFY statsChanged)


private:

    QString                          m_Date;
    QString                          m_Stats;

    bb::cascades::ListView          *m_ListView;



public:
    SummaryController               (QObject *parent = 0);
    virtual ~SummaryController      ()                               {};





public Q_SLOTS:

    inline const QString &getDate   () const                        { return m_Date; }
    inline const QString &getStats  () const                        { return m_Stats; }


    inline void setListView         (QObject *list)                 { m_ListView = dynamic_cast<bb::cascades::ListView*>(list);};

    void getInfos                   ();



Q_SIGNALS:

    void dateChanged();
    void statsChanged();


};


#endif /* SUMMARYCONTROLLER_HPP_ */
