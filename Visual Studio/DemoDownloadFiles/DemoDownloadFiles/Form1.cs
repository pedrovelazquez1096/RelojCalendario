using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using System.Net;

namespace DemoDownloadFiles
{
    public partial class Form1 : Form
    {
        private GoogleDriveFileManager GdriveManager;
        private byte[] dataPixels;
        private byte[] bytein = new byte[1];
        private byte[] byteOut = new byte[16]; 
        private byte[] horayfecha = new byte[6];
        private DateTime dt = DateTime.Now;
        private string[] puertosDisponibles;
        private bool fechaHora = false, imagen = false, imagenesFaltantes = false, deviceConnected = false, portOpen = false;
        private byte lastComTested = 0;
        private byte imagenesPorEnviar = 0;
        private int contadorOffset = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }

        private void portcom_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            try
            {
                portcom.Read(bytein, 0, 1);
                Console.WriteLine($">>>Recibi el eco {Convert.ToChar(bytein[0])} :: {bytein[0]}");

                if (bytein[0] == 'u')
                {
                    enviaSiguientePaquete();
                }
                else if (bytein[0] == 'k')
                {
                    portSwiper.Enabled = false;
                    deviceConnected = true;
                    dataTimerUpdate();
                }
                else if(bytein[0] == 'e')
                {
                    imagenesPorEnviar--;
                    if (GdriveManager.checaSiHayNuevasImagenes() && imagenesPorEnviar > 0)
                    {
                        enviarFechaExtra(GdriveManager.downloadFile());
                        decodeImageToArray("currentImage.ctz");
                        enviaSiguientePaquete();
                    }
                }
                else
                {
                    imagenesPorEnviar = bytein[0];
                    if (GdriveManager.checaSiHayNuevasImagenes() && imagenesPorEnviar > 0)
                    {
                        enviarFechaExtra(GdriveManager.downloadFile());
                        decodeImageToArray("currentImage.ctz");
                        enviaSiguientePaquete();
                    }
                }
            }
            catch (Exception exp)
            {

            }
        }

        private void enviaSiguientePaquete()
        {
            if(portOpen)
                if (contadorOffset < 4736)
                {
                    portcom.Write(dataPixels, contadorOffset, 16);
                    contadorOffset = contadorOffset + 16;
                    Console.WriteLine("Paquetes totales enviados:" + Convert.ToString(contadorOffset));
                }
                else
                {
                    Console.WriteLine("Ya termine de mandar los paquetes pero arduino pidio otro");
                }
        }

        private void abrirPuerto(string puerto)
        {
            if (puerto != null)
                try
                {
                    Console.WriteLine($"abriendo el puerto: {puerto}");
                    portcom.PortName = puerto;
                    portcom.Open();
                    portOpen = true;
                    portcom.DiscardInBuffer();
                    portcom.DiscardOutBuffer();
                    requestDevice();
                    portSwiper.Enabled = true;
                }
                catch (Exception exp)
                {

                }
            else
                Console.WriteLine("No se encontro el arduino");
        }
       
        private void closePort()
        {
            Console.WriteLine(">>>Cerrando el puerto");
            if (portOpen)
                try
                {
                    portcom.DiscardInBuffer();
                    portcom.DiscardOutBuffer();
                    portcom.Close();
                    portOpen = false;
                    Console.WriteLine(">>>Puerto cerrado");
                }
                catch (Exception exp)
                {

                }
        }

        private string asignadorDelSiguientePuerto()
        {
            string port = null;
            if (!deviceConnected && lastComTested < puertosDisponibles.Count())
            {
                port = puertosDisponibles[lastComTested];
                lastComTested++;
            }
            return port;
        }

        private void listadorDePuertosCom()
        {
            if (SerialPort.GetPortNames().Any())
            {
                puertosDisponibles = SerialPort.GetPortNames();
            }
        }

        private void nextImage_Click(object sender, EventArgs e)
        {
            byteOut[0] = Convert.ToByte('P');
            byteOut[1] = Convert.ToByte('e');
            byteOut[2] = Convert.ToByte('d');
            byteOut[3] = Convert.ToByte('r');
            byteOut[4] = Convert.ToByte('o');
            byteOut[5] = Convert.ToByte('E');
            byteOut[6] = Convert.ToByte('d');
            byteOut[7] = Convert.ToByte('u');
            byteOut[8] = Convert.ToByte('a');
            byteOut[9] = Convert.ToByte('r');
            byteOut[10] = Convert.ToByte('d');
            byteOut[11] = Convert.ToByte('o');
            byteOut[12] = Convert.ToByte('G');
            byteOut[13] = Convert.ToByte('o');
            byteOut[14] = Convert.ToByte('V');
            byteOut[15] = Convert.ToByte('e');
            portcom.Write(byteOut, 0, 16);
        }

        private void requestDevice()
        {
            byteOut[0] = Convert.ToByte('P');
            byteOut[1] = Convert.ToByte('e');
            byteOut[2] = Convert.ToByte('d');
            byteOut[3] = Convert.ToByte('r');
            byteOut[4] = Convert.ToByte('o');
            byteOut[5] = Convert.ToByte('E');
            byteOut[6] = Convert.ToByte('d');
            byteOut[7] = Convert.ToByte('u');
            byteOut[8] = Convert.ToByte('a');
            byteOut[9] = Convert.ToByte('r');
            byteOut[10] = Convert.ToByte('d');
            byteOut[11] = Convert.ToByte('o');
            byteOut[12] = Convert.ToByte('V');
            byteOut[13] = Convert.ToByte('e');
            byteOut[14] = Convert.ToByte('G');
            byteOut[15] = Convert.ToByte('o');
            Console.WriteLine($"Enviando llave");
            if (portOpen)
                portcom.Write(byteOut, 0, 16);
        }

        private void enviarFechaExtra(string fechaExtra)
        {
            int anio;
            string[] salida2 = fechaExtra.Split('-');
            anio = Convert.ToInt32(salida2[2]) - 2000;
            byteOut[0] = Convert.ToByte(salida2[0]);
            byteOut[1] = Convert.ToByte(salida2[1]);
            byteOut[2] = Convert.ToByte(anio);
            byteOut[3] = Convert.ToByte('F');
            byteOut[4] = Convert.ToByte('e');
            byteOut[5] = Convert.ToByte('c');
            byteOut[6] = Convert.ToByte('h');
            byteOut[7] = Convert.ToByte('a');
            byteOut[8] = Convert.ToByte('E');
            byteOut[9] = Convert.ToByte('x');
            byteOut[10] = Convert.ToByte('t');
            byteOut[11] = Convert.ToByte('r');
            byteOut[12] = Convert.ToByte('a');
            byteOut[13] = Convert.ToByte('x');
            byteOut[14] = Convert.ToByte('x');
            byteOut[15] = Convert.ToByte('x');
            Console.WriteLine(">>>Envia fecha extra " + Convert.ToString(byteOut[2]) + ":" + Convert.ToString(byteOut[1]) + ":" + Convert.ToString(byteOut[0]) + "  " + Convert.ToString(byteOut[3]) + "/" + Convert.ToString(byteOut[4]) + "/" + Convert.ToString(byteOut[5]));
            if (portOpen)
                portcom.Write(byteOut, 0, 16);
        }

        private void dataTimerUpdate()
        {
            int anio;
            anio = dt.Year - 2000;
            byteOut[0] = Convert.ToByte(dt.Second);
            byteOut[1] = Convert.ToByte(dt.Minute);
            byteOut[2] = Convert.ToByte(dt.Hour);
            byteOut[3] = Convert.ToByte(dt.Day);
            byteOut[4] = Convert.ToByte(dt.Month);
            byteOut[5] = Convert.ToByte(anio);
            byteOut[6] = Convert.ToByte('F');
            byteOut[7] = Convert.ToByte('e');
            byteOut[8] = Convert.ToByte('c');
            byteOut[9] = Convert.ToByte('h');
            byteOut[10] = Convert.ToByte('a');
            byteOut[11] = Convert.ToByte('H');
            byteOut[12] = Convert.ToByte('o');
            byteOut[13] = Convert.ToByte('r');
            byteOut[14] = Convert.ToByte('a');
            byteOut[15] = Convert.ToByte('P');
            Console.WriteLine(">>>Obteniendo hora y fecha " + Convert.ToString(byteOut[2]) + ":" + Convert.ToString(byteOut[1]) + ":" + Convert.ToString(byteOut[0]) + "  " + Convert.ToString(byteOut[3]) + "/" + Convert.ToString(byteOut[4]) + "/" + Convert.ToString(byteOut[5]));
            if (portOpen)
                portcom.Write(byteOut, 0, 16);
        }

        private void portSwiper_Tick(object sender, EventArgs e)
        {
            closePort();
            abrirPuerto(asignadorDelSiguientePuerto());
            requestDevice();
        }

        private bool InternetConnectionCheck()
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

        private void decodeImageToArray(string archivo)
        {
            Console.Write("Convirtiendo string a numeros: ");
            dataPixels = ImageProsses.lectorDePixeles(archivo);
            Console.WriteLine("LISTO");
        }

        private void killEver_Click(object sender, EventArgs e)
        {

            if (portOpen)
            {
                portcom.Close();
                portOpen = false;
            }
            else
            {               
                if (InternetConnectionCheck())
                    GdriveManager = new GoogleDriveFileManager("pedro");
                else
                    MessageBox.Show("Necesito internet :c");
                
                deviceConnected = false;
                listadorDePuertosCom();
                Console.WriteLine($"puertos enlistados");
                abrirPuerto(asignadorDelSiguientePuerto());
                contadorOffset = 0;
                requestDevice();
            }
        }
    }
}
