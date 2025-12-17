#include "ROOTOpenXLSXLib.hh"
#include <TSystem.h>
#include <TROOT.h>
#pragma once

ROOTOpenXLSX::ROOTOpenXLSX()
{
	doc=new XLDocument();
}
void ROOTOpenXLSX::Open(string Name, string OpenOption, string FirstWSName)
{
	doc=new XLDocument();
	if(OpenOption=="r" || OpenOption=="read" || OpenOption=="Read" || OpenOption=="READ")
	{
		doc->open(Name);
		auto wb = doc->workbook();
		auto sheetNames = wb.worksheetNames();
		int iter=0;
		for (const auto& name : sheetNames) 
		{
			ROOTXLWorksheet ws;
			ws.Name=name;
			ws.ws=doc->workbook().worksheet(name);
			Worksheets[name]=ws;
			if(iter==0)
			{
				CurrentWorksheet=&(Worksheets[name]);
			}
		}
	}
	else if(OpenOption=="w" || OpenOption=="write" || OpenOption=="Write" || OpenOption=="WRITE")
	{
		doc->create(Name, XLForceOverwrite);
		ROOTXLWorksheet ws;
		ws.Name=FirstWSName;
		if(FirstWSName!="Sheet1")
		{
			doc->workbook().addWorksheet(FirstWSName);
			doc->workbook().deleteSheet("Sheet1");
		}
		ws.ws=doc->workbook().worksheet(FirstWSName);
		Worksheets[FirstWSName]=ws;
		CurrentWorksheet=&(Worksheets[FirstWSName]);
		ForWrite=true;
	}
	
}
void ROOTOpenXLSX::GoToWorksheet(string Name)
{
	if(!doc)
	{
		cout<<"Cannot select worksheet because of invalid pointer to doc!\n";
		return;
	}
	if(!CurrentWorksheet)
	{
		ROOTXLWorksheet ws;
		ws.Name=Name;
		doc->workbook().addWorksheet(Name);
		ws.ws=doc->workbook().worksheet(Name);
		Worksheets[Name]=ws;
		CurrentWorksheet=&(Worksheets[Name]);
	}
	if(CurrentWorksheet->Name != Name)
	{
		if(Worksheets.find(Name) == Worksheets.end())
		{
			ROOTXLWorksheet ws;
			ws.Name=Name;
			doc->workbook().addWorksheet(Name);
			ws.ws=doc->workbook().worksheet(Name);
			Worksheets[Name]=ws;
			CurrentWorksheet=&(Worksheets[Name]);
		}
		else
		{
			CurrentWorksheet=&(Worksheets[Name]);
		}
	}
}
void ROOTOpenXLSX::WriteCeil(uint32_t x,uint32_t y,double value)
{
	if(!CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::WriteCeil("<<x<<","<<y<<","<<value<<"): pointer to CurrentWorksheet is invalid!\n";
		return;
	}
	CurrentWorksheet->ws.cell(x,y).value()=value;
}
void ROOTOpenXLSX::WriteCeil(uint32_t x,uint32_t y,int value)
{
	if(!CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::WriteCeil("<<x<<","<<y<<","<<value<<"): pointer to CurrentWorksheet is invalid!\n";
		return;
	}
	CurrentWorksheet->ws.cell(x,y).value()=value;
}
void ROOTOpenXLSX::WriteCeil(uint32_t x,uint32_t y,long unsigned int value)
{
	if(!CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::WriteCeil("<<x<<","<<y<<","<<value<<"): pointer to CurrentWorksheet is invalid!\n";
		return;
	}
	CurrentWorksheet->ws.cell(x,y).value()=value;
}
void ROOTOpenXLSX::WriteCeil(uint32_t x,uint32_t y,string value)
{
	if(!CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::WriteCeil("<<x<<","<<y<<","<<value<<"): pointer to CurrentWorksheet is invalid!\n";
		return;
	}
	CurrentWorksheet->ws.cell(x,y).value()=value;
}
void ROOTOpenXLSX::SetInitCeil(uint32_t x,uint32_t y)
{
	if(!CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::SetInitCeil("<<x<<","<<y<<"): pointer to CurrentWorksheet is invalid!\n";
		return;
	}
	CurrentWorksheet->CellX=x;
	CurrentWorksheet->CellY=y;
	CurrentWorksheet->XBorder=x;
	CurrentWorksheet->YBorder=y;
}

void ROOTOpenXLSX::Close()
{
	if(ForWrite)
	{
		doc->save();
		doc->close();
	}
	ForWrite=false;
}

ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, double value)
{
	tx.WriteCeil(tx.CurrentWorksheet->CellY,tx.CurrentWorksheet->CellX,value);
	tx.CurrentWorksheet->CellX++;
	return tx;
}
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, float value)
{
	tx.WriteCeil(tx.CurrentWorksheet->CellY,tx.CurrentWorksheet->CellX,value);
	tx.CurrentWorksheet->CellX++;
	return tx;
}
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, int value)
{
	tx.WriteCeil(tx.CurrentWorksheet->CellY,tx.CurrentWorksheet->CellX,value);
	tx.CurrentWorksheet->CellX++;
	return tx;
}
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx,long unsigned int value)
{
	tx.WriteCeil(tx.CurrentWorksheet->CellY,tx.CurrentWorksheet->CellX,value);
	tx.CurrentWorksheet->CellX++;
	return tx;
}
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, char value)
{
	if(value=='\n')
	{
		tx.CurrentWorksheet->CellY++;
		tx.CurrentWorksheet->CellX=tx.CurrentWorksheet->XBorder;
	}
	else
	{
		string s;
		s+=value;
		tx.WriteCeil(tx.CurrentWorksheet->CellY,tx.CurrentWorksheet->CellX,value);
		tx.CurrentWorksheet->CellX++;
	}
	return tx;
}
ROOTOpenXLSX &operator << (ROOTOpenXLSX &tx, string value)
{
	if(value=="\n")
	{
		tx.CurrentWorksheet->CellY++;
		tx.CurrentWorksheet->CellX=tx.CurrentWorksheet->XBorder;
	}
	else
	{
		tx.WriteCeil(tx.CurrentWorksheet->CellY,tx.CurrentWorksheet->CellX,value);
		tx.CurrentWorksheet->CellX++;
	}
	return tx;
}
int ROOTOpenXLSX::NRows()
{
	if(!CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::NRows(): pointer to CurrentWorksheet is invalid!\n";
		return 0;
	}
	return CurrentWorksheet->ws.rowCount();
}

int ROOTOpenXLSX::NColumns()
{
	if(!CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::NColumns(): pointer to CurrentWorksheet is invalid!\n";
		return 0;
	}
	return CurrentWorksheet->ws.columnCount();
}

void ROOTOpenXLSX::SelectRow(int rn)
{
	if(!CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::SelectRow("<<rn<<"): pointer to CurrentWorksheet is invalid!\n";
		return ;
	}
	CurrentWorksheet->CellY=rn+1;
	CurrentWorksheet->CellX=CurrentWorksheet->XBorder;
	
}

ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, double &value)
{
	if(!tx.CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::operator >>: pointer to CurrentWorksheet is invalid!\n";
	}
	int CellX=tx.CurrentWorksheet->CellX;
	int CellY=tx.CurrentWorksheet->CellY;
	//cout<<"t:double\n";
	//cout<<"Cell: ("<<CellX<<","<<CellY<<"): "<<tx.CurrentWorksheet->ws.cell(CellY,CellX).value().get<std::string>()<<" double \n";
	if(CellX<tx.NColumns())
	tx.CurrentWorksheet->CellX++;
	if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type()!=OpenXLSX::XLValueType::Empty && ( tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Float || tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Integer))
	{
		if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Integer)
		{
			int val=tx.CurrentWorksheet->ws.cell(CellY,CellX).value();
			value=double(val);
		}
		else
		{
			value=tx.CurrentWorksheet->ws.cell(CellY,CellX).value();
		}
	}
	return tx;
}
ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, float &value)
{
	if(!tx.CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::operator >>: pointer to CurrentWorksheet is invalid!\n";
	}
	int CellX=tx.CurrentWorksheet->CellX;
	int CellY=tx.CurrentWorksheet->CellY;
	//cout<<"t:float\n";
	if(CellX<tx.NColumns())
	tx.CurrentWorksheet->CellX++;
	//cout<<"Cell: ("<<CellX<<","<<CellY<<"): "<<tx.CurrentWorksheet->ws.cell(CellY,CellX).value().get<std::string>()<<" float \n";
	if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type()!=OpenXLSX::XLValueType::Empty && ( tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Float || tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Integer))
	{
		if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Integer)
		{
			int val=tx.CurrentWorksheet->ws.cell(CellY,CellX).value();
			value=float(val);
		}
		else
		{
			value=tx.CurrentWorksheet->ws.cell(CellY,CellX).value();
		}
	}
	return tx;
}
ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, int &value)
{
	if(!tx.CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::operator >>: pointer to CurrentWorksheet is invalid!\n";
	}
	int CellX=tx.CurrentWorksheet->CellX;
	int CellY=tx.CurrentWorksheet->CellY;
	//cout<<"t:int\n";
	//cout<<"Cell: ("<<CellX<<","<<CellY<<"): "<<tx.CurrentWorksheet->ws.cell(CellY,CellX).value().get<std::string>()<<" int \n";
	if(CellX<tx.NColumns())
	tx.CurrentWorksheet->CellX++;
	if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type()!=OpenXLSX::XLValueType::Empty && tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Integer)
	{
		if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Float)
		{
			float val=tx.CurrentWorksheet->ws.cell(CellY,CellX).value();
			value=int(val);
		}
		else
		{
			value=tx.CurrentWorksheet->ws.cell(CellY,CellX).value();
		}
	}
	return tx;
}
ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, unsigned int &value)
{
	if(!tx.CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::operator >>: pointer to CurrentWorksheet is invalid!\n";
	}
	int CellX=tx.CurrentWorksheet->CellX;
	int CellY=tx.CurrentWorksheet->CellY;
	//cout<<"t:uint\n";
	//cout<<"Cell: ("<<CellX<<","<<CellY<<"): "<<tx.CurrentWorksheet->ws.cell(CellY,CellX).value().get<std::string>()<<" unsigned int \n";
	if(CellX<tx.NColumns())
	tx.CurrentWorksheet->CellX++;
	if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type()!=OpenXLSX::XLValueType::Empty && tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Integer)
	{
		if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Float)
		{
			float val=tx.CurrentWorksheet->ws.cell(CellY,CellX).value();
			value=uint(val);
		}
		else
		{
			value=tx.CurrentWorksheet->ws.cell(CellY,CellX).value();
		}
	}
	return tx;
}

ROOTOpenXLSX &operator >> (ROOTOpenXLSX &tx, string &value)
{
	if(!tx.CurrentWorksheet)
	{
		cout<<"This is ROOTOpenXLSX::operator >>: pointer to CurrentWorksheet is invalid!\n";
	}
	int CellX=tx.CurrentWorksheet->CellX;
	int CellY=tx.CurrentWorksheet->CellY;
	if(CellX<tx.NColumns())
	tx.CurrentWorksheet->CellX++;
	//cout<<"t:string\n";
	//cout<<"Cell: ("<<CellX<<","<<CellY<<"): "<<tx.CurrentWorksheet->ws.cell(CellY,CellX).value().get<std::string>()<<" string \n";
	if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type()!=OpenXLSX::XLValueType::Empty)
	{
		if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Float)
		{
			float val=tx.CurrentWorksheet->ws.cell(CellY,CellX).value();
			value=to_string(val);
		}
		else if(tx.CurrentWorksheet->ws.cell(CellY,CellX).value().type() == OpenXLSX::XLValueType::Integer)
		{
			int val=tx.CurrentWorksheet->ws.cell(CellY,CellX).value();
			value=to_string(val);
		}
		else
		{
			value=tx.CurrentWorksheet->ws.cell(CellY,CellX).value().get<std::string>();
		}
	}
	return tx;
}
