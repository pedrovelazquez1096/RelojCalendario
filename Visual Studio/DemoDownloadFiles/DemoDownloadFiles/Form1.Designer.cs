namespace DemoDownloadFiles
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
            this.portcom = new System.IO.Ports.SerialPort(this.components);
            this.killEver = new System.Windows.Forms.Button();
            this.nextImage = new System.Windows.Forms.Button();
            this.portSwiper = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // portcom
            // 
            this.portcom.BaudRate = 115200;
            this.portcom.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.portcom_DataReceived);
            // 
            // killEver
            // 
            this.killEver.Location = new System.Drawing.Point(37, 78);
            this.killEver.Name = "killEver";
            this.killEver.Size = new System.Drawing.Size(75, 23);
            this.killEver.TabIndex = 6;
            this.killEver.Text = "Kill";
            this.killEver.UseVisualStyleBackColor = true;
            this.killEver.Click += new System.EventHandler(this.killEver_Click);
            // 
            // nextImage
            // 
            this.nextImage.Location = new System.Drawing.Point(37, 49);
            this.nextImage.Name = "nextImage";
            this.nextImage.Size = new System.Drawing.Size(75, 23);
            this.nextImage.TabIndex = 7;
            this.nextImage.Text = "Next";
            this.nextImage.UseVisualStyleBackColor = true;
            this.nextImage.Click += new System.EventHandler(this.nextImage_Click);
            // 
            // portSwiper
            // 
            this.portSwiper.Interval = 1000;
            this.portSwiper.Tick += new System.EventHandler(this.portSwiper_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(146, 127);
            this.Controls.Add(this.nextImage);
            this.Controls.Add(this.killEver);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion
        private System.IO.Ports.SerialPort portcom;
        private System.Windows.Forms.Button killEver;
        private System.Windows.Forms.Button nextImage;
        private System.Windows.Forms.Timer portSwiper;
    }
}

