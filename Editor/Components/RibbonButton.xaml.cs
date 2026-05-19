using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Editor.Components {
	/// <summary>
	/// Interaction logic for RibbonButton.xaml
	/// </summary>
	public partial class RibbonButton : UserControl {
		public RibbonButton() {
			InitializeComponent();
			this.DataContext = this;
		}

		public required string Title { get; set; }
		public required string IconSource { get; set; }
	}

}