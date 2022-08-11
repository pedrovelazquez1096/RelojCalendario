using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace InternetConnectionCheckEx
{
    class Program
    {
        static void Main(string[] args)
        {
            string entrada;
            entrada = Console.ReadLine();
            if (entrada.Contains("pedro"))
            {
                string[] salida = entrada.Split('_');
                foreach (string sal in salida)
                {
                    Console.WriteLine(sal);
                }
                string[] salida2 = salida[1].Split('-');
                foreach (string sal in salida2)
                {
                    Console.WriteLine(sal);
                }
            }
            Console.ReadLine();
        }

        public static bool CheckForInternetConnection()
        {
            try
            {
                using (var client = new WebClient())
                using (client.OpenRead("http://google.com/generate_204"))
                    return true;
            }
            catch
            {
                return false;
            }
        }
    }
}
