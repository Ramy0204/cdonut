    #include <stdio.h>
    #include <math.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>



    const float theta_spacing = 0.7;
    const float phi_spacing   = 0.7;
    const double pi = 3.14;
    const int screen_width = 80;
    const int screen_height = 22;
    const float R1 = 2;
    const float R2 = 1;
    const float K2 = 6;
    char array[] = {46,44,45,126,58,59,61,33,42,35,36,64};
    // Calculate K1 based on screen size: the maximum x-distance occurs
    // roughly at the edge of the torus, which is at x=R1+R2, z=0.  we
    // want that to be displaced 3/8ths of the width of the screen, which
    // is 3/4th of the way from the center to the side of the screen.
    // screen_width*3/8 = K1*(R1+R2)/(K2+0)
    // screen_width*K2*3/(8*(R1+R2)) = K1
    const float K1 = screen_width*K2*3/(8*(R1+R2));



    float f(float u,float v,float a,float t){
        return ((R1 + R2*cos(u))*cos(v))*cos(a) + (R2*sin(u)*cos(t) + ((R1 + R2*cos(u))*sin(v))*sin(t))*sin(a);
    }

    float g(float u,float v,float a,float T){
        float y = (R2 * sin(u) *cos(T) + (R1 + R2 * cos(u))*sin(v)*sin(T))* cos(a) - (R1 + R2 *cos(u)) * cos(v) *sin(a);
        return y;
    }
    float h(float u,float v,float a,float T){
        return ((R1 + R2 * cos(u)) * sin(v) * cos(T) -R2 * sin(u) * sin(T));
    }
    float L(float theta, float phi, float psi, float A){
        // Calculate the normal vector at this point on the torus
    float nx = cos(theta)*cos(phi)*cos(A) + (sin(theta)*cos(psi)+cos(theta)*sin(phi)*sin(psi))*sin(A);
    float ny = -cos(theta)*cos(phi)*sin(A) + (sin(theta)*cos(psi)+cos(theta)*sin(phi)*sin(psi))*cos(A);
    float nz = sin(theta)*sin(psi) - cos(theta)*sin(phi)*cos(psi);

    
    return (ny + nz*(-1));
    }
    void render_frame(float A, float B) {
        char output[1760];
        float zbuffer[1760];
        memset(output, 32, 1760);
        memset(zbuffer, 0, sizeof(float)*1760);

        for (float theta = 0; theta <= 2*pi; theta += 0.07){
            for (float phi = 0; phi <= 2*pi; phi += 0.02){
                float ooz = 1/(K2 - g(theta, phi, A, B));
                int xp = (int) (screen_width/2)+(30*f(theta, phi, A, B)*ooz);
                int yp = (int) (screen_height/2)-(15*h(theta, phi, A, B)*ooz);
                int o = xp + 80*yp;
                int d = 8*L(theta, phi, B, A);
                if (22>yp&&yp>0&&xp>0&&80>xp && ooz > zbuffer[o]){
                        zbuffer[o] = ooz;
                        char p = ".,-~:;=!*#$@"[d>0?d:0];
                        output[o] = p;
                }
            }
        }
        printf("\e[1;1H\e[2J");
        for (int i = 0; i < 1760; i++){
            putchar(i%80?output[i]:10);
        }
    }


    int main(){
        printf("hello World");
        float A=0, B=0;
        for (;;){
            render_frame(A, B);
            A += 0.01f, B += 0.03f;
        }
        
        
    }
