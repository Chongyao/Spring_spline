#include "halfedge.h"
#include<fstream>
#include<sstream>
#include<cmath>
#include <iostream>
#include<memory>
#include<vector>
#include<string>

using namespace std;

bool operator<(const H_edge &a, const H_edge &b)
      {
       return a.length<b.length;
      }


halfedge::halfedge()
{
}

int halfedge::ReadData(const string &InputFile){


    ifstream fin(InputFile);
    if (fin.is_open()==false)
        return -1;

    string keyword;
    fin>>keyword;


    while(!fin.eof()){  //read the data line by line
       if(keyword=="#")
            ReadAnno(fin,keyword);

        else if(keyword=="v")
           ReadVertex(fin,keyword);

        else if(keyword=="f")
            ReadFace(fin,keyword);

        else if(keyword=="vn")
           ReadAnno(fin,keyword);

        else
           cout<<"this identity is not exit";

        }


    cout<<"data is read.\n";
    fin.close();
    return 0;
}


void halfedge::ReadVertex( ifstream &fin,string &keyword){
    H_vertex vertex_temp;

    fin>>vertex_temp.x;
    fin>>vertex_temp.y;
    fin>>vertex_temp.z;
    //    vertex_temp.is_exist=true;
    //        auto pointer=make_shared<H_vertex>(vertex_temp);
    this->Vertexs.push_back(vertex_temp);

    fin>>keyword;
}

void halfedge::ReadFace(ifstream &fin,string &keyword){
    fin>>keyword;
    while(keyword != "f" && !fin.eof()){
        int vert_id; { // atof(keyword)
            keyword.erase(keyword.begin()+keyword.find("/",0),keyword.end());
            stringstream temp;
            temp << keyword;
            temp >> vert_id;
        }
        InitFaces.push_back(vert_id);

        fin>>keyword;
    }
}

void halfedge::ReadAnno(ifstream &fin, string &keyword){
    string temp;
    getline(fin,temp);
    fin>>keyword;

}
void halfedge::ConstructHalfedge(){
    //    H_face face_temp;
    //    H_edge edge_temp;
    size_t num=InitFaces.size();

    Faces=vector<H_face>(num/3);
//    for(auto it=Faces.begin();it!=Faces.end();it++)
//        *it=make_shared<H_face>();

    HalfEdges=vector<H_edge> (num);
//    for(auto it=HalfEdges.begin();it!=HalfEdges.end();it++)
//        *it=make_shared<H_edge>();

    for(size_t i=0;i<num/3;i++){
        Faces[i].edge_=i*3;
        //       Faces[i].is_exist=true;
        for(size_t k=i*3;k<(i+1)*3;k++){
            HalfEdges[k].vertex_=InitFaces[k];
            Vertexs[InitFaces[k]].edge_=k;
            //  HalfEdges[k].is_exist=true;

            HalfEdges[k].face_=i;

            if(k==i*3){
                HalfEdges[k].prev_=k+2;
                HalfEdges[k].length= sqrt(pow((Vertexs[InitFaces[k+2]].x-Vertexs[InitFaces[k]].x),2)+
                        pow((Vertexs[InitFaces[k+2]].y-Vertexs[InitFaces[k]].y),2)+
                        pow((Vertexs[InitFaces[k+2]].z-Vertexs[InitFaces[k]].z),2));
            }
            else{
                HalfEdges[k].prev_=k-1;
                HalfEdges[k].length=sqrt(pow((Vertexs[InitFaces[k-1]].x-Vertexs[InitFaces[k]].x),2)+
                        pow((Vertexs[InitFaces[k-1]].y-Vertexs[InitFaces[k]].y),2)+
                        pow((Vertexs[InitFaces[k-1]].z-Vertexs[InitFaces[k]].z),2));
            }
            if(k==i*3+2)
                HalfEdges[k].next_=k-2;
            else
                HalfEdges[k].next_=k+1;
        }
    }
    vector<bool>isFind(num);
    for(size_t i=0;i<num;++i){
        if(isFind[i]) continue;
        int next1=InitFaces[i];
        int prev1;{
            if (i % 3 == 0)
                prev1 = InitFaces[i + 2];
            else
                prev1 = InitFaces[i - 1];
        }
        for(size_t j=i+1;j<num;j++){
            if(isFind[j]) continue;
            int next2 = InitFaces[j];

            if(next2 != prev1) continue;

            int prev2;{
                if (j % 3 == 0)
                    prev2 = InitFaces[j + 2];
                else
                    prev2 = InitFaces[j - 1];
            }

            if (next1 == prev2 && next2 == prev1 ){
                isFind[i] = true;
                isFind[j] = true;
//                cout<<i<<"   "<<j<<"   \n";
                HalfEdges[i].oppo_ = j;
                HalfEdges[j].oppo_ = i;
            }
        }
        //        if(isFind[i]==false) cout<<"halfedge"<< i <<"can't find it's opposite!\n";
    }

    cout<<"the data has been converted to halfedge constructure.\n";
}

size_t halfedge::Get_edge_next(size_t edge_id)
{
    return this->HalfEdges[edge_id].next_;
}

