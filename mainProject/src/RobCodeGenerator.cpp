#include "./header/RobCodeGenerator.h"
#include "./header/Point3D.h"

CRobCodeGenerator::CRobCodeGenerator(void)
{
	speed = 0;
	speedManual = 0;
	orientationManual = 0;
	A = 0;
	B = 0;
	C = 0;
}

CRobCodeGenerator::CRobCodeGenerator(double Speed, bool SpeedManual, bool OrientationManual, tuple<double, double, double> angles)
{
	speed = Speed;
	speedManual = SpeedManual;
	orientationManual = OrientationManual;
	A = get<0>(angles);
	B = get<1>(angles);
	C = get<2>(angles);
}

CRobCodeGenerator::~CRobCodeGenerator(void)
{
}

void CRobCodeGenerator::generateRobCode(vector<CInputPoint3D>& points, string filename)
{
	postProcessing(points);

	errno_t err;
	
	FILE* fid;

	if ((err = fopen_s(&fid, filename.c_str(), "w")) != 0)
	{ 
		string msg = "Open file: ";
		msg += filename;
		msg += " failed!";

		throw exception(msg.c_str());
	}

	COutputPoint3D currentPoint;

	filename.erase(filename.end()-4,filename.end());
	fprintf(fid, "DEF %s \n", filename.c_str());

	fputs("PTP $POS_ACT\n", fid);

	if (speedManual)
	{
		fprintf(fid, "&VEL.CP %f", speed);
	}

	for (size_t s = 0; s < points.size(); s++)
	{
		currentPoint.set(points[s].getX(),points[s].getY(),points[s].getZ());
		
		//currentPoint = Transformation * currentPoint; //Here wee need a transformationmatrix containing public parameters scaleX,offsetX...
		if(!speedManual)
			fprintf(fid, "&VEL.CP %f", currentPoint.getSpeed());
		fprintf(fid, "LIN {X %f, Y %f, Z %f, A %f, B %f, C %f}\n", currentPoint.getX(), currentPoint.getY(), currentPoint.getZ(), 
			currentPoint.getA(), currentPoint.getB(), currentPoint.getC());
	}

	fputs("END", fid);
}

void CRobCodeGenerator::postProcessing(vector<CInputPoint3D>& path)
{
	COutputPoint3D p;
	CInputPoint3D  pIn;
	double timePrev = 0;

	for (size_t s = 0; s < path.size(); s++)
	{
		p.set(path[s].getX(), path[s].getY(), path[s].getZ());
		if (speedManual)
		{
			if (speed > MAX_SPEED) //Wenn maximale Geschwindigkeit überschritten wird, Geschwindigkeit begrenzen
				speed = MAX_SPEED;
		}
		else
		{
			if (s == 0)
				p.setSpeed(1); //Der erste Punkt(0) wird mit Standardgeschwindigkeit 1m/s angefahren.

			else
				p.setSpeed(calculateSpeed(path[s], s, timePrev)); //Die Geschwindigkeit zwischen den weiteren Punkten wird berechnet.
		}

		if (!orientationManual)
		{
			p.setA(A);
			p.setB(B);
			p.setC(C);
		}
		else
			calculateAngles(p, pIn);
		timePrev = path[s].getTime();
		processedPath.push_back(p);
	}

}

double CRobCodeGenerator::calculateSpeed(CInputPoint3D& p, size_t s, double timePrev)
{
	double distance = 0;
	double time = 0;

	distance = processedPath[s - 1].distanceTo(p); //Strecke zwischen p und dem Punkt zuvor
	time = p.getTime() - timePrev; //Zeit zwischen p-1 und p

	speed = distance / time; // Berechnug Geschwindigkeit zwischen zwei Punkten

	if (speed > MAX_SPEED) //Begrenzung auf maximale Geschwindigkeit, falls Trackerdaten höheren Wert aufweisen
		speed = MAX_SPEED;

	return speed; //Zuweisung der Geschwindigkeit
}

void CRobCodeGenerator::calculateAngles(COutputPoint3D& p, CInputPoint3D& pIn)
{
	double a, b, c;

	// Funktion in Eulermatrix aufrufen die a/b/c neu berechnet

	p.setA(a);
	p.setB(b);
	p.setC(c);
}
