#include <OpenXLSX.hpp>
#include <iostream>
#include <cmath>
#include <TObject.h>
#include <typeinfo>
#include <map>

using namespace std;
using namespace OpenXLSX;

namespace {
    // Функция, гарантирующая линковку ВСЕХ необходимых классов OpenXLSX
    void __attribute__((used)) force_link_openxlsx_classes() {
        // Используем void* вместо volatile auto*
        const void* volatile ptrs[] = {
            // Базовые классы
            (void*)typeid(OpenXLSX::XLCellAssignable).name(),
            (void*)typeid(OpenXLSX::XLCellValue).name(),
            (void*)typeid(OpenXLSX::XLCell).name(),
            (void*)typeid(OpenXLSX::XLWorksheet).name(),
            (void*)typeid(OpenXLSX::XLWorkbook).name(),
            (void*)typeid(OpenXLSX::XLDocument).name(),
            // Шаблонные классы
            (void*)typeid(OpenXLSX::XLColor).name(),
            (void*)typeid(OpenXLSX::XLDateTime).name(),
            (void*)typeid(OpenXLSX::XLFormula).name(),
            // Важные внутренние классы
            (void*)typeid(OpenXLSX::XLXmlData).name(),
            //(void*)typeid(OpenXLSX::XLAbstractXMLFile).name(),
            (void*)typeid(OpenXLSX::XLContentTypes).name(),
        };
        (void)ptrs; // Подавление предупреждений
    }
}
class ROOTXLWorksheet
{
	public:
	OpenXLSX::XLWorksheet ws;
	string Name;
	int CellX=1,CellY=1, XBorder=1, YBorder=1;
	
	ClassDef(ROOTXLWorksheet, 1);
};

class ROOTOpenXLSX: public TObject
{
	public:
	XLDocument *doc=0;
	bool Opened=false;
	bool ForWrite=false;
	void Open(string Name, string OpenOption="r", string FirstWSName="Sheet1");
	void Close();
	void GoToWorksheet(string Name);
	map<string, ROOTXLWorksheet> Worksheets;
	ROOTXLWorksheet *CurrentWorksheet=0;
	void WriteCeil(uint32_t x,uint32_t y,double value);
	void WriteCeil(uint32_t x,uint32_t y,int value);
	void WriteCeil(uint32_t x,uint32_t y,long unsigned int value);
	void WriteCeil(uint32_t x,uint32_t y,string value);
	void SetInitCeil(uint32_t x,uint32_t y);
	void SelectRow(int rn);
	int NRows();
	ROOTOpenXLSX();
	
	ClassDef(ROOTOpenXLSX, 1);
};
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, double value);
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, float value);
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, int value);
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, unsigned int value);
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, char value);
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, string value);

ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, double &value);
ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, float &value);
ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, int &value);
ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, unsigned int &value);
ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, char &value);
ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, string &value);
