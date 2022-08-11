using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ImageProcessing_ColortoGray_demo1
{
    class ImageProsses
    {
        public ImageProsses()
        {

        }

        public static byte[] lectorDePixeles(string filename)
        {
            List<string> pixeles = new List<string>();
            int contador = 0;
            byte[] dataPixels = new byte[4736];
            FileLoggerManager.readAllLines(filename, ref pixeles);
            foreach (string file in pixeles)
            {
                string[] dataPixelsRaw = file.Split('\n');

                foreach (string pixel in dataPixelsRaw)
                {
                    //Console.WriteLine(pixel);
                    string[] temp = pixel.Split(',');
                    for(int i = 0; i < 16;i++)
                    {
                        dataPixels[contador] = Convert.ToByte(Convert.ToInt16(temp[i], 16));
                        contador++;
                    }
                    //Console.WriteLine(contador);
                }
            }
            return dataPixels;
        }
    }   
}
