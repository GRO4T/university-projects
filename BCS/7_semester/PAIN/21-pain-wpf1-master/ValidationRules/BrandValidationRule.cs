using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace CarsWPF.ValidationRules
{
    public class BrandValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            if (value == null)
                return new ValidationResult(false, "Brand cannot be null");
            string brand = value.ToString();
            if (brand.Equals(""))
                return new ValidationResult(false, "Brand cannot be empty.");
            if (!brand.All(Char.IsLetterOrDigit))
                return new ValidationResult(false, "Brand must contain only letters or digits");
            return ValidationResult.ValidResult;

        }
    }
}
