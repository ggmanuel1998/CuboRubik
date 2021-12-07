#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
#define PI 3.14159265

typedef vector<vector<float>> matrix_type;
struct Matriz{
    matrix_type *data;
    Matriz(int x_size=4,int y_size=4){
        data=new vector<vector<float>> (x_size,vector<float>(y_size,0));
    }
    ///
    Matriz(vector<float> &data_tempo){
        data=new vector<vector<float>> (4,vector<float>(data_tempo.size()/6));
        for(int i=0;i<data_tempo.size()/ 8;i++){
            int pos=i*8;
            (*data)[0][i]=data_tempo[pos];
            (*data)[1][i]=data_tempo[pos+1];
            (*data)[2][i]=data_tempo[pos+2];
            (*data)[3][i]=1;
        }
    }
    ///
    void convert_matrix_identity(int size){
        for(int i=0;i<size;++i){
            (*data)[i][i]=1;
        }
    }
    /////
    void convert_matrix_rotation_2d_x(float angle) {
        convert_matrix_identity(3);
        (*data)[1][1] = -1;
    }
    void convert_matrix_rotation_2d_y(float angle) {
        convert_matrix_identity(3);
        (*data)[0][0] = -1;
    }
    ///////////////////////////////////////////////////////////////////////////////////////
    void convert_matrix_rotation_origin(float angle){
        convert_matrix_identity(4);
        (*data)[0][0]=cos(angle* PI / 180.0 );(*data)[0][1]=sin(angle* PI / 180.0 );
        (*data)[1][0]=-sin(angle* PI / 180.0 );(*data)[1][1]=cos(angle* PI / 180.0 );     
    }
    ///
    void convert_matrix_rotation_x(float angle){
        convert_matrix_identity(4);
        (*data)[1][1]=cos(angle* PI / 180.0 );   (*data)[1][2]=-sin(angle* PI / 180.0 );
        (*data)[2][1]=sin(angle* PI / 180.0 );  (*data)[2][2]=cos(angle* PI / 180.0 );
    }
    ///
    void convert_matrix_rotation_y(float angle){
        convert_matrix_identity(4);
        (*data)[0][0]=cos(angle* PI / 180.0 );    (*data)[0][2]=sin(angle* PI / 180.0 );    
        (*data)[2][0]=-sin(angle* PI / 180.0 );   (*data)[2][2]=cos(angle* PI / 180.0 );    
    }
    ///
    void convert_matrix_rotation_z(float angle){
        convert_matrix_identity(4);
        (*data)[0][0]=cos(angle* PI / 180.0 );           (*data)[0][1]=-sin(angle* PI / 180.0 );
        (*data)[1][0]=sin(angle* PI / 180.0 );            (*data)[1][1]=cos(angle* PI / 180.0 );
        
    }
    void convert_matrix_rotation_eje(float angle) {
        float d = cos(-angle);
        convert_matrix_identity(4);
        (*data)[1][1] = cos(angle * PI / 180.0);   (*data)[1][2] = -sin(angle * PI / 180.0);
        (*data)[2][1] = sin(angle * PI / 180.0);  (*data)[2][2] = cos(angle * PI / 180.0);
        (*data)[3][1] = sin(angle * PI / 180.0);  (*data)[3][2] = cos(angle * PI / 180.0);
    }
    ///
    void convert_matrix_traslation(float x,float y, float z){
        convert_matrix_identity(4);
        (*data)[0][3]=x;
        (*data)[1][3]=y;
        (*data)[2][3]=z;
    }
    /// escalado
    void convert_matrix_scale(float x,float y, float z){
        convert_matrix_identity(4);
        (*data)[0][0]=x;
        (*data)[1][1]=y;
        (*data)[2][2]=z;
    }
    void convert_escalado_mas() {
        convert_matrix_identity(4);
        (*data)[0][0] = 1.5;
        (*data)[1][1] = 1.5;
        (*data)[2][2] = 0.0;
    }
    void convert_escalado_menos() {
        convert_matrix_identity(4);
        (*data)[0][0] = 0.76;
        (*data)[1][1] = 0.76;
        (*data)[2][2] = 0.0;
    }
    ///
    void create_matrix_point(vector<float> p){
        convert_matrix_identity(4);
        (*data)[0][3]=p[0];
        (*data)[1][3]=p[1];
        (*data)[2][3]=p[2];
    }
    ///
    void export_to_vertex(vector<float> &vertices){
        for(int i=0;i<vertices.size()/8;i++){
            int pos=i*8;
            vertices[pos]=(*data)[0][i];
            vertices[pos+1]=(*data)[1][i];
            vertices[pos+2]=(*data)[2][i];
        }
    }
    ///
    void print_matrix(){
        for(int i=0;i<(*data).size();i++){
            for(int j=0;j<(*data)[0].size();j++){
                cout<<(*data)[i][j]<<"\t";
            }
            cout<<endl;
        
        }
        cout<<"//////////////////////////"<<endl;
    }
};
void Multiply(Matriz &a, Matriz &b,Matriz &c){
    const int n = (*a.data).size();     // a rows
    const int m = (*a.data)[0].size();  // a cols
    const int p = (*b.data)[0].size();  // b cols

    for (auto j = 0; j < p; ++j){
        for (auto k = 0; k < m; ++k){
            for (auto i = 0; i < n; ++i){
                (*c.data)[i][j] += ((*a.data)[i][k] * (*b.data)[k][j]);
            }
        }
    }
}
