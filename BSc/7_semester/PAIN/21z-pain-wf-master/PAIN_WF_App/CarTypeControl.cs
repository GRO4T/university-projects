using PAIN_WF_App.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Design;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace PAIN_WF_App
{
    [System.Security.Permissions.PermissionSet(System.Security.Permissions.SecurityAction.Demand, Name = "FullTrust")]
    public class CarTypeEditor : System.Drawing.Design.UITypeEditor
    {
        public CarTypeEditor()
        {
        }

        public override System.Drawing.Design.UITypeEditorEditStyle GetEditStyle(System.ComponentModel.ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.DropDown;
        }

        public override object EditValue(System.ComponentModel.ITypeDescriptorContext context, System.IServiceProvider provider, object value)
        {
            // Return the value if the value is not of type Int32, Double and Single.
            if (value.GetType() != typeof(Car.CarType))
                return value;

            // Uses the IWindowsFormsEditorService to display a 
            // drop-down UI in the Properties window.
            IWindowsFormsEditorService edSvc = (IWindowsFormsEditorService)provider.GetService(typeof(IWindowsFormsEditorService));
            if (edSvc != null)
            {
                // Display an angle selection control and retrieve the value.
                CarTypeControl carTypeControl = new CarTypeControl((Car.CarType)value);
                edSvc.DropDownControl(carTypeControl);

                // Return the value in the appropraite data format.
                if (value.GetType() == typeof(Car.CarType))
                    return carTypeControl.CarType;
            }
            return value;
        }
    }

    public class CarTypeControl : System.Windows.Forms.UserControl
    {
        [Category("My Properties")]
        [Description("Type of car")]
        [DisplayName("Car Type")]
        [BrowsableAttribute(true)]
        [EditorAttribute(typeof(CarTypeEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public Car.CarType CarType {
            get => carType;
            set {
                carType = value;
                carTypeLabel.Text = CarType.ToString();
                switch (carType)
                {
                    case Car.CarType.Passenger:
                        carTypePicture.Image = Resources.car;
                        break;
                    case Car.CarType.Motorcycle:
                        carTypePicture.Image = Resources.motorcycle;
                        break;
                    case Car.CarType.Truck:
                        carTypePicture.Image = Resources.truck;
                        break;
                    default:
                        throw new Exception("Invalid car type!");
                }
            }
        }
        private Car.CarType carType;

        private PictureBox carTypePicture;
        private Label carTypeLabel;

        private FlowLayoutPanel controlLayout;

        public CarTypeControl(): this(Car.CarType.Passenger)
        {
        }

        public CarTypeControl(Car.CarType initialType)
        {
            InitializeComponent();
            this.CarType = initialType;
        }

        public void InitializeComponent()
        {
            AutoSize = true;

            controlLayout = new FlowLayoutPanel();
            controlLayout.FlowDirection = FlowDirection.TopDown;
            
            carTypePicture = new PictureBox();
            carTypePicture.Image = Resources.cars;
            carTypePicture.SizeMode = PictureBoxSizeMode.StretchImage;
            carTypePicture.MouseDown += carTypePicture_OnMouseDown;

            carTypeLabel = new Label();
            carTypeLabel.Text = CarType.ToString();
            carTypeLabel.TextAlign = ContentAlignment.MiddleCenter;

            controlLayout.Controls.Add(carTypePicture);
            controlLayout.Controls.Add(carTypeLabel);

            // Add the controls to the user control.
            Controls.AddRange(new System.Windows.Forms.Control[]
            {
                controlLayout
            });
        }

        private void carTypePicture_OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            CarType = (Car.CarType)((int)(CarType + 1) % Enum.GetNames(typeof(Car.CarType)).Length);
            Console.WriteLine($"Car type: {CarType}");
        }
    }
}
