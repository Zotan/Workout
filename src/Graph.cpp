/*
 * Graph.cpp
 *
 *  Created on: 1 juin 2015
 *      Author: pierre
 */

#include "Graph.hpp"

#include <bb/cascades/Image>
#include <bb/cascades/Label>
#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/ImageData>
#include <limits>

Graph::Graph(QObject *parent) : QObject(parent), m_Container(NULL), m_Height(0), m_Width(0) {
    m_MarginW = 40;
    m_MarginH = 40;
    m_XTickCount = 0;
    m_YTickCount = 0;
}




void Graph::draw() {

    QList<float> x, y;
    x.push_back(-2); x.push_back(2); x.push_back(4); x.push_back(10);
    y.push_back(-1); y.push_back(1); y.push_back(-1); y.push_back(3);  y.push_back(3);  y.push_back(3);

    QList<QString> labels;
    labels.push_back("29/05/2015 10:10");
    labels.push_back("30/05/2015 10:10");
    labels.push_back("31/05/2015 10:10");
    labels.push_back("01/06/2015 10:10");

    plot(labels, y);

}

#define XUNIT_PX(x) (5+m_MarginW/2 + ((x) - minX)*scalingX)
#define YUNIT_PX(y) (yOffset+m_Height - m_MarginH/2 - ((y) - minY)*scalingY)


void Graph::clear() {
    m_Image = QImage(QSize(m_Width, m_Height), QImage::Format_RGB32);
    m_Image.fill(Qt::white);

    if(m_Container != NULL) {
        for(int i = 0 ; i < m_Labels.size() ; ++i) {
            m_Container->remove(m_Labels.at(i));
            m_Labels.at(i)->deleteLater();
        }
        m_Labels.clear();
    }

}


void Graph::drawBoundingBox() {
    QPainter painter(&m_Image);
    painter.drawRect(QRect(m_MarginW/2, m_MarginH/2, m_Width-m_MarginW, m_Height-m_MarginH));
}

void Graph::drawAxis() {

}

void Graph::plot(const QList<float> &x, const QList<float> &y, const QList<QString> &labels) {
    m_MarginH = 130;
    if(!y.isEmpty() && y.at(0) > 1000)
        m_MarginW = 260;
    else
        m_MarginW = 170;

    m_YTickCount = 5;
    if(labels.isEmpty())
        m_XTickCount = 5;
    else
        m_XTickCount = labels.size()-1;

    clear();
    drawBoundingBox();


    QPainter painter(&m_Image);

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();



    for(int i = 0 ; i < x.size() ; ++i) {
        if(minX > x.at(i)) {
            minX = x.at(i);
        }
        if(maxX < x.at(i)) {
            maxX = x.at(i);
        }
    }

    for(int i = 0 ; i < y.size() ; ++i) {
        if(minY > y.at(i)) {
            minY = y.at(i);
        }
        if(maxY < y.at(i)) {
            maxY = y.at(i);
        }
    }

    if(maxX - minX < 0.00001) return;


    float scalingX = (m_Width-m_MarginW-10) / (maxX-minX);
    float scalingY = (m_Height-m_MarginH-10) / (maxY-minY);
    float aspectRatio = static_cast<float>(m_Width)/static_cast<float>(m_Height);

    float yOffset = -5;
    if(maxY-minY < 0.00001) {
        scalingY = 2;
        yOffset = -m_Height/2;
    }

    // plot ticks & text
    // Apparently it is not possible to use the function drawText() from QPainter --> result in systematic crash...
    // The official solution from BB forum is to use either a WebView, OpenGL, ... or as here, use labels.
    // This requires to create a overall container with a absolute layout which contain the picture on which I am drawing
    painter.setPen(QPen(QColor("#c8298b"), 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    float stepY = (maxY-minY) / m_YTickCount;
    for(int i = 0 ; i < m_YTickCount+1 ; ++i) {
        painter.drawLine(QLine(m_MarginW/2-5, YUNIT_PX(minY+stepY*i)+5, m_MarginW/2, YUNIT_PX(minY+stepY*i)+5));

        if(m_Container != NULL) {
            bb::cascades::Label *label = new bb::cascades::Label();
            label->setText(QString::number(minY+stepY*i));
            label->textStyle()->setTextAlign(bb::cascades::TextAlign::Right);
            label->setPreferredWidth(m_MarginW/2-10);

            bb::cascades::AbsoluteLayoutProperties* pProperties = bb::cascades::AbsoluteLayoutProperties::create();
            pProperties->setPositionX(0.0);
            pProperties->setPositionY(YUNIT_PX(minY+stepY*i)-30);


            label->setLayoutProperties(pProperties);

            m_Container->add(label);
            m_Labels.push_back(label);
        }

    }

    float stepX = (maxX-minX) / m_XTickCount;
    for(int i = 0 ; i < m_XTickCount+1 ; ++i) {
        painter.drawLine(QLine(XUNIT_PX(minX+stepX*i), m_Height-m_MarginH/2+5, XUNIT_PX(minX+stepX*i), m_Height-m_MarginH/2));

        if(m_Container != NULL) {
            bb::cascades::Label *label = new bb::cascades::Label();
            if(labels.size() > i) {
                label->setText(labels.at(i));
                label->textStyle()->setFontSize(bb::cascades::FontSize::XXSmall);
                label->setPreferredWidth(stepX*scalingX-10);
                label->setMultiline(true);
            } else {
                if(labels.isEmpty())
                    label->setText(QString::number(minX+stepX*i));
            }
            label->textStyle()->setTextAlign(bb::cascades::TextAlign::Left);
            //label->setPreferredWidth(100);

            bb::cascades::AbsoluteLayoutProperties* pProperties = bb::cascades::AbsoluteLayoutProperties::create();
            pProperties->setPositionX(XUNIT_PX(minX+stepX*i));
            pProperties->setPositionY(m_Height-m_MarginH/2+10);




            label->setLayoutProperties(pProperties);

            m_Container->add(label);
            m_Labels.push_back(label);

        }
    }

    // plot lines
    painter.setPen(QPen(QColor("#c8298b"), 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    for(int i = 0 ; i < std::min(x.size(), y.size())-1 ; ++i) {
        painter.drawLine(QLine(XUNIT_PX(x.at(i))+5, YUNIT_PX(y.at(i))+5, XUNIT_PX(x.at(i+1))+5, YUNIT_PX(y.at(i+1))+5));
    }

    // plot points
    painter.setBrush(QBrush(QColor("#0e5a77")));
    painter.setPen(QPen(QColor("#0e5a77"), 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
    for(int i = 0 ; i < std::min(x.size(), y.size()) ; ++i) {
        painter.drawEllipse(QRectF(XUNIT_PX(x.at(i)), YUNIT_PX(y.at(i)), 10, 10*aspectRatio));
    }


    renderGraph();
}

void Graph::plot(const QList<QString> &labels, const QList<float> &y) {
    QList<float> x;
    for(int i = 0 ; i < y.size(); ++i) x.push_back(i);

    plot(x, y, labels);
}

void Graph::renderGraph() {
    QImage swapped     = m_Image.rgbSwapped();

    const bb::ImageData imageData = bb::ImageData::fromPixels(swapped.bits(), bb::PixelFormat::RGBX, swapped.width(), swapped.height(), swapped.bytesPerLine());

    m_ImageGraph = bb::cascades::Image(imageData);
    emit imageChanged();
}
