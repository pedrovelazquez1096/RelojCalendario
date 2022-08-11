using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Google.Apis.Auth.OAuth2;
using Google.Apis.Drive.v3;
using Google.Apis.Services;
using Google.Apis.Util.Store;
using File = Google.Apis.Drive.v3.Data.File;

namespace DemoDownloadFiles
{
    class GoogleDriveFileManager
    {
        private static string[] Scopes = { DriveService.Scope.Drive };
        private static string ApplicationName = "GoogleDriveAPIStart";
        private static string folderId = "1EKEUCfkgCET-AI20IvsuyJYPjwUL7FdP";
        private UserCredential credential;
        private DriveService service;
        private string userName = "noUser";

        public GoogleDriveFileManager(string user)
        {
            Console.WriteLine("Create credential");
            credential = GetUserCredential();
            Console.WriteLine("Get Service");
            service = GetDriveService(credential);
            Console.WriteLine("Service started");
            this.userName = user;
        }

        public string downloadFile()                            //Descarga la imagen y regresa su nombre, ya que, ahi esta la fecha en la que se debe desplegar
        {
            string[] dataFile = buscadorDeNuevaImagen();
            if (dataFile[1] != null)
            {
                downloadSpecificFile(dataFile[1], "currentImage.ctz");
            }
            else
                Console.WriteLine("No hay archivos nuevos");
            string[] salida = dataFile[0].Split('_');
            return salida[1];                            
        }

        public void uploadFile()
        {
            string fileName = "loggingUsage.csv";
            string path = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            path = Path.Combine(path, "driveApiCredentials", "drive-credentials.json\\log.ctz");
            string contentType = "text/plain";
            var filemetada = new File();

            filemetada.Name = fileName;
            filemetada.Parents = new List<string> { folderId };
            FilesResource.CreateMediaUpload request;
            Console.WriteLine("Subiendo archivo");
            using (var stream = new FileStream(path, FileMode.Open))
            {
                request = service.Files.Create(filemetada, stream, contentType);
                request.Upload();
            }
            Console.WriteLine("Archivo Subido");
        }

        public bool checaSiHayNuevasImagenes() //Regresa true sí hay nuevas imagenes disponibles
        {
            bool isThereACopy = true;
            string pageToken = null;
            string fileName = "log.ctz";
            string[] datos = new string[2];
            List<string> fileLogger = new List<string>();
            FileLoggerManager.readAllLines(fileName, ref fileLogger);

            do
            {
                ListFiles(ref pageToken, ref datos);
                if (fileLogger.Any() && datos.Any())
                {
                    foreach (string file in fileLogger)
                    {
                        if ((datos[0].Equals(file)))
                        {
                            isThereACopy = true;
                            break;
                        }
                        else
                            isThereACopy = false;
                    }
                    if (!isThereACopy)
                        break;
                }
                else if (!datos.Any())
                {
                    return false;
                }
                else
                {
                    isThereACopy = false;
                    pageToken = null;
                    break;
                }
            } while (pageToken != null);

            return !isThereACopy;
        }

        public void setUser(string user)
        {
            this.userName = user;
        }

        public string getUser()
        {
            return this.userName;
        }

        private static UserCredential GetUserCredential()
        {
            UserCredential credential;
            using (var stream = new FileStream("client_secret.json", FileMode.Open, FileAccess.Read))
            {
                string credPath = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
                credPath = Path.Combine(credPath, "driveApiCredentials", "drive-credentials.json");
                credential = GoogleWebAuthorizationBroker.AuthorizeAsync(
                    GoogleClientSecrets.Load(stream).Secrets,
                    Scopes,
                    "user",
                    CancellationToken.None,
                    new FileDataStore(credPath, true)).Result;
                // Console.WriteLine("Credential file saved to: " + credPath);
            }

            return credential;
        }

        private static DriveService GetDriveService(UserCredential credential)
        {
            return new DriveService(
                new BaseClientService.Initializer()
                {
                    HttpClientInitializer = credential,
                    ApplicationName = ApplicationName
                }
            );
        }

        private void ListFiles(ref string pageToken, ref string[] datos)
        {
            FilesResource.ListRequest listRequest = service.Files.List();
            listRequest.PageSize = 1;                                               //Ire de uno en uno en los archivos
            listRequest.Fields = "nextPageToken, files(id, name)";                  //Solicitare solo id y nombre
            listRequest.PageToken = pageToken;
            listRequest.Q = "mimeType='text/html'";                                 //Buscare solo archivos con extencion HTML

            var request = listRequest.Execute();                                    //Se solicita la lista de archivos

            if (request.Files != null && request.Files.Count > 0)                   //Sí los archivos NO son NULL y hay mas de 0
            {
                foreach (var file in request.Files)                                 //Por cada archivo en Files
                {                                                                   //Guardare solo los que contengan el nomber del usuario
                    if (file.Name.Contains(userName))
                    {
                        datos[0] = file.Name;
                        datos[1] = file.Id;
                    }
                }

                pageToken = request.NextPageToken;
                /*
                if (request.NextPageToken != null)
                {
                    //Console.WriteLine("Press any key to conti...");
                    //Console.ReadLine();
                }
                */
            }
            else
            {
                Console.WriteLine("No files found.");
            }
        }

        private string[] buscadorDeNuevaImagen() //Regresa el nombre de la imagen nueva para descargar
        {
            bool isThereACopy = true;
            string pageToken = null;
            string fileName = "log.ctz";
            string[] datos = new string[2];
            List<string> fileLogger = new List<string>();
            FileLoggerManager.readAllLines(fileName, ref fileLogger);

            Console.WriteLine("Buscando archivos nuevos");
            do
            {
                ListFiles(ref pageToken, ref datos);
                if (fileLogger.Any() && datos.Any())
                {
                    foreach (string file in fileLogger)
                    {
                        if ((datos[0].Equals(file)))
                        {
                            isThereACopy = true;
                            break;
                        }
                        else
                            isThereACopy = false;
                    }
                    if (!isThereACopy)
                        break;
                }
                else
                {
                    Console.WriteLine("La lista esta vacia");
                    isThereACopy = false;
                    pageToken = null;
                    break;
                }
            } while (pageToken != null);

            if (!isThereACopy)                                              //Sí la imagen es nueva
            {
                fileLogger.Add(datos[0]);                                   //La agergo al logger
                foreach (string file in fileLogger)
                    Console.WriteLine(file);
                FileLoggerManager.writeAllLines(fileName, ref fileLogger);
            }
            else                                                            //Sí recorri todas las imagenes en drive y no encontre una nueva imagen regreso NULL
                datos[1] = null;

            return datos;
        }

        private void downloadSpecificFile(string itemId, string fileName)
        {
            var request = service.Files.Get(itemId);
            string path = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            path = Path.Combine(path, "driveApiCredentials", "drive-credentials.json\\" + fileName);
            Console.WriteLine("Descargando archivo");
            using (var memorystream = new MemoryStream())
            {
                request.Download(memorystream);
                using (var fileStream = new FileStream(path, FileMode.Create, FileAccess.Write))
                {
                    fileStream.Write(memorystream.GetBuffer(), 0, memorystream.GetBuffer().Length);
                }
            }
            Console.WriteLine("Archivo descargado");
        }

    }
}
