///////////////////////////////////////////////////////////////////////////
// program.cs ----Autotest unit for Project 3                            //
// ver 1.3                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
//  Source: MainWindow.xaml.cs from prof Jim Fawcett                     //
///////////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp1
{
    class Program
    {
        public void requirement1()
        {
            Console.WriteLine("Requirement2: Using WPF and C# to implement the client GUI");
            Console.WriteLine("GUI-OOD-PJ3 is the package that uses WPF and C# to inplement");
            Console.WriteLine("GUI Client for this project");
        }

        public void requirement2()
        {
            Console.WriteLine("Requirement3: provide an interface for code publisher");
            Console.WriteLine("in project2. Cdinterface.h provides an interface to ");
            Console.WriteLine("access function in code publisher. It includes dependency.h ");
            Console.WriteLine("to generate html and add dependency link. And it includes ");
            Console.WriteLine("disply.h to display html file in browser. It includes Executive.h");
            Console.WriteLine("to process command line. It creat an objective factory to be used");
            Console.WriteLine("by C++/CLI package");
        }

        public void requirement3()
        {
            Console.WriteLine("Requirement4: build the Publisher as a dynamic link library that ");
            Console.WriteLine("exposes its interface and object factory. Cdpublisher is built as ");
            Console.WriteLine("a static library which expose its interface and object factory. Transl ");
            Console.WriteLine("is built as a dynamic link and expose it interface and object factory");
            Console.WriteLine("to be used by GUI Client");
        }

        public void requirement4()
        {
            Console.WriteLine("Requirement5: provide a C++\\CLI translater that delegates calls ");
            Console.WriteLine("from the Client GUI into the publisher's methods, as declared in its interface.");
            Console.WriteLine("itanslator provides an interface Itranslator and object factory TraslatorFactory");
            Console.WriteLine("to delegate methods from code publisher to be used in GUI and C# code. ");
        }

        public void requirement5()
        {
            Console.WriteLine("Requirement6: provide a Client package that implements a WPF GUI with a least two ");
            Console.WriteLine("views, one for navigation, and one to display file conversion results.");
            Console.WriteLine("GUI-OOD-Pr3 provides a GUI Client that you can navigate files in root directory");
            Console.WriteLine("and browse html files in GUI. Tab Find Source file provides view of directories ");
            Console.WriteLine("and view of files in the directory. It provide a button Publish Files to process");
            Console.WriteLine("files in path defined in Command lines and you can change regex in textbox I set ");
            Console.WriteLine("it wiil process default command lines regex if you do not input anything");
            Console.WriteLine("When you select files in Find Source file tab, there will be a popup window");
            Console.WriteLine("you can convert selected files in popup windows. Tab Convert Files shows");
            Console.WriteLine("converted files in ../ConvertedWebpages when you select files in the list ");
            Console.WriteLine("the thired tab Browse Result will show exacted .html files with ie11.");
            Console.WriteLine("double click files in Convert Files will invoke Chrome to show .html files.");
        }

        public void requirement6()
        {
            Console.WriteLine("Requirement 7: Auto test. Well this is Autotest Units, and GUI will be invoked later, it will process command line");
            Console.WriteLine("Command line will be showed on GUI");
        }

        public void GUI_USE()
        {
            Console.WriteLine("This is the document to show you how to use this GUI.");
            Console.WriteLine("1. Click Publish Files button without other operation will process arguments from command line");
            Console.WriteLine("-- and convert files to ../ConvertedWebpages");
            Console.WriteLine("2. you can chenge path in batch files to process files in other directory");
            Console.WriteLine("3. you can change regex to convert other files with different initial letters");
            Console.WriteLine("4. by changing *.cpp *.h in command line, you can convert different types of files");
            Console.WriteLine("-- files navigator in Find source files page will only show file types in command line");
            Console.WriteLine("5. you can select files from files nacigator in Find Source Files, and add it to a pop up");
            Console.WriteLine("-- window. in the pop up window you can convert selected files directly");
            Console.WriteLine("6. in Conver Files page you, by single click files, you can browse .html files in ");
            Console.WriteLine("-- Browse Result page. By double click files in Convert Files page, you can open files in Browser");
        }

        static void Main(string[] args)
        {
            string[] temp = Environment.GetCommandLineArgs();
            List<String> temp1 = new List<string>();
            foreach(var i in temp)
            {
                temp1.Add(i);
            }
            Program pg= new Program();
            pg.requirement1();
            Console.WriteLine();
            Console.WriteLine();
            pg.requirement2();
            Console.WriteLine();
            Console.WriteLine();
            pg.requirement3();
            Console.WriteLine();
            Console.WriteLine();
            pg.requirement4();
            Console.WriteLine();
            Console.WriteLine();
            pg.requirement5();
            Console.WriteLine();
            Console.WriteLine();
            pg.requirement6();
            Console.WriteLine();
            Console.WriteLine();
            pg.GUI_USE();
            Console.WriteLine();
            Console.WriteLine();
            Console.Write("Done");
            Console.ReadKey();
        }
    }
}
