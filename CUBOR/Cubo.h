#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Polygon.h"
using namespace std;
typedef vector<float> vec;

void TAPAS(vec& LADO_4, vec& LADO_5, vec P1, vec P2) {
    /// 
    ///
    /// tapas 4 - 5 (tapas arriba y abajo)
    /// 
    //tapa 4
    LADO_4.push_back(P1[0]); LADO_4.push_back(P2[1]); LADO_4.push_back(P2[2]);
    LADO_4.push_back(P1[0]); LADO_4.push_back(P2[1]); LADO_4.push_back(P1[2]);
    LADO_4.push_back(P2[0]); LADO_4.push_back(P2[1]); LADO_4.push_back(P1[2]);
    LADO_4.push_back(P2[0]); LADO_4.push_back(P2[1]); LADO_4.push_back(P2[2]);
    //tapa 5
    LADO_5.push_back(P1[0]); LADO_5.push_back(P1[1]); LADO_5.push_back(P1[2]);
    LADO_5.push_back(P1[0]); LADO_5.push_back(P1[1]); LADO_5.push_back(P2[2]);
    LADO_5.push_back(P2[0]); LADO_5.push_back(P1[1]); LADO_5.push_back(P2[2]);
    LADO_5.push_back(P2[0]); LADO_5.push_back(P1[1]); LADO_5.push_back(P1[2]);
}

void LADOS(vec& LADO_0, vec& LADO_1, vec& LADO_2, vec& LADO_3, vec P1, vec P2) {
    /// 
    /// Lados 0 - 1 (tapas frente y atras)
    /// 

    // lado 0
    LADO_0.push_back(P2[0]); LADO_0.push_back(P1[1]); LADO_0.push_back(P1[2]);
    LADO_0.push_back(P2[0]); LADO_0.push_back(P2[1]); LADO_0.push_back(P1[2]);
    LADO_0.push_back(P1[0]); LADO_0.push_back(P2[1]); LADO_0.push_back(P1[2]);
    LADO_0.push_back(P1[0]); LADO_0.push_back(P1[1]); LADO_0.push_back(P1[2]);
    // lado 1
    LADO_1.push_back(P1[0]); LADO_1.push_back(P1[1]); LADO_1.push_back(P2[2]);
    LADO_1.push_back(P1[0]); LADO_1.push_back(P2[1]); LADO_1.push_back(P2[2]);
    LADO_1.push_back(P2[0]); LADO_1.push_back(P2[1]); LADO_1.push_back(P2[2]);
    LADO_1.push_back(P2[0]); LADO_1.push_back(P1[1]); LADO_1.push_back(P2[2]);
 

    ///
    /// Lados 2 - 3 (tapas izquierda y derecha)
    /// 
 
    /// lado 2
    LADO_2.push_back(P2[0]); LADO_2.push_back(P1[1]); LADO_2.push_back(P2[2]);
    LADO_2.push_back(P2[0]); LADO_2.push_back(P2[1]); LADO_2.push_back(P2[2]);
    LADO_2.push_back(P2[0]); LADO_2.push_back(P2[1]); LADO_2.push_back(P1[2]);
    LADO_2.push_back(P2[0]); LADO_2.push_back(P1[1]); LADO_2.push_back(P1[2]);
    ///  lado 3
    LADO_3.push_back(P1[0]); LADO_3.push_back(P1[1]); LADO_3.push_back(P1[2]);
    LADO_3.push_back(P1[0]); LADO_3.push_back(P2[1]); LADO_3.push_back(P1[2]);
    LADO_3.push_back(P1[0]); LADO_3.push_back(P2[1]); LADO_3.push_back(P2[2]);
    LADO_3.push_back(P1[0]); LADO_3.push_back(P1[1]); LADO_3.push_back(P2[2]);

}


struct Cubo{

    vector<Polygono> caras;
    vector<float> eje_x;
    vector<float> eje_y;
    vector<float> eje_z;

    Cubo(vec P1,vec P2){
        vec cara[6];
        caras.reserve(6);// tiene 6 cara cada rubik 
        eje_x.reserve(2);
        eje_y.reserve(2);
        eje_z.reserve(2);
        eje_x.resize(2);
        eje_y.resize(2);
        eje_z.resize(2);

        for (int w=0;w<6;++w)

            cara[w].reserve(12);
            LADOS(cara[0], cara[1], cara[2], cara[3], P1, P2);
            TAPAS(cara[4], cara[5], P1, P2);

            for (int i = 0; i < 6; ++i) {

            Polygono *ptr_poly=new Polygono(cara[i],0.512,0.512,0.512);
            caras.push_back(*ptr_poly);
        }
        actualizar_ejes();
    }
    ///
    void rot_x(float grados){
        for (int i = 0; i < 6; ++i) {
            caras[i].pro_rotation_x(grados);
        }
    }
    void rot_y(float grados) {
        for (int i = 0; i < 6; ++i) {
            caras[i].pro_rotation_y(grados);
        }
    }
    void rot_z(float grados) {
        for (int i = 0; i < 6; ++i) {
            caras[i].pro_rotation_z(grados);
        }
    }
    ///
    void trasladar(float x, float y, float z) {
        for (int i = 0; i < 6; ++i) {
            caras[i].trans(x, y, z);
        }
    }
    ///
    void cambiarcolor_cara(int position, float R, float G, float B) {
        caras[position].cambiarcolor(R, G, B);
    }
    ///
    void draw(GLFWwindow* window,bool wired=false){
        for(int i=0;i<6;++i){
            caras[i].draw(window,wired);
        }
    }
    ///
    void escala(float x, float y, float z){
        for (int i = 0; i < 6; ++i){
       //     cout << "gg1" << endl;
            caras[i].escalao(x, y, z);
       //     cout << "terminado gg1" << endl;

        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void actualizar_ejes() {

        for (int i = 0; i < 6; ++i) {
            caras[i].actualizar_ejes();
        }

        vector<float> new_x;
        vector<float> new_y;
        vector<float> new_z;

        for (int i = 0; i < 6; ++i) {
            new_x.insert(new_x.end(), caras[i].eje_x.begin(), caras[i].eje_x.end());
            new_y.insert(new_y.end(), caras[i].eje_y.begin(), caras[i].eje_y.end());
            new_z.insert(new_z.end(), caras[i].eje_z.begin(), caras[i].eje_z.end());
        }

        sort(new_x.begin(), new_x.end());
        sort(new_y.begin(), new_y.end());
        sort(new_z.begin(), new_z.end());
      
        eje_x[0] = new_x[0];//menor
        eje_x[1] = new_x[11];//mayor

        eje_y[0] = new_y[0];
        eje_y[1] = new_y[11];

        eje_z[0] = new_z[0];
        eje_z[1] = new_z[11];
    }

    ///
    vector<float> res_x() {
        return eje_x;
    }
    ///
    vector<float> res_y() {
        return eje_y;
    }
    ///
    vector<float> res_z() {
        return eje_z;
    }
    ///
    void print_min_max_axis() {
        cout << eje_x[0] << " " << eje_x[1] << endl;
        cout << eje_y[0] << " " << eje_y[1] << endl;
        cout << eje_z[0] << " " << eje_z[1] << endl;
        cout << "//////////////////////////////////" << endl;
    }
    ///
    void change_textura_cara(int position, float min_x, float min_y, float max_x, float max_y) {
        caras[position].change_texture_pos(min_x, min_y, max_x, max_y);
    }
};
