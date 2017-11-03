#include <iostream>
#include<sstream>
#include"halfedge.h"
#include"simplifymesh.h"
#define iter_times_default 500
using namespace std;
void read_argv( int argc, char** argv,string &Input, string &Output, size_t &itertime);
int main(int argc,char* argv[])
{
    halfedge data;
    string Input="../data/input/",
        Output = "../data/output/";
    size_t itertime = 0 ;
    read_argv(argc-1,argv+1,Input,Output,itertime);

    
    
    if( data.read_data(Input) != -1){
    data.construct_half_edges();
    simplify_mesh data_simp(data);
    data_simp.simp_shorstest(itertime,Output);
    Output.append("last.obj");
    data_simp.mesh_init. halfedge_to_obj(Output);
    }
    else
      cout << "read error";
    return 0;
}



void read_argv( int argc, char** argv,string &Input, string &Output, size_t &itertime){
  for (size_t i = 0; i < argc ;i++){
    string keyword = argv[i];
    if( keyword.find("infile") != -1){
      Input.append(keyword.begin()+7,keyword.end());
      Input.append(".obj");
      
    }
    else if (keyword.find("outfile") != -1){
      Output.append(keyword.begin()+8,keyword.end());
      Output.append("/");
      Output.append(keyword.begin()+8,keyword.end());
    }
    else if (keyword.find("itertime") != -1){
      stringstream iter_;
      keyword.erase(keyword.begin(),keyword.begin()+9);
      iter_ << keyword;
      iter_ >> itertime;
    }
    else
      cout << "error in argv \n";
  }

  if (itertime == 0)
    itertime = iter_times_default;
    
  
}


