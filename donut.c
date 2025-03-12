#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char **argv) {
    typedef struct char_with_depth {
        char ch;
        int depth;
    } char_with_depth;

    const int width = 40, height = 80;

    //Signed Angular Velocities about x, y, and z axes
    float omega_x = 0.3,
          omega_y = 0.5,
          omega_z = 0.7,
          t = 0,
          dt = 0.1f,
          PI = 3.14159265f;

    //Light Source Coordinates
    float light_x = 0,
          light_y = 0,
          light_z = 1110;

    int xc = width / 2,
        yc = height / 2;

    int R = 25,
        r = 8;
    
    int eye_dist = 30,
        cam_dist = 30;

    char_with_depth frame[width][height];

    struct timespec ts, ts1;
    ts.tv_sec = 0;
    ts.tv_nsec = 40 * 1000000;
    ts1.tv_sec = 0;
    ts1.tv_nsec = 40 * 1000000;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            frame[i][j].ch = ' ';
            frame[i][j].depth = -(R + r + 1);
        }
    }

    double x, y, z, xl, yl, zl;
    while (t < 50) {
        //Tranasformations on Light Source
        xl = (light_x * cos(-omega_y * t) + (light_y * sin(-omega_x * t) + light_z * cos(-omega_x * t)) * sin(-omega_y * t)) * cos(-omega_z * t) - (light_y * cos(-omega_x * t) - light_z * sin(-omega_x * t)) * sin(-omega_z * t);
        yl = (light_x * cos(-omega_y * t) + (light_y * sin(-omega_x * t) + light_z * cos(-omega_x * t)) * sin(-omega_y * t)) * sin(-omega_z * t) + (light_y * cos(-omega_x * t) - light_z * sin(-omega_x * t)) * cos(-omega_z * t);
        zl = -light_x * sin(-omega_y * t) + (light_y * sin(-omega_x * t) + light_z * cos(-omega_x * t)) * cos(-omega_y * t);
        
        for (double theta = 0; theta < 2 * PI; theta += 1.0f / R) {
            for (double phi = 0; phi < 2 * PI; phi += 1.0f / r) {
                x = (R + r * sin(phi)) * cos(theta);
                y = (R + r * sin(phi)) * sin(theta);
                z = -r * cos(phi);

                //Surface Normal
                float xn = r * sin(phi) * cos(theta),
                      yn = r * sin(phi) * sin(theta),
                      zn = -r * cos(phi);
                
                //Returns a value in the range [-1, 1]
                float illumination = (xn * (xl - x) + yn  * (yl - y) + zn * (zl - z)) /
                                     (sqrt(xn * xn + yn * yn + zn * zn) * sqrt((xl - x) * (xl - x) + (yl - y) * (yl - y) + (zl - z) * (zl - z)));
                
                double _x, _y, _z;

                _y = y * cos(omega_x * t) - z * sin(omega_x * t);
                _z = y * sin(omega_x * t) + z * cos(omega_x * t);
                y = _y;
                z = _z;

                _x = x * cos(omega_y * t) + z * sin(omega_y * t);
                _z = -x * sin(omega_y * t) + z * cos(omega_y * t);
                x = _x;
                z = _z;

                _x = x * cos(omega_z * t) - y * sin(omega_z * t);
                _y = x * sin(omega_z * t) + y * cos(omega_z * t);
                x = _x;
                y = _y;

                if ((int)(x * eye_dist / (eye_dist + cam_dist - z)) + xc > -1 &&
                    (int)(x * eye_dist / (eye_dist + cam_dist - z)) + xc < width &&
                    (int)(y * eye_dist / (eye_dist + cam_dist - z)) + yc > -1 &&
                    (int)(y * eye_dist / (eye_dist + cam_dist - z)) + yc < height &&
                    z > frame[(int)(x * eye_dist / (eye_dist + cam_dist - z)) + xc][(int)(y * eye_dist / (eye_dist + cam_dist - z)) + yc].depth) {
                    frame[(int)(x * eye_dist / (eye_dist + cam_dist - z)) + xc][(int)(y * eye_dist / (eye_dist + cam_dist - z)) + yc].depth = z;
                    frame[(int)(x * eye_dist / (eye_dist + cam_dist - z)) + xc][(int)(y * eye_dist / (eye_dist + cam_dist - z)) + yc].ch = ".,~:;=!*#$@"[(int)(illumination * 11) >= 0 ? (int)(illumination * 11) : 0];
                }
            }
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                printf(" %c", frame[i][j].ch);
                frame[i][j].ch = ' ';
                frame[i][j].depth = -(R + r + 1);
            }
            printf("\n");
        }
        nanosleep(&ts, NULL);

        system("clear");
        t += dt;
    }
}