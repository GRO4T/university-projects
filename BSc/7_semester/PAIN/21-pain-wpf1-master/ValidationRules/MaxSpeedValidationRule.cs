using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace CarsWPF.ValidationRules
{
    public class MaxSpeedValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            if (value == null)
                return new ValidationResult(false, "Speed cannot be null");
            try
            {
                int maxSpeed = Convert.ToInt32(value.ToString());
                if (maxSpeed <= 0)
                    return new ValidationResult(false, "Speed should be positive");
            }
            catch (System.OverflowException)
            {
                return new ValidationResult(false, "Speed too large!");
            }
            catch
            {
                return new ValidationResult(false, "Speed should be a number");
            }
            return ValidationResult.ValidResult;
        }
    }
}
