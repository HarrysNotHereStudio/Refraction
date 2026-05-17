
namespace Editor {
	internal class Program {
		public const string ResourcesPath = "../../Resources/";

		static void Main(string[] args) {
			Console.WriteLine("Initialising Refraction Editor...");
			//RuntimeComm.SetResourcePath(ResourcesPath);

			// Init and test Lua runtime
			RuntimeComm.InitLua();
			RuntimeComm.ParseLua(File.ReadAllText(ResourcesPath + "test.lua"));

			RuntimeComm.InitEngine();
			
			Console.WriteLine("yay!!!");
			RuntimeComm.Print("im a print");
			RuntimeComm.Warn("im a warning");
			RuntimeComm.Error("im an error");
		}
	}
}
