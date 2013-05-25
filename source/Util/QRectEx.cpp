#include "QRectEx.h"
#include <math.h>
#include <stdio.h>

/**
 * Default constructor.
 */
QRectEx::QRectEx()
{
}

/**
 * Constructor.
 * @param [in] source Source for the.
 */
QRectEx::QRectEx(const QRectF& source) : QRectF(source)
{
}

/**
 * Calculates distance to point.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return The distance.
 */
int QRectEx::DistToPoint(qreal x, qreal y)
{
	int minDist = std::numeric_limits<int>::max(), dist;
	
	//calculate distances to each side
	minDist = DistToSegment(x, y, left(), top(), left(), bottom());
	
	dist = DistToSegment(x, y, left(), top(), right(), top());
	if (minDist > dist)
		minDist = dist;

	dist = DistToSegment(x, y, right(), top(), right(), bottom());
	if (minDist > dist)
		minDist = dist;

	dist = DistToSegment(x, y, left(), bottom(), right(), bottom());
	if (minDist > dist)
		minDist = dist;
	
	return minDist;
}

/**
 * Calculates distance to segment.
 * @param x  The x coordinate.
 * @param y  The y coordinate.
 * @param x1 The first x value.
 * @param y1 The first y value.
 * @param x2 The second x value.
 * @param y2 The second y value.
 * @return .
 */
int QRectEx::DistToSegment(qreal x, qreal y, qreal x1, qreal y1, qreal x2, qreal y2)
{
	if (x1 == x2 && y1 == y2)
		return round(sqrt(pow(x - x1, 2) + pow(y - y1, 2)));

	double d1 = sqrt(pow(x - x1, 2) + pow(y - y1, 2));
	double d2 = sqrt(pow(x - x2, 2) + pow(y - y2, 2));
	double r = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	double p = (d1 + d2 + r) / 2;

	if (p - d1 < 0)
		d1 = p;
	if (p - d2 < 0)
		d2 = p;

	double h = 2 * sqrt(p * (p - r) * (p - d1) * (p - d2)) / r;
	double s = sqrt(pow(d1, 2) - pow(h, 2));

	if (s > r)
		return round(d2);

	s = sqrt(pow(d2, 2) - pow(h, 2));

	if (s > r)
		return round(d1);

	return round(h);
}
