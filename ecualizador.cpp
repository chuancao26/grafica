#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

// funcion para generar el histograma de una imagen
vector<int> histograma(const Mat& image)
{
  vector<int> histogram(256, 0);
  for (int i = 0; i < image.rows; i++)
    {
      for (int j = 0; j < image.cols; j++)
        {
          histogram[image.at<uchar>(i, j)]++;
        }
    }
  return histogram;
}

// funcion que recibe un histograma y genera el archivo con el histograma
void guardarHistogramaCSV(const vector<int>& histo, const string& nombreArchivo)
{
  ofstream archivo(nombreArchivo);
  if (!archivo.is_open())
    {
      cerr << "No se pudo abrir el archivo para escribir.\n";
      return;
    }
  archivo << "Valor,Cantidad\n";
  for (int i = 0; i < (int)histo.size(); ++i)
    {
      archivo << i << "," << histo[i] << "\n";
    }
  archivo.close();
}

// devuelve un histograma ecualizado
vector<int> getFuncionEcualizadora(const vector<int>& histo, int totalPixeles)
{
  vector<int> funcion(256, 0);
  int acumulado = 0;
  for (int i = 0; i < 256; i++)
    {
      acumulado += histo[i];
      funcion[i] = acumulado * 255 / totalPixeles;
    }
  return funcion;
}

// funcion que aplica el histograma ecualizado a la nueva imagen ecualizada
Mat ecualizar(const vector<int>& histo, const Mat& original)
{
  Mat img(original.size(), original.type());
  vector<int> funcion = getFuncionEcualizadora(histo, original.rows * original.cols);
  for (int i = 0; i < img.rows; i++)
    {
      for (int j = 0; j < img.cols; j++)
        {
          img.at<uchar>(i, j) = (uchar)funcion[original.at<uchar>(i, j)];
        }
    }
  return img;
}

int main()
{
  //procesamiento de las 4 imagenes con sus nombres
  vector<string> nombres = {"img1", "img2", "img3", "img4"};
  for (auto name : nombres)
  {
    Mat escalaGrises = imread(name + ".jpg", IMREAD_GRAYSCALE);

    if (escalaGrises.empty())
      {
        cerr << "Error al cargar la imagen.\n";
        return -1;
      }

    // histograma
    vector<int> histo = histograma(escalaGrises);
    guardarHistogramaCSV(histo, name + "_histograma.csv");

    // img ecualizaada
    Mat imagenEcualizada = ecualizar(histo, escalaGrises);

    // histograma ecualizado 
    vector<int> histoEq = histograma(imagenEcualizada);
    guardarHistogramaCSV(histoEq, name + "_ecu_histograma.csv");
    
    imwrite(name + "_ecu.jpg", imagenEcualizada);
  }

  waitKey(0);

  return 0;
}
