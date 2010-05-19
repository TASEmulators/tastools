using System;
using System.Collections.Generic;
using System.Text;

namespace MovieSplicer.Data.moviegenerator
{
    class IntegerBase
    {
        private int numericbase;
        private int[] digits;
        private bool overflowed;

        IntegerBase( int numericbase, int numberdigits )
        {
            this.numericbase = numericbase;
            digits = new int[numberdigits];
            Set(0);
        }

        public void Add( int value )
        {
            int i = 0;
            while (value > 0)
            {
                int d = digits[i] + value;
                value = d / numericbase;
                digits[i] = d - numericbase*value;
                ++i;
                if (i >= digits.Length)
                {
                    overflowed = true;
                    Set(0);
                    return;
                };
            }
        }

        public void Set(int value)
        {
            for (int i = 0; i < digits.Length; i++)
            {
                digits[i] = 0;
            }
            Add(value);
        }

        public bool isOverflowed()
        {
            return overflowed;
        }
        public void resetOverflowed()
        {
            overflowed = false;
        }
    }
}
