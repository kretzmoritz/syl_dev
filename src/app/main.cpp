#include "../common/unit_testing.h"
#include "../framework/window_desc.h"
#include "../framework/window.h"
#include "window_procedure.h"

// In progress:
// - Input system

// To do:
// - Quaternions
// - File loading (texture, meshes)
// - Basic renderer

int main()
{
	SylDev::Common::TestPrinterStream streamPrinter;
	SylDev::Common::TestEnvironment::GetInstance().AssignPrinter(&streamPrinter);

	bool testResult = SylDev::Common::TestEnvironment::GetInstance().Run();

	streamPrinter.OutputToConsole();
	streamPrinter.OutputToFile("unit_tests.log");

	SylDev::Framework::WindowCreationResult result;
	SylDev::Framework::WindowClassDesc classDesc;
	SylDev::Framework::WindowDesc wndDesc;

	SylDev::Framework::Window<SylDev::App::WindowProcedure> Window(result, classDesc, wndDesc);

	return 0;
}