namespace ConfiguradorDeCalendario
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
            this.horafecha = new System.Windows.Forms.Timer(this.components);
            this.portcom = new System.IO.Ports.SerialPort(this.components);
            this.checaCom = new System.Windows.Forms.Timer(this.components);
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.proceso = new System.Windows.Forms.ProgressBar();
            this.Status = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // horafecha
            // 
            this.horafecha.Enabled = true;
            this.horafecha.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // portcom
            // 
            this.portcom.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.portcom_DataReceived);
            // 
            // checaCom
            // 
            this.checaCom.Enabled = true;
            this.checaCom.Interval = 1000;
            this.checaCom.Tick += new System.EventHandler(this.checaCom_Tick);
            // 
            // comboBox1
            // 
            this.comboBox1.Enabled = false;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(56, 257);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 21);
            this.comboBox1.TabIndex = 0;
            this.comboBox1.Visible = false;
            // 
            // timer1
            // 
            this.timer1.Interval = 2000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick_1);
            // 
            // proceso
            // 
            this.proceso.Location = new System.Drawing.Point(12, 55);
            this.proceso.Name = "proceso";
            this.proceso.Size = new System.Drawing.Size(259, 23);
            this.proceso.TabIndex = 1;
            // 
            // Status
            // 
            this.Status.AutoSize = true;
            this.Status.BackColor = System.Drawing.Color.Transparent;
            this.Status.Font = new System.Drawing.Font("Sitka Text", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Status.Location = new System.Drawing.Point(12, 22);
            this.Status.Name = "Status";
            this.Status.Size = new System.Drawing.Size(46, 20);
            this.Status.TabIndex = 2;
            this.Status.Text = "label1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(283, 350);
            this.Controls.Add(this.Status);
            this.Controls.Add(this.proceso);
            this.Controls.Add(this.comboBox1);
            this.Name = "Form1";
            this.Text = "Son solo sentimientos hechos codigo";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Timer horafecha;
        private System.IO.Ports.SerialPort portcom;
        private System.Windows.Forms.Timer checaCom;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ProgressBar proceso;
        private System.Windows.Forms.Label Status;
    }
}

