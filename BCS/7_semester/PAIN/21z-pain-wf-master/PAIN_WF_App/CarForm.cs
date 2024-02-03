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
    public partial class CarForm : Form
    {
        private Car car;
        private List<Car> carList;

        public string Brand { get => brandTextBox.Text; }

        public int MaxSpeed { get => Convert.ToInt32(maxSpeedTextBox.Text);  }

        public DateTime ProductionDate { get => productionDatePicker.Value;  }

        public Car.CarType CarType { get => carTypeControl1.CarType;  }

        private bool editMode;

        public CarForm(Car car, List<Car> carList)
        {
            InitializeComponent();
            this.car = car;
            this.carList = carList;
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            brandTextBox.CausesValidation = true;
            if (ValidateChildren())
                DialogResult = DialogResult.OK;
        }

        private void CarForm_Load(object sender, EventArgs e)
        {
            if (car != null)
            {
                brandTextBox.Text = car.Brand;
                maxSpeedTextBox.Text = car.MaxSpeed.ToString();
                productionDatePicker.Value = car.ProductionDate;
                carTypeControl1.CarType = car.Type;
            }
            else
            {
                brandTextBox.Text = "Audi";
                maxSpeedTextBox.Text = "180";
                productionDatePicker.Value = DateTime.Now;
                carTypeControl1.CarType = Car.CarType.Passenger;
            }
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void brandTextBox_Validating(object sender, CancelEventArgs e)
        {
            foreach (Car car in carList)
            {
                if (brandTextBox.Text.Equals(car.Brand) && !ReferenceEquals(car, this.car))
                {
                    Console.WriteLine(DialogResult);
                    e.Cancel = true;
                    errorProvider.SetError(brandTextBox, $"{car.Brand} already exists");
                }
            }
        }

        private void maxSpeedTextBox_Validating(object sender, CancelEventArgs e)
        {
            try
            {
                int maxSpeed = Convert.ToInt32(maxSpeedTextBox.Text);
                if (maxSpeed < 0)
                {
                    e.Cancel = true;
                    errorProvider.SetError(maxSpeedTextBox, "Speed should be positive");
                }
            }
            catch (System.OverflowException)
            {
                e.Cancel = true;
                errorProvider.SetError(maxSpeedTextBox, "Speed too large!");
            }
            catch
            {
                e.Cancel = true;
                errorProvider.SetError(maxSpeedTextBox, "Speed should be a number");
            }
        }

        private void maxSpeedTextBox_Validated(object sender, EventArgs e)
        {
            errorProvider.SetError(maxSpeedTextBox, "");
        }

        private void brandTextBox_Validated(object sender, EventArgs e)
        {
            errorProvider.SetError(brandTextBox, "");
        }

        private void productionDatePicker_Validating(object sender, CancelEventArgs e)
        {
            if (productionDatePicker.Value > DateTime.Now)
            {
                e.Cancel = true;
                errorProvider.SetError(productionDatePicker, "Production date should be in the past");
            }
                
        }

        private void productionDatePicker_Validated(object sender, EventArgs e)
        {
            errorProvider.SetError(productionDatePicker, "");
        }
    }
}
