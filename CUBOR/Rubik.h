#include <iostream>
#include <vector>
#include "Cubo.h"

using namespace std;
vector<float> colore{
    0.1,0.1,0.1,
};
struct Rubik{

    vector<Cubo> tempcub;
    vector<Cubo> cubos;
    vector<Cubo*> U;
    vector<Cubo*> F;
    vector<Cubo*> B;
    vector<Cubo*> L;
    vector<Cubo*> R;
    vector<Cubo*> D;

    vector<int> change;
    vector<Cubo*> CR;
    vector<Cubo*> CL;
    ///
    float animation_vel = 3;//2
    float reg_grados = 0;
    float cant_fps = 90 / animation_vel;
    /// solver 
    int contador_solucion = -1;
    string actual_animation = "N";

    Rubik(){

        U.reserve(9);
        F.reserve(9);
        B.reserve(9);
        L.reserve(9);
        R.reserve(9);
        D.reserve(9);

        cubos.reserve(27);
        for(float i=3;i!=-3;i-=2){
            for(float j=3;j!=-3;j-=2){
                for(float k=-3;k!=3;k+=2){

                    Cubo *ptr_c=new Cubo({i-2,j-2,k+2},{i,j,k});
                    cubos.push_back(*ptr_c);
                }
            }   
        }
        actualizar_caras_pos();


        vector<float> tempo = get_texture_coord(0.3, 0.5);//0.33
        //pintar verde cara 0
        for (int i = 0; i < 3; ++i) {
            int p_cubo = 2 + (i * 3);
            int p_textura = 12 * i;
       
            cubos[p_cubo].cambiarcolor_cara(0, 0, 1, 0);
            cubos[p_cubo].change_textura_cara(0, tempo[p_textura], tempo[p_textura + 1], tempo[p_textura + 2], tempo[p_textura + 3]);
            //
            cubos[p_cubo + 9].cambiarcolor_cara(0, 0, 1, 0);
            cubos[p_cubo + 9].change_textura_cara(0, tempo[p_textura + 4], tempo[p_textura + 5], tempo[p_textura + 6], tempo[p_textura + 7]);
            //
            cubos[p_cubo + 18].cambiarcolor_cara(0, 0, 1, 0);
            cubos[p_cubo + 18].change_textura_cara(0, tempo[p_textura + 8], tempo[p_textura + 9], tempo[p_textura + 10], tempo[p_textura + 11]);
            //
        }
        //
        //pintar azul cara 1
        tempo = get_texture_coord(0.0, 0.15);// 0.0 0.1666
        for (int i = 0; i < 3; ++i) {
            int p_cubo = 18 + (i * 3);// patron 
            int p_t = 12 * i;
            cubos[p_cubo].cambiarcolor_cara(1, 0, 0, 1);
            cubos[p_cubo].change_textura_cara(1, tempo[p_t], tempo[p_t + 1], tempo[p_t + 2], tempo[p_t + 3]);
            cubos[p_cubo - 9].cambiarcolor_cara(1, 0, 0, 1);
            cubos[p_cubo - 9].change_textura_cara(1, tempo[p_t + 4], tempo[p_t + 5], tempo[p_t + 6], tempo[p_t + 7]);
            cubos[p_cubo - 18].cambiarcolor_cara(1, 0, 0, 1);
            cubos[p_cubo - 18].change_textura_cara(1, tempo[p_t + 8], tempo[p_t + 9], tempo[p_t + 10], tempo[p_t + 11]);
        }
        //pintar amarillo cara 2
        tempo = get_texture_coord(0.166, 0.33);//0.166 0.33
        for (int i = 0; i < 9; ++i) {
            int p_t = 4 * i;
            cubos[i].cambiarcolor_cara(2, 1, 1, 0);
            cubos[i].change_textura_cara(2, tempo[p_t], tempo[p_t + 1], tempo[p_t + 2], tempo[p_t + 3]);
        }
        //pintar naranja cara 3
        tempo = get_texture_coord(0.50, 0.66);
        for (int i = 0; i < 3; ++i) {
            int p_cubo = 20 + (i * 3);
            int p_t = 12 * i;
            cubos[p_cubo].cambiarcolor_cara(3, 1, 0.5, 0);
            cubos[p_cubo].change_textura_cara(3, tempo[p_t], tempo[p_t + 1], tempo[p_t + 2], tempo[p_t + 3]);
            //
            cubos[p_cubo - 1].cambiarcolor_cara(3, 1, 0.5, 0);
            cubos[p_cubo - 1].change_textura_cara(3, tempo[p_t + 4], tempo[p_t + 5], tempo[p_t + 6], tempo[p_t + 7]);
            //
            cubos[p_cubo - 2].cambiarcolor_cara(3, 1, 0.5, 0);
            cubos[p_cubo - 2].change_textura_cara(3, tempo[p_t + 8], tempo[p_t + 9], tempo[p_t + 10], tempo[p_t + 11]);

        }

        //pintar rojo cara 4

        tempo = get_texture_coord(0.166, 0.33);
        for (int i = 0; i < 3; ++i) {
            int p_cubo = 20 - i;
            int p_t = 12 * i;

            cubos[p_cubo].cambiarcolor_cara(4, 1, 0, 0);
            cubos[p_cubo].change_textura_cara(4, tempo[p_t], tempo[p_t + 1], tempo[p_t + 2], tempo[p_t + 3]);
            cubos[p_cubo - 9].cambiarcolor_cara(4, 1, 0, 0);
            cubos[p_cubo - 9].change_textura_cara(4, tempo[p_t + 4], tempo[p_t + 5], tempo[p_t + 6], tempo[p_t + 7]);
            cubos[p_cubo - 18].cambiarcolor_cara(4, 1, 0, 0);
            cubos[p_cubo - 18].change_textura_cara(4, tempo[p_t + 8], tempo[p_t + 9], tempo[p_t + 10], tempo[p_t + 11]);
        }
        //pintar blanco cara 5

        tempo = get_texture_coord(0.825, 1);
        for (int i = 0; i < 3; ++i) {
            int p_cubo = 24 + i;
            int p_t = 12 * i;

            cubos[p_cubo].cambiarcolor_cara(5, 1, 1, 1);
            cubos[p_cubo].change_textura_cara(5, tempo[p_t], tempo[p_t + 1], tempo[p_t + 2], tempo[p_t + 3]);
            cubos[p_cubo - 9].cambiarcolor_cara(5, 1, 1, 1);
            cubos[p_cubo - 9].change_textura_cara(5, tempo[p_t + 4], tempo[p_t + 5], tempo[p_t + 6], tempo[p_t + 7]);
            cubos[p_cubo - 18].cambiarcolor_cara(5, 1, 1, 1);
            cubos[p_cubo - 18].change_textura_cara(5, tempo[p_t + 8], tempo[p_t + 9], tempo[p_t + 10], tempo[p_t + 11]);
        }

        cant_fps = 90 / animation_vel;
        actualizar_caras_pos();

    }
    ///
    vector<float> get_texture_coord(float min_x, float max_x) {
        vector<float> result;
        result.reserve(18);
        float x_temp = max_x - min_x;
        float x_pasos = x_temp / 3;
        float y_pasos = 1.0 / 3.0;

        for (int j = 0; j < 3; ++j) {
            for (int i = 0; i < 3; ++i) {
                result.push_back((x_pasos * i) + min_x);
                result.push_back(1.0 - (y_pasos * (j + 1)));
                result.push_back((x_pasos * (i + 1)) + min_x);
                result.push_back((1.0) - (y_pasos * j));

            }

        }
        return result;
    }
    ///


    void rot_x(float grados) {
        for (int i = 0; i < 27; ++i) {
            cubos[i].rot_x(grados);
        }
    }
    void rot_y(float grados) {
        for (int i = 0; i < 27; ++i) {
            cubos[i].rot_y(grados);
        }
    }
    void rot_z(float grados) {
        for (int i = 0; i < 27; ++i) {
            cubos[i].rot_z(grados);
        }
    }

    void escalar() {
        for (int i = 0; i < 27; ++i) {
          //  cout << "gg2" << endl;
            cubos[i].escala(0.99, 0.99, 0.99);
          //  cout << "terminado gg2" << endl;
        }
    }
    void masescalar() {
        for (int i = 0; i < 27; ++i) {
            //  cout << "gg2" << endl;
            cubos[i].escala(45, 45, 45);
            //  cout << "terminado gg2" << endl;
        }
    }

    void cambiarcolor_cubo(int pos, float r, float g, float b) {
        for (int i = 0; i < 6; ++i) {
            cubos[pos].cambiarcolor_cara(i, r, g, b);
        }
    }
    void cambiarcolor_cara(int num_cubo, int num_cara, float r, float g, float b) {
        cubos[num_cubo].cambiarcolor_cara(num_cara, r, g, b);
        cout << "ite:" << num_cubo << ";" << num_cara << ";" << r << ";" << g << ";" << b << endl;
    }
    ///
    void draw(GLFWwindow* window, bool wired = false) {
        for (int i = 0; i < 27; ++i) {
            cubos[i].draw(window, wired);
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void actualizar_caras_pos() {
        L.clear();
        U.clear();
        F.clear();
        B.clear();
        L.clear();
        R.clear();
        D.clear();
        
        for (int i = 0; i < 27; ++i) {
            cubos[i].actualizar_ejes();//hallar min-max todos los cubos
        }
        //X
        for (int i = 0; i < 27; ++i) {
            vector<float> new_eje_x = cubos[i].res_x();//copio los min y max del eje X
            if (new_eje_x[1] > 2.8) {                   // trabajaremos por camadas en este caso el eje X y
                R.push_back(&cubos[i]);                 //comparamos los maximos son + mayores a 2.8 -> hacenos un push back en nuestro vector de lado derecha
            }
            if (new_eje_x[0] < -2.8) {
                L.push_back(&cubos[i]);                 // izquierda
            }
        }
        //Y
        for (int i = 0; i < 27; ++i) {
            vector<float> new_eje_y = cubos[i].res_y();
            if (new_eje_y[1] > 2.8) {
                U.push_back(&cubos[i]);
            }
            if (new_eje_y[0] < -2.8) {
                D.push_back(&cubos[i]);
            }

        }
        //Z
        for (int i = 0; i < 27; ++i) {
            vector<float> new_eje_z = cubos[i].res_z();
            if (new_eje_z[1] > 2.8) {
                B.push_back(&cubos[i]);
            }
            if (new_eje_z[0] < -2.8) {
                F.push_back(&cubos[i]);
            }
        }
    }
    //////////////////////////////
    string animation_F(int veces = 1) {
       
        if (reg_grados == 0) {
            for (int i = 0; i < 9; ++i) {
                F[i]->trasladar(0, 0, -1);
            }
        }
        if (reg_grados != cant_fps * veces) {
            
            for (int i = 0; i < 9; ++i) {
                F[i]->cambiarcolor_cara(1, 0, 1, 1);
                F[i]->rot_z(-animation_vel);
            }

            reg_grados++;
            return "F";
        }
        reg_grados = 0;
        for (int i = 0; i < 9; ++i) {
            F[i]->cambiarcolor_cara(1, 0, 0, 1);
            F[i]->trasladar(0, 0, 1);            
        }
       // escalar();
        actualizar_caras_pos();
        return "N";
        
    }
    string animation_R(int veces = 1) {
        if (reg_grados == 0) {
            for (int i = 0; i < 9; ++i) {
                R[i]->trasladar(1, 0, 0);
            }
        }
        if (reg_grados != cant_fps * veces) {
            for (int i = 0; i < 9; ++i) {
                R[i]->cambiarcolor_cara(2, 0, 1, 1);
                R[i]->rot_x(animation_vel);
            }
            reg_grados++;
            return "R";
        }

        reg_grados = 0;
        for (int i = 0; i < 9; ++i) {
            
            R[i]->cambiarcolor_cara(2, 1, 1, 0);
            R[i]->trasladar(-1, 0, 0);
        }
        actualizar_caras_pos();
        return "N";
    }
    string animation_U(int veces = 1) {
        if (reg_grados == 0) {
            for (int i = 0; i < 9; ++i) {
               U[i]->trasladar(0, 1, 0);
            }
        }
        if (reg_grados != cant_fps * veces) {
            for (int i = 0; i < 9; ++i) {
                U[i]->cambiarcolor_cara(4, 0, 1, 1);
                U[i]->rot_y(animation_vel);
            }
            reg_grados++;
            return "U";
        }

        reg_grados = 0;
        for (int i = 0; i < 9; ++i) {
            U[i]->cambiarcolor_cara(4, 1, 0, 0);
            U[i]->trasladar(0, -1, 0);
        }
            
        actualizar_caras_pos();
        return "N";
    }
    ///
    string animation_B(int veces = 1) {
        if (reg_grados == 0) {
            for (int i = 0; i < 9; ++i) {
                B[i]->trasladar(0, 0, 1);
            }
        }
        if (reg_grados != cant_fps * veces) {
            for (int i = 0; i < 9; ++i) {
                B[i]->cambiarcolor_cara(0, 0, 1, 1);
                B[i]->rot_z(animation_vel);
            }
            reg_grados++;
            return "B";
        }
        reg_grados = 0;
        for (int i = 0; i < 9; ++i)
        {
            B[i]->cambiarcolor_cara(0, 0, 1, 0);
            B[i]->trasladar(0, 0, -1);
        }
        actualizar_caras_pos();
        return "N";
    }
    ///
    string animation_L(int veces = 1) {
        if (reg_grados == 0) {
            for (int i = 0; i < 9; ++i) {
                L[i]->trasladar(-1, 0, 0);
            }
        }
        if (reg_grados != cant_fps * veces) {
            for (int i = 0; i < 9; ++i) {
                L[i]->cambiarcolor_cara(3, 0, 1, 1);
                L[i]->rot_x(-animation_vel);
            }
            reg_grados++;
            return "L";
        }
        reg_grados = 0;
        for (int i = 0; i < 9; ++i)
        {
            L[i]->cambiarcolor_cara(3, 1, 0.5, 0);
            L[i]->trasladar(1, 0, 0);
        }

        actualizar_caras_pos();
        return "N";
    }
    ///
    string animation_D(int veces = 1) {
        if (reg_grados == 0) {
            for (int i = 0; i < 9; ++i) {
                D[i]->trasladar(0, -1, 0);
            }
        }
        if (reg_grados != cant_fps * veces) {
            for (int i = 0; i < 9; ++i) {
                D[i]->cambiarcolor_cara(5, 0, 1, 1);
                D[i]->rot_y(-animation_vel);
            }
            reg_grados++;
            return "D";
        }
        reg_grados = 0;
        for (int i = 0; i < 9; ++i)
        {
            D[i]->cambiarcolor_cara(5, 1, 1, 1);
            D[i]->trasladar(0, 1, 0);
        }

        actualizar_caras_pos();
        return "N";
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    string aplicar_solucion(vector<string>& sol) {
        if (actual_animation == "N") {
            cout<<"contador_solucion "<<contador_solucion<<endl;
            contador_solucion++;
            //escalar();
            cout << "manin" << endl;
            if (contador_solucion == (sol.size())) {
                contador_solucion = -1;
                sol.clear();
                
                return "M";

            }/*
            if (contador_solucion % 2 == 0){
                escalar();
            }*/
        }
        actual_animation = sol[contador_solucion];

        if (actual_animation == "R") {
            actual_animation = animation_R();
            return "A";
        }
        if (actual_animation == "R2") {
            actual_animation = animation_R(2);
            return "A";
        }
        if (actual_animation == "R'") {
            actual_animation = animation_R(3);
            return "A";
        }
        //================================================
        if (actual_animation == "U") {
            actual_animation = animation_U();
            return "A";
        }
        if (actual_animation == "U2") {
            actual_animation = animation_U(2);
            return "A";
        }
        if (actual_animation == "U'") {
            actual_animation = animation_U(3);
            return "A";
        }
        //================================================
        if (actual_animation == "D") {
            actual_animation = animation_D();
            return "A";
        }
        if (actual_animation == "D2") {
            actual_animation = animation_D(2);
            return "A";
        }
        if (actual_animation == "D'") {
            actual_animation = animation_D(3);
            return "A";
        }
        //================================================
        if (actual_animation == "F") {
            actual_animation = animation_F();
            return "A";    
        }
        if (actual_animation == "F2") {
            actual_animation = animation_F(2);
            return "A";
        }
        if (actual_animation == "F'") {
            actual_animation = animation_F(3);
            return "A"; 
        }
        //================================================
        if (actual_animation == "B") {
            actual_animation = animation_B();
            return "A";
        }
        if (actual_animation == "B2") {
            actual_animation = animation_B(2);
            return "A";
        }
        if (actual_animation == "B'") {
            actual_animation = animation_B(3);
            return "A";
        }
        //================================================
        if (actual_animation == "L") {
            actual_animation = animation_L();
            return "A";
        }
        if (actual_animation == "L2") {
            actual_animation = animation_L(2);
            return "A";
        }
        if (actual_animation == "L'") {
            actual_animation = animation_L(3);
            return "A";
        }
        return "A";

    }
    int contescala = 0;
    string bucle() {
        
       // cout << "entraste" << endl;
        if (contescala < 400) {

            escalar();
           // cout << "habla" << endl;
            contescala++;
            return "M";
        } 
        contescala = 0;
        masescalar();
        return "N";
    }
};