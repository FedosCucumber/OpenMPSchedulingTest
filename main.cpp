#include <vector>
#include <fstream>
#include <malloc.h>
#include <iostream>
#include <string>
#include <vector>
#include <istream>
#include <omp.h>
#include <chrono>
#include <cmath>

using namespace std;

typedef struct {
    string P5;
    int width;
    int height;
    int maxval;

    vector <vector <int>> pixel_map;

    vector <string> comments;
} pgm_file;

int get_box_radius(float sigma, int num_boxes) {
    return round(sqrt((12 * sigma * sigma / num_boxes) + 1));
}

int main(int argc, char* argv[]) {

    int num_threads = stoi(argv[1]);
    ifstream input_file;
    ofstream output_file;
    input_file.open(string(argv[2]));
    output_file.open(string(argv[3]), ios_base::binary);
    float sigma = stof(argv[4]);
    int num_boxes = stoi(argv[5]);

    pgm_file file;
    input_file >> file.P5;
    input_file >> file.width;
    input_file >> file.height;
    input_file >> file.maxval;

    file.pixel_map = vector <vector <int>>(file.height, vector <int>(file.width));

    for (vector <int>& vect : file.pixel_map)
        for (int& pix : vect) {
            char temp = 0;
            input_file.get(temp);
            pix = (unsigned char) temp;
            if (file.maxval > 255) {
                input_file.get(temp);
                pix = (pix << 8) + (unsigned char) temp;
            }
        }


    vector <vector <int>> new_map = file.pixel_map;


    omp_set_num_threads(num_threads);
    int box_r = get_box_radius(sigma, num_boxes);

    auto t1 = chrono::high_resolution_clock::now();

    for (int b = 0; b < 3; b++) {
        //Horizontal box approximation
#pragma omp parallel for schedule(static, 100)
        for (int i = 0; i < file.height; i++) {
            for (int j = 0; j < file.width; j++) {
                double kernel = 0;
                for (int x = max(j - box_r, 0); x < min(j + box_r + 1, (int)file.width); x++)
                    kernel += file.pixel_map[i][x];
                kernel /= (box_r + box_r + 1);
                new_map[i][j] = round(kernel);
            }
        }

        file.pixel_map = new_map;

        //vertical box approximation

#pragma omp parallel for schedule(static, 100)
        for (int i =0; i < file.height; i++) {
            for (int j = 0; j < file.width; j++) {
                double kernel = 0;
                for (int y = max(i - box_r, 0); y < min(i + box_r + 1, (int)file.height); y++)
                    kernel += file.pixel_map[y][j];
                kernel /= (box_r + box_r + 1);
                new_map[i][j] = round(kernel);
            }
        }
    }

    auto t2 = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast <chrono::milliseconds> (t2 - t1);
    cout << "Vertical/Horizontal Box approxiamtion algorithm runtime: " << time.count() << "ms\n";
    file.pixel_map = new_map;

    output_file.write(file.P5.c_str(), 2);
    output_file.write(" ", 1);
    output_file.write(to_string(file.width).c_str(), to_string(file.width).size());
    output_file.write(" ", 1);
    output_file.write(to_string(file.height).c_str(), to_string(file.height).size());
    output_file.write(" ", 1);
    output_file.write(to_string(file.maxval).c_str(), to_string(file.maxval).size());
    output_file.write(" ", 1);


    int i, j;
    for (i = 0; i < file.height; i++)
        for (j = 0; j < file.width; j++) {
            if (file.maxval > 255) {
                output_file.write((char*)&new_map[i][j], 2);
            }
            else {
                uint8_t temp = new_map[i][j];
                output_file << temp;
            }
        }


    input_file.close();
    output_file.close();
    return 0;
}
