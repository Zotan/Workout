/*
 * DataObject.hpp
 *
 *  Created on: 19 avr. 2015
 *      Author: pierre
 */

#ifndef DATAOBJECT_HPP_
#define DATAOBJECT_HPP_



class Exercise : public QObject {
    Q_OBJECT

    Q_PROPERTY( int id              READ getId          WRITE setId           NOTIFY idChanged)
    Q_PROPERTY( QString title       READ getTitle       WRITE setTitle        NOTIFY titleChanged)
    Q_PROPERTY( int     category    READ getCategory    WRITE setCategory     NOTIFY categoryChanged)

    // ----------------------------------------------------------------------------------------------


private:
    QString m_Title;
    int     m_Category;
    int m_Id;


    // ----------------------------------------------------------------------------------------------

public:
    Exercise(QObject *parent = 0) : QObject(parent), m_Id(0) {}
    virtual ~Exercise() {}



    inline const QString &getTitle() const                  { return m_Title; }
    inline void           setTitle(const QString &s)        { m_Title = s; }

    inline int            getCategory() const               { return m_Category; }
    inline void           setCategory(int c)                { m_Category = c; }

    inline int            getId() const                     { return m_Id; }
    inline void           setId(int c)                      { m_Id = c; }


    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void titleChanged();
        void idChanged();
        void categoryChanged();

};

class Cardio : public QObject {
    Q_OBJECT

    Q_PROPERTY( int id              READ getId          WRITE setId             NOTIFY idChanged)
    Q_PROPERTY( qint64 time         READ getTime        WRITE setTime           NOTIFY timeChanged)
    Q_PROPERTY( QString note        READ getNote        WRITE setNote           NOTIFY noteChanged)
    Q_PROPERTY( int distance        READ getDistance    WRITE setDistance       NOTIFY distanceChanged)
    Q_PROPERTY( int duration        READ getDuration    WRITE setDuration       NOTIFY durationChanged)
    Q_PROPERTY( int heartRate       READ getHeartRate   WRITE setHeartRate      NOTIFY heartRateChanged)
    Q_PROPERTY( int calories        READ getCalories    WRITE setCalories       NOTIFY caloriesChanged)

    // ----------------------------------------------------------------------------------------------


private:
    int m_Id;
    qint64 m_Time;
    QString m_Note;
    int m_Distance;
    int m_Duration;
    int m_HeartRate;
    int m_Calories;


    // ----------------------------------------------------------------------------------------------

public:
    Cardio(QObject *parent = 0) : QObject(parent), m_Id(0), m_Time(0), m_Distance(0), m_HeartRate(0), m_Calories(0) {}
    virtual ~Cardio() {}


    inline int            getId() const                     { return m_Id; }
    inline void           setId(int c)                      { m_Id = c; }

    inline qint64         getTime() const                   { return m_Time; }
    inline void           setTime(qint64 c)                 { m_Time = c; }

    inline const QString &getNote() const                   { return m_Note; }
    inline void           setNote(const QString &s)         { m_Note = s; }

    inline int            getDistance() const               { return m_Distance; }
    inline void           setDistance(int c)                { m_Distance = c; }

    inline int            getDuration() const               { return m_Duration; }
    inline void           setDuration(int c)                { m_Duration = c; }

    inline int            getHeartRate() const              { return m_HeartRate; }
    inline void           setHeartRate(int c)               { m_HeartRate = c; }

    inline int            getCalories() const               { return m_Calories; }
    inline void           setCalories(int c)                { m_Calories = c; }



    // ----------------------------------------------------------------------------------------------
Q_SIGNALS:
    void idChanged();
    void timeChanged();
    void noteChanged();
    void distanceChanged();
    void heartRateChanged();
    void caloriesChanged();
    void durationChanged();

};

class Set : public QObject {
    Q_OBJECT

    Q_PROPERTY( int id              READ getId          WRITE setId             NOTIFY idChanged)
    Q_PROPERTY( qint64 time         READ getTime        WRITE setTime           NOTIFY timeChanged)
    Q_PROPERTY( QString note        READ getNote        WRITE setNote           NOTIFY noteChanged)
    Q_PROPERTY( int repetition      READ getRepetition  WRITE setRepetition     NOTIFY repetitionChanged)
    Q_PROPERTY( float weight        READ getWeight      WRITE setWeight         NOTIFY weightChanged)

    // ----------------------------------------------------------------------------------------------


private:
    int m_Id;
    qint64 m_Time;
    QString m_Note;
    int m_Repetition;
    float m_Weight;


    // ----------------------------------------------------------------------------------------------

public:
    Set(QObject *parent = 0) : QObject(parent), m_Id(0), m_Time(0), m_Repetition(0), m_Weight(0) {}
    virtual ~Set() {}


    inline int            getId() const                     { return m_Id; }
    inline void           setId(int c)                      { m_Id = c; }

    inline qint64         getTime() const                   { return m_Time; }
    inline void           setTime(qint64 c)                 { m_Time = c; }

    inline const QString &getNote() const                   { return m_Note; }
    inline void           setNote(const QString &s)         { m_Note = s; }

    inline int            getRepetition() const             { return m_Repetition; }
    inline void           setRepetition(int c)              { m_Repetition = c; }

    inline float          getWeight() const                 { return m_Weight; }
    inline void           setWeight(float c)                { m_Weight = c; }



    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void idChanged();
        void timeChanged();
        void noteChanged();
        void repetitionChanged();
        void weightChanged();

};



class Detail : public QObject {
    Q_OBJECT

    Q_PROPERTY( QString title       READ getTitle       WRITE setTitle        NOTIFY titleChanged)
    Q_PROPERTY( QString text        READ getText        WRITE setText         NOTIFY textChanged)

    // ----------------------------------------------------------------------------------------------


private:
    QString m_Title;
    QString m_Text;


    // ----------------------------------------------------------------------------------------------

public:
    Detail(QObject *parent = 0) : QObject(parent) {}
    virtual ~Detail() {}



    inline const QString &getTitle() const                  { return m_Title; }
    inline void           setTitle(const QString &s)        { m_Title = s; }

    inline const QString &getText() const                   { return m_Text; }
    inline void           setText(const QString &s)         { m_Text = s; }


    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void titleChanged();
        void textChanged();
};


class Routine : public QObject {
    Q_OBJECT

    Q_PROPERTY( int id              READ getId          WRITE setId           NOTIFY idChanged)
    Q_PROPERTY( QString title       READ getTitle       WRITE setTitle        NOTIFY titleChanged)

    // ----------------------------------------------------------------------------------------------


private:
    QString m_Title;
    int m_Id;


    // ----------------------------------------------------------------------------------------------

public:
    Routine(QObject *parent = 0) : QObject(parent), m_Id(0) {}
    virtual ~Routine() {}



    inline const QString &getTitle() const                  { return m_Title; }
    inline void           setTitle(const QString &s)        { m_Title = s; }

    inline int            getId() const                     { return m_Id; }
    inline void           setId(int c)                      { m_Id = c; }


    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void titleChanged();
        void idChanged();

};



class RoutineExercise : public QObject {
    Q_OBJECT

    Q_PROPERTY( int id              READ getId          WRITE setId           NOTIFY idChanged)
    Q_PROPERTY( QString title       READ getTitle       WRITE setTitle        NOTIFY titleChanged)
    Q_PROPERTY( int     category    READ getCategory    WRITE setCategory     NOTIFY categoryChanged)
    Q_PROPERTY( int     position    READ getPosition    WRITE setPosition     NOTIFY positionChanged)
    Q_PROPERTY( int     exercise    READ getExercise    WRITE setExercise     NOTIFY exerciseChanged)

    // ----------------------------------------------------------------------------------------------


private:
    QString m_Title;
    int     m_Category;
    int     m_Id;
    int     m_Position;
    int     m_Exercise;

    // ----------------------------------------------------------------------------------------------

public:
    RoutineExercise(QObject *parent = 0) : QObject(parent), m_Category(0), m_Id(0), m_Position(0), m_Exercise(0) {}
    virtual ~RoutineExercise() {}



    inline const QString &getTitle() const                  { return m_Title; }
    inline void           setTitle(const QString &s)        { m_Title = s; }

    inline int            getCategory() const               { return m_Category; }
    inline void           setCategory(int c)                { m_Category = c; }

    inline int            getExercise() const               { return m_Exercise; }
    inline void           setExercise(int c)                { m_Exercise = c; }

    inline int            getId() const                     { return m_Id; }
    inline void           setId(int c)                      { m_Id = c; }

    inline int            getPosition() const               { return m_Position; }
    inline void           setPosition(int c)                { if(c != m_Position) {m_Position = c; emit positionChanged(); }}


    // ----------------------------------------------------------------------------------------------
    Q_SIGNALS:
        void titleChanged();
        void idChanged();
        void categoryChanged();
        void positionChanged();
        void exerciseChanged();

};


#endif /* DATAOBJECT_HPP_ */
