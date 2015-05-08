/*
 * AppSettings.hpp
 *
 *  Created on: 8 mai 2015
 *      Author: pierre
 */

#ifndef APPSETTINGS_HPP_
#define APPSETTINGS_HPP_






class AppSettings : public QObject {
    Q_OBJECT;

    Q_PROPERTY( int theme     READ getTheme     WRITE setTheme       NOTIFY themeChanged)
    Q_PROPERTY( int unit      READ getUnit      WRITE setUnit        NOTIFY unitChanged)


private:

    int                          m_Theme;
    int                          m_Unit;


public:
    AppSettings               (QObject *parent = 0);
    virtual ~AppSettings      ()                               {};



public Q_SLOTS:

    inline int  getTheme   () const                        { return m_Theme; }
    inline void setTheme   (int c)                         { if(c != m_Theme) {m_Theme = c; emit  themeChanged(); }}

    inline int  getUnit   () const                         { return m_Unit; }
    inline void setUnit   (int c)                          { if(c != m_Unit) {m_Unit = c; emit  unitChanged(); }}


    void save             ();


Q_SIGNALS:

    void themeChanged();
    void unitChanged();


};


#endif /* APPSETTINGS_HPP_ */
