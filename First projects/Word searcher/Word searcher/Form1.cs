using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Word_searcher
{
    public partial class Form1 : Form
    {
        string Dictionary = "";
        public Form1()
        {
            InitializeComponent();
            StreamReader sr = new StreamReader("ENG.txt");

            while (!sr.EndOfStream)
                Dictionary += sr.ReadLine();
        }

        void swap(ref int[] a, int i, int j)
        {
            int s = a[i];
            a[i] = a[j];
            a[j] = s;
        }

        bool NextSet(ref int[] a, int n)
        {
            int j = n - 2;
            while (j != -1 && a[j] >= a[j + 1]) j--;
            if (j == -1)
                return false;
            int k = n - 1;
            while (a[j] >= a[k]) k--;
            swap(ref a, j, k);
            int l = j + 1, r = n - 1; // сортуємо решту послідовності
            while (l < r)
                swap(ref a, l++, r--);
            return true;
        }

        void wordBuilder(ref int[] a, int n, string text)
        {
            string output = ",";
            for (int i = 0; i < n; i++)
                output += text[a[i] - 1];
            output+=",";
            if (Dictionary.IndexOf(output) != -1)
            {
                listBox1.Items.Add(output.Replace(",",""));
            }
        }

        public IEnumerable<string> Bruteforce(int length,string alphabet)
        {
            if (length > 0 && alphabet != null)
            {
                int[] indexes = new int[length];
                int index = 0;
                int iteration = 0;

                var permutations = Math.Pow(alphabet.Length, length);
                while (iteration < permutations)
                {
                    var target = alphabet[index].ToString();
                    for(int i = 1; i < length; i++)
                    {
                        if (indexes[i - 1] >= alphabet.Length)
                        {
                            indexes[i]++;
                            indexes[i - 1] = 0;
                        }
                        target += alphabet[indexes[i] < alphabet.Length ? indexes[i] : 0];
                    }
                    indexes[0] = ++index;
                    if (index >= alphabet.Length)
                        index = 0;
                    yield return target;
                    iteration++;
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            listBox1.Items.Clear();
            int n;
            string Word = textBox1.Text;
            if (!int.TryParse(textBox2.Text, out n))
                MessageBox.Show("Incorect format, only int");
            var chars = new HashSet<char>(Word);
            int[] a;
            if (Word == "")
            {
                MessageBox.Show("Введіть слово");
            }
            else
            {
                a = new int[Word.Length];

                if (checkBox1.Checked)
                {
                    for (int i = 0; i < Word.Length; i++)
                        a[i] = i + 1;
                    wordBuilder(ref a, Word.Length, Word);
                    while (NextSet(ref a, Word.Length))
                        wordBuilder(ref a, Word.Length, Word);
                }
                else
                {
                    foreach (var s in Bruteforce(n, Word))
                        if (Dictionary.IndexOf("," + s + ",") != -1)
                            listBox1.Items.Add(s);
                }
            }
        }
    }
}
