#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "Matrices.h"
using namespace std;

// ---------------------------------------------------------------------------------------------
struct Polygono{
    /// Ejes
    vector<float> eje_x;
    vector<float> eje_y;
    vector<float> eje_z;
    /// 2D
    vector<float> vertices;
    vector<float> bordes;
    vector<float> directions;
    float polygon_color_R=0,polygon_color_G=0,polygon_color_B=0;
    float vel=2;
    /// cubos
    int points_size = 0;
    int size_bits_polygon = 0;
    /// 
   
    Polygono(vector<float> temp_vertices,float R,float G,float B){
        eje_x.reserve(2);
        eje_y.reserve(2);

        eje_z.reserve(2);
        eje_x.resize(2);

        eje_y.resize(2);
        eje_z.resize(2);

        vertices.reserve(temp_vertices.size()+((temp_vertices.size()/3)*5));

        for(int j=1;j<=temp_vertices.size();++j){
			vertices.push_back(temp_vertices[j-1]);
            bordes.push_back(temp_vertices[j - 1]);
			//RGB
			if(j%3==0){
				vertices.push_back(R);
				vertices.push_back(G);
				vertices.push_back(B);
                //Texturas
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }
        points_size = vertices.size() / 8;
        bordes = vertices;
        for (int i = 0; i < points_size; i++)
        {
            int pos = i * 8;
            bordes[pos + 3] = 0;
            bordes[pos + 4] = 0;
            bordes[pos + 5] = 0;
        }

        //directions.reserve((temp_vertices.size()/3)*2);
        //get_all_directions();
        size_bits_polygon = vertices.size() * sizeof(vertices);
        min_max_x();
        min_max_y();
        min_max_z();
        
    }
    //cambiar color
    void cambiarcolor(float R,float G,float B){
        for (int i = 0; i < points_size; i++)
        {
            int pos = i * 8;
            vertices[pos + 3] = R;
            vertices[pos + 4] = G;
            vertices[pos + 5] = B;
        }
    }
    void draw(GLFWwindow* window,bool wired=false){
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size()* sizeof(vertices), this->vertices.data(), GL_STATIC_DRAW);
        if(wired)
            glDrawArrays(GL_LINE_LOOP, 0, vertices.size()/8);   
        else
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size()/8);

        glBufferData(GL_ARRAY_BUFFER, this->bordes.size() * sizeof(bordes), this->bordes.data(), GL_STATIC_DRAW);
        glLineWidth(10);//grosor
      //  glDrawArrays(GL_LINE_LOOP, 0, bordes.size() / 8);
    }

    ///traslaciones
    void trans(float new_x, float new_y, float new_z) {
        for (int i = 0; i < points_size; i++) {
            int pos = i * 8;
            vertices[pos] += new_x;
            vertices[pos + 1] += new_y;
            vertices[pos + 2] += new_z;
            bordes[pos] += new_x;
            bordes[pos + 1] += new_y;
            bordes[pos + 2] += new_z;
        }
    }
    void pro_traslate(vector<float> point){
        Matriz m1(4,4);
        Matriz m2(vertices);
        Matriz m3(4,4);
        m1.create_matrix_point(point);
        Multiply(m1,m2,m3);
        m3.export_to_vertex(vertices);
    }
    ///rotaciones 
    void pro_rotation_x(float angle){
        Matriz m1(4,4);
        Matriz m2(vertices);
        Matriz m3(4,(vertices.size()/6));

        m1.convert_matrix_rotation_x(angle);
        Multiply(m1,m2,m3);
        m3.export_to_vertex(vertices);
        m3.export_to_vertex(bordes);
    }
 
    void pro_rotation_y(float angle){
        Matriz m1(4,4);
        Matriz m2(vertices);
        Matriz m3(4,(vertices.size()/6));
        
        m1.convert_matrix_rotation_y(angle);
        Multiply(m1,m2,m3);
        m3.export_to_vertex(vertices);
        m3.export_to_vertex(bordes);
    }

    void pro_rotation_z(float angle) {
        Matriz m1(4, 4);
        Matriz m2(vertices);
        Matriz m3(4, (vertices.size() / 6));

        m1.convert_matrix_rotation_z(angle);
        Multiply(m1, m2, m3);
        m3.export_to_vertex(vertices);
        m3.export_to_vertex(bordes);

    }
    ///escalados
    void escalao(float x, float y, float z) {
    //    cout << "gg0" << endl;
        Matriz m1(4, 4);
        Matriz m2(vertices);
        Matriz m3(4, (vertices.size()));

        m1.convert_matrix_scale(x, y, z);
        Multiply(m1, m2, m3);
        m3.export_to_vertex(vertices);
    }
    void escalaomas(float x, float y, float z) {
        Matriz m1(4, 4);
        Matriz m2(vertices);
        Matriz m3(4, points_size);

        m1.convert_matrix_scale(x, y, z);
        //m1.print_matrix();
        Multiply(m1, m2, m3);
        m3.print_matrix();
        m3.export_to_vertex(vertices);
    }
    void escalaomenos(float x, float y, float z) {
        Matriz m1(4, 4);
        Matriz m2(vertices);
        Matriz m3(4, points_size);

        m1.convert_matrix_scale(x, y, z);
        //m1.print_matrix();
        Multiply(m1, m2, m3);
        m3.print_matrix();
        m3.export_to_vertex(vertices);
    }
   
    //imprimir
    void print_position(){
        char ejes[3] = { 'x', 'y', 'z' };
        for (int i = 0; i < points_size; i++)
        {
            int pos = i * 8;
            cout << ejes[i] << ": ";
            cout << vertices[pos] << "\t";
            cout << vertices[pos + 1] << "\t";
            cout << vertices[pos + 2] << endl;
        }
        cout << "///////////////////////\n";
    }
    //////////////////////////////////////////////////////////////////////////////////////////////
    void min_max_x()
    {
        vector<float> result;
        result.reserve(2);
        result.resize(2);
        result[0] = vertices[0]; 
        result[1] = vertices[0]; 
        for (int i = 1; i < points_size; i++)
        {
            int pos = i * 8;
            if (vertices[pos] < result[0])
                result[0] = vertices[pos];
            if (vertices[pos] > result[1])
                result[1] = vertices[pos];
        }
        eje_x = result;
    }
    ///
    void min_max_y()
    {
        vector<float> result;
        result.reserve(2);
        result.resize(2);
        result[0] = vertices[1]; //min
        result[1] = vertices[1]; //max
        for (int i = 1; i < points_size; i++)
        {
            int pos = i * 8;
            if (vertices[pos + 1] < result[0])
                result[0] = vertices[pos + 1];
            if (vertices[pos + 1] > result[1])
                result[1] = vertices[pos + 1];
        }
        eje_y = result;
    }
    ///
    void min_max_z()
    {
        vector<float> result;
        result.reserve(2);
        result.resize(2);
        result[0] = vertices[2]; //min
        result[1] = vertices[2]; //max
        for (int i = 1; i < points_size; i++)
        {
            int pos = i * 8;
            if (vertices[pos + 2] < result[0])
                result[0] = vertices[pos + 2];
            if (vertices[pos + 2] > result[1])
                result[1] = vertices[pos + 2];
        }
        eje_z = result;
    }
    void actualizar_ejes()
    {
        min_max_x();
        min_max_y();
        min_max_z();
    }
    ///
    ///
    void change_texture_pos(float min_x, float min_y, float max_x, float max_y)
    {
        vertices[6] = min_x;
        vertices[7] = min_y;
        vertices[14] = min_x;
        vertices[15] = max_y;
        vertices[22] = max_x;
        vertices[23] = max_y;
        vertices[30] = max_x;
        vertices[31] = min_y;
    }
};