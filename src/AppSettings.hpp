/*
 * AppSettings.hpp
 *
 *  Created on: 8 mai 2015
 *      Author: pierre
 */

#ifndef APPSETTINGS_HPP_
#define APPSETTINGS_HPP_


#include <bb/system/SystemUiResult>



class AppSettings : public QObject {
    Q_OBJECT;

    Q_PROPERTY( int theme     READ getTheme     WRITE setTheme       NOTIFY themeChanged)
    Q_PROPERTY( int unit      READ getUnit      WRITE setUnit        NOTIFY unitChanged)
    Q_PROPERTY( int ledNotif  READ getLedNotif  WRITE setLedNotif    NOTIFY ledNotifChanged)
    Q_PROPERTY( int soundNotif  READ getSoundNotif  WRITE setSoundNotif    NOTIFY soundNotifChanged)


private:

    int                          m_Theme;
    int                          m_Unit;
    int                          m_LedNotif;
    int                          m_SoundNotif;


public:
    AppSettings               (QObject *parent = 0);
    virtual ~AppSettings      ()                               {};



public Q_SLOTS:

    inline int  getTheme   () const                        { return m_Theme; }
    inline void setTheme   (int c)                         { if(c != m_Theme) {m_Theme = c; emit  themeChanged(); }}

    inline int  getUnit   () const                         { return m_Unit; }
    inline void setUnit   (int c)                          { if(c != m_Unit) {m_Unit = c; emit  unitChanged(); }}

    inline int  getLedNotif   () const                     { return m_LedNotif; }
    inline void setLedNotif   (int c)                      { if(c != m_LedNotif) {m_LedNotif = c; emit  ledNotifChanged(); }}

    inline int  getSoundNotif   () const                   { return m_SoundNotif; }
    inline void setSoundNotif   (int c)                    { if(c != m_SoundNotif) {m_SoundNotif = c; emit  soundNotifChanged(); }}


    void save             ();

    void saveDB           ();
    void saveFileSelected (const QStringList&);

    void loadDB           ();
    void loadFileSelected (const QStringList&);

    void exportDB         ();
    void exportFileSelected (const QStringList&);

    void canceled         ();

    void onPromptFinishedLoadDB(bb::system::SystemUiResult::Type);


Q_SIGNALS:

    void themeChanged();
    void unitChanged();
    void ledNotifChanged();
    void soundNotifChanged();


};


#endif /* APPSETTINGS_HPP_ */
