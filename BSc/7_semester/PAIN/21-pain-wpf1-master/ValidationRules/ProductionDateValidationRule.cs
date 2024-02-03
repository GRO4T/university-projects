using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace CarsWPF.ValidationRules
{
    public class ProductionDateValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            if (value == null)
                return new ValidationResult(false, "Production date cannot be null");
            if ((DateTime) value > DateTime.Now)
                return new ValidationResult(false, "Production date cannot be in the future");
            return ValidationResult.ValidResult;
        }
    }
}
