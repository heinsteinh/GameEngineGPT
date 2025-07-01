#include <Utils/FilePath.h>
#include <Utils/ResourcePath.h>
#include <ftspch.h>

namespace fts
{

    std::string getRootResourcePath()
    {
        std::stringstream ss;
        ss << GAME_ENGINE_ROOT_RESOURCE;
        return ss.str();
    }

    std::string getGameExampleRootPath()
    {
        std::stringstream ss;
        ss << GAME_EXAMPLE_RESOURCE;
        return ss.str();
    }

    std::string getGameCmdRootPath()
    {
        std::stringstream ss;
        ss << GAME_CMD_RESOURCE;
        return ss.str();
    }

    std::string getRootBinaireResourcePath()
    {
        std::stringstream ss;
        ss << GAME_EXECUTABLE_OUTPUT_PATH;
        return ss.str();
    }

    std::string getPathRelativeRoot(const std::string& path)
    {
        fts::util::FilePath pathRoot(getRootResourcePath() + std::string("/") + path);
        return pathRoot;
    }

    std::string getPathGameExample(const std::string& path)
    {
        fts::util::FilePath pathRoot(getGameExampleRootPath() + std::string("/") + path);
        return pathRoot;
    }

    std::string getPathGameCmd(const std::string& path)
    {
        fts::util::FilePath pathRoot(getGameCmdRootPath() + std::string("/") + path);
        return pathRoot;
    }

    std::string getPathRelativeBinary(const std::string& path)
    {
        fts::util::FilePath pathRoot(getRootBinaireResourcePath() + std::string("/") + path);
        return pathRoot;
    }

} // namespace fts
