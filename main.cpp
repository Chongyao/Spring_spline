#include <iostream>
#include<sstream>
#include"halfedge.h"
#include"simplifymesh.h"
#define iter_times_default 500
using namespace std;
void read_argv( int argc, char** argv,string &input_, string &output_, size_t &itertime);
int main(int argc,char* argv[])
{
    halfedge data;
    string input_="../data/input/",
        output_ = "../data/output/";
    size_t itertime = 0 ;
    read_argv(argc-1,argv+1,input_,output_,itertime);

    
    
    if( data.read_data(input_) != -1){
    data.construct_half_edges();
    simplify_mesh data_simp(data);
    data_simp.simp_shorstest(itertime,output_);
    output_.append("last.obj");
    data_simp.mesh_init. halfedge_to_obj(output_);
    }
    else
      cout << "read error";
    return 0;
}



void read_argv( int argc, char** argv,string &input_, string &output_, size_t &itertime){
  for (size_t i = 0; i < argc ;i++){
    string keyword = argv[i];
    if( keyword.find("infile") != -1){
      input_.append(keyword.begin()+7,keyword.end());
      input_.append(".obj");
      
    }
    else if (keyword.find("outfile") != -1){
      output_.append(keyword.begin()+8,keyword.end());
      output_.append("/");
      output_.append(keyword.begin()+8,keyword.end());
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


