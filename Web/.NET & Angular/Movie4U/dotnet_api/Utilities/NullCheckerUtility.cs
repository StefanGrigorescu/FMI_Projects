using System.Reflection;

namespace Movie4U.Utilities
{
    public static class NullCheckerUtility
    {
        /** <summary>
         * Counts null or empty (for strings) fields of given object.
         * <param> obj object to be checked. </param>
         * <return> counter (integer). </return> 
         * </summary> **/
        public static int NullCount(object obj)
        {
            int counter = 0;
            foreach (PropertyInfo pi in obj.GetType().GetProperties())
            {
                var value = pi.GetValue(obj);
                if (pi.PropertyType == typeof(string)  &&  string.IsNullOrEmpty((string)value))
                    counter++;
                else if (value == null)
                    counter++;
            }
            return counter;
        }

        /** <summary>
         * Checks wether the given object has null or empty (for strings) fields
         * </summary> */
        public static bool HasNulls(object obj)
        {
            foreach (PropertyInfo pi in obj.GetType().GetProperties())
            {
                var value = pi.GetValue(obj);
                if (pi.PropertyType == typeof(string) && string.IsNullOrEmpty((string)value))
                    return true;
                else if (value == null)
                    return true;
            }
            return false;
        }

    }
}
