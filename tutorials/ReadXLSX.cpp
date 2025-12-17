#include "ROOTOpenXLSXLib.hh"

void ReadXLSX()
{
	ROOTOpenXLSX x;
	x.Open("C.xlsx");
	x.GoToWorksheet("Gammas");
	string header;
	vector<double> E_g_v,E_i_v,E_f_v,CS_v;
	vector<string> JPi_v,JPf_v,Targ_v,Reac_v,Prod_v;
	//for(int i=0;i<x.NRows();i++)
	for(int i=0;i<x.NRows();i++)
	{
		if(i==0)
		{
			for(int j=0;j<x.NColumns();j++)
			{
				cout<<"i,j: "<<i<<" "<<j<<" "<<x.NRows()<<" "<<x.NColumns()<<"\n";
				
				{
					string tmp;
					x>>tmp;
					header+=tmp+"\t";
				}
			}
		}
		else
		{
			double E_g,E_i,E_f,CS;
			string JPi,JPf,Targ,Reac,Prod;
			x>>E_g>>E_i>>JPi>>E_f>>JPf>>Targ>>Reac>>Prod>>CS;
			E_g_v.push_back(E_g);
			E_i_v.push_back(E_i);
			E_f_v.push_back(E_f);
			CS_v.push_back(CS);
			JPi_v.push_back(JPi);
			JPf_v.push_back(JPf);
			Targ_v.push_back(Targ);
			Reac_v.push_back(Reac);
			Prod_v.push_back(Prod);
		}
		x<<"\n";
	}
	cout<<header<<"\n";
	for(unsigned int i=0;i<E_g_v.size();i++)
	{
		cout<<E_g_v[i]<<"\t"<<E_i_v[i]<<"\t"<<JPi_v[i]<<"\t"<<E_f_v[i]<<"\t"<<JPf_v[i]<<"\t"<<Targ_v[i]<<"\t"<<Reac_v[i]<<"\t"<<Prod_v[i]<<"\t"<<CS_v[i]<<"\n";
	}
	x.Close();
}
