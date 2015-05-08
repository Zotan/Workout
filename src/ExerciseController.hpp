/*
 * ExerciseController.hpp
 *
 *  Created on: 19 avr. 2015
 *      Author: pierre
 */

#ifndef EXERCISECONTROLLER_HPP_
#define EXERCISECONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>
#include <bb/system/SystemUiResult>

class Exercise;

class ExerciseController : public QObject {
    Q_OBJECT;


private:

    static QMap<QString, int>        m_Labels;

    bb::cascades::ListView          *m_ListView;

    int                              m_tmp_id;

public:
    ExerciseController              (QObject *parent = 0);
    virtual ~ExerciseController     ()                               {};



public Q_SLOTS:

    inline void setListView          (QObject *list)                  { m_ListView = dynamic_cast<bb::cascades::ListView*>(list); };
    void getExerciseList             ();
    void filter                      (const QString &key);


    void newExercise                 (const QString &label, int type);
    void deleteExercise              (int id);
    void onPromptFinishedDeleteExercise(bb::system::SystemUiResult::Type);
    void renameExercise             (int id, const QString &label);
    void onPromptFinishedRenameExercise(bb::system::SystemUiResult::Type);



Q_SIGNALS:

    void completed();



};





#endif /* EXERCISECONTROLLER_HPP_ */
