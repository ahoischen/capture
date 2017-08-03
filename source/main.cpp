#include <memory>
#include <string>
#include <iostream>

#include "3ds.h"

#include "capture/config/CaptureConfig.h"
#include "capture/interactive/InteractiveSession.h"
#include "capture/interactive/IsRunningInteractively.h"
#include "capture/resources/Console.h"
#include "capture/resources/GFX.h"
#include "capture/resources/RomFS.h"
#include "capture/RunAutomaticTests.h"
#include "capture/RunTests.h"
#include "capture/WriteExitCode.h"

const std::string kConfigFilename="romfs:/CaptureConfig.toml";

int main(int argc, char** argv) 
{
    std::unique_ptr<capture::resources::GFX> gfx = std::make_unique<capture::resources::GFX>();
    capture::resources::Console console(GFX_TOP);
    
    // RomFS initialization has to be delayed for unknown reasons. Wrapping it in a unique_ptr keeps its RAII properties.
    std::unique_ptr<capture::resources::RomFS> romFS;
    romFS = std::make_unique<capture::resources::RomFS>();

    capture::config::CaptureConfig config(capture::config::CreateConfigFromFile(kConfigFilename));
    int exit_code;

    if (capture::interactive::IsRunningInteractively()) {
        exit_code = capture::interactive::InteractiveSession().Run(config, gfx.get(), std::move(console));
    }
    else {
        exit_code = capture::RunAutomaticTests(config);
    }
    
    capture::WriteExitCode(exit_code, config.GetExitCodeFile());
    return exit_code;
}
