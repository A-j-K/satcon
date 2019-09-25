#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QColor>
#include <QString>
#include <QTextStream>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
	QString s = QString("Hello world");
	_ui->statusbar->showMessage(s);

	auto table = prepareTable();
	//auto table = ui->centralwidget->findChild<QTableWidget*>("satellitesTable");
	//table->setRowCount(10);
	//table->setColumnCount(5);
	//table->show();

	int row = 0, column = 0;

	QTableWidgetItem* newItem = new QTableWidgetItem(tr("%1").arg(
		(row + 1) * (column + 1)));

	QBrush cellForground = Qt::yellow;
	newItem->setForeground(cellForground);
	setCell(row, column, newItem);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

QTableWidget* MainWindow::prepareTable()
{
	QTableWidgetItem *p_item;
	QFont font;
	QColor c(255, 255, 255);
	font.setBold(true);
	auto table = getTablePtr();
	auto header = table->horizontalHeader();
	header->setSectionResizeMode(QHeaderView::Stretch);
	table->setRowCount(10);
	table->setColumnCount(4);
	p_item = new QTableWidgetItem(tr("SatID"));
	p_item->setFont(font);
	table->setHorizontalHeaderItem(MainWindow::COL_SATID, p_item);
	p_item = new QTableWidgetItem(tr("ETA"));
	p_item->setFont(font);
	table->setHorizontalHeaderItem(MainWindow::COL_ETA, p_item);
	p_item = new QTableWidgetItem(tr("Azm"));
	p_item->setFont(font);
	table->setHorizontalHeaderItem(MainWindow::COL_AZM, p_item);
	p_item = new QTableWidgetItem(tr("Alt"));
	p_item->setFont(font);
	table->setHorizontalHeaderItem(MainWindow::COL_ALT, p_item);
	return table;
}

QTableWidget* MainWindow::getTablePtr()
{
	return _ui->centralwidget->findChild<QTableWidget*>("satellitesTable");
}

QTableWidgetItem* MainWindow::getCell(int row, int col)
{
	return getTablePtr()->item(row, col);
}

void MainWindow::setCell(int row, int col, QTableWidgetItem* item)
{
	getTablePtr()->setItem(row, col, item);
}

int MainWindow::loadTleFile(const QString filename, TleList& outToList)
{
	int rval = 0, state = 0;
	QFile f(filename);
	if (f.open(QIODevice::ReadOnly))
	{
		QString line0, line1, line2;
		QTextStream in(&f);
		while (!in.atEnd())
		{
			QString line = in.readLine();
			QChar fc = line.at(0);
			switch (state) {
			case 0:
				if (fc != '1' && fc != '2') {
					line0 = line;
					state = 1;
				}
				break;
			case 1:
				if (fc == '1') {
					line1 = line;
					state = 2;
				}
				break;
			case 2:
				if (fc == '2') {
					line2 = line;
					state = 3;
				}
			default:
				state = 0;
				break;
			}
			if (state == 3) {
				// Chop off "0 " if it is at the front of the string.
				QChar fc = line0.at(0);
				if (fc == '0') {
					QChar fc = line0.at(1);
					if (fc == ' ') {
						line0 = line0.right(line0.size() - 2);
					}
				}
				outToList.push_back(makeTle(line0, line1, line2, filename));
				rval++;
				line0.clear();
				line1.clear();
				line2.clear();
				state = 0;
			}
		}
		f.close();
	}
	return rval;
}

Tle MainWindow::makeTle(const QString l0, const QString l1, const QString l2, const QString filename)
{
	// Interface to std::string world used by the SGP4 library.
	Tle tle = Tle(l0.toStdString().c_str(), l1.toStdString().c_str(), l2.toStdString().c_str());
	tle.setFromFile(filename.toStdString().c_str());
	return tle;
}


topocentricTargetParams::topocentricTargetParams(double altl, double alth, double azml, double azmh) :
	  _alth(alth)
	, _altl(altl)
	, _azmh(azmh)
	, _azml(azml)
{}

topocentricTargetParams::topocentricTargetParams() :
	  _alth(0.)
	, _altl(0.)
	, _azmh(0.)
	, _azml(0.)
{}

topocentricTargetParams::~topocentricTargetParams()
{}

bool topocentricTargetParams::inTargetZone(double alt, double azm)
{
	return (azm >= _azml && azm < _azmh && alt >= _altl && alt < _alth);
}

bool topocentricTargetParams::inTargetZone(const CoordTopocentric& in)
{
	return inTargetZone(in.elevation, in.azimuth);
}

void topocentricTargetParams::setParamsFromDegrees(double altl, double alth, double azml, double azmh)
{
	_alth = Util::DegreesToRadians(alth);
	_altl = Util::DegreesToRadians(altl);
	_azmh = Util::DegreesToRadians(azmh);
	_azml = Util::DegreesToRadians(azml);
}

tleExclusionManager::tleExclusionManager() 
{}

tleExclusionManager::~tleExclusionManager() 
{}

tleExclusionManager&
tleExclusionManager::add(int satid, const DateTime& d)
{
	_list[satid] = d;
	return *this;
}

bool tleExclusionManager::check(int satid, const DateTime& d)
{
	bool rval = false;
	return rval;
}

void
tleExclusionManager::clean(const DateTime & in_d)
{
	// Remove any entries that are "in the past" (i.e. before in_d)
	auto itor = _list.begin();
	while (itor != _list.end()) {
		DateTime t = itor->second;
		if (t < in_d) {
			_list.erase(itor++);
		}
		else {
			++itor;
		}
	}
}

bool 
tleExclusionManager::find(int in_satid, DateTime & out_d)
{
	for (auto itor = _list.begin(); itor != _list.end(); ++itor) {
		if (itor->first == in_satid) {
			out_d = itor->second;
			return true;
		}
	}
	return false;
}


