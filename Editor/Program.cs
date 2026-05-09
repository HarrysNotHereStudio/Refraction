
namespace Editor {
	internal class Program {
		public const string ResourcesPath = @"..\..\Resources";

		static void Main(string[] args) {
			RuntimeComm.TestPrint();
			Console.WriteLine("Initialising Refraction Editor...");
			RuntimeComm.SetResourcePath(ResourcesPath);

			// Init and test Lua runtime
			RuntimeComm.InitLua();
			RuntimeComm.ParseLua(File.ReadAllText(ResourcesPath + "/test.lua"));

			RuntimeComm.InitEngine();
			
			Console.WriteLine("yay!!!");
		}
	}
}
