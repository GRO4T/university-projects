using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PAIN_WF_App
{
    public partial class MainForm : Form
    {
        public bool IsClosing { get; set; } = false;

        public MainForm()
        {
            InitializeComponent();
            IsMdiContainer = true;
        }

        private void NewWindowButton_Click(object sender, EventArgs e)
        {
            CarListForm newCarListForm = new CarListForm(carsDocument);
            newCarListForm.MdiParent = this;
            newCarListForm.Show();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            CarListForm newCarListForm = new CarListForm(carsDocument);
            newCarListForm.MdiParent = this;
            newCarListForm.Show();
        }

        private void exitButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private CarsDocument carsDocument = new CarsDocument();

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CarListForm newCarListForm = new CarListForm(carsDocument);
            newCarListForm.MdiParent = this;
            newCarListForm.Show();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
