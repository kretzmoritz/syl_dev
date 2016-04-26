#include "../common/unit_testing.h"
#include "../framework/window_desc.h"
#include "../framework/window.h"
#include "window_procedure.h"
#include "../framework/input_context.h"

using namespace SylDev::Common;
using namespace SylDev::Framework;

// In progress:
// - Input system

// To do:
// - Quaternions
// - File loading (texture, meshes)
// - Basic renderer

void RunTests()
{
	TestPrinterStream streamPrinter;
	TestEnvironment::GetInstance().AssignPrinter(&streamPrinter);

	bool testResult = TestEnvironment::GetInstance().Run();

	streamPrinter.OutputToConsole();
	streamPrinter.OutputToFile("unit_tests.log");

	TestEnvironment::GetInstance().AssignPrinter(nullptr);
}

int main(int _argc, char* _argv[])
{
	RunTests();

	WindowCreationResult::Type result;
	WindowClassDesc classDesc;
	WindowDesc wndDesc;

	Window<SylDev::App::WindowProcedure> Window(result, classDesc, wndDesc, GetCommandLine());

	InputAction ACTION_CONFIRM = InputAction::GetAction("ACTION_CONFIRM");

	InputContext inputContext;
	inputContext.MapButtonToAction(RawInputButton::Enter, ACTION_CONFIRM);

	inputContext.WriteToFile("input/default.ini");

	return 0;
}