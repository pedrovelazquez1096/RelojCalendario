namespace ImageProcessing_ColortoGray_demo1
{
    partial class Form1
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.pictureBoxInput = new System.Windows.Forms.PictureBox();
            this.pictureBoxOutput = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.openimage = new System.Windows.Forms.Button();
            this.convertir = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.pixelesLeidos = new System.Windows.Forms.Label();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.enviar = new System.Windows.Forms.Button();
            this.Kill = new System.Windows.Forms.Button();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.OutputUmbral = new System.Windows.Forms.PictureBox();
            this.umbral = new System.Windows.Forms.TrackBar();
            this.labelRojo = new System.Windows.Forms.Label();
            this.labelVerde = new System.Windows.Forms.Label();
            this.labelAzul = new System.Windows.Forms.Label();
            this.trackBarRojo = new System.Windows.Forms.TrackBar();
            this.trackBarVerde = new System.Windows.Forms.TrackBar();
            this.trackBarAzul = new System.Windows.Forms.TrackBar();
            this.textBox1 = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxInput)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxOutput)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.OutputUmbral)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.umbral)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRojo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVerde)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAzul)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBoxInput
            // 
            this.pictureBoxInput.Location = new System.Drawing.Point(12, 28);
            this.pictureBoxInput.Name = "pictureBoxInput";
            this.pictureBoxInput.Size = new System.Drawing.Size(296, 128);
            this.pictureBoxInput.TabIndex = 0;
            this.pictureBoxInput.TabStop = false;
            // 
            // pictureBoxOutput
            // 
            this.pictureBoxOutput.Location = new System.Drawing.Point(12, 184);
            this.pictureBoxOutput.Name = "pictureBoxOutput";
            this.pictureBoxOutput.Size = new System.Drawing.Size(296, 128);
            this.pictureBoxOutput.TabIndex = 1;
            this.pictureBoxOutput.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Input";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(11, 168);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(39, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Output";
            // 
            // openimage
            // 
            this.openimage.Location = new System.Drawing.Point(314, 77);
            this.openimage.Name = "openimage";
            this.openimage.Size = new System.Drawing.Size(77, 35);
            this.openimage.TabIndex = 4;
            this.openimage.Text = "Abrir";
            this.openimage.UseVisualStyleBackColor = true;
            this.openimage.Click += new System.EventHandler(this.openimage_Click);
            // 
            // convertir
            // 
            this.convertir.Location = new System.Drawing.Point(314, 227);
            this.convertir.Name = "convertir";
            this.convertir.Size = new System.Drawing.Size(77, 35);
            this.convertir.TabIndex = 5;
            this.convertir.Text = "Iniciar";
            this.convertir.UseVisualStyleBackColor = true;
            this.convertir.Click += new System.EventHandler(this.convertir_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(93, 168);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(118, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Numero de pixels leidos";
            // 
            // pixelesLeidos
            // 
            this.pixelesLeidos.AutoSize = true;
            this.pixelesLeidos.Location = new System.Drawing.Point(218, 168);
            this.pixelesLeidos.Name = "pixelesLeidos";
            this.pixelesLeidos.Size = new System.Drawing.Size(0, 13);
            this.pixelesLeidos.TabIndex = 7;
            // 
            // serialPort
            // 
            this.serialPort.BaudRate = 115200;
            this.serialPort.PortName = "COM7";
            this.serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.eco);
            // 
            // enviar
            // 
            this.enviar.Location = new System.Drawing.Point(277, 350);
            this.enviar.Name = "enviar";
            this.enviar.Size = new System.Drawing.Size(75, 23);
            this.enviar.TabIndex = 16;
            this.enviar.Text = "Iniciar";
            this.enviar.UseVisualStyleBackColor = true;
            this.enviar.Click += new System.EventHandler(this.enviar_Click);
            // 
            // Kill
            // 
            this.Kill.Location = new System.Drawing.Point(400, 350);
            this.Kill.Name = "Kill";
            this.Kill.Size = new System.Drawing.Size(75, 23);
            this.Kill.TabIndex = 18;
            this.Kill.Text = "End";
            this.Kill.UseVisualStyleBackColor = true;
            this.Kill.Click += new System.EventHandler(this.Kill_Click);
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(12, 376);
            this.progressBar1.Maximum = 4736;
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(731, 23);
            this.progressBar1.Step = 16;
            this.progressBar1.TabIndex = 19;
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 50;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // OutputUmbral
            // 
            this.OutputUmbral.Location = new System.Drawing.Point(431, 184);
            this.OutputUmbral.Name = "OutputUmbral";
            this.OutputUmbral.Size = new System.Drawing.Size(296, 128);
            this.OutputUmbral.TabIndex = 9;
            this.OutputUmbral.TabStop = false;
            // 
            // umbral
            // 
            this.umbral.BackColor = System.Drawing.SystemColors.Control;
            this.umbral.Location = new System.Drawing.Point(0, 318);
            this.umbral.Maximum = 255;
            this.umbral.Name = "umbral";
            this.umbral.Size = new System.Drawing.Size(743, 45);
            this.umbral.TabIndex = 8;
            this.umbral.Value = 128;
            this.umbral.ValueChanged += new System.EventHandler(this.actualizaUmbral);
            // 
            // labelRojo
            // 
            this.labelRojo.AutoSize = true;
            this.labelRojo.Location = new System.Drawing.Point(428, -1);
            this.labelRojo.Name = "labelRojo";
            this.labelRojo.Size = new System.Drawing.Size(10, 13);
            this.labelRojo.TabIndex = 13;
            this.labelRojo.Text = "r";
            // 
            // labelVerde
            // 
            this.labelVerde.AutoSize = true;
            this.labelVerde.Location = new System.Drawing.Point(428, 47);
            this.labelVerde.Name = "labelVerde";
            this.labelVerde.Size = new System.Drawing.Size(13, 13);
            this.labelVerde.TabIndex = 14;
            this.labelVerde.Text = "g";
            // 
            // labelAzul
            // 
            this.labelAzul.AutoSize = true;
            this.labelAzul.Location = new System.Drawing.Point(428, 88);
            this.labelAzul.Name = "labelAzul";
            this.labelAzul.Size = new System.Drawing.Size(13, 13);
            this.labelAzul.TabIndex = 15;
            this.labelAzul.Text = "b";
            // 
            // trackBarRojo
            // 
            this.trackBarRojo.LargeChange = 1;
            this.trackBarRojo.Location = new System.Drawing.Point(417, 12);
            this.trackBarRojo.Maximum = 1000;
            this.trackBarRojo.Name = "trackBarRojo";
            this.trackBarRojo.Size = new System.Drawing.Size(326, 45);
            this.trackBarRojo.TabIndex = 10;
            this.trackBarRojo.Value = 299;
            this.trackBarRojo.ValueChanged += new System.EventHandler(this.updateRed);
            // 
            // trackBarVerde
            // 
            this.trackBarVerde.LargeChange = 1;
            this.trackBarVerde.Location = new System.Drawing.Point(417, 63);
            this.trackBarVerde.Maximum = 1000;
            this.trackBarVerde.Name = "trackBarVerde";
            this.trackBarVerde.Size = new System.Drawing.Size(326, 45);
            this.trackBarVerde.TabIndex = 11;
            this.trackBarVerde.Value = 587;
            this.trackBarVerde.ValueChanged += new System.EventHandler(this.updateGreen);
            // 
            // trackBarAzul
            // 
            this.trackBarAzul.LargeChange = 1;
            this.trackBarAzul.Location = new System.Drawing.Point(417, 114);
            this.trackBarAzul.Maximum = 1000;
            this.trackBarAzul.Name = "trackBarAzul";
            this.trackBarAzul.Size = new System.Drawing.Size(326, 45);
            this.trackBarAzul.TabIndex = 12;
            this.trackBarAzul.Value = 114;
            this.trackBarAzul.ValueChanged += new System.EventHandler(this.updateBlue);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(325, 292);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 20;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(752, 411);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.Kill);
            this.Controls.Add(this.enviar);
            this.Controls.Add(this.labelAzul);
            this.Controls.Add(this.labelVerde);
            this.Controls.Add(this.labelRojo);
            this.Controls.Add(this.trackBarAzul);
            this.Controls.Add(this.trackBarVerde);
            this.Controls.Add(this.trackBarRojo);
            this.Controls.Add(this.OutputUmbral);
            this.Controls.Add(this.umbral);
            this.Controls.Add(this.pixelesLeidos);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.convertir);
            this.Controls.Add(this.openimage);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pictureBoxOutput);
            this.Controls.Add(this.pictureBoxInput);
            this.Name = "Form1";
            this.Text = "ImageProcessing_ColortoGray_demo1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxInput)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxOutput)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.OutputUmbral)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.umbral)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRojo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVerde)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAzul)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxInput;
        private System.Windows.Forms.PictureBox pictureBoxOutput;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button openimage;
        private System.Windows.Forms.Button convertir;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label pixelesLeidos;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.Button enviar;
        private System.Windows.Forms.Button Kill;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.PictureBox OutputUmbral;
        private System.Windows.Forms.TrackBar umbral;
        private System.Windows.Forms.Label labelRojo;
        private System.Windows.Forms.Label labelVerde;
        private System.Windows.Forms.Label labelAzul;
        private System.Windows.Forms.TrackBar trackBarRojo;
        private System.Windows.Forms.TrackBar trackBarVerde;
        private System.Windows.Forms.TrackBar trackBarAzul;
        private System.Windows.Forms.TextBox textBox1;
    }
}

