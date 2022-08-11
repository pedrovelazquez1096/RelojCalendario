using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;                      //Libreria para poder manipular los puertos com
using System.Diagnostics;                   //Libreria para poder debugear e imprimir en la consola

namespace ConfiguradorDeCalendario
{
    public partial class Form1 : Form
    {
        static byte kk = 107;
        private byte[] bytein = new byte[1];
        private byte[] byteOut = new byte[2] { kk, kk };
        private byte[] horayfecha = new byte[6];
        private DateTime dt = DateTime.Now;
        String[] puertosDisponibles;
        bool requestStatus = false, deviceReady = false;
        byte lastComTested = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Status.Text = "";
        }

        private void portcom_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Debug.WriteLine(">>>Recibi el eco");
            if (requestStatus)
            {
                try
                {
                    portcom.Read(bytein,0,1);
                    Debug.WriteLine(">>>Leyendo eco");
                    if (bytein[0] == 'k')
                    {
                        Debug.WriteLine(">>>Recibi ok");
                        portcom.Write(horayfecha, 0, 6);
                        Debug.WriteLine(">>>Configuracion enviada");
                        timer1.Enabled = true;
                        horafecha.Enabled = false;
                        deviceReady = true;
                    }
                }catch(Exception exp)
                {
                    
                }

            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            int anio;
            anio = dt.Year - 2000;
            horayfecha[0] = Convert.ToByte(dt.Second);
            horayfecha[1] = Convert.ToByte(dt.Minute);
            horayfecha[2] = Convert.ToByte(dt.Hour);
            horayfecha[3] = Convert.ToByte(dt.Day);
            horayfecha[4] = Convert.ToByte(dt.Month);
            horayfecha[5] = Convert.ToByte(anio);
            Debug.WriteLine(">>>Obteniendo hora y fecha " + Convert.ToString(horayfecha[2]) + ":" + Convert.ToString(horayfecha[1]) + ":" + Convert.ToString(horayfecha[0]) + "  " + Convert.ToString(horayfecha[3]) + "/" + Convert.ToString(horayfecha[4]) + "/" + Convert.ToString(horayfecha[5]));
        }

        private void checaCom_Tick(object sender, EventArgs e)
        {
            Status.Text = "Buscando el coso ese";
            if (requestStatus == false)
            {
                Debug.WriteLine(">>>Checando coms disponibles");
                puertosDisponibles = SerialPort.GetPortNames();
                comboBox1.Items.Clear();
                foreach (String puerto in puertosDisponibles)
                {
                    comboBox1.Items.Add(puerto);
                    Debug.WriteLine(">>>Puerto encontrado: " + puerto);
                }
                proceso.Value = 10;
            }
            if (comboBox1.Items.Count > 0 && lastComTested < comboBox1.Items.Count)
            {
                Debug.WriteLine(">>>Abriendo el puerto " + puertosDisponibles[lastComTested] + " contador de lastcomTested: " + lastComTested);
                portcom.PortName = puertosDisponibles[lastComTested];
                try
                {
                    Debug.WriteLine(">>>Opening port");
                    portcom.Open();
                    portcom.DiscardInBuffer();
                    portcom.DiscardOutBuffer();
                    requestStatus = true;
                    portcom.Write(byteOut, 0, 2);
                    timer1.Enabled = true;
                    checaCom.Enabled = false;
                    proceso.Value+=10;
                }
                catch (Exception exc)
                {

                }
            }
            else
            {
                Status.Text = "No encontre el coso ese";
                Debug.WriteLine(">>>Ningun port o no se conecnto el sistema");
                requestStatus = false;
                lastComTested = 0;
                proceso.Value = 0;
            }
        }

        private void timer1_Tick_1(object sender, EventArgs e)
        {
            Debug.WriteLine(">>>Cerrando el puerto");
            try
            {
                portcom.DiscardInBuffer();
                portcom.DiscardOutBuffer();
                portcom.Close();
                Debug.WriteLine(">>>Puerto cerrado");
                if (!deviceReady)
                {
                    lastComTested++;
                    checaCom.Enabled = true;
                    Debug.WriteLine(">>>Abriendo el siguiente puerto con id: " + lastComTested);
                }
                else
                {
                    Status.Text = "Hora y fecha configuradas";
                    timer1.Enabled = false;
                    Debug.WriteLine(">>>Sistema configurado exitosamente");
                    proceso.Value = 100;
                }
            }catch(Exception exp)
            {

            }
        }
    }
}
