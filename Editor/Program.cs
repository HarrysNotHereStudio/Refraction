
namespace Editor {
	internal class Program {
		public const string ResourcesPath = @"..\..\..\..\..\Resources";

		static void Main(string[] args) {
			RuntimeComm.TestPrint();
			Console.WriteLine("Initialising Refraction Editor...");
			RuntimeComm.SetResourcePath(ResourcesPath);
			RuntimeComm.InitLua();
			RuntimeComm.ParseLua("print('gurt: yo')");
			RuntimeComm.ParseLua("--sumi if you see this fuck you; print(':3')");

			RuntimeComm.InitEngine();

			Console.WriteLine("yay!!!");
		}
	}
}
