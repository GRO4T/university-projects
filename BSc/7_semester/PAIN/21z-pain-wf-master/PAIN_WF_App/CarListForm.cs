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
    public partial class CarListForm : Form
    {
        private CarsDocument Document { get; }

        public CarListForm(CarsDocument document)
        {
            Document = document;
            InitializeComponent();
            carFilterComboBox.SelectedIndex = 0;
            carListView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
            carListView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
        }

        private void CarListForm_Load(object sender, EventArgs e)
        {
            UpdateItems();
            Document.AddCarEvent += Document_AddCarEvent;
            Document.UpdateCarEvent += Document_UpdateCarEvent;
            Document.DeleteCarEvent += Document_DeleteCarEvent;
            UpdateCommandAvailability();
        }

        private void Document_AddCarEvent(Car car)
        {
            if (ShowCar(car))
            {
                ListViewItem item = new ListViewItem();
                item.Tag = car;
                UpdateItem(item);
                carListView.Items.Add(item);
                UpdateCarCount();
            }
        }

        private void Document_UpdateCarEvent(Car car)
        {
            foreach (ListViewItem item in carListView.Items)
            {
                if (ReferenceEquals(item.Tag, car))
                {
                    if (ShowCar(car))
                        UpdateItem(item);
                    else
                    {
                        carListView.Items.Remove(item);
                        UpdateCarCount();
                    }
                    return;
                }
            }

            if (ShowCar(car))
            {
                var item = new ListViewItem();
                item.Tag = car;
                carListView.Items.Add(item);
                UpdateCarCount();
            }
        }

        private void Document_DeleteCarEvent(Car car)
        {
            foreach (ListViewItem item in carListView.Items)
            {
                if (ReferenceEquals(item.Tag, car))
                {
                    carListView.Items.Remove(item);
                    UpdateCarCount();
                    return;
                }
            }
        }

        private void UpdateItems()
        {
            carListView.Items.Clear();
            foreach (Car car in Document.Cars)
            {
                if (ShowCar(car))
                {
                    ListViewItem item = new ListViewItem();
                    item.Tag = car;
                    UpdateItem(item);
                    carListView.Items.Add(item);
                }
            }
            UpdateCarCount();
        }

        private void UpdateItem(ListViewItem item)
        {
            Car car = (Car)item.Tag;
            while (item.SubItems.Count < 4)
                item.SubItems.Add(new ListViewItem.ListViewSubItem());
            item.SubItems[0].Text = car.Brand;
            item.SubItems[1].Text = $"{car.MaxSpeed}km/h";
            item.SubItems[2].Text = car.ProductionDate.ToShortDateString();
            item.SubItems[3].Text = car.Type.ToString();
        }

        private void UpdateCommandAvailability()
        {
            bool editEnabled = carListView.SelectedItems.Count == 1;
            bool deleteEnabled = carListView.SelectedItems.Count > 0;
            editCarButton.Enabled = editEnabled;
            deleteCarButton.Enabled = deleteEnabled;
            editToolStripMenuItem1.Enabled = editEnabled;
            deleteToolStripMenuItem.Enabled = deleteEnabled;
            editContextMenuItem.Enabled = editEnabled;
            deleteContextMenuItem.Enabled = deleteEnabled;
            
        }

        private void UpdateCarCount()
        {
            carCountStatusLabel.Text = carListView.Items.Count.ToString();
        }

        private bool ShowCar(Car car)
        {
            switch (carFilterComboBox.SelectedIndex)
            {
                case 0: // All
                    return true;
                case 1: // Max speed less than 100
                    return car.MaxSpeed < 100;
                case 2: // Max speed at least 100
                    return car.MaxSpeed >= 100;
                default:
                    throw new Exception("Invalid filter.");
            }
        }

        private void addCarButton_Click(object sender, EventArgs e)
        {
            AddCar();
        }
        
        private void AddCar()
        {
            CarForm carForm = new CarForm(null, Document.Cars);
            if (carForm.ShowDialog() == DialogResult.OK)
            {
                Car newCar = new Car(carForm.Brand, carForm.MaxSpeed, carForm.ProductionDate, carForm.CarType);
                Document.AddCar(newCar);
            }
        }

        private void editCarButton_Click(object sender, EventArgs e)
        {
            EditCar();
        }

        private void EditCar()
        {
            if (carListView.SelectedItems.Count != 1)
                return;
            Car car = (Car)carListView.SelectedItems[0].Tag;
            CarForm carForm = new CarForm(car, Document.Cars);
            if (carForm.ShowDialog() == DialogResult.OK)
            {
                car.Brand = carForm.Brand;
                car.MaxSpeed = carForm.MaxSpeed;
                car.ProductionDate = carForm.ProductionDate;
                car.Type = carForm.CarType;
                Document.UpdateCar(car);
            }
        }

        private void deleteCarButton_Click(object sender, EventArgs e)
        {
            DeleteCar();
        }

        private void DeleteCar()
        {
            if (carListView.SelectedItems.Count == 0) return;

            if (MessageBox.Show("Are you sure you want to delete selected cars?") == DialogResult.OK)
            {
                while (carListView.SelectedItems.Count > 0)
                {
                    Car car = (Car)carListView.SelectedItems[0].Tag;
                    carListView.Items.Remove(carListView.SelectedItems[0]);
                    Document.DeleteCar(car);
                }
            }
        }

        private void CarListForm_Activated(object sender, EventArgs e)
        {
            ToolStripManager.Merge(menuStrip1, ((MainForm)MdiParent).menuStrip1);
            ToolStripManager.Merge(toolStrip, ((MainForm)MdiParent).toolStrip1);
        }

        private void CarListForm_Deactivate(object sender, EventArgs e)
        {
            ToolStripManager.RevertMerge(((MainForm)MdiParent).menuStrip1, menuStrip1);
            ToolStripManager.RevertMerge(((MainForm)MdiParent).toolStrip1, toolStrip);
        }

        private void CarListForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            var mdi_parent = ((MainForm)MdiParent);
            if (e.CloseReason == CloseReason.UserClosing && mdi_parent.MdiChildren.Length == 1)
                e.Cancel = true;
        }

        private void carListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateCommandAvailability();
        }

        private void carFilterComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateItems();
        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddCar();
        }

        private void editToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            EditCar();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DeleteCar();
        }

        private void addContextMenuItem_Click(object sender, EventArgs e)
        {
            AddCar();
        }

        private void editContextMenuItem_Click(object sender, EventArgs e)
        {
            EditCar();
        }

        private void deleteContextMenuItem_Click(object sender, EventArgs e)
        {
            DeleteCar();
        }
    }
}
