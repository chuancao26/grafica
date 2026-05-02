#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

using namespace std;
using namespace cv;

// funciones de ayuda
uchar getMediana(vector<uchar> kernel)
{
  sort(kernel.begin(), kernel.end());
  return kernel[kernel.size() / 2];
}

uchar getMedia(vector<uchar> kernel)
{
  int total = std::accumulate(kernel.begin(), kernel.end(), 0);
  return (uchar)(total / kernel.size());
}

// funcion encargada de aplicar un foltro basado en el tipo 
// (string filtro por ahora solo media y mediana)
Mat aplicarFiltro(const Mat& original, int n, string filtro)
{
  Mat resultado = original.clone();
  int fuera = n / 2;

  for (int y = fuera; y < original.rows - fuera; y++)
    {
      for (int x = fuera; x < original.cols - fuera; x++)
        {
          vector<uchar> kernel;
          for (int i = -fuera; i <= fuera; i++)
            {
              for (int j = -fuera; j <= fuera; j++)
                {
                  kernel.push_back(original.at<uchar>(y + i, x + j));
                }
            }

          if (filtro == "mediana")
            {
              resultado.at<uchar>(y, x) = getMediana(kernel);
            }
          else if (filtro == "media")
            {
              resultado.at<uchar>(y, x) = getMedia(kernel);
            }
        }
    }
  return resultado;
}

int main()
{
  vector<string> nombres = {"figuraV.png", "figures.jpg", "riceB.png"};
  vector<int> mascaras = {3, 5, 7, 9, 11};

  for (const string& nombre : nombres)
    {
      Mat img = imread(nombre, IMREAD_GRAYSCALE);
      
      if (img.empty())
        {
          cout << "Error cargando imagen: " << nombre << endl;
          continue;
        }

      for (int n : mascaras)
        {
          // Procesar Mediana
          Mat outMediana = aplicarFiltro(img, n, "mediana");
          imwrite("mediana_" + to_string(n) + "_" + nombre, outMediana);

          // Procesar Media
          Mat outMedia = aplicarFiltro(img, n, "media");
          imwrite("media_" + to_string(n) + "_" + nombre, outMedia);
        }
    }

  return 0;
}
