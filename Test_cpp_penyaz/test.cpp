#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

bool Parse(const char* filename, unsigned char*& pixels, int& width, int& height);
bool Save(const char* filename, unsigned char* pixels, int width, int height);
bool mirroring(const char* filename, unsigned char* pixels, int width, int height);
bool flip_over_90(const char* filename, unsigned char* pixels, int width, int height);
bool flip_over_180(const char* filename, unsigned char* pixels, int width, int height);
bool colors_inverting(const char* filename, unsigned char* pixels, int width, int height);
bool color_changing(const char* filename, unsigned char* pixels, int width, int height);

// TEST COMMENT = dev1

int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "ru");

    if (argc != 2) {
        cout << "Ошибка использование: " << argv[0] << " <имя_файла.ppm>\n";
        return 1;
    }
    int width, height;
    unsigned char* pixels;
    if (Parse(argv[1], pixels, width, height)) {
        if (Save("test_save.ppm", pixels, width, height)) {
            if (!mirroring("test_mirroring.ppm", pixels, width, height)) {
                cout << "Ошибка : mirroring";
            } else
            if (!flip_over_90("test_flip_over_90.ppm", pixels, width, height)) {
                cout << "Ошибка : flip_over_90";
            } else
            if (!flip_over_180("test_flip_over_180.ppm", pixels, width, height)) {
                cout << "Ошибка : flip_over_180";
            } else
            if (!colors_inverting("test_colors_inverting.ppm", pixels, width, height)) {
                cout << "Ошибка : colors_inverting";
            } else
            if (!color_changing("test_color_changing.ppm", pixels, width, height)) {
                cout << "Ошибка : color_changing";
            } 
        }  else cout << "Ошибка : Save";
    }
    else cout << "Ошибка : Parse";
    delete[] pixels;
    return 0;
}

bool Parse(const char* filename, unsigned char*& pixels, int& width, int& height) {
    ifstream out_file;
    out_file.open(filename, ios::binary);
    if (!out_file.is_open()) {
        cout << "Ошибка открытия файла для чтения данных" << endl;
        return false;
    }
      char file_title[3] = { 0 };
      out_file >> file_title;
        if (strcmp(file_title, "P6") != 0) {
            cout << "Ошибка формат файла, ожидалось P6" << endl;
            out_file.close();
            return false;
        }
        out_file.seekg(1, ios::cur);
    char ch;
    out_file.get(ch);
    while (ch == '#') {
        while (ch != '\n') out_file.get(ch); 
        out_file.get(ch);
    }
    out_file.unget();
      int max_color;
      out_file >> width >> height >> max_color;
      if (width < 0 || height < 0 || max_color > 255) {
          cout << "Ошибка в чтении параметров файла  ";
          out_file.close();
          return false;
      }
        pixels = new unsigned char[width * height * 3];
        out_file.seekg(1, ios::cur);
        out_file.read(static_cast<char*>(static_cast<void*>(pixels)), width * height * 3);
    out_file.close();
  return true;
}

bool Save(const char* filename, unsigned char* pixels, int width, int height) {
    fstream in_file;
    in_file.open(filename, ios::out | ios::binary);
        if (!in_file.is_open()) {
            cout << "Ошибка открытия файла для записи данных из файла : Save" << endl;
            return false;
        }
          in_file << "P6\n" << width << " " << height << "\n255\n";
          in_file.write(static_cast<char*>(static_cast<void*>(pixels)), width * height * 3);
          in_file.close();
  return true;
}

bool mirroring(const char* filename, unsigned char* pixels, int width, int height) {
    fstream mirroring_file;
    mirroring_file.open(filename, ios::out | ios::binary);
    if (!mirroring_file.is_open()) {
        cout << "Ошибка открытия файла для записи данных в файл : mirroring" << endl;
        return false;
    }
    unsigned char* pixels_mirroring = new unsigned char[width * height * 3];
    int k = 0;
    for (int i = 3 * width; i <height* 3 * width; i += (3 * width)) {
        for (int j = i; j > i - 3*width; j -= 3) {
            pixels_mirroring[j-2] = pixels[k+=1];
            pixels_mirroring[j-1] = pixels[k+=1];
            pixels_mirroring[j] = pixels[k+=1];
        }
    }
        mirroring_file << "P6\n" << width << " " << height << "\n255\n";
        mirroring_file.write(static_cast<char*>(static_cast<void*>(pixels_mirroring)), width * height * 3);
        mirroring_file.close();
    delete[] pixels_mirroring;
    return true;
}

bool flip_over_90(const char* filename, unsigned char* pixels, int width, int height) {
    fstream flip_file;
    flip_file.open(filename, ios::out | ios::binary);
    if (!flip_file.is_open()) {
        cout << "Ошибка открытия файла для записи данных в файл : flip_over_90" << endl;
        return false;
    }
    unsigned char* pixels_rotate = new unsigned char[width * height * 3];
    int k = 0;
    for (int i = 0; i < 3*width ; i+=3) {
        for (int j = height-1; j >= 0; j-=1) {
            int index = 3 * (width * j )+ i;
            pixels_rotate[k++] = pixels[index];
            pixels_rotate[k++] = pixels[index + 1];
            pixels_rotate[k++] = pixels[index + 2];
        }
    }
        flip_file << "P6\n" << height << " " << width << "\n255\n";
        flip_file.write(static_cast<char*>(static_cast<void*>(pixels_rotate)), width * height * 3);
        flip_file.close();
    delete[] pixels_rotate;
    return true;
}

bool flip_over_180(const char* filename, unsigned char* pixels, int width, int height) {
    fstream overturn_file;
    overturn_file.open(filename, ios::out | ios::binary);
    if (!overturn_file.is_open()) {
        cout << "Ошибка открытия файла для записи данных в файл : flip_over_180" << endl;
        return false;
    }
    unsigned char* pixels_overturn = new unsigned char[width * height * 3];
    int k = 0;
    for (int i = height * 3 * width; i > 0; i -= (3 * width)) {
        for (int j = i; j > i - (3 * width); j -= 3) {
            pixels_overturn[j - 3] = pixels[k++];
            pixels_overturn[j - 2] = pixels[k++];
            pixels_overturn[j - 1] = pixels[k++];
        }
    }
        overturn_file << "P6\n" << width << " " << height << "\n255\n";
        overturn_file.write(static_cast<char*>(static_cast<void*>(pixels_overturn)), width * height * 3);
        overturn_file.close();
    delete[] pixels_overturn;
    return true;
}

bool colors_inverting(const char* filename, unsigned char* pixels, int width, int height) {
    fstream inverting_file;
    inverting_file.open(filename, ios::out | ios::binary);
    if (!inverting_file.is_open()) {
        cout << "Ошибка открытия файла для записи данных в файл : colors_inverting" << endl;
        return false;
    }
         unsigned char* pixels_inverting = new unsigned char[width * height * 3];
         for (int i = 0; i < 3 * width * height; i += 1) pixels_inverting[i] = 255 - pixels[i];
   inverting_file << "P6\n" << width << " " << height << "\n255\n";
   inverting_file.write(static_cast<char*>(static_cast<void*>(pixels_inverting)), width * height * 3);
   inverting_file.close();
    delete[] pixels_inverting;
    return true;
}

bool color_changing(const char* filename, unsigned char* pixels, int width, int height) {
    // изменение синего в 2 раза и зеленого в 3 раза
    fstream changing_file;
    changing_file.open(filename, ios::out | ios::binary);
    if (!changing_file.is_open()) {
        cout << "Ошибка открытия файла для записи данных в файл : colors_inverting" << endl;
        return false;
    }
        unsigned char* pixels_changing = new unsigned char[width * height * 3];
        for (int i = 0; i < 3 * width * height; i += 3) {
            pixels_changing[i] = pixels[i];
            pixels_changing[i + 1] = min(3 * pixels[i + 1], 255);
            pixels_changing[i + 2] = min(2 * pixels[i + 2], 255);
        }
            changing_file << "P6\n" << width << " " << height << "\n255\n";
            changing_file.write(static_cast<char*>(static_cast<void*>(pixels_changing)), width * height * 3);
            changing_file.close();
    delete[] pixels_changing;
    return true;
}