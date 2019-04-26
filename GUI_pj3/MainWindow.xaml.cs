///////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs ----Mainwindow.cs of GUI for Project 3             //
// ver 1.3                                                               //
//                                                                       // 
//  Platform:      Dell Alienware 17R5, Windows 10                       //
//  Application:   Project for CSE687 - OOD Projects                     //
//  Author:        Xiao Chen                                             //
//  Source: MainWindow.xaml.cs from prof Jim Fawcett                     //
///////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class MainWindow that provides tabs:
 * - Find Libs: Navigate through local directory to find files for testing
 *   - Shows local directories and files
 *   - Can navigate by double clicking directories
 *   - Can select files by selecting.  That displays selections in a popup window.
 * - Request Tests: left for students
 * - Show Results:  left for students
 *  - Use functions from itranslator to use function of code publisher from project2
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs
 * SelectionWindow.xaml, SelectionWindow.xaml.cs
 * Itranslator.h translatorcpp.cpp
 * 
 * Maintenance History:
 * --------------------
 * ver 1.3 : 7 April 2019
 * - add button to use code publisher
 * - add a tab to show html files
 * - add webbrowser to browse html files converted 
 * ver 1.2 : 28 Mar 2019
 * - fixed bug in DirsMouse_DoubleClick by using try catch block to
 *   handle exception that occurs if user clicks on unpopulated listbox item.
 * ver 1.1 : 16 Nov 2018
 * - fixed bug in Files_SelectionChanged by checking e.AddedItems.Count
 *   and returning if 0.
 * ver 1.0 : 15 Nov 2018
 * - first release
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Threading;
using System.ComponentModel;
using Microsoft.Win32;
using MsgPassingCommunication;

namespace PrototypeGui_OOD_Pr4
{
    ///////////////////////////////////////////////////////////////////
    // MainWindow class
    // - Provides navigation and selection to find libraries to test.
    // - Creates a popup window to handle selections.

    public partial class MainWindow : Window
    {
        public string path { get; set; }
        List<string> selectedFiles { get; set; } = new List<string>();
        public SelectionWindow swin { get; set; } = null;
        bool unselecting = false;

        private Stack<string> pathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();


        //----< process incoming messages on child thread >----------------

        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearDirs()
        {
            Dirsremo.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------

        private void addDir(string dir)
        {
            Dirsremo.Items.Add(dir);
        }
        //----< function dispatched by child thread to main thread >-------

        private void insertParent()
        {
            Dirsremo.Items.Insert(0, "..");
        }
        //----< function dispatched by child thread to main thread >-------

        private void clearFiles()
        {
            Filesremo.Items.Clear();
        }
        //----< function dispatched by child thread to main thread >-------

        private void addFile(string file)
        {
            Filesremo.Items.Add(file);
        }
        //----< add client processing for message with key >---------------

        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }
        //----< load getDirs processing into dispatcher dictionary >-------

        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    clearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            addDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
                {
                    insertParent();
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }
        //----< load getFiles processing into dispatcher dictionary >------

        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getFiles", getFiles);
        }
        //----< load all dispatcher processing >---------------------------

        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
        }

        private void loadremote()
        {
            // start Comm
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost";
            endPoint_.port = 8082;
            translater = new Translater();
            translater.listen(endPoint_);

            // start processing messages
            processMessages();

            // load dispatcher
            loadDispatcher();

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;

            //PathTextBlock.Text = "Storage";
            pathStack_.Push("../Storage");
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
        }


        public MainWindow()
        {
            InitializeComponent();
        }
        //----< open window showing contents of project directory >------

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            loadremote();

            path = Directory.GetCurrentDirectory();
            path = getAncestorPath(3, path);
            LoadNavTab(path);

            string[] regex = Environment.GetCommandLineArgs();
            List<String> temp1 = new List<string>();
            foreach (var i in regex)
            {
                if (i.Contains(".*"))
                {
                    Regex.Text = i;
                    Regex2.Text = i;
                }
            }
        }
        //----< find parent paths >--------------------------------------

        string getAncestorPath(int n, string path)
        {
            for (int i = 0; i < n; ++i)
                path = Directory.GetParent(path).FullName;
            return path;
        }
        //----< file Find Libs tab with subdirectories and files >-------
        
        void LoadNavTab(string path)
        {
            Dirs.Items.Clear();
            CurrPath.Text = path;
            string[] dirs = Directory.GetDirectories(path);
            Dirs.Items.Add("..");
            foreach (string dir in dirs) {
                DirectoryInfo di = new DirectoryInfo(dir);
                string name = System.IO.Path.GetDirectoryName(dir);
                Dirs.Items.Add(di.Name);
            }
            string[] regex = Environment.GetCommandLineArgs();
            List<string> reg = new List<string>();
            SearchBox.Text = "";
            int aa = 0;
            foreach (var i in regex){
                if (i.Contains("*.")){
                    SearchBox.Text = SearchBox.Text+i;
                    reg.Add(i.Remove(0, 1));
                    aa++;
                }
            }
            Files.Items.Clear();
            string[] files = Directory.GetFiles(path);
            if (aa != 0) {
                foreach (string file in files) {
                    if (reg.Contains(System.IO.Path.GetExtension(file))) {
                        string name = System.IO.Path.GetFileName(file);
                        Files.Items.Add(name);
                    }
                }
            }
            else {
                foreach (string file in files){
                        string name = System.IO.Path.GetFileName(file);
                        Files.Items.Add(name);
                }
            }
            string[] temp = Environment.GetCommandLineArgs();
            string cmd_path = "";
            for(int i = 1;i<temp.Length;i++)
            {
                cmd_path = cmd_path + temp[i];
            }
            cmd.Text = cmd_path;
        }
        //----< Load html dirs and files >-------------------------------

        void LoadHtmlFile(string path)
        {
            CurrPathCF.Text = path;
            FilesCF.Items.Clear();
            string[] files = Directory.GetFiles(path);
            foreach (string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                FilesCF.Items.Add(name);
            }
        }
        //----< handle selections in files listbox >---------------------
        
        private void Files_SelectionChanged(
          object sender, SelectionChangedEventArgs e
        )
        {
            if (unselecting)
            {
                unselecting = false;
                return;
            }
            if (e.AddedItems.Count == 0)
                return;
            string selStr = e.AddedItems[0].ToString();
            selStr = System.IO.Path.Combine(path, selStr);
            string[] regex = Environment.GetCommandLineArgs();
            List<string> reg = new List<string>();
            foreach (var i in regex)
            {
                if (i.Contains("*."))
                {
                    reg.Add(i.Remove(0, 1));
                }
            }
            if (!selectedFiles.Contains(selStr))
            {
                if(reg.Contains(System.IO.Path.GetExtension(selStr)))
                {
                    if (swin == null)
                    {
                        swin = new SelectionWindow();
                        swin.setMainWindow(this);
                    }
                    swin.Show();
                    selectedFiles.Add(selStr);
                    swin.Add(selStr);
                }
            }
        }
        //----< Single Click items inside html files list box open webbrowser in webbrowser tab>------

        private void cFiles_SelectionChanged(
          object sender, SelectionChangedEventArgs e
        )
        {
            if (unselecting)
            {
                unselecting = false;
                return;
            }
            if (e.AddedItems.Count == 0)
                return;
            string selStr = e.AddedItems[0].ToString();
            selStr = System.IO.Path.Combine(path, selStr);
            SetWebBrowserFeatures(11);
            webbrowser.Navigate(selStr);
        }
        //----< button to publish code into html files >----------------------------- 

        private void Publish_files_Click(object sender, RoutedEventArgs e)
        {
            try {
                string[] temp = Environment.GetCommandLineArgs();
                List<String> temp1 = new List<string>();
                foreach (var i in temp)
                {
                    if (i.Contains(".*") && Regex.Text != "")
                    {
                        temp1.Add(Regex.Text);
                        continue;
                    }
                    temp1.Add(i);
                }
                Itranslator trans = TraslatorFactory.createTranslator();
                trans.pcfile(temp1.Count, temp1);
            }
            catch
            {

            }
        }

        //----< unselect files called by unloading SelectionWindow >-----

        public void unselectFiles()
        {
            unselecting = true;  // needed to avoid using selection logic
                                 //selectedFiles.Clear();
            Files.UnselectAll();
            FilesCF.UnselectAll();
            selectedFiles.Clear();
        }
        //----< move into double-clicked directory, display contents >---

        private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            try
            {
                string selectedDir = Dirs.SelectedItem.ToString();
                if (selectedDir == "..")
                    path = getAncestorPath(1, path);
                else
                    path = System.IO.Path.Combine(path, selectedDir);
                LoadNavTab(path);
            }
            catch
            {
                // just return
            }
        }
        //----< open html files in chrome>-------------------------------

        private void Convert_MOuseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            try
            {
                path = Directory.GetCurrentDirectory();
                path = getAncestorPath(3, path);
                path = path + "\\ConvertedWebpages\\";
                path = System.IO.Path.GetFullPath(path);
                string selectedfiles = FilesCF.SelectedItem.ToString();
                selectedfiles = path + selectedfiles;
                Itranslator trans = TraslatorFactory.createTranslator();
                //SetWebBrowserFeatures(11);
                //webbrowser.Navigate(selectedfiles);
                trans.display(selectedfiles);
            }
            catch
            {
                // just return
            }
        }

        //----< shut down the SelectionWindow if open >------------------

        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            swin.Close();
        }

        private void CFiles_TabClick(object sender, SelectionChangedEventArgs e)
        {
            if(e.Source is TabControl)
            {
                try
                {
                    if (cpptab.IsSelected == true)
                    {
                        path = Directory.GetCurrentDirectory();
                        path = getAncestorPath(3, path);
                        LoadNavTab(path);
                    }
                    else if (htmlfiles.IsSelected == true)
                    {
                        path = Directory.GetCurrentDirectory();
                        path = getAncestorPath(3, path);
                        path = path + "\\ConvertedWebpages";
                        path = System.IO.Path.GetFullPath(path);
                        LoadHtmlFile(path);
                    }
                    else if(remotefiles.IsSelected == true)
                    {
                        loadremote();
                    }
                }
                catch
                {

                }
            }
        }
        //-----< set ie browser to ie11>------------------------------------

        static void SetWebBrowserFeatures(int ieVersion)
        {
            if (LicenseManager.UsageMode != LicenseUsageMode.Runtime)
                return;
            var appName = System.IO.Path.GetFileName(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName);
            UInt32 ieMode = GeoEmulationModee(ieVersion);
            var featureControlRegKey = @"HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\Main\FeatureControl\";
            Registry.SetValue(featureControlRegKey + "FEATURE_BROWSER_EMULATION",
                appName, ieMode, RegistryValueKind.DWord);
            Registry.SetValue(featureControlRegKey + "FEATURE_ENABLE_CLIPCHILDREN_OPTIMIZATION",
                appName, 1, RegistryValueKind.DWord);
        }
        //----< select ie browser version >----------------------------------

        static UInt32 GeoEmulationModee(int browserVersion)
        {
            UInt32 mode = 11000; // Internet Explorer 11. Webpages containing standards-based !DOCTYPE directives are displayed in IE11 Standards mode. 
            switch (browserVersion)
            {
                case 7:
                    mode = 7000; // Webpages containing standards-based !DOCTYPE directives are displayed in IE7 Standards mode. 
                    break;
                case 8:
                    mode = 8000; // Webpages containing standards-based !DOCTYPE directives are displayed in IE8 mode. 
                    break;
                case 9:
                    mode = 9000; // Internet Explorer 9. Webpages containing standards-based !DOCTYPE directives are displayed in IE9 mode.                    
                    break;
                case 10:
                    mode = 10000; // Internet Explorer 10.
                    break;
                case 11:
                    mode = 11000; // Internet Explorer 11
                    break;
            }
            return mode;
        }


    }
}
