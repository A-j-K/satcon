

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QVector>
#include <QSharedPointer>

#include "libsgp4/Globals.h"
#include "libsgp4/SGP4.h"
#include "libsgp4/Util.h"
#include "libsgp4/Vector.h"
#include "libsgp4/Observer.h"
#include "libsgp4/SolarPosition.h"
#include "libsgp4/CoordGeodetic.h"
#include "libsgp4/CoordTopocentric.h"

#define GRAVES_FREQ 143050000.0
#define SPEED_OF_LIGHT 299792456.0
#define KM_2_M(x) (x*1000)
#define HZ_2_MHZ(x) (x/1000000.0)

typedef QVector<::Tle> TleList;

QT_BEGIN_NAMESPACE
namespace Ui { 
	class MainWindow; 
}
QT_END_NAMESPACE

class tleExclusionManager
{
public:
	typedef std::map<int, ::DateTime> List;
	tleExclusionManager();
	~tleExclusionManager();
	bool check(int satid, const DateTime& d);
	bool find(int satid, DateTime & out_p);
	void clean(const DateTime& d);
	tleExclusionManager& add(int satid, const DateTime& d);
	
private:
	List	_list;
};

class topocentricTargetParams
{
public:
	topocentricTargetParams();
	topocentricTargetParams(double altl, double alth, double azml, double azmh);
	~topocentricTargetParams();
	bool inTargetZone(double alt, double azm);
	bool topocentricTargetParams::inTargetZone(const CoordTopocentric& in);
	void setParamsFromDegrees(double altl, double alth, double azml, double azmh);
	
private:
	double	_azml; // In radians.
	double	_azmh; // In radians.
	double	_altl; // In radians.
	double	_alth; // In radians.
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

	static const int COL_SATID = 0;
	static const int COL_ETA = 1;
	static const int COL_AZM = 2;
	static const int COL_ALT = 3;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

	QTableWidget* getTablePtr();
	QTableWidget* prepareTable();
	QTableWidgetItem* getCell(int row, int col);
	void setCell(int row, int col, QTableWidgetItem* item);

	/**
	* Used to load a TLE file into a TleList.
	* @param in QString filename The name of the file.
	* @param out TleList The list to put all the TLEs found in the file.
	* @return the number of TLEs loaded into the list.
	*/
	int loadTleFile(const QString filename, TleList& outToList);

	Tle makeTle(const QString l0, const QString l1, const QString l2, const QString filename);

    Ui::MainWindow		*_ui;
	TleList				_tlelist;

};
#endif // MAINWINDOW_H
