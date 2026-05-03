
namespace Editor {
	internal class Program {
		public const string ResourcesPath = @"..\..\..\..\..\Resources";

		static void Main(string[] args) {
			RuntimeComm.TestPrint();
			Console.WriteLine("Initialising Refraction Editor...");
			RuntimeComm.SetResourcePath(ResourcesPath);
			RuntimeComm.InitEngine();

			Console.WriteLine("yay!!!");
		}
	}
}
