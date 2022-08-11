using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DemoDownloadFiles
{
    class FileLoggerManager
    {
        public FileLoggerManager()
        {
            
        }

        public static void readAllLines(string nameFile, ref List<string> fileLogger)
        {
            string path = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            path = Path.Combine(path, "driveApiCredentials", "drive-credentials.json\\"+nameFile);
            //Console.WriteLine($"Leyendo archivo {path}");
            if (!File.Exists(path))
            {
                using (StreamWriter sw = File.CreateText(path))
                {
                    ;
                }
            }
            readLines(path, ref fileLogger);
        }

        private static void readLines(string path, ref List<string> fileLogger)
        {
            fileLogger = File.ReadAllLines(path).ToList();
        }

        public static void writeAllLines(string nameFile, ref List<string> fileLogger)
        {
            string path = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            path = Path.Combine(path, "driveApiCredentials", "drive-credentials.json\\" + nameFile);
            File.WriteAllLines(path, fileLogger);
        }

        public static void deleteFile(string nameFile)
        {
            string path = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            path = Path.Combine(path, "driveApiCredentials", "drive-credentials.json\\" + nameFile);
            if (!File.Exists(path))
                File.Delete(path);
        }
    }
}
