#include "MeanFilter.h"
#include <math.h>

CMeanFilter::CMeanFilter()
{
	setWindowSize(0);
	setLenth(0);
}

CMeanFilter::CMeanFilter(int Window, long lenth)
{
	setWindowSize(Window);
	setLenth(lenth);
}

CMeanFilter::~CMeanFilter()
{
}

void CMeanFilter::setWindowSize(int Window)
{
	windowSize = Window;
}

void CMeanFilter::setLenth(long lenth)
{
	lengthArray = lenth;
}

int CMeanFilter::getWindowSize()
{
	return windowSize;
}

vector<CPoint3D>& CMeanFilter::getPath()
{
	return path;
}

void CMeanFilter::calculateMean(const vector<list<CPoint3D>>& sourcePath)
{

//TODO: anpassen damit CPoint3D verwendet wird
	double sumX = 0, sumY = 0, sumZ = 0;		// oder long??
	double div = 0;

	int i = 0;
	int m = 0;
	int OffsetPos = 0;
	int OffsetNeg = 0;

	if (windowSize % 2)
	// ungerades Fenster
	{
		OffsetPos = windowSize / 2;
		OffsetNeg = windowSize / 2;
	}
	else
	// gerades Fenster
	{
		OffsetPos = (windowSize / 2) - 1;
		OffsetNeg = windowSize / 2;
	}

	for (i = 0; i < sourcePath.size(); i++)
	{
		vector<list<CPoint3D>> source = sourcePath;
		sum = 0;
		div = 0;

		//positiven Offset addieren
		for (m = 0; m < OffsetPos; m++)
		{
			if ((i + m) > sourcePath.size())
			{
				break;
			}

			sum += *source;
			div++;
			*source++;
		}

		*source =- m;

		// negatives Offset addieren
		for (m = 1; m < OffsetNeg; m++ )
		{
			if ((i - m) < 0)
			{
				break;
			}

			sum += *source;
			div++;
			*source--;
		}

		*target = sum / div;
		target++;
	}

}