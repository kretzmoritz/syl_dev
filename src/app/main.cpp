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

void run_tests()
{
	SylDev::Common::TestPrinterStream streamPrinter;
	SylDev::Common::TestEnvironment::GetInstance().AssignPrinter(&streamPrinter);

	bool testResult = SylDev::Common::TestEnvironment::GetInstance().Run();

	streamPrinter.OutputToConsole();
	streamPrinter.OutputToFile("unit_tests.log");

	SylDev::Common::TestEnvironment::GetInstance().AssignPrinter(nullptr);
}

int main(int _argc, char* _argv[])
{
	run_tests();

	SylDev::Framework::WindowCreationResult result;
	SylDev::Framework::WindowClassDesc classDesc;
	SylDev::Framework::WindowDesc wndDesc;

	SylDev::Framework::Window<SylDev::App::WindowProcedure> Window(result, classDesc, wndDesc, GetCommandLine());

	return 0;
}