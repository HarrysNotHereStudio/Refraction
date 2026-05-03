using System.Runtime.InteropServices;

namespace Editor {
	internal class RuntimeComm {
		public const string RuntimeDLLPath = @"RefractionRuntime.dll";
		[DllImport(RuntimeDLLPath, CallingConvention = CallingConvention.Cdecl)]
		public static extern int TestFunction();
		[DllImport(RuntimeDLLPath, CallingConvention = CallingConvention.Cdecl)]
		public static extern void TestPrint();
		[DllImport(RuntimeDLLPath, CallingConvention = CallingConvention.Cdecl)]
		public static extern void InitEngine();
		[DllImport(RuntimeDLLPath, CallingConvention = CallingConvention.Cdecl)]
		public static extern void SetResourcePath(string path);
	}
}
