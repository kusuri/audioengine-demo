﻿using System;
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

namespace AudioEngineDemo
{
    public partial class MainWindow : Window, DemoAudioEngine.DemoAudioEngineCallbacksIntf
    {
        public MainWindow()
        {
            demoAudioEngine = CreateDemoAudioEngine();
            InitializeComponent();

            demoAudioEngine.setRoomSize((float)roomSizeSlider.Value);
            demoAudioEngine.setLowpassCutoff((float)lowPassSlider.Value);
            demoAudioEngine.setCallback(this);

            statusLabel.Content = "Playback stopped";

            juceWindowHolder = new JuceWindowHolder(ref demoAudioEngine);
            juceStage.Child = juceWindowHolder;
        }

        ~MainWindow()
        {
            if (juceWindowHolder != null)
            {
                juceWindowHolder.Dispose();
                juceWindowHolder = null;
            }

            demoAudioEngine = null;
        }

        private void PlayButtonClicked(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            dlg.DefaultExt = ".mp3";
            dlg.Filter = "MP3 Files (*.mp3)|*.mp3|Wav Files (*.wav)|*.wav|m4a Files (*.m4a)|*.m4a";

            Nullable<bool> result = dlg.ShowDialog();

            if (result == true)
            {
                string url = (new System.Uri(dlg.FileName)).AbsoluteUri;
                demoAudioEngine.play(url);

                statusLabel.Content = "File is playing...";
            }
        }

        private void StopButtonClicked(object sender, RoutedEventArgs e)
        {
            demoAudioEngine.stop();
            statusLabel.Content = "Playback stopped";
        }

        private void PauseButtonClicked(object sender, RoutedEventArgs e)
        {
            demoAudioEngine.pause();
        }

        private void ResumeButtonClicked(object sender, RoutedEventArgs e)
        {
            demoAudioEngine.resume();
        }

        private void RoomSizeSliderValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            demoAudioEngine.setRoomSize((float)e.NewValue);
        }

        private void LowPassSliderValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            demoAudioEngine.setLowpassCutoff((float)e.NewValue);
        }

        public void filePlaybackFinished()
        {
            statusLabel.Content = "Playback stopped";
        }

        [System.Runtime.InteropServices.DllImport("DemoAudioEngine.dll")]
        private static extern DemoAudioEngine.DemoAudioEngineIntf CreateDemoAudioEngine();

        private DemoAudioEngine.DemoAudioEngineIntf demoAudioEngine = null;
        private JuceWindowHolder juceWindowHolder = null;
    }
}
