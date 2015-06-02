/*
 * Graph.hpp
 *
 *  Created on: 1 juin 2015
 *      Author: pierre
 */

#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <bb/cascades/Image>
#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <QList>

class Graph : public QObject {
    Q_OBJECT;

    Q_PROPERTY( const bb::cascades::Image& image    READ getImage                               NOTIFY imageChanged)
    Q_PROPERTY( int width                           READ getWidth       WRITE setWidth          NOTIFY widthChanged)
    Q_PROPERTY( int height                          READ getHeight      WRITE setHeight         NOTIFY heightChanged)


private:

    bb::cascades::Image              m_ImageGraph;
    bb::cascades::Container         *m_Container;
    QImage                           m_Image;
    int                              m_Height;
    int                              m_Width;
    QList<bb::cascades::Label*>      m_Labels;


    int                              m_MarginW;
    int                              m_MarginH;
    int                              m_XTickCount;
    int                              m_YTickCount;

    void renderGraph    ();


public:
    Graph               (QObject *parent = 0);
    virtual ~Graph      ()                                      {};



public Q_SLOTS:
    inline const bb::cascades::Image&      getImage() const      { return m_ImageGraph; }

    inline int  getWidth   () const                        { return m_Width; }
    inline void setWidth   (int c)                         { if(c != m_Width) {m_Width = c; emit  widthChanged(); }}

    inline int  getHeight   () const                        { return m_Height; }
    inline void setHeight   (int c)                         { if(c != m_Height) {m_Height = c; emit  heightChanged(); }}

    void draw               ();

    void clear              ();
    void drawBoundingBox    ();
    void drawAxis           ();
    void plot               (const QList<float> &x,        const QList<float> &y, const QList<QString> &labels = QList<QString>());
    void plot               (const QList<QString> &labels, const QList<float> &y);


    inline void setContainer(QObject *container)                 { m_Container = dynamic_cast<bb::cascades::Container*>(container);};



Q_SIGNALS:
    void imageChanged();
    void widthChanged();
    void heightChanged();



};



#endif /* GRAPH_HPP_ */
