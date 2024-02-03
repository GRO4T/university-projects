using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CarsWPF.MVVM
{
    public class Filter
    {
        public Filter(string name, Func<object, bool> condition)
        {
            Name = name;
            Condition = condition;
        }
        public string Name { get; set; }
        public Func<object, bool> Condition { get; set; }
    }
}
