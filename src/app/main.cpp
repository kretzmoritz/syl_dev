#include "../common/unit_testing.h"
#include "../common/test_printer_stream.h"
#include "../framework/window_desc.h"
#include "../framework/window.h"
#include "window_procedure.h"
#include "../common/enum.h"
#include "../framework/input_context.h"

BETTER_ENUM(InputAction, int,
	Confirm)

BETTER_ENUM(InputState, int,
	Placeholder)

BETTER_ENUM(InputRange, int,
	Placeholder)

// In progress:
// - Input system

// To do:
// - Quaternions
// - File loading (texture, meshes)
// - Basic renderer
// - Basic sound system
// - Basic video player

void RunTests()
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
	RunTests();

	SylDev::Framework::WindowCreationResult::Type result;
	SylDev::Framework::WindowClassDesc classDesc;
	SylDev::Framework::WindowDesc wndDesc;

	SylDev::Framework::Window<SylDev::App::WindowProcedure> window(result, classDesc, wndDesc, GetCommandLine());

	SylDev::Framework::InputContext<InputAction, InputState, InputRange> inputContext;
	inputContext.MapButtonToAction(SylDev::Framework::RawInputButton::Enter, InputAction::Confirm);

	inputContext.WriteToFile("input/default.ini");
	inputContext.ReadFromFile("input/default.ini");

	return 0;
}