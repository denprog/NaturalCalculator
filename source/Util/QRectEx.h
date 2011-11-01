#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QRect>
#include <limits>

#define round(fp) (int)((fp) >= 0 ? (fp) + 0.5 : (fp) - 0.5)

/**
 * QRectEx class.
 * Extends the QRectF class with some methods.
 */
class QRectEx : public QRectF
{
public:
	QRectEx();
	QRectEx(QRectF& source);

public:
	int DistToPoint(qreal x, qreal y);
	int DistToSegment(qreal x, qreal y, qreal x1, qreal y1, qreal x2, qreal y2);
};

#endif
