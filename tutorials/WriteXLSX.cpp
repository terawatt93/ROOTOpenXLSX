#include "ROOTOpenXLSXLib.hh"

void WriteXLSX()
{
	ROOTOpenXLSX x;
	x.Open("test.xlsx","w","FirstSheet");//имя файла, чтение/запись, название первого листа (если нужно)
	//по умолчанию, ввод на первый лист
	x<<"aa"<<"b"<<"1111"<<1111<<"\n";
	x<<"cc dd"<<"\n";
	x.GoToWorksheet("SecondSheet");
	x<<"second"<<"third"<<"\n";
	x<<3.1416<<"\n";
	x.Close();
}
